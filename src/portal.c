#include "portal.h"
#include "levels.h"
#include "menu.h"
#include "portal_chars.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

InitPosition init_pos;
int player_row, player_col;
int look_row, look_col;
int bportal_row, bportal_col;
int oportal_row, oportal_col;
bool bportal_set, oportal_set;
bool reached_end;
Direction looking;
Direction shooting;
CurrentPortal curr_portal;

void init_level(const int level, const int rows, const int cols,
                char grid[rows][cols]) {
    for (int i = 0; i < rows; i++)
        for (int j = 0; j < cols; j++) grid[i][j] = EMPTY;
    if (level == 0) init_pos = init_level_000(rows, cols, grid);
    if (level == 1) init_pos = init_level_001(rows, cols, grid);
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
                case END:
                    attron(COLOR_PAIR(END_COLOR_PAIR));
                    mvprintw(i, j, "%c", grid[i][j]);
                    attroff(COLOR_PAIR(END_COLOR_PAIR));
                    break;
                default:
                    mvprintw(i, j, "%c", grid[i][j]);
            }
            attroff(A_BOLD);
        }
    }
}

void clean_grid(const int rows, const int cols, char grid[rows][cols]) {
    if (look_row <= 0 || look_col <= 0) return;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (grid[i][j] == PLAYER || grid[i][j] == '-' ||
                grid[i][j] == '|' || grid[i][j] == LOOK_UP ||
                grid[i][j] == LOOK_DOWN || grid[i][j] == LOOK_LEFT ||
                grid[i][j] == LOOK_RIGHT)
                grid[i][j] = EMPTY;
        }
    }
}

