#include "menu.h"
#include "portal.h"

void setup();
void run();
void exit_game(int code);

int main(int argc, char **argv) {
    (void)argc, (void)argv;
    setup();
    run();
    exit_game(EXIT_SUCCESS);
}

void setup() {
    // ncurses initialization
    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);
    curs_set(0);

    // damn that sucks
    if (!has_colors()) {
        endwin();
        fprintf(stderr, "Your terminal does not support colors\n");
        exit(1);
    }

    // initializing some colors
    start_color();
    use_default_colors();
    init_pair(EMPTY_COLOR_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(WALL_COLOR_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(PLAYER_COLOR_PAIR, COLOR_GREEN, COLOR_GREEN);
    init_pair(BLUE_PORTAL_COLOR_PAIR, COLOR_BLUE, COLOR_BLUE);
    init_pair(ORANGE_PORTAL_COLOR_PAIR, COLOR_RED, COLOR_RED);
    init_pair(MENU_SELECTED_COLOR_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(MENU_BORDER_COLOR_PAIR, COLOR_WHITE, COLOR_WHITE);
}

void run() {
    // getting terminal size info
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    char grid[rows][cols];

    int level = 0; // TODO let player choose level
    init_level(level, rows, cols, grid);
    play(rows, cols, grid);
}

void exit_game(int code) {
    endwin();
    exit(code);
}

