#ifndef LEVELS_H_
#define LEVELS_H_

#include "portal_chars.h"

typedef struct position {
    int row;
    int col;
} Position;
typedef enum direction { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum current_portal { BLUE, ORANGE } CurrentPortal;

void init_level_000(const int rows, const int cols, char grid[rows][cols]);
void play_level_000(const int rows, const int cols, char grid[rows][cols]);

void init_level_001(const int rows, const int cols, char grid[rows][cols]);
void init_level_002(const int rows, const int cols, char grid[rows][cols]);
void init_level_003(const int rows, const int cols, char grid[rows][cols]);
void init_level_004(const int rows, const int cols, char grid[rows][cols]);
void init_level_005(const int rows, const int cols, char grid[rows][cols]);

#endif // LEVELS_H_
