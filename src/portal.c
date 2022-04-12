#include "portal.h"
#include "levels.h"
#include "portal_chars.h"

int player_row, player_col;
int look_row, look_col;
int bportal_row, bportal_col;
int oportal_row, oportal_col;
bool bportal_set, oportal_set;
Direction looking;
CurrentPortal curr_portal;

void init_level(const int level, const int rows, const int cols,
                char grid[rows][cols]) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) grid[i][j] = EMPTY;
    if (level == 0) init_level_000(rows, cols, grid);
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

void update_grid(const int rows, const int cols, char grid[rows][cols]) {
    grid[player_row][player_col] = PLAYER;
    if (bportal_set) grid[bportal_row][bportal_col] = BLUE_PORTAL;
    if (oportal_set) grid[oportal_row][oportal_col] = ORANGE_PORTAL;
    int tmp_row = player_row;
    int tmp_col = player_col;
    if (looking == UP) {
        tmp_row--;
        while (grid[tmp_row - 1][tmp_col] == EMPTY) {
            grid[tmp_row][tmp_col] = '|';
            tmp_row--;
        }
        grid[tmp_row][tmp_col] = LOOK_UP;
    } else if (looking == DOWN) {
        tmp_row++;
        while (grid[tmp_row + 1][tmp_col] == EMPTY) {
            grid[tmp_row][tmp_col] = '|';
            tmp_row++;
        }
        grid[tmp_row][tmp_col] = LOOK_DOWN;
    } else if (looking == LEFT) {
        tmp_col--;
        while (grid[tmp_row][tmp_col - 1] == EMPTY) {
            grid[tmp_row][tmp_col] = '-';
            tmp_col--;
        }
        grid[tmp_row][tmp_col] = LOOK_LEFT;
    } else if (looking == RIGHT) {
        tmp_col++;
        while (grid[tmp_row][tmp_col + 1] == EMPTY) {
            grid[tmp_row][tmp_col] = '-';
            tmp_col++;
        }
        grid[tmp_row][tmp_col] = LOOK_RIGHT;
    }
    look_row = tmp_row;
    look_col = tmp_col;
}

void play(const int rows, const int cols, char grid[rows][cols]) {
    player_row = rows / 2;
    player_col = cols / 2;
    looking = UP;
    look_row = -1;
    look_col = -1;
    bportal_row = -1;
    bportal_col = -1;
    oportal_row = -1;
    oportal_col = -1;
    bportal_set = false;
    oportal_set = false;

    bool keep_playing = true;
    do {
        update_grid(rows, cols, grid);
        print_grid(rows, cols, grid);
        switch (getch()) {
            case KEY_UP:
                looking = UP;
                break;
            case KEY_DOWN:
                looking = DOWN;
                break;
            case KEY_LEFT:
                looking = LEFT;
                break;
            case KEY_RIGHT:
                looking = RIGHT;
                break;
            case QUIT_KEY:
                keep_playing = false;
                break;
        }
    } while (keep_playing);
}
