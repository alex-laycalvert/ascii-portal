#include "portal.h"
#include "levels.h"
#include "menu.h"
#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>

Node **map;
Node *player, *bportal, *oportal, *looking_at;
int num_special_items;
Node **special_items;
Direction looking, shooting;
CurrentPortal curr_portal;
int rows, cols;
bool completed;

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
}

void init_level(const int level) {
    if (level == 0)
        init_level_000(rows, cols, map);
    else if (level == 1)
        init_level_001(rows, cols, map);
    else
        init_level_001(rows, cols, map);
    num_special_items = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            switch (map[i][j].type) {
                case PLAYER:
                    player = &map[i][j];
                    break;
                case HOLD_BUTTON:
                    num_special_items++;
                    break;
                default:
                    break;
            }
        }
    }
    bportal = NULL;
    oportal = NULL;
    looking_at = NULL;
    curr_portal = BLUE;
    looking = UP;
    shooting = UP;
    completed = false;
    if (num_special_items == 0) return;
    special_items = (Node **)malloc(num_special_items * sizeof(Node *));
    if (special_items == NULL) {
        fprintf(stderr, "Error: Failed to allocate memory\n");
        exit(1);
    }
    int in = 0;
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            switch (map[i][j].type) {
                case HOLD_BUTTON:
                    special_items[in] = &map[i][j];
                    in++;
                    break;
                default:
                    break;
            }
        }
    }
}

void clean_map() {}

void destroy_map() {
    if (special_items != NULL) free(special_items);
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) free((&map[i][j])->linked_nodes);
        free(map[i]);
    }
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
                    attron(COLOR_PAIR(BPORTAL_COLOR_PAIR));
                    mvprintw(i, j, "%c", map[i][j].ch);
                    attroff(COLOR_PAIR(BPORTAL_COLOR_PAIR));
                    break;
                case ORANGE_PORTAL:
                    attron(COLOR_PAIR(OPORTAL_COLOR_PAIR));
                    mvprintw(i, j, "%c", map[i][j].ch);
                    attroff(COLOR_PAIR(OPORTAL_COLOR_PAIR));
                    break;
                case END:
                    attron(COLOR_PAIR(END_COLOR_PAIR));
                    mvprintw(i, j, "%c", map[i][j].ch);
                    attroff(COLOR_PAIR(END_COLOR_PAIR));
                    break;
                case BLOCK:
                    attron(COLOR_PAIR(BLOCK_COLOR_PAIR));
                    mvprintw(i, j, "%c", map[i][j].ch);
                    attroff(COLOR_PAIR(BLOCK_COLOR_PAIR));
                    break;
                case TOGGLE_BLOCK:
                    attron(COLOR_PAIR(TOGGLE_BLOCK_COLOR_PAIR));
                    mvprintw(i, j, "%c", map[i][j].ch);
                    attroff(COLOR_PAIR(TOGGLE_BLOCK_COLOR_PAIR));
                    break;
                case HOLD_BUTTON:
                    attron(COLOR_PAIR(HOLD_BUTTON_COLOR_PAIR));
                    mvprintw(i, j, "%c", map[i][j].ch);
                    attroff(COLOR_PAIR(HOLD_BUTTON_COLOR_PAIR));
                    break;
                default:
                    mvprintw(i, j, "%c", map[i][j].ch);
                    break;
            }
            attroff(A_BOLD);
        }
    }
}

