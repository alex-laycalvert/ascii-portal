#ifndef PORTAL_H_
#define PORTAL_H_

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define WALL 'I'
#define PLAYER 'X'
#define EMPTY ' '

#define BLUE_PORTAL 'B'
#define ORANGE_PORTAL 'R'
#define PORTAL 'O'

#define LOOK_UP '^'
#define LOOK_DOWN 'v'
#define LOOK_LEFT '<'
#define LOOK_RIGHT '>'

#define QUIT_KEY 'q'

#define LOOK_UP_KEY 'k'
#define LOOK_DOWN_KEY 'j'
#define LOOK_LEFT_KEY 'h'
#define LOOK_RIGHT_KEY 'l'

#define MOVE_UP_KEY 'w'
#define MOVE_DOWN_KEY 's'
#define MOVE_LEFT_KEY 'a'
#define MOVE_RIGHT_KEY 'd'

#define SHOOT_PORTAL_KEY ' '
#define TOGGLE_CURR_PORTAL_KEY 'p'

#define EMPTY_COLOR_PAIR 1
#define WALL_COLOR_PAIR 2
#define PLAYER_COLOR_PAIR 3
#define BLUE_PORTAL_COLOR_PAIR 4
#define ORANGE_PORTAL_COLOR_PAIR 5
#define MENU_SELECTED_COLOR_PAIR 6

#define MENU_ROWS 30
#define MENU_COLS 80
#define MENU_CHOICE_COLS (MENU_COLS - 8)

#define MENU_CHOICE_PLAY 0
#define MENU_CHOICE_SETTINGS 1
#define MENU_CHOICE_HELP 2
#define MENU_CHOICE_EXIT 3

typedef enum direction { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum current_portal { BLUE, ORANGE } CurrentPortal;

void print_game_title(const int ctr_row, const int ctr_col);
void print_menu_choice(const int ctr_row, const int ctr_col, const char *choice,
                       const bool selected);
void clear_menu_space(const int rows, const int cols);
int display_menu(const int rows, const int cols);
void init_grid(const int rows, const int cols, char grid[rows][cols]);
void print_grid(const int rows, const int cols, const char grid[rows][cols]);

#endif // PORTAL_H_
