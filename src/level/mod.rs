pub mod node;

use crossterm::{
    cursor,
    event::{read, Event, KeyCode},
    execute,
    style::{Color, Print, ResetColor, SetBackgroundColor, SetForegroundColor, Stylize},
    terminal::{disable_raw_mode, enable_raw_mode, Clear, ClearType},
    ExecutableCommand,
};
use node::*;
use std::fs;
use std::io::Stdout;

/// Represents a level of `ascii-portal`.
pub struct Level {
    nodes: Vec<Node>,
    selected_portal: NodeType,
    looking_at: (u16, u16),
    rows: u16,
    cols: u16,
    term_rows: u16,
    term_cols: u16,
    row_offset: u16,
    col_offset: u16,
}

impl Level {
    /// Sets the value `looking_at` to be the (`row`, `col`) location of the
    /// farthest empty space in the direction the player is looking in.
    fn set_player_looking_at(&mut self) -> Result<(), &str> {
        let player_index = match self.get_player() {
            Some(i) => i,
            None => return Ok(()),
        };
        let player = &self.nodes[player_index];
        let mut current_non_empty_row: u16 = 0;
        let mut current_non_empty_col: u16 = 0;
        let mut closest_non_empty_index: i16 = -1;
        for (i, n) in self.nodes.iter().enumerate() {
            if matches!(n.node_type, NodeType::ToggleBlock) && n.toggled {
                continue;
            }
            match player.dir {
                Direction::UP => {
                    if n.row < player.row && n.col == player.col {
                        if current_non_empty_row == 0 || n.row > current_non_empty_row {
                            current_non_empty_row = n.row;
                            closest_non_empty_index = i as i16;
                        }
                    }
                }
                Direction::DOWN => {
                    if n.row > player.row && n.col == player.col {
                        if current_non_empty_row == 0 || n.row < current_non_empty_row {
                            current_non_empty_row = n.row;
                            closest_non_empty_index = i as i16;
                        }
                    }
                }
                Direction::LEFT => {
                    if n.row == player.row && n.col < player.col {
                        if current_non_empty_col == 0 || n.col > current_non_empty_col {
                            current_non_empty_col = n.col;
                            closest_non_empty_index = i as i16;
                        }
                    }
                }
                Direction::RIGHT => {
                    if n.row == player.row && n.col > player.col {
                        if current_non_empty_col == 0 || n.col < current_non_empty_col {
                            current_non_empty_col = n.col;
                            closest_non_empty_index = i as i16;
                        }
                    }
                }
                _ => closest_non_empty_index = 0,
            }
        }
        if closest_non_empty_index >= 0 {
            let node = &self.nodes[closest_non_empty_index as usize];
            self.looking_at = match player.dir {
                Direction::UP => (node.row, player.col),
                Direction::DOWN => (node.row, player.col),
                Direction::LEFT => (player.row, node.col),
                Direction::RIGHT => (player.row, node.col),
                _ => (0, 0),
            };
        } else {
            self.looking_at = match player.dir {
                Direction::UP => (0, player.col),
                Direction::DOWN => (self.rows - 1, player.col),
                Direction::LEFT => (player.row, 0),
                Direction::RIGHT => (player.row, self.cols - 1),
                _ => (0, 0),
            };
        }
        Ok(())
    }

    /// Returns the index of the blue portal on the level in the `nodes` vector.
    fn get_blue_portal(&self) -> Option<usize> {
        self.nodes
            .iter()
            .position(|n| matches!(n.node_type, NodeType::BluePortal))
    }

    /// Returns the index of the orange portal on the level in the `nodes` vector.
    fn get_orange_portal(&self) -> Option<usize> {
        self.nodes
            .iter()
            .position(|n| matches!(n.node_type, NodeType::OrangePortal))
    }

    /// Returns the index of the player on the level in the `nodes` vector.
    fn get_player(&self) -> Option<usize> {
        match self
            .nodes
            .iter()
            .position(|n| matches!(n.node_type, NodeType::Player))
        {
            Some(i) => Some(i),
            None => None,
        }
    }

    /// Toggles every `ToggleBlock` in the level
    fn toggle_blocks(&mut self) {
        for index in 0..self.nodes.len() {
            if matches!(self.nodes[index].node_type, NodeType::ToggleBlock) {
                self.nodes[index].toggled = !self.nodes[index].toggled;
            }
        }
    }

    /// Returns the index of the node at the given position on the level in the `nodes` vector.
    fn get_node_by_position(&self, row: u16, col: u16) -> Option<usize> {
        match self.nodes.iter().position(|n| n.row == row && n.col == col) {
            Some(i) => Some(i),
            None => None,
        }
    }

