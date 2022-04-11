#include "levels.h"

/*
 * Level 000: Lonely
 *
 * Objective: Nothing. There is nothing to do but shoot portals at walls.
 *
 * Strategy: Die alone.
 */
void init_level_000(const int rows, const int cols, char grid[rows][cols]) {
    // add walls
    for (int i = 0; i < rows; i++) {
        grid[i][0] = WALL;
        grid[i][cols - 1] = WALL;
    }
    for (int i = 0; i < cols; i++) {
        grid[0][i] = WALL;
        grid[rows - 1][i] = WALL;
    }
}

void play_level_000(const int rows, const int cols, char grid[rows][cols]) {
    int player_row = rows / 2;
    int player_col = cols / 2;

    Direction look_dir = UP;
    int look_row, look_col;
}

/*
 * Level 001: TODO
 *
 * Objective: TODO
 *
 * Strategy: TODO
 */
void init_level_001(const int rows, const int cols, char grid[rows][cols]) {}

/*
 * Level 002: TODO
 *
 * Objective: TODO
 *
 * Strategy: TODO
 */
void init_level_002(const int rows, const int cols, char grid[rows][cols]) {}

/*
 * Level 003: TODO
 *
 * Objective: TODO
 *
 * Strategy: TODO
 */
void init_level_003(const int rows, const int cols, char grid[rows][cols]) {}

/*
 * Level 004: TODO
 *
 * Objective: TODO
 *
 * Strategy: TODO
 */
void init_level_004(const int rows, const int cols, char grid[rows][cols]) {}

/*
 * Level 005: TODO
 *
 * Objective: TODO
 *
 * Strategy: TODO
 */
void init_level_005(const int rows, const int cols, char grid[rows][cols]) {}
