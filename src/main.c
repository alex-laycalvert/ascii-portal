#include "levels.h"
#include "menu.h"
#include "portal.h"
#include <stdio.h>
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
    init_pair(MENU_SELECTED_COLOR_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(MENU_BORDER_COLOR_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(EMPTY_COLOR_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(WALL_COLOR_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(PLAYER_COLOR_PAIR, COLOR_GREEN, COLOR_GREEN);
    init_pair(BPORTAL_COLOR_PAIR, COLOR_BLUE, COLOR_BLUE);
    init_pair(OPORTAL_COLOR_PAIR, COLOR_RED, COLOR_RED);
    init_pair(HOLD_BUTTON_COLOR_PAIR, COLOR_BLUE, COLOR_BLACK);
    init_pair(BLOCK_COLOR_PAIR, COLOR_BLACK, COLOR_BLACK);
    init_pair(LEVER_ON_COLOR_PAIR, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(LEVER_OFF_COLOR_PAIR, COLOR_RED, COLOR_BLACK);
    init_pair(KEY_COLOR_PAIR, COLOR_YELLOW, COLOR_YELLOW);
    init_pair(END_COLOR_PAIR, COLOR_YELLOW, COLOR_BLACK);
}

void run() {
    // getting terminal size info
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    int menu_choice = display_menu(rows, cols);
    if (menu_choice == MENU_CHOICE_EXIT) exit_game(EXIT_SUCCESS);

    init_map(rows, cols);
    play();
    destroy_map();
}

void exit_game(int code) {
    endwin();
    exit(code);
}

