use crossterm::style::Color;

#[derive(Clone)]
pub enum NodeType {
    Player,
    BluePortal,
    OrangePortal,
    Block,
    Wall,
    Switch,
    ToggleBlock,
    Button,
}

impl NodeType {
    pub fn from(ch: char) -> Option<NodeType> {
        match ch {
            'X' => Some(NodeType::Player),
            'K' => Some(NodeType::Block),
            'I' => Some(NodeType::Wall),
            'S' => Some(NodeType::Switch),
            'B' => Some(NodeType::Button),
            'T' => Some(NodeType::ToggleBlock),
            _ => None,
        }
    }
}

pub struct Node {
    pub node_type: NodeType,
    pub col: u16,
    pub row: u16,
    pub ch: char,
    pub fg_color: Color,
    pub bg_color: Color,
    pub toggled_fg_color: Color,
    pub toggled_bg_color: Color,
    pub dir: Direction,
    pub toggled: bool,
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
        let (ch, fg_color, bg_color, toggled_fg_color, toggled_bg_color) = match node_type {
            NodeType::Player => ('X', Color::Green, Color::Green, Color::Green, Color::Green),
            NodeType::BluePortal => ('O', Color::Blue, Color::Blue, Color::Blue, Color::Blue),
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
                'K',
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
            NodeType::Wall => ('I', Color::White, Color::White, Color::White, Color::White),
            NodeType::Switch => ('S', Color::Yellow, Color::Red, Color::Red, Color::Yellow),
            NodeType::Button => ('B', Color::Yellow, Color::Red, Color::Red, Color::Yellow),
            NodeType::ToggleBlock => (
                'T',
                Color::Magenta,
                Color::Magenta,
                Color::Magenta,
                Color::Magenta,
            ),
        };
        Node {
            node_type,
            row,
            col,
            ch,
            fg_color,
            bg_color,
            toggled_fg_color,
            toggled_bg_color,
            dir: Direction::DOWN,
            toggled: false,
        }
    }
}
