#include "portal.h"
#include "levels.h"
#include "menu.h"
#include "portal_chars.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
            switch (grid[i][j]) {
                case PLAYER:
                    attron(COLOR_PAIR(PLAYER_COLOR_PAIR));
                    mvprintw(i, j, "%c", grid[i][j]);
                    attroff(COLOR_PAIR(PLAYER_COLOR_PAIR));
                    break;
                case WALL:
                    attron(COLOR_PAIR(WALL_COLOR_PAIR));
                    mvprintw(i, j, "%c", grid[i][j]);
                    attroff(COLOR_PAIR(WALL_COLOR_PAIR));
                    break;
                case BLUE_PORTAL:
                    attron(COLOR_PAIR(BPORTAL_COLOR_PAIR));
                    mvprintw(i, j, "%c", PORTAL);
                    attroff(COLOR_PAIR(BPORTAL_COLOR_PAIR));
                    break;
                case ORANGE_PORTAL:
                    attron(COLOR_PAIR(OPORTAL_COLOR_PAIR));
                    mvprintw(i, j, "%c", PORTAL);
                    attroff(COLOR_PAIR(OPORTAL_COLOR_PAIR));
                    break;
                case HOLD_BUTTON:
                    attron(COLOR_PAIR(HOLD_BUTTON_COLOR_PAIR));
                    mvprintw(i, j, "%c", grid[i][j]);
                    attroff(COLOR_PAIR(HOLD_BUTTON_COLOR_PAIR));
                    break;
                case BLOCK:
                    attron(COLOR_PAIR(BLOCK_COLOR_PAIR));
                    mvprintw(i, j, "%c", grid[i][j]);
                    attroff(COLOR_PAIR(BLOCK_COLOR_PAIR));
                    break;
                case LEVER_ON:
                    attron(COLOR_PAIR(LEVER_ON_COLOR_PAIR));
                    mvprintw(i, j, "%c", grid[i][j]);
                    attroff(COLOR_PAIR(LEVER_ON_COLOR_PAIR));
                    break;
                case LEVER_OFF:
                    attron(COLOR_PAIR(LEVER_OFF_COLOR_PAIR));
                    mvprintw(i, j, "%c", grid[i][j]);
                    attroff(COLOR_PAIR(LEVER_OFF_COLOR_PAIR));
                    break;
                case KEY:
                    attron(COLOR_PAIR(KEY_COLOR_PAIR));
                    mvprintw(i, j, "%c", grid[i][j]);
                    attroff(COLOR_PAIR(KEY_COLOR_PAIR));
                    break;
                default:
                    mvprintw(i, j, "%c", grid[i][j]);
            }
            attroff(A_BOLD);
        }
    }
}

void clean_grid(const int rows, const int cols, char grid[rows][cols]) {
    grid[player_row][player_col] = EMPTY;
    if (look_row <= 0 || look_col <= 0) return;
    if (looking == UP) {
        for (int i = look_row; i < player_row; i++) grid[i][look_col] = EMPTY;
    } else if (looking == DOWN) {
        for (int i = look_row; i > player_row; i--) grid[i][look_col] = EMPTY;
    } else if (looking == LEFT) {
        for (int i = look_col; i < player_col; i++) grid[look_row][i] = EMPTY;
    } else if (looking == RIGHT) {
        for (int i = look_col; i > player_col; i--) grid[look_row][i] = EMPTY;
    }
}

void recheck_aim(const int rows, const int cols, char grid[rows][cols]) {
    switch (looking) {
        case UP:
            if (grid[look_row - 1][look_col] == EMPTY) look_row--;
            break;
        case DOWN:
            if (grid[look_row + 1][look_col] == EMPTY) look_row++;
            break;
        case LEFT:
            if (grid[look_row][look_col - 1] == EMPTY) look_col--;
            break;
        case RIGHT:
            if (grid[look_row][look_col + 1] == EMPTY) look_col++;
            break;
    }
}

void shoot_portal(const int rows, const int cols, char grid[rows][cols]) {
    if (look_row <= 0 || look_col <= 0) return;
    if (curr_portal == BLUE) {
        if (bportal_set) {
            grid[bportal_row][bportal_col] = EMPTY;
            recheck_aim(rows, cols, grid);
        }
        bportal_row = look_row;
        bportal_col = look_col;
        bportal_set = true;
        return;
    }
    if (oportal_set) {
        grid[oportal_row][oportal_col] = EMPTY;
        recheck_aim(rows, cols, grid);
    }
    oportal_row = look_row;
    oportal_col = look_col;
    oportal_set = true;
}

