# ascii-portal

A stupid simple implementation of something vaguely resembling the game Portal.

***NOTE***: While the initial version of the game *is* functional, I'm in the
process of refactoring the code and hopefully adding more features so this is a
work in progress.

This game uses the ncurses library and terminal colors.

## Installation and Running

On Linux (and macOS?):

```bash
# Clone the repository
git clone https://github.com/alex-laycalvert/ascii-portal.git

# Build using make
make

# Optionally build using gcc
gcc src/main.c -lncurses -o portal

# Or make a dev build if you want to contribute
make dev

# Run
./portal

```

On Windows:

```bash
# Sorry (maybe coming soon...)
```

## Gameplay

You are the green box that starts in the middle of the map.

To quit the game at any time, press Q.

To move, use the keybindings:

- W (Up)
- A (Left)
- S (Down)
- D (Right)

To change the direction you're looking (the arrow around your character) use
the keybindings:

- H (Left)
- J (Down)
- K (Up)
- L (Right)

To shoot a portal, press Space.

The default portal you shoot is the blue portal, to toggle to the orange portal,
press P.

Walking into a portal will transport you to the opposite colored portal,
assuming it has been set.
