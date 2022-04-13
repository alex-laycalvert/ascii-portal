#include "portal.h"
#include "levels.h"
#include "menu.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

Node **map;
Node *player;
int rows, cols;

void init_map(const int t_rows, const int t_cols) {
    rows = t_rows;
    cols = t_cols;
    map = (Node **)malloc(rows * sizeof(Node *));
    if (map == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        map[i] = (Node *)malloc(cols * sizeof(Node));
        if (map[i] == NULL) {
            fprintf(stderr, "Error: Failed to allocate memory\n");
            exit(1);
        }
    }
    init_level_000(rows, cols, map);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j].type == PLAYER) player = &map[i][j];
        }
    }
}

void destroy_map() {
    for (int i = 0; i < rows; i++) free(map[i]);
    free(map);
}

void print_map() {
    if (map == NULL) {
        fprintf(stderr, "Error: Map is not initialized\n");
        return;
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            attron(A_BOLD);
            switch (map[i][j].type) {
                case PLAYER:
                    attron(COLOR_PAIR(PLAYER_COLOR_PAIR));
                    mvprintw(i, j, "%c", map[i][j].ch);
                    attroff(COLOR_PAIR(PLAYER_COLOR_PAIR));
                    break;
                case WALL:
                    attron(COLOR_PAIR(WALL_COLOR_PAIR));
                    mvprintw(i, j, "%c", map[i][j].ch);
                    attroff(COLOR_PAIR(WALL_COLOR_PAIR));
                    break;
                case BLUE_PORTAL:
                    attron(COLOR_PAIR(BLUE_PORTAL));
                    mvprintw(i, j, "%c", map[i][j].ch);
                    attroff(COLOR_PAIR(BLUE_PORTAL));
                    break;
                case ORANGE_PORTAL:
                    attron(COLOR_PAIR(ORANGE_PORTAL));
                    mvprintw(i, j, "%c", map[i][j].ch);
                    attroff(COLOR_PAIR(ORANGE_PORTAL));
                    break;
                case END:
                    break;
                default:
                    mvprintw(i, j, "%c", map[i][j].ch);
                    break;
            }
            attroff(A_BOLD);
        }
    }
}

void update() {}

void play() {
    bool keep_playing = true;
    do {
        print_map();
        switch (getch()) {
            case MOVE_UP:
                move_player(UP);
                break;
            case MOVE_DOWN:
                move_player(DOWN);
                break;
            case MOVE_LEFT:
                move_player(LEFT);
                break;
            case MOVE_RIGHT:
                move_player(RIGHT);
                break;
            case QUIT_KEY:
                if (display_menu(rows, cols) == MENU_CHOICE_EXIT)
                    keep_playing = false;
                break;
        }
        update();
    } while (keep_playing);
}

void move_player(Direction dir) {
    int row_offset = 0;
    int col_offset = 0;
    if (dir == UP) row_offset = -1;
    if (dir == DOWN) row_offset = 1;
    if (dir == LEFT) col_offset = -1;
    if (dir == RIGHT) col_offset = 1;
    if (map[player->row + row_offset][player->col + col_offset].type != EMPTY)
        return;
    Node *tmp = &map[player->row + row_offset][player->col + col_offset];
    tmp->ch = PLAYER_C;
    tmp->type = PLAYER;
    player->ch = EMPTY_C;
    player->type = EMPTY;
    player = tmp;
}
