// Copyright 2021 Alexander Lay-Calvert
#include "portal.h"

void init_grid(const int rows, const int cols, char grid[rows][cols]) {
    // empty everything before adding chars
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) grid[i][j] = EMPTY;

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