void update() {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            if (map[i][j].ch == VERT_BAR || map[i][j].ch == HOR_BAR ||
                map[i][j].ch == LOOK_UP || map[i][j].ch == LOOK_DOWN ||
                map[i][j].ch == LOOK_LEFT || map[i][j].ch == LOOK_RIGHT) {
                Node *n = &map[i][j];
                n->ch = EMPTY_C;
            }
        }
    }
    bool done = false;
    looking_at = player;
    shooting = looking;
    if (bportal != NULL) {
        bportal->type = BLUE_PORTAL;
        bportal->ch = PORTAL_C;
    }
    if (oportal != NULL) {
        oportal->type = ORANGE_PORTAL;
        oportal->ch = PORTAL_C;
    }
    if (player != NULL) {
        player->type = PLAYER;
        player->ch = PLAYER_C;
    }
    while (!done) {
        switch (shooting) {
            case UP:
                if (map[looking_at->row - 1][looking_at->col].type == EMPTY) {
                    looking_at = &map[looking_at->row - 1][looking_at->col];
                    looking_at->ch = VERT_BAR;
                } else if (map[looking_at->row - 1][looking_at->col].type ==
                           F_REFLECTOR) {
                    shooting = RIGHT;
                    looking_at = &map[looking_at->row - 1][looking_at->col];
                } else if (map[looking_at->row - 1][looking_at->col].type ==
                           B_REFLECTOR) {
                    shooting = LEFT;
                    looking_at = &map[looking_at->row - 1][looking_at->col];
                } else {
                    done = true;
                    if (looking_at == player ||
                        looking_at->type == F_REFLECTOR ||
                        looking_at->type == B_REFLECTOR)
                        break;
                    looking_at->ch = LOOK_UP;
                }
                break;
            case DOWN:
                if (map[looking_at->row + 1][looking_at->col].type == EMPTY) {
                    looking_at = &map[looking_at->row + 1][looking_at->col];
                    looking_at->ch = VERT_BAR;
                } else if (map[looking_at->row + 1][looking_at->col].type ==
                           F_REFLECTOR) {
                    shooting = LEFT;
                    looking_at = &map[looking_at->row + 1][looking_at->col];
                } else if (map[looking_at->row + 1][looking_at->col].type ==
                           B_REFLECTOR) {
                    shooting = RIGHT;
                    looking_at = &map[looking_at->row + 1][looking_at->col];
                } else {
                    done = true;
                    if (looking_at == player) break;
                    looking_at->ch = LOOK_DOWN;
                }
                break;
            case LEFT:
                if (map[looking_at->row][looking_at->col - 1].type == EMPTY) {
                    looking_at = &map[looking_at->row][looking_at->col - 1];
                    looking_at->ch = HOR_BAR;
                } else if (map[looking_at->row][looking_at->col - 1].type ==
                           F_REFLECTOR) {
                    shooting = DOWN;
                    looking_at = &map[looking_at->row][looking_at->col - 1];
                } else if (map[looking_at->row][looking_at->col - 1].type ==
                           B_REFLECTOR) {
                    shooting = UP;
                    looking_at = &map[looking_at->row][looking_at->col - 1];
                } else {
                    done = true;
                    if (looking_at == player) break;
                    looking_at->ch = LOOK_LEFT;
                }
                break;
            case RIGHT:
                if (map[looking_at->row][looking_at->col + 1].type == EMPTY) {
                    looking_at = &map[looking_at->row][looking_at->col + 1];
                    looking_at->ch = HOR_BAR;
                } else if (map[looking_at->row][looking_at->col + 1].type ==
                           F_REFLECTOR) {
                    shooting = UP;
                    looking_at = &map[looking_at->row][looking_at->col + 1];
                } else if (map[looking_at->row][looking_at->col + 1].type ==
                           B_REFLECTOR) {
                    shooting = DOWN;
                    looking_at = &map[looking_at->row][looking_at->col + 1];
                } else {
                    done = true;
                    if (looking_at == player) break;
                    looking_at->ch = LOOK_RIGHT;
                }
                break;
        }
    }
    for (int i = 0; i < num_special_items; i++) {
        Node *special_item = special_items[i];
        bool holding = false;
        switch (special_item->type) {
            case HOLD_BUTTON:
                if (special_item->num_linked_nodes == 0) break;
                for (int i = 0; i < 3; i++) {
                    for (int j = 0; j < 3; j++) {
                        int r = special_item->row + i - 1;
                        int c = special_item->col + j - 1;
                        if ((i + (j % 3)) % 2 == 0) continue;
                        if (r <= 0 || r >= rows - 1) break;
                        if (c <= 0 || c >= cols - 1) continue;
                        if (map[r][c].type == PLAYER ||
                            map[r][c].type == BLOCK) {
                            holding = true;
                            break;
                        }
                    }
                    if (holding) break;
                }
                for (int i = 0; i < special_item->num_linked_nodes; i++) {
                    Node *linked = special_item->linked_nodes[i];
                    if (holding) {
                        linked->type = EMPTY;
                        linked->ch = EMPTY_C;
                    } else {
                        linked->type = TOGGLE_BLOCK;
                        linked->ch = TOGGLE_BLOCK_C;
                    }
                }
                break;
            default:
                break;
        }
    }
}