    /// Instantiates a new `Level` struct where `rows` and `cols` refer to the size of the
    /// level and `term_rows` and `term_cols` refer to the size of the terminal window.
    pub fn new(term_rows: u16, term_cols: u16) -> Level {
        Level {
            nodes: vec![],
            selected_portal: NodeType::BluePortal,
            looking_at: (0, 0),
            rows: 0,
            cols: 0,
            term_rows,
            term_cols,
            row_offset: 0,
            col_offset: 0,
        }
    }

    pub fn generate_from_string(&mut self, level_content: String) -> Result<(u16, u16), String> {
        let lines: Vec<&str> = level_content.trim().split('\n').collect();
        let total_rows = lines.len() as u16;
        let total_cols: u16;
        if total_rows > 2 {
            total_cols = lines[0].len() as u16
        } else {
            return Err("file contains an empty level".to_string());
        }
        if total_cols <= 2 {
            return Err("file contains an empty level".to_string());
        }
        for (r, line) in lines.iter().enumerate() {
            if r == 0 || r as u16 == total_cols - 1 {
                for c in line.chars() {
                    if c != 'I' {
                        return Err("file must surround the level entirely in an even box of `I` characters representing the walls".to_string());
                    }
                }
                continue;
            }
            if line.len() < total_cols as usize {
                return Err("all lines in the file must be of equal length and be surrounded by an even box of `I` characters representing the walls".to_string());
            }
            for (c, ch) in line.chars().enumerate() {
                if c == 0 || c == total_cols as usize - 1 {
                    if ch != 'I' {
                        return Err("file must surround the level entirely in an even box of `I` characters representing the walls".to_string());
                    }
                    continue;
                }
                if let Some(node_type) = NodeType::from(ch) {
                    self.nodes.push(Node::new(node_type, r as u16, c as u16));
                }
            }
        }
        self.rows = total_rows;
        self.cols = total_cols;
        self.row_offset = (self.term_rows - self.rows) / 2;
        self.col_offset = (self.term_cols - self.cols) / 2;
        Ok((self.rows, self.cols))
    }

    /// Parses the contetns from the given `filename` and initializes the level with
    /// those contents if able to.
    pub fn generate_from_file(&mut self, filename: String) -> Result<(u16, u16), String> {
        let file_content = match fs::read_to_string(filename) {
            Ok(c) => c,
            Err(e) => return Err(e.to_string()),
        };
        self.generate_from_string(file_content)
    }

    /// Sets the position of the player of the level. Instantiates a new `Node` with type
    /// `Player` if the player does not currently exist on the level.
    pub fn set_player(&mut self, row: u16, col: u16) -> Result<(), &str> {
        if row == 0 || row >= self.rows - 1 || col == 0 || col >= self.cols - 1 {
            return Err("cannot set player outside bounds of level");
        }
        match self.get_player() {
            Some(i) => {
                self.nodes[i].row = row;
                self.nodes[i].col = col;
            }
            None => self.nodes.push(Node::new(NodeType::Player, row, col)),
        }
        self.set_player_looking_at()
    }

    /// Attempts to move the player in the specified direction.
    pub fn move_player(&mut self, direction: Direction) -> Result<(), &str> {
        let player_index = match self.get_player() {
            Some(i) => i,
            None => return Ok(()),
        };
        let players_next_row = (self.nodes[player_index].row as i16 + direction.0) as u16;
        let players_next_col = (self.nodes[player_index].col as i16 + direction.1) as u16;
        if let Some(i) = self.get_node_by_position(players_next_row, players_next_col) {
            match self.nodes[i].node_type {
                NodeType::BluePortal => {
                    if let Some(index) = self.get_orange_portal() {
                        self.nodes[player_index].row = self.nodes[index].row;
                        self.nodes[player_index].col = self.nodes[index].col;
                    }
                }
                NodeType::OrangePortal => {
                    if let Some(index) = self.get_blue_portal() {
                        self.nodes[player_index].row = self.nodes[index].row;
                        self.nodes[player_index].col = self.nodes[index].col;
                    }
                }
                NodeType::Block => {
                    let blocks_next_row = (players_next_row as i16 + direction.0) as u16;
                    let blocks_next_col = (players_next_col as i16 + direction.1) as u16;
                    if let Some(_) = self.get_node_by_position(blocks_next_row, blocks_next_col) {
                        return Ok(());
                    }
                    if blocks_next_row < 1
                        || blocks_next_row >= self.rows - 1
                        || blocks_next_col < 1
                        || blocks_next_col >= self.cols - 1
                    {
                        return Ok(());
                    }
                    self.set_player(players_next_row, players_next_col).ok();
                    self.nodes[i].row = blocks_next_row;
                    self.nodes[i].col = blocks_next_col;
                }
                NodeType::Switch => {
                    self.nodes[i].toggled = !self.nodes[i].toggled;
                    self.toggle_blocks();
                }
                NodeType::ToggleBlock => {
                    if self.nodes[i].toggled {
                        self.set_player(players_next_row, players_next_col).ok();
                    }
                }
                _ => (),
            }
        } else {
            self.set_player(players_next_row, players_next_col).ok();
        }
        for i in 0..self.nodes.len() {
            if !matches!(self.nodes[i].node_type, NodeType::Button) {
                continue;
            }
            if (self.nodes[i].row == self.nodes[player_index].row
                && (self.nodes[i].col == self.nodes[player_index].col - 1
                    || self.nodes[i].col == self.nodes[player_index].col + 1))
                || (self.nodes[i].col == self.nodes[player_index].col
                    && (self.nodes[i].row == self.nodes[player_index].row - 1
                        || self.nodes[i].row == self.nodes[player_index].row + 1))
            {
                if !self.nodes[i].toggled {
                    self.toggle_blocks();
                }
                self.nodes[i].toggled = true;
            } else {
                if self.nodes[i].toggled {
                    self.toggle_blocks();
                }
                self.nodes[i].toggled = false;
            }
        }
        self.set_player_looking_at()
    }

