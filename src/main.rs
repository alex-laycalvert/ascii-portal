use crossterm::{
    cursor,
    event::{read, Event, KeyCode},
    execute,
    style::{Color, Print, ResetColor, SetBackgroundColor, SetForegroundColor},
    terminal::{disable_raw_mode, enable_raw_mode, size, Clear, ClearType},
    ExecutableCommand,
};
use std::io::{stdout, Stdout};

enum NodeType {
    Player,
}

struct Node {
    pub node_type: NodeType,
    pub col: u16,
    pub row: u16,
    pub ch: char,
    pub fg_color: Color,
    pub bg_color: Color,
}

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
        };
        Node {
            node_type,
            row,
            col,
            ch,
            fg_color,
            bg_color,
        }
    }
}

struct Level {
    nodes: Vec<Node>,
    rows: u16,
    cols: u16,
}

impl Level {
    pub fn new(rows: u16, cols: u16) -> Level {
        Level {
            nodes: vec![],
            rows,
            cols,
        }
    }

    pub fn set_player(&mut self, row: u16, col: u16) -> Result<(), &str> {
        if row == 0 || row == self.rows - 1 || col == 0 || col == self.cols - 1 {
            return Err("cannot move player to be on the walls of the level");
        }
        let player_index = self.get_player();
        match player_index {
            Some(i) => {
                self.nodes[i].row = row;
                self.nodes[i].col = col;
            }
            None => self.nodes.push(Node::new(NodeType::Player, row, col)),
        }
        Ok(())
    }

    pub fn get_player(&self) -> Option<usize> {
        let player_index = self
            .nodes
            .iter()
            .position(|n| matches!(n.node_type, NodeType::Player));
        match player_index {
            Some(i) => Some(i),
            None => None,
        }
    }

    pub fn move_player(&mut self, direction: Direction) -> Result<(), &str> {
        let player_index = self
            .get_player()
            .expect("attempted to move a non-existent player");
        let player = &self.nodes[player_index];
        let node_index: Option<usize> = self.nodes.iter().position(|n| {
            n.row as i16 == player.row as i16 + direction.0
                && n.col as i16 == player.col as i16 + direction.1
        });
        match node_index {
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
            _ => (),
        }
        Ok(())
    }

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

    loop {
        level.draw(&mut stdout)?;
        match read().unwrap() {
            Event::Key(event) => {
                match event.code {
                    KeyCode::Up | KeyCode::Char('w') => level
                        .move_player(Direction::UP)
                        .expect("could not move player"),
                    KeyCode::Down | KeyCode::Char('s') => level
                        .move_player(Direction::DOWN)
                        .expect("could not move player"),
                    KeyCode::Left | KeyCode::Char('a') => level
                        .move_player(Direction::LEFT)
                        .expect("could not move player"),
                    KeyCode::Right | KeyCode::Char('d') => level
                        .move_player(Direction::RIGHT)
                        .expect("could not move player"),
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
