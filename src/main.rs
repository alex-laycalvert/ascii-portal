mod level;

use crossterm::terminal::size;
use level::{node::NodeType, Level};
use std::io::stdout;

fn main() -> crossterm::Result<()> {
    let (cols, rows) = size()?;
    let mut stdout = stdout();

    let mut level = Level::new(20, 40, rows, cols);
    level.set_player(10, 20).expect("failed to set player");
    level
        .add_node(NodeType::Block, 5, 20)
        .expect("failed to set node");
    level.play(&mut stdout)?;

    Ok(())
}
