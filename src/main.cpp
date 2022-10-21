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
    curs_set(0);
    start_color();
    use_default_colors();
    init_pair(EMPTY_CP, -1, -1);
    init_pair(PLAYER_CP, COLOR_GREEN, COLOR_GREEN);
    init_pair(BLUE_PORTAL_CP, COLOR_BLUE, COLOR_BLUE);
    init_pair(ORANGE_PORTAL_CP, COLOR_RED, COLOR_RED);
    init_pair(BLOCK_CP, COLOR_BLACK, COLOR_BLACK);
    init_pair(WALL_CP, COLOR_WHITE, COLOR_WHITE);
}

void run() {
    string home = getenv("HOME");
    Config config = loadConfig(home + DEFAULT_CONFIG_FILE);
    Portal *portal = Portal::getPortal();
    portal->loadConfig(config);
    portal->loadLevel("./levels/0.portal");
    portal->play();

    /* int rows, columns; */
    /* getmaxyx(stdscr, rows, columns); */
}

void exit_game() {
    clear();
    endwin();
    exit(0);
}
