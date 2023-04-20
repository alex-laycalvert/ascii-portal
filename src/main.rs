mod level;

use crossterm::terminal::size;
use level::Level;
use std::env::args;
use std::io::stdout;

fn main() -> crossterm::Result<()> {
    let args: Vec<String> = args().collect();
    if args.len() < 2 {
        println!("Please provide a `*.portal` file to read the level from");
        return Ok(());
    }
    let filename = args[1].to_string();
    let (cols, rows) = size()?;
    let mut stdout = stdout();

    let mut level = Level::new(rows, cols);
    match level.generate_from_file(filename) {
        Ok(_) => {
            level.play(&mut stdout)?;
        }
        Err(e) => println!("{}", e),
    };
    Ok(())
}
