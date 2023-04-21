use crossterm::style::Color;

#[derive(Clone)]
pub enum NodeType {
    Player,
    BluePortal,
    OrangePortal,
    Block,
    Wall,
}

pub struct Node {
    pub node_type: NodeType,
    pub col: u16,
    pub row: u16,
    pub ch: char,
    pub fg_color: Color,
    pub bg_color: Color,
    pub dir: Direction,
}

/// A cardinal direction (`UP`, `DOWN`, `LEFT`, or `RIGHT`) that a node
/// can be facing/looking in. Contains the unit-coordinates of the direction
/// relative to the node.
#[derive(PartialEq, Eq)]
pub struct Direction(pub i16, pub i16);

impl Direction {
    pub const UP: Self = Self(-1, 0);
    pub const DOWN: Self = Self(1, 0);
    pub const LEFT: Self = Self(0, -1);
    pub const RIGHT: Self = Self(0, 1);
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
                    g: 127,
                    b: 0,
                },
                Color::Rgb {
                    r: 255,
                    g: 127,
                    b: 0,
                },
            ),
            NodeType::Block => (
                'B',
                Color::Rgb {
                    r: 100,
                    g: 100,
                    b: 100,
                },
                Color::Rgb {
                    r: 100,
                    g: 100,
                    b: 100,
                },
            ),
            NodeType::Wall => ('I', Color::White, Color::White),
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
