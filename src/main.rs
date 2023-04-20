use crossterm::{
    cursor,
    event::{read, Event, KeyCode},
    execute,
    style::{Color, Print, ResetColor, SetBackgroundColor, SetForegroundColor},
    terminal::{disable_raw_mode, enable_raw_mode, size, Clear, ClearType},
    ExecutableCommand,
};
use std::io::{stdout, Stdout};

#[derive(Clone)]
enum NodeType {
    Player,
    BluePortal,
    OrangePortal,
    Block,
}

struct Node {
    pub node_type: NodeType,
    pub col: u16,
    pub row: u16,
    pub ch: char,
    pub fg_color: Color,
    pub bg_color: Color,
    pub dir: Direction,
}

#[derive(PartialEq, Eq)]
struct Direction(i16, i16);

impl Direction {
    const UP: Self = Self(-1, 0);
    const DOWN: Self = Self(1, 0);
    const LEFT: Self = Self(0, -1);
    const RIGHT: Self = Self(0, 1);
}

impl Node {
    pub fn new(node_type: NodeType, row: u16, col: u16) -> Node {
        let (ch, fg_color, bg_color) = match node_type {
            NodeType::Player => ('X', Color::Green, Color::Green),
            NodeType::BluePortal => ('O', Color::Blue, Color::Blue),
            NodeType::OrangePortal => (
                'O',
                Color::Rgb {
                    r: 255,
                    g: 255,
                    b: 0,
                },
                Color::Rgb {
                    r: 255,
                    g: 255,
                    b: 0,
                },
            ),
            NodeType::Block => ('B', Color::Black, Color::Black),
        };
        Node {
            node_type,
            row,
            col,
            ch,
            fg_color,
            bg_color,
            dir: Direction::DOWN,
        }
    }
}

struct Level {
    nodes: Vec<Node>,
    selected_portal: NodeType,
    looking_at: (u16, u16),
    rows: u16,
    cols: u16,
}

impl Level {
    fn set_player_looking_at(&mut self) -> Result<(), &str> {
        let player_index = self.get_player().expect("failed to get player");
        let player = &self.nodes[player_index];
        let first_nonempty_index: Option<usize> = match player.dir {
            Direction::UP => self
                .nodes
                .iter()
                .position(|n| n.row < player.row && n.col == player.col),
            Direction::DOWN => self
                .nodes
                .iter()
                .position(|n| n.row > player.row && n.col == player.col),
            Direction::LEFT => self
                .nodes
                .iter()
                .position(|n| n.row == player.row && n.col < player.col),

            Direction::RIGHT => self
                .nodes
                .iter()
                .position(|n| n.row == player.row && n.col > player.col),
            _ => None,
        };
        match first_nonempty_index {
            Some(i) => {
                let node = &self.nodes[i];
                self.looking_at = match player.dir {
                    Direction::UP => (node.row, player.col),
                    Direction::DOWN => (node.row, player.col),
                    Direction::LEFT => (player.row, node.col),
                    Direction::RIGHT => (player.row, node.col),
                    _ => (0, 0),
                }
            }
            None => {
                self.looking_at = match player.dir {
                    Direction::UP => (0, player.col),
                    Direction::DOWN => (self.rows - 1, player.col),
                    Direction::LEFT => (player.row, 0),
                    Direction::RIGHT => (player.row, self.cols - 1),
                    _ => (0, 0),
                };
            }
        }
        Ok(())
    }

    /// Returns the index of the blue portal on the level in the `nodes` vector.
    fn get_blue_portal(&self) -> Option<usize> {
        match self
            .nodes
            .iter()
            .position(|n| matches!(n.node_type, NodeType::BluePortal))
        {
            Some(i) => Some(i),
            None => None,
        }
    }

