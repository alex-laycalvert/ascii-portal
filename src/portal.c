#include "portal.h"

void init_grid(const int level, const int rows, const int cols,
               char grid[rows][cols]) {
    // empty everything before adding chars
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) grid[i][j] = EMPTY;
    if (level == 0) init_level_000(rows, cols, grid);
    if (level == 2) init_level_000(rows, cols, grid); // TODO
    if (level == 3) init_level_000(rows, cols, grid); // TODO
    if (level == 4) init_level_000(rows, cols, grid); // TODO
    if (level == 5) init_level_000(rows, cols, grid); // TODO
}

void print_grid(const int rows, const int cols, const char grid[rows][cols]) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            attron(A_BOLD);
            if (grid[i][j] == PLAYER) {
                attron(COLOR_PAIR(PLAYER_COLOR_PAIR));
                mvprintw(i, j, "%c", grid[i][j]);
                attroff(COLOR_PAIR(PLAYER_COLOR_PAIR));
            } else if (grid[i][j] == WALL) {
                attron(COLOR_PAIR(WALL_COLOR_PAIR));
                mvprintw(i, j, "%c", grid[i][j]);
                attroff(COLOR_PAIR(WALL_COLOR_PAIR));
            } else if (grid[i][j] == BLUE_PORTAL) {
                attron(COLOR_PAIR(BLUE_PORTAL_COLOR_PAIR));
                mvprintw(i, j, "%c", PORTAL);
                attroff(COLOR_PAIR(BLUE_PORTAL_COLOR_PAIR));
            } else if (grid[i][j] == ORANGE_PORTAL) {
                attron(COLOR_PAIR(ORANGE_PORTAL_COLOR_PAIR));
                mvprintw(i, j, "%c", PORTAL);
                attroff(COLOR_PAIR(ORANGE_PORTAL_COLOR_PAIR));
            } else {
                mvprintw(i, j, "%c", grid[i][j]);
            }
            attroff(A_BOLD);
        }
    }
}
