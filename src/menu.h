// Copyright 2021 Alexander Lay-Calvert
#ifndef MENU_H_
#define MENU_H_

#include <ncurses.h>
#include <string.h>

#define MENU_SELECTED_COLOR_PAIR 6
#define MENU_BORDER_COLOR_PAIR 7

#define MENU_BORDER 'I'

#define MENU_ROWS 30
#define MENU_COLS 80
#define MENU_CHOICE_COLS (MENU_COLS - 8)

#define MENU_CHOICE_PLAY 0
#define MENU_CHOICE_SETTINGS 1
#define MENU_CHOICE_HELP 2
#define MENU_CHOICE_EXIT 3

void print_game_title(const int ctr_row, const int ctr_col);
void print_menu_choice(const int ctr_row, const int ctr_col, const char *choice,
                       const bool selected);
void clear_menu_space(const int rows, const int cols);
int display_menu(const int rows, const int cols);

#endif // MENU_H_