    /// Returns the index of the orange portal on the level in the `nodes` vector.
    fn get_orange_portal(&self) -> Option<usize> {
        match self
            .nodes
            .iter()
            .position(|n| matches!(n.node_type, NodeType::OrangePortal))
        {
            Some(i) => Some(i),
            None => None,
        }
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

    /// Returns the index of the node at the given position on the level in the `nodes` vector.
    fn get_node_by_position(&self, row: u16, col: u16) -> Option<usize> {
        match self.nodes.iter().position(|n| n.row == row && n.col == col) {
            Some(i) => Some(i),
            None => None,
        }
    }

    /// Instantiates a new `Level` struct where `rows` and `cols` is the size of the terminal
    /// window.
    pub fn new(rows: u16, cols: u16) -> Level {
        Level {
            nodes: vec![],
            selected_portal: NodeType::BluePortal,
            looking_at: (0, 0),
            rows,
            cols,
        }
    }

    /// Sets the position of the player of the level. Instantiates a new `Node` with type
    /// `Player` if the player does not currently exist on the level.
    pub fn set_player(&mut self, row: u16, col: u16) -> Result<(), &str> {
        if row == 0 || row == self.rows - 1 || col == 0 || col == self.cols - 1 {
            return Err("cannot set player to be on the walls of the level");
        }
        match self.get_player() {
            Some(i) => {
                self.nodes[i].row = row;
                self.nodes[i].col = col;
            }
            None => self.nodes.push(Node::new(NodeType::Player, row, col)),
        }
        self.set_player_looking_at()?;
        Ok(())
    }

    /// Sets the position of the given node on the level.
    pub fn set_node(&mut self, node_type: NodeType, row: u16, col: u16) -> Result<(), &str> {
        if row == 0 || row == self.rows - 1 || col == 0 || col == self.cols - 1 {
            return Err("cannot set node to be on the walls of the level");
        }
        self.nodes.push(Node::new(node_type, row, col));
        Ok(())
    }

    /// Attempts to move the player in the specified direction.
    ///
    /// # Panics
    ///
    /// Panics if the player has not been set on the level.
    pub fn move_player(&mut self, direction: Direction) -> Result<(), &str> {
        let player_index = self
            .get_player()
            .expect("attempted to move a non-existent player");
        let player = &self.nodes[player_index];
        match self.nodes.iter().position(|n| {
            n.row as i16 == player.row as i16 + direction.0
                && n.col as i16 == player.col as i16 + direction.1
        }) {
            Some(_) => return Ok(()),
            None => {
                let new_row = (self.nodes[player_index].row as i16 + direction.0) as u16;
                let new_col = (self.nodes[player_index].col as i16 + direction.1) as u16;
                if new_row > 0 && new_row < self.rows - 1 {
                    self.nodes[player_index].row = new_row;
                }
                if new_col > 0 && new_col < self.cols - 1 {
                    self.nodes[player_index].col = new_col;
                }
            }
        };
        self.set_player_looking_at()?;
        Ok(())
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
        self.set_player_looking_at()?;
        Ok(())
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
        match portal_index {
            Some(i) => {
                self.nodes[i].row = row;
                self.nodes[i].col = col;
            }
            None => self
                .nodes
                .push(Node::new(self.selected_portal.clone(), row, col)),
        }
        self.set_player_looking_at()?;
        Ok(())
    }

    pub fn toggle_portal(&mut self) {
        match self.selected_portal {
            NodeType::BluePortal => self.selected_portal = NodeType::OrangePortal,
            _ => self.selected_portal = NodeType::BluePortal,
        }
    }

    /// Draws the current level, including walls, on the terminal window.
    pub fn draw(&self, stdout: &mut Stdout) -> crossterm::Result<()> {
        stdout.execute(Clear(ClearType::All))?;
        for c in 0..self.cols {
            execute!(
                stdout,
                SetForegroundColor(Color::White),
                SetBackgroundColor(Color::White),
                cursor::MoveTo(c, 0),
                Print('I'),
                ResetColor
            )?;
        }
        for r in 1..(self.rows - 1) {
            execute!(
                stdout,
                SetForegroundColor(Color::White),
                SetBackgroundColor(Color::White),
                cursor::MoveTo(0, r),
                Print('I'),
                cursor::MoveTo(self.cols - 1, r),
                Print('I'),
                ResetColor
            )?;
        }
        for c in 0..self.cols {
            execute!(
                stdout,
                SetForegroundColor(Color::White),
                SetBackgroundColor(Color::White),
                cursor::MoveTo(c, self.rows - 1),
                Print('I'),
                ResetColor
            )?;
        }
        for (_, n) in self.nodes.iter().enumerate() {
            execute!(
                stdout,
                cursor::MoveTo(n.col as u16, n.row as u16),
                SetForegroundColor(n.fg_color),
                SetBackgroundColor(n.bg_color),
                Print(n.ch),
                ResetColor
            )?;
        }

        let player_index = match self.get_player() {
            Some(i) => i,
            None => return Ok(()),
        };
        let player = &self.nodes[player_index];

        match player.dir {
            Direction::UP => {
                for i in (self.looking_at.0 + 1)..player.row {
                    if i == self.looking_at.0 + 1 {
                        execute!(stdout, cursor::MoveTo(self.looking_at.1, i), Print('^'))?;
                    } else {
                        execute!(stdout, cursor::MoveTo(self.looking_at.1, i), Print('|'))?;
                    }
                }
            }
            Direction::DOWN => {
                for i in (player.row + 1)..self.looking_at.0 {
                    if i == self.looking_at.0 - 1 {
                        execute!(stdout, cursor::MoveTo(self.looking_at.1, i), Print('v'))?;
                    } else {
                        execute!(stdout, cursor::MoveTo(self.looking_at.1, i), Print('|'))?;
                    }
                }
            }
            Direction::LEFT => {
                for i in (self.looking_at.1 + 1)..player.col {
                    if i == self.looking_at.1 + 1 {
                        execute!(stdout, cursor::MoveTo(i, self.looking_at.0), Print('<'))?;
                    } else {
                        execute!(stdout, cursor::MoveTo(i, self.looking_at.0), Print('-'))?;
                    }
                }
            }
            Direction::RIGHT => {
                for i in (player.col + 1)..self.looking_at.1 {
                    if i == self.looking_at.1 - 1 {
                        execute!(stdout, cursor::MoveTo(i, self.looking_at.0), Print('>'))?;
                    } else {
                        execute!(stdout, cursor::MoveTo(i, self.looking_at.0), Print('-'))?;
                    }
                }
            }
            _ => (),
        }
        stdout.execute(cursor::MoveTo(0, 0))?;

        Ok(())
    }
}

fn main() -> crossterm::Result<()> {
    let (cols, rows) = size()?;
    let mut stdout = stdout();
    enable_raw_mode()?;

    let mut level = Level::new(rows, cols);
    level
        .set_player(rows / 2, cols / 2)
        .expect("failed to set player");
    level
        .set_node(NodeType::BluePortal, 5, 20)
        .expect("failed to set node");

    loop {
        level.draw(&mut stdout)?;
        match read().unwrap() {
            Event::Key(event) => {
                match event.code {
                    KeyCode::Char('w') => level
                        .move_player(Direction::UP)
                        .expect("could not move player"),
                    KeyCode::Char('s') => level
                        .move_player(Direction::DOWN)
                        .expect("could not move player"),
                    KeyCode::Char('a') => level
                        .move_player(Direction::LEFT)
                        .expect("could not move player"),
                    KeyCode::Char('d') => level
                        .move_player(Direction::RIGHT)
                        .expect("could not move player"),
                    KeyCode::Up => level
                        .change_player_direction(Direction::UP)
                        .expect("could not change player direction"),
                    KeyCode::Down => level
                        .change_player_direction(Direction::DOWN)
                        .expect("could not change player direction"),
                    KeyCode::Left => level
                        .change_player_direction(Direction::LEFT)
                        .expect("could not change player direction"),
                    KeyCode::Right => level
                        .change_player_direction(Direction::RIGHT)
                        .expect("could not change player direction"),
                    KeyCode::Char('t') => level.toggle_portal(),
                    KeyCode::Char(' ') => level.shoot_portal().expect("could not shoot portal"),
                    KeyCode::Char('q') => break,
                    _ => (),
                };
            }
            _ => (),
        }
    }

    disable_raw_mode()?;

    Ok(())
}
