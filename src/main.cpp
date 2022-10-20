// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#include <ncurses.h>

#include "config.hpp"
#include "portal.hpp"

void setup();
void run();
void exit_game();

int main(int argc, char **argv) {
    (void)argc, (void)argv;
    setup();
    run();
    exit_game();
}

void setup() {
    initscr();
    keypad(stdscr, true);
    raw();
    noecho();
}

void run() {
    string home = getenv("HOME");
    Configuration config(home + DEFAULT_CONFIG_FILE);
    Portal *portal = Portal::getPortal();

    int rows, columns;
    getmaxyx(stdscr, rows, columns);
}

void exit_game() {
    clear();
    endwin();
    exit(0);
}
