#ifndef LEVELS_H_
#define LEVELS_H_

typedef struct init_position {
    int row;
    int col;
} InitPosition;

InitPosition init_level_000(const int rows, const int cols, char grid[rows][cols]);
InitPosition init_level_001(const int rows, const int cols, char grid[rows][cols]);

#endif // LEVELS_H_