    /// Attempts to change the direction that the player is looking in
    ///
    /// # Panics
    ///
    /// Panics if the player has not been set on the level.
    pub fn change_player_direction(&mut self, direction: Direction) -> Result<(), &str> {
        let player_index = self
            .get_player()
            .expect("attempted to move a non-existent player");
        self.nodes[player_index].dir = direction;
        self.set_player_looking_at()
    }

    pub fn shoot_portal(&mut self) -> Result<(), &str> {
        let portal_index: Option<usize> = match self.selected_portal {
            NodeType::OrangePortal => self.get_orange_portal(),
            _ => self.get_blue_portal(),
        };
        let player_index = self.get_player().expect("failed to get player");
        let player = &self.nodes[player_index];
        let row = (self.looking_at.0 as i16 - player.dir.0) as u16;
        let col = (self.looking_at.1 as i16 - player.dir.1) as u16;
        if player.row == row && player.col == col {
            return Ok(());
        }
        match portal_index {
            Some(i) => {
                if self.nodes[i].row != self.looking_at.0 || self.nodes[i].col != self.looking_at.1
                {
                    self.nodes[i].row = row;
                    self.nodes[i].col = col;
                }
            }
            None => self
                .nodes
                .push(Node::new(self.selected_portal.clone(), row, col)),
        }
        self.set_player_looking_at()
    }

    /// Toggles the currently selected portal that the player will shoot.
    /// Either `BluePortal` or `OrangePortal`.
    pub fn toggle_portal(&mut self) -> Result<(), &str> {
        match self.selected_portal {
            NodeType::BluePortal => self.selected_portal = NodeType::OrangePortal,
            _ => self.selected_portal = NodeType::BluePortal,
        }
        self.set_player_looking_at()
    }

    /// Draws the walls surrounding the level grid.
    pub fn draw_walls(&self, stdout: &mut Stdout) -> crossterm::Result<()> {
        for r in self.row_offset..(self.rows + self.row_offset) {
            for c in self.col_offset..(self.cols + self.col_offset) {
                if r == self.row_offset
                    || r == self.rows + self.row_offset - 1
                    || c == self.col_offset
                    || c == self.cols + self.col_offset - 1
                {
                    execute!(
                        stdout,
                        SetForegroundColor(Color::White),
                        SetBackgroundColor(Color::White),
                        cursor::MoveTo(c, r),
                        Print('I'),
                        ResetColor
                    )?;
                }
            }
        }
        Ok(())
    }

    /// Draws all nodes on the level grid to the terminal except for the `player` node.
    pub fn draw_nodes(&self, stdout: &mut Stdout) -> crossterm::Result<()> {
        for (_, n) in self.nodes.iter().enumerate() {
            if matches!(n.node_type, NodeType::Player) {
                continue;
            }
            if matches!(n.node_type, NodeType::ToggleBlock) && n.toggled {
                continue;
            }
            execute!(
                stdout,
                cursor::MoveTo(
                    n.col as u16 + self.col_offset,
                    n.row as u16 + self.row_offset
                ),
                SetForegroundColor(if n.toggled {
                    n.toggled_fg_color
                } else {
                    n.fg_color
                }),
                SetBackgroundColor(if n.toggled {
                    n.toggled_bg_color
                } else {
                    n.bg_color
                }),
                Print(n.ch.bold()),
                ResetColor
            )?;
        }
        Ok(())
    }

