#include "menu.h"
#include "portal.h"

void setup();
void run();
void exit_game(int code);

int main(int argc, char **argv) {
    (void)argc, (void)argv;
    setup();
    run();
    exit_game(EXIT_SUCCESS);
}

void setup() {
    // ncurses initialization
    initscr();
    noecho();
    raw();
    keypad(stdscr, TRUE);
    curs_set(0);

    // damn that sucks
    if (!has_colors()) {
        endwin();
        fprintf(stderr, "Your terminal does not support colors\n");
        exit(1);
    }

    // initializing some colors
    start_color();
    use_default_colors();
    init_pair(EMPTY_COLOR_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(WALL_COLOR_PAIR, COLOR_WHITE, COLOR_WHITE);
    init_pair(PLAYER_COLOR_PAIR, COLOR_GREEN, COLOR_GREEN);
    init_pair(BLUE_PORTAL_COLOR_PAIR, COLOR_BLUE, COLOR_BLUE);
    init_pair(ORANGE_PORTAL_COLOR_PAIR, COLOR_RED, COLOR_RED);
    init_pair(MENU_SELECTED_COLOR_PAIR, COLOR_BLACK, COLOR_WHITE);
    init_pair(MENU_BORDER_COLOR_PAIR, COLOR_WHITE, COLOR_WHITE);
}

void run() {
    // getting terminal size info
    int rows, cols;
    getmaxyx(stdscr, rows, cols);

    char grid[rows][cols];
    int level = 0; // TODO let player choose level
    init_grid(level, rows, cols, grid);

    Direction look_dir = UP;
    int player_row = rows / 2;
    int player_col = cols / 2;
    int look_row, look_col;

    CurrentPortal curr_portal = BLUE;
    int blue_portal_row = -1;
    int blue_portal_col = -1;
    int orange_portal_row = -1;
    int orange_portal_col = -1;
    bool blue_portal_set = false;
    bool orange_portal_set = false;

    int player_choice = display_menu(rows, cols);
    switch (player_choice) {
        case MENU_CHOICE_PLAY:
            break;
        case MENU_CHOICE_SETTINGS:
            // TODO
            break;
        case MENU_CHOICE_HELP:
            // TODO
            break;
        case MENU_CHOICE_EXIT:
            exit_game(EXIT_SUCCESS);
            break;
        default:
            break;
    }

    /* TODO */
    // refactor
    // add features/levels
    // pretty much everything

    // the game loop
    char move;
    do {
        int tmp_row = -1;
        int tmp_col = -1;
        look_row = -1;
        look_col = -1;
        switch (look_dir) {
            case UP:
                tmp_row = player_row - 1;
                tmp_col = player_col;
                if (tmp_row - 1 < 0) break;
                while (grid[tmp_row - 1][tmp_col] == EMPTY) {
                    grid[tmp_row][tmp_col] = '|';
                    tmp_row--;
                }
                grid[tmp_row][tmp_col] = LOOK_UP;
                look_row = tmp_row;
                look_col = tmp_col;
                break;
            case DOWN:
                tmp_row = player_row + 1;
                tmp_col = player_col;
                if (tmp_row + 1 > rows - 1) break;
                while (grid[tmp_row + 1][tmp_col] == EMPTY) {
                    grid[tmp_row][tmp_col] = '|';
                    tmp_row++;
                }
                grid[tmp_row][tmp_col] = LOOK_DOWN;
                look_row = tmp_row;
                look_col = tmp_col;
                break;
            case LEFT:
                tmp_row = player_row;
                tmp_col = player_col - 1;
                if (tmp_col - 1 < 0) break;
                while (grid[tmp_row][tmp_col - 1] == EMPTY) {
                    grid[tmp_row][tmp_col] = '-';
                    tmp_col--;
                }
                grid[tmp_row][tmp_col] = LOOK_LEFT;
                look_row = tmp_row;
                look_col = tmp_col;
                break;
            case RIGHT:
                tmp_row = player_row;
                tmp_col = player_col + 1;
                if (tmp_col + 1 > cols - 1) break;
                while (grid[tmp_row][tmp_col + 1] == EMPTY) {
                    grid[tmp_row][tmp_col] = '-';
                    tmp_col++;
                }
                grid[tmp_row][tmp_col] = LOOK_RIGHT;
                look_row = tmp_row;
                look_col = tmp_col;
                break;
            default:
                break;
        }
        if (blue_portal_set)
            grid[blue_portal_row][blue_portal_col] = BLUE_PORTAL;
        if (orange_portal_set)
            grid[orange_portal_row][orange_portal_col] = ORANGE_PORTAL;
        grid[player_row][player_col] = PLAYER;
        print_grid(rows, cols, grid);
        move(0, 0);
        move = getch();
        grid[player_row][player_col] = EMPTY;
        switch (look_dir) {
            case UP:
                tmp_row = player_row - 1;
                tmp_col = player_col;
                if (tmp_row - 1 < look_row) break;
                while (tmp_row > 0) {
                    grid[tmp_row][tmp_col] = EMPTY;
                    tmp_row--;
                }
                break;
            case DOWN:
                tmp_row = player_row + 1;
                tmp_col = player_col;
                if (tmp_row + 1 > look_row) break;
                while (tmp_row < rows - 1) {
                    grid[tmp_row][tmp_col] = EMPTY;
                    tmp_row++;
                }
                break;
            case LEFT:
                tmp_row = player_row;
                tmp_col = player_col - 1;
                if (tmp_col - 1 < look_col) break;
                while (tmp_col > 0) {
                    grid[tmp_row][tmp_col] = EMPTY;
                    tmp_col--;
                }
                break;
            case RIGHT:
                tmp_row = player_row;
                tmp_col = player_col + 1;
                if (tmp_col + 1 > look_col) break;
                while (tmp_col < cols - 1) {
                    grid[tmp_row][tmp_col] = EMPTY;
                    tmp_col++;
                }
                break;
            default:
                break;
        }

        switch (move) {
            case LOOK_UP_KEY:
                look_dir = UP;
                break;
            case LOOK_DOWN_KEY:
                look_dir = DOWN;
                break;
            case LOOK_LEFT_KEY:
                look_dir = LEFT;
                break;
            case LOOK_RIGHT_KEY:
                look_dir = RIGHT;
                break;
            case MOVE_UP_KEY:
                if (player_row - 1 > 0) player_row--;
                break;
            case MOVE_DOWN_KEY:
                if (player_row + 1 < rows - 1) player_row++;
                break;
            case MOVE_LEFT_KEY:
                if (player_col - 1 > 0) player_col--;
                break;
            case MOVE_RIGHT_KEY:
                if (player_col + 1 < cols - 1) player_col++;
                break;
            case TOGGLE_CURR_PORTAL_KEY:
                if (curr_portal == BLUE)
                    curr_portal = ORANGE;
                else
                    curr_portal = BLUE;
                break;
            case SHOOT_PORTAL_KEY:
                if (blue_portal_row > 0 && blue_portal_col > 0 &&
                    curr_portal == BLUE)
                    grid[blue_portal_row][blue_portal_col] = EMPTY;
                if (orange_portal_row > 0 && orange_portal_col > 0 &&
                    curr_portal == ORANGE)
                    grid[orange_portal_row][orange_portal_col] = EMPTY;
                int tmp_portal_row = -1;
                int tmp_portal_col = -1;
                switch (look_dir) {
                    case UP:
                        if (player_row - 1 == 0) break;
                        tmp_portal_row = player_row - 1;
                        tmp_portal_col = player_col;
                        while (grid[tmp_portal_row - 1][tmp_portal_col] ==
                               EMPTY)
                            tmp_portal_row--;
                        break;
                    case DOWN:
                        if (player_row + 1 == rows - 1) break;
                        tmp_portal_row = player_row + 1;
                        tmp_portal_col = player_col;
                        while (grid[tmp_portal_row + 1][tmp_portal_col] ==
                               EMPTY)
                            tmp_portal_row++;
                        break;
                    case LEFT:
                        if (player_col - 1 == 0) break;
                        tmp_portal_col = player_col - 1;
                        tmp_portal_row = player_row;
                        while (grid[tmp_portal_row][tmp_portal_col - 1] ==
                               EMPTY)
                            tmp_portal_col--;
                        break;
                    case RIGHT:
                        if (player_col + 1 == cols - 1) break;
                        tmp_portal_col = player_col + 1;
                        tmp_portal_row = player_row;
                        while (grid[tmp_portal_row][tmp_portal_col + 1] ==
                               EMPTY)
                            tmp_portal_col++;
                        break;
                    default:
                        break;
                }
                if (tmp_portal_row < 0 || tmp_portal_col < 0) break;
                if (curr_portal == BLUE) {
                    blue_portal_row = tmp_portal_row;
                    blue_portal_col = tmp_portal_col;
                    blue_portal_set = true;
                } else {
                    orange_portal_row = tmp_portal_row;
                    orange_portal_col = tmp_portal_col;
                    orange_portal_set = true;
                }
                break;
        }
        if (player_row == blue_portal_row && player_col == blue_portal_col) {
            if (orange_portal_set) {
                player_row = orange_portal_row;
                player_col = orange_portal_col;
            }
        } else if (player_row == orange_portal_row &&
                   player_col == orange_portal_col) {
            if (blue_portal_set) {
                player_row = blue_portal_row;
                player_col = blue_portal_col;
            }
        }
    } while (move != QUIT_KEY);
}

void exit_game(int code) {
    endwin();
    exit(code);
}

