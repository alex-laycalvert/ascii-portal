#ifndef PORTAL_H_
#define PORTAL_H_

#include "levels.h"
#include "portal_chars.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define QUIT_KEY 'q'

#define LOOK_UP_KEY (char)KEY_UP
#define LOOK_DOWN_KEY (char)KEY_DOWN
#define LOOK_LEFT_KEY (char)KEY_LEFT
#define LOOK_RIGHT_KEY (char)KEY_RIGHT

#define MOVE_UP_KEY 'w'
#define MOVE_DOWN_KEY 's'
#define MOVE_LEFT_KEY 'a'
#define MOVE_RIGHT_KEY 'd'

#define SHOOT_PORTAL_KEY ' '
#define TOGGLE_CURR_PORTAL_KEY '\t'

#define EMPTY_COLOR_PAIR 1
#define WALL_COLOR_PAIR 2
#define PLAYER_COLOR_PAIR 3
#define BLUE_PORTAL_COLOR_PAIR 4
#define ORANGE_PORTAL_COLOR_PAIR 5

typedef enum direction { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum current_portal { BLUE, ORANGE } CurrentPortal;

void init_grid(const int level, const int rows, const int cols,
               char grid[rows][cols]);
void print_grid(const int rows, const int cols, const char grid[rows][cols]);

#endif // PORTAL_H_