    /// Draws overlay arrow to show where the player is looking and which portal they
    /// have selected.
    pub fn draw_player_looking_at(&self, stdout: &mut Stdout) -> crossterm::Result<()> {
        let player_index = match self.get_player() {
            Some(i) => i,
            None => return Ok(()),
        };
        let player = &self.nodes[player_index];
        if self.looking_at.0 == 0 && self.looking_at.1 == 0 {
            return Ok(());
        }
        let (
            start_i,
            end_i,
            arrow_i,
            row_i_factor,
            col_i_factor,
            row_la_factor,
            col_la_factor,
            line_ch,
            arrow_ch,
        ): (u16, u16, u16, u16, u16, u16, u16, char, char) = match player.dir {
            Direction::UP => (
                self.looking_at.0 + 1,
                player.row,
                self.looking_at.0 + 1,
                1,
                0,
                0,
                1,
                '|',
                '^',
            ),
            Direction::DOWN => (
                player.row + 1,
                self.looking_at.0,
                self.looking_at.0 - 1,
                1,
                0,
                0,
                1,
                '|',
                'v',
            ),
            Direction::LEFT => (
                self.looking_at.1 + 1,
                player.col,
                self.looking_at.1 + 1,
                0,
                1,
                1,
                0,
                '-',
                '<',
            ),
            Direction::RIGHT => (
                player.col + 1,
                self.looking_at.1,
                self.looking_at.1 - 1,
                0,
                1,
                1,
                0,
                '-',
                '>',
            ),
            _ => (0, 0, 0, 0, 0, 0, 0, ' ', ' '),
        };
        for i in start_i..end_i {
            stdout.execute(cursor::MoveTo(
                i * col_i_factor + self.looking_at.1 * col_la_factor + self.col_offset,
                i * row_i_factor + self.looking_at.0 * row_la_factor + self.row_offset,
            ))?;
            if matches!(self.selected_portal, NodeType::OrangePortal) {
                execute!(
                    stdout,
                    SetForegroundColor(Color::Rgb {
                        r: 255,
                        g: 127,
                        b: 0
                    })
                )?;
            } else {
                execute!(stdout, SetForegroundColor(Color::Blue))?;
            }
            if i == arrow_i {
                stdout.execute(Print(arrow_ch.bold()))?;
            } else {
                stdout.execute(Print(line_ch.bold()))?;
            }
        }
        Ok(())
    }

    /// Draws the player node on the level with the arrow in the direction they
    /// are looking.
    pub fn draw_player(&self, stdout: &mut Stdout) -> crossterm::Result<()> {
        let player_index = match self.get_player() {
            Some(i) => i,
            None => return Ok(()),
        };
        let player = &self.nodes[player_index];
        execute!(
            stdout,
            cursor::MoveTo(
                player.col as u16 + self.col_offset,
                player.row as u16 + self.row_offset
            ),
            SetForegroundColor(player.fg_color),
            SetBackgroundColor(player.bg_color),
            Print(player.ch.bold()),
            ResetColor
        )?;
        self.draw_player_looking_at(stdout)?;
        Ok(())
    }

    /// Draws the current level, including walls, on the terminal window.
    pub fn draw(&self, stdout: &mut Stdout) -> crossterm::Result<()> {
        stdout.execute(Clear(ClearType::All))?;
        self.draw_walls(stdout)?;
        self.draw_nodes(stdout)?;
        self.draw_player(stdout)?;
        stdout.execute(cursor::MoveTo(0, 0))?;
        Ok(())
    }

    pub fn play(&mut self, stdout: &mut Stdout) -> crossterm::Result<()> {
        if self.nodes.len() < 1 {
            return Ok(());
        }
        enable_raw_mode()?;
        stdout.execute(cursor::Hide)?;
        loop {
            self.draw(stdout)?;
            match read().unwrap() {
                Event::Key(event) => {
                    match event.code {
                        KeyCode::Char('w') => self.move_player(Direction::UP),
                        KeyCode::Char('s') => self.move_player(Direction::DOWN),
                        KeyCode::Char('a') => self.move_player(Direction::LEFT),
                        KeyCode::Char('d') => self.move_player(Direction::RIGHT),
                        KeyCode::Up => self.change_player_direction(Direction::UP),
                        KeyCode::Down => self.change_player_direction(Direction::DOWN),
                        KeyCode::Left => self.change_player_direction(Direction::LEFT),
                        KeyCode::Right => self.change_player_direction(Direction::RIGHT),
                        KeyCode::Tab => self.toggle_portal(),
                        KeyCode::Char(' ') => self.shoot_portal(),
                        KeyCode::Char('q') => break,
                        _ => Ok(()),
                    }
                    .expect("failed to perform action");
                }
                _ => (),
            }
        }
        stdout.execute(cursor::Show)?;
        disable_raw_mode()?;
        Ok(())
    }
}
