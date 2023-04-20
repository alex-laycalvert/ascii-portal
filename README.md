# ascii-portal

A stupid simple implementation of something vaguely resembling the game Portal built
with [Rust](https://www.rust-lang.org/).

## Installation and Running

On Linux (and macOS):

**_NOTE_**: Right now, you must have [cargo](https://www.rust-lang.org/tools/install) from
the rust toolchain to run the project.

```bash
# Clone the repository
git clone https://github.com/alex-laycalvert/ascii-portal
cd ascii-portal/

# Run the project with cargo where <filename> is the name of
# any `*.portal` file such as the provided `My_First_Level.portal`
cargo run -- <filename>

```

On Windows:

```bash
# Sorry (maybe coming soon... probably not)
```

## Gameplay

You are the green box that starts in the middle of the map.

To quit the game at any time, press Q.

To move, use the keybindings:

-   W (Up)
-   A (Left)
-   S (Down)
-   D (Right)

To change the direction you're looking (the arrow around your character) use
the arrow keys.

To shoot a portal, press Space.

The default portal you shoot is the blue portal, to toggle which portal you shoot,
press Tab.

Walking into a portal will transport you to the opposite colored portal,
assuming it has been set.