bool play() {
    bool keep_playing = true;
    do {
        update();
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
            case SHOOT_PORTAL:
                shoot_portal();
                break;
            case TOGGLE_PORTAL:
                if (curr_portal == BLUE)
                    curr_portal = ORANGE;
                else if (curr_portal == ORANGE)
                    curr_portal = BLUE;
                break;
            case QUIT_KEY:
                if (display_menu(rows, cols) == MENU_CHOICE_EXIT)
                    keep_playing = false;
                break;
        }
        if (completed) keep_playing = false;
    } while (keep_playing);
    return completed;
}

void move_player(Direction dir) {
    int row_offset = 0;
    int col_offset = 0;
    if (dir == UP) row_offset = -1;
    if (dir == DOWN) row_offset = 1;
    if (dir == LEFT) col_offset = -1;
    if (dir == RIGHT) col_offset = 1;
    Node *tmp;
    switch (map[player->row + row_offset][player->col + col_offset].type) {
        case EMPTY:
            tmp = &map[player->row + row_offset][player->col + col_offset];
            tmp->type = PLAYER;
            tmp->ch = PLAYER_C;
            if (player == bportal) {
                player->type = BLUE_PORTAL;
                player->ch = PORTAL_C;
            } else if (player == oportal) {
                player->type = ORANGE_PORTAL;
                player->ch = PORTAL_C;
            } else {
                player->type = EMPTY;
                player->ch = EMPTY_C;
            }
            player = tmp;
            return;
        case END:
            completed = true;
            return;
        case BLUE_PORTAL:
            if (oportal == NULL) return;
            player->type = EMPTY;
            player->ch = EMPTY_C;
            player = oportal;
            return;
        case ORANGE_PORTAL:
            if (bportal == NULL) return;
            player->type = EMPTY;
            player->ch = EMPTY_C;
            player = bportal;
            return;
        case BLOCK:
            if (map[player->row + 2 * row_offset][player->col + 2 * col_offset]
                    .type != EMPTY)
                return;
            (&map[player->row + 2 * row_offset][player->col + 2 * col_offset])
                ->type = BLOCK;
            (&map[player->row + 2 * row_offset][player->col + 2 * col_offset])
                ->ch = BLOCK_C;
            tmp = &map[player->row + row_offset][player->col + col_offset];
            tmp->type = PLAYER;
            tmp->ch = PLAYER_C;
            if (player == bportal) {
                player->type = BLUE_PORTAL;
                player->ch = PORTAL_C;
            } else if (player == oportal) {
                player->type = ORANGE_PORTAL;
                player->ch = PORTAL_C;
            } else {
                player->type = EMPTY;
                player->ch = EMPTY_C;
            }
            player = tmp;
            return;
        default:
            return;
    }
}

void shoot_portal() {
    if (looking_at == NULL) return;
    if (curr_portal == BLUE) {
        if (bportal != NULL) {
            bportal->type = EMPTY;
            bportal->ch = EMPTY_C;
            bportal = NULL;
            update();
        }
        bportal = looking_at;
        bportal->type = BLUE_PORTAL;
        bportal->ch = PORTAL_C;
    } else {
        if (oportal != NULL) {
            oportal->type = EMPTY;
            oportal->ch = EMPTY_C;
            oportal = NULL;
            update();
        }
        oportal = looking_at;
        oportal->type = ORANGE_PORTAL;
        oportal->ch = PORTAL_C;
    }
}