void recheck_aim(const int rows, const int cols, char grid[rows][cols]) {
    switch (shooting) {
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
    if (look_row <= 0 || look_col <= 0 || grid[look_row][look_col] != EMPTY)
        return;
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
    look_row = player_row;
    look_col = player_col;
    bool done = false;
    shooting = looking;
    while (!done) {
        switch (shooting) {
            case UP:
                if (grid[look_row - 1][look_col] == EMPTY) {
                    look_row--;
                    grid[look_row][look_col] = '|';
                } else if (grid[look_row - 1][look_col] == F_REFLECTOR) {
                    shooting = RIGHT;
                    look_row--;

                } else if (grid[look_row - 1][look_col] == B_REFLECTOR) {
                    shooting = LEFT;
                    look_row--;
                } else {
                    done = true;
                    if (look_row == player_row && look_col == player_col) break;
                    if (grid[look_row][look_col] == F_REFLECTOR ||
                        grid[look_row][look_col] == B_REFLECTOR)
                        break;
                    grid[look_row][look_col] = LOOK_UP;
                }
                break;
            case DOWN:
                if (grid[look_row + 1][look_col] == EMPTY) {
                    look_row++;
                    grid[look_row][look_col] = '|';
                } else if (grid[look_row + 1][look_col] == F_REFLECTOR) {
                    shooting = LEFT;
                    look_row++;

                } else if (grid[look_row + 1][look_col] == B_REFLECTOR) {
                    shooting = RIGHT;
                    look_row++;
                } else {
                    done = true;
                    if (look_row == player_row && look_col == player_col) break;
                    if (grid[look_row][look_col] == F_REFLECTOR ||
                        grid[look_row][look_col] == B_REFLECTOR)
                        break;
                    grid[look_row][look_col] = LOOK_DOWN;
                }
                break;
            case LEFT:
                if (grid[look_row][look_col - 1] == EMPTY) {
                    look_col--;
                    grid[look_row][look_col] = '-';
                } else if (grid[look_row][look_col - 1] == F_REFLECTOR) {
                    shooting = DOWN;
                    look_col--;
                } else if (grid[look_row][look_col - 1] == B_REFLECTOR) {
                    shooting = UP;
                    look_col--;
                } else {
                    done = true;
                    if (look_row == player_row && look_col == player_col) break;
                    if (grid[look_row][look_col] == F_REFLECTOR ||
                        grid[look_row][look_col] == B_REFLECTOR)
                        break;
                    grid[look_row][look_col] = LOOK_LEFT;
                }
                break;
            case RIGHT:
                if (grid[look_row][look_col + 1] == EMPTY) {
                    look_col++;
                    grid[look_row][look_col] = '-';
                } else if (grid[look_row][look_col + 1] == F_REFLECTOR) {
                    shooting = UP;
                    look_col++;
                } else if (grid[look_row][look_col + 1] == B_REFLECTOR) {
                    shooting = DOWN;
                    look_col++;
                } else {
                    done = true;
                    if (look_row == player_row && look_col == player_col) break;
                    if (grid[look_row][look_col] == F_REFLECTOR ||
                        grid[look_row][look_col] == B_REFLECTOR)
                        break;
                    grid[look_row][look_col] = LOOK_RIGHT;
                }
                break;
        }
    }
}

void move_up(const int rows, const int cols, char grid[rows][cols]) {
    if (grid[player_row - 1][player_col] == END) reached_end = true;
    if (player_row - 1 == bportal_row && player_col == bportal_col &&
        oportal_set) {
        player_row = oportal_row;
        player_col = oportal_col;
    } else if (player_row - 1 == oportal_row && player_col == oportal_col &&
               bportal_set) {
        player_row = bportal_row;
        player_col = bportal_col;
    } else {
        if (grid[player_row - 1][player_col] == EMPTY) {
            player_row--;
            return;
        }
        int empty_row = player_row;
        while (grid[empty_row - 1][player_col] != EMPTY && empty_row > 0) {
            if (grid[empty_row - 1][player_col] == END ||
                grid[empty_row - 1][player_col] == WALL)
                return;
            empty_row--;
        }
        empty_row--;
        if (empty_row <= 0) return;
        for (int i = empty_row; i < player_row - 1; i++)
            grid[i][player_col] = grid[i + 1][player_col];
        player_row--;
    }
}

void move_down(const int rows, const int cols, char grid[rows][cols]) {
    if (grid[player_row + 1][player_col] == END) reached_end = true;
    if (player_row + 1 == bportal_row && player_col == bportal_col &&
        oportal_set) {
        player_row = oportal_row;
        player_col = oportal_col;
    } else if (player_row + 1 == oportal_row && player_col == oportal_col &&
               bportal_set) {
        player_row = bportal_row;
        player_col = bportal_col;
    } else {
        if (grid[player_row + 1][player_col] == EMPTY) {
            player_row++;
            return;
        }
        int empty_row = player_row;
        while (grid[empty_row + 1][player_col] != EMPTY &&
               empty_row < rows - 1) {

            if (grid[empty_row + 1][player_col] == END ||
                grid[empty_row + 1][player_col] == WALL)
                return;
            empty_row++;
        }
        empty_row++;
        if (empty_row >= rows - 1) return;
        for (int i = empty_row; i > player_row + 1; i--)
            grid[i][player_col] = grid[i - 1][player_col];
        player_row++;
    }
}

void move_left(const int rows, const int cols, char grid[rows][cols]) {
    if (grid[player_row][player_col - 1] == END) reached_end = true;
    if (player_row == bportal_row && player_col - 1 == bportal_col &&
        oportal_set) {
        player_row = oportal_row;
        player_col = oportal_col;
    } else if (player_row == oportal_row && player_col - 1 == oportal_col &&
               bportal_set) {
        player_row = bportal_row;
        player_col = bportal_col;
    } else {
        if (grid[player_row][player_col - 1] == EMPTY) {
            player_col--;
            return;
        }
        int empty_col = player_col;
        while (grid[player_row][empty_col - 1] != EMPTY && empty_col - 1 > 0) {
            if (grid[player_row][empty_col - 1] == END ||
                grid[player_row][empty_col - 1] == WALL)
                return;
            empty_col--;
        }
        empty_col--;
        if (empty_col <= 0) return;
        for (int i = empty_col; i < player_col - 1; i++)
            grid[player_row][i] = grid[player_row][i + 1];
        player_col--;
    }
}

void move_right(const int rows, const int cols, char grid[rows][cols]) {
    if (grid[player_row][player_col + 1] == END) reached_end = true;
    if (player_row == bportal_row && player_col + 1 == bportal_col &&
        oportal_set) {
        player_row = oportal_row;
        player_col = oportal_col;
    } else if (player_row == oportal_row && player_col + 1 == oportal_col &&
               bportal_set) {
        player_row = bportal_row;
        player_col = bportal_col;
    } else {
        if (grid[player_row][player_col + 1] == EMPTY) {
            player_col++;
            return;
        }
        int empty_col = player_col;
        while (grid[player_row][empty_col + 1] != EMPTY &&
               empty_col + 1 < cols - 1) {
            if (grid[player_row][empty_col + 1] == END ||
                grid[player_row][empty_col + 1] == WALL)
                return;
            empty_col++;
        }
        empty_col++;
        if (empty_col >= cols - 1) return;
        for (int i = empty_col; i > player_col + 1; i--)
            grid[player_row][i] = grid[player_row][i - 1];
        player_col++;
    }
}

void play(const int rows, const int cols, char grid[rows][cols]) {
    player_row = init_pos.row;
    player_col = init_pos.col;
    looking = UP;
    shooting = UP;
    look_row = -1;
    look_col = -1;
    bportal_set = false;
    oportal_set = false;

    bool keep_playing = true;
    reached_end = false;
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
        if (reached_end) keep_playing = false;
    } while (keep_playing);
    printf("YOU WON!\n");
}