void update_grid(const int rows, const int cols, char grid[rows][cols]) {
    if (bportal_set) grid[bportal_row][bportal_col] = BLUE_PORTAL;
    if (oportal_set) grid[oportal_row][oportal_col] = ORANGE_PORTAL;
    grid[player_row][player_col] = PLAYER;
    int tmp_row = player_row;
    int tmp_col = player_col;
    switch (looking) {
        case UP:
            tmp_row--;
            if (grid[tmp_row][tmp_col] != EMPTY) {
                tmp_row = -1;
                tmp_col = -1;
                break;
            }
            while (grid[tmp_row - 1][tmp_col] == EMPTY) {
                grid[tmp_row][tmp_col] = '|';
                tmp_row--;
            }
            grid[tmp_row][tmp_col] = LOOK_UP;
            break;
        case DOWN:
            tmp_row++;
            if (grid[tmp_row][tmp_col] != EMPTY) {
                tmp_row = -1;
                tmp_col = -1;
                break;
            }
            while (grid[tmp_row + 1][tmp_col] == EMPTY) {
                grid[tmp_row][tmp_col] = '|';
                tmp_row++;
            }
            grid[tmp_row][tmp_col] = LOOK_DOWN;
            break;
        case LEFT:
            tmp_col--;
            if (grid[tmp_row][tmp_col] != EMPTY) {
                tmp_row = -1;
                tmp_col = -1;
                break;
            }
            while (grid[tmp_row][tmp_col - 1] == EMPTY) {
                grid[tmp_row][tmp_col] = '-';
                tmp_col--;
            }
            grid[tmp_row][tmp_col] = LOOK_LEFT;
            break;
        case RIGHT:
            tmp_col++;
            if (grid[tmp_row][tmp_col] != EMPTY) {
                tmp_row = -1;
                tmp_col = -1;
                break;
            }
            while (grid[tmp_row][tmp_col + 1] == EMPTY) {
                grid[tmp_row][tmp_col] = '-';
                tmp_col++;
            }
            grid[tmp_row][tmp_col] = LOOK_RIGHT;
            break;
    }
    look_row = tmp_row;
    look_col = tmp_col;
}

void move_up(const int rows, const int cols, char grid[rows][cols]) {
    if (player_row - 1 == bportal_row && player_col == bportal_col &&
        oportal_set) {
        player_row = oportal_row;
        player_col = oportal_col;
    } else if (player_row - 1 == oportal_row && player_col == oportal_col &&
               bportal_set) {
        player_row = bportal_row;
        player_col = bportal_col;
    } else {
        if (grid[player_row - 1][player_col] == EMPTY) player_row--;
    }
}

void move_down(const int rows, const int cols, char grid[rows][cols]) {
    if (player_row + 1 == bportal_row && player_col == bportal_col &&
        oportal_set) {
        player_row = oportal_row;
        player_col = oportal_col;
    } else if (player_row + 1 == oportal_row && player_col == oportal_col &&
               bportal_set) {
        player_row = bportal_row;
        player_col = bportal_col;
    } else {
        if (grid[player_row + 1][player_col] == EMPTY) player_row++;
    }
}

void move_left(const int rows, const int cols, char grid[rows][cols]) {
    if (player_row == bportal_row && player_col - 1 == bportal_col &&
        oportal_set) {
        player_row = oportal_row;
        player_col = oportal_col;
    } else if (player_row == oportal_row && player_col - 1 == oportal_col &&
               bportal_set) {
        player_row = bportal_row;
        player_col = bportal_col;
    } else {
        if (grid[player_row][player_col - 1] == EMPTY) player_col--;
    }
}

void move_right(const int rows, const int cols, char grid[rows][cols]) {
    if (player_row == bportal_row && player_col + 1 == bportal_col &&
        oportal_set) {
        player_row = oportal_row;
        player_col = oportal_col;
    } else if (player_row == oportal_row && player_col + 1 == oportal_col &&
               bportal_set) {
        player_row = bportal_row;
        player_col = bportal_col;
    } else {
        if (grid[player_row][player_col + 1] == EMPTY) player_col++;
    }
}

void play(const int rows, const int cols, char grid[rows][cols]) {
    player_row = rows / 2;
    player_col = cols / 2;
    looking = UP;
    look_row = -1;
    look_col = -1;
    bportal_set = false;
    oportal_set = false;

    bool keep_playing = true;
    int menu_choice;
    do {
        update_grid(rows, cols, grid);
        print_grid(rows, cols, grid);
        int move = getch();
        clean_grid(rows, cols, grid);
        switch (move) {
            // looking directions
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
            // moving directions
            case MOVE_UP:
                move_up(rows, cols, grid);
                break;
            case MOVE_DOWN:
                move_down(rows, cols, grid);
                break;
            case MOVE_LEFT:
                move_left(rows, cols, grid);
                break;
            case MOVE_RIGHT:
                move_right(rows, cols, grid);
                break;
            // shooting portal
            case SHOOT_PORTAL:
                shoot_portal(rows, cols, grid);
                break;
            case TOGGLE_PORTAL:
                if (curr_portal == BLUE)
                    curr_portal = ORANGE;
                else if (curr_portal == ORANGE)
                    curr_portal = BLUE;
                break;
            case QUIT_KEY:
                menu_choice = display_menu(rows, cols);
                switch (menu_choice) {
                    case MENU_CHOICE_PLAY:
                        break;
                    case MENU_CHOICE_SETTINGS:
                        // TODO
                        break;
                    case MENU_CHOICE_HELP:
                        // TODO
                        break;
                    case MENU_CHOICE_EXIT:
                        keep_playing = false;
                        break;
                }
                break;
        }
    } while (keep_playing);
}
