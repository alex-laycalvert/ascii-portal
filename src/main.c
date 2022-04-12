#include "menu.h"
#include "portal.h"
#include "portal_chars.h"
#include <stdlib.h>

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
    init_pair(BPORTAL_COLOR_PAIR, COLOR_BLUE, COLOR_BLUE);
    init_pair(OPORTAL_COLOR_PAIR, COLOR_RED, COLOR_RED);
    init_pair(HOLD_BUTTON_COLOR_PAIR, COLOR_BLUE, COLOR_BLUE);
    init_pair(BLOCK_COLOR_PAIR, COLOR_BLACK, COLOR_BLACK);
    init_pair(LEVER_ON_COLOR_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(LEVER_OFF_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(KEY_COLOR_PAIR, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(MENU_SELECTED_COLOR_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(MENU_BORDER_COLOR_PAIR, COLOR_WHITE, COLOR_WHITE);
}

void run() {
    // getting terminal size info
    int rows, cols;
    getmaxyx(stdscr, rows, cols);
    char grid[rows][cols];

    int menu_choice = display_menu(rows, cols);
    switch (menu_choice) {
        case MENU_CHOICE_PLAY:
            break;
        case MENU_CHOICE_SETTINGS:
            // TODO
            break;
        case MENU_CHOICE_HELP:
            // TODO
            break;
        case MENU_CHOICE_EXIT:
            exit_game(EXIT_SUCCESS);
            break;
    }

    int level = 0; // TODO let player choose level
    init_level(level, rows, cols, grid);
    play(rows, cols, grid);
}

void exit_game(int code) {
    endwin();
    exit(code);
}

