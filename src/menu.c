#include "menu.h"
#include "portal.h"

void print_game_title(const int ctr_row, const int ctr_col) {
    char *title[7];
    title[0] = "                _ _                    _        _ ";
    title[1] = "  __ _ ___  ___(_|_)  _ __   ___  _ __| |_ __ _| |";
    title[2] = " / _` / __|/ __| | | | '_ \\ / _ \\| '__| __/ _` | |";
    title[3] = "| (_| \\__ \\ (__| | | | |_) | (_) | |  | || (_| | |";
    title[4] = " \\__,_|___/\\___|_|_| | .__/ \\___/|_|   \\__\\__,_|_|";
    title[5] = "                     |_|                          ";
    title[6] = "                               by: alex-laycalvert";
    for (int i = 0; i < 7; i++) {
        mvprintw(ctr_row - 3 + i, ctr_col - ((int)strlen(title[i]) / 2), "%s",
                 title[i]);
    }
}

void print_menu_choice(const int ctr_row, const int ctr_col, const char *choice,
                       const bool selected) {
    if (selected) attron(COLOR_PAIR(MENU_SELECTED_COLOR_PAIR));
    for (int i = ctr_col - (MENU_CHOICE_COLS / 2);
         i < ctr_col - ((int)strlen(choice) / 2); i++) {
        mvprintw(ctr_row, i, " ");
    }
    mvprintw(ctr_row, ctr_col - ((int)strlen(choice) / 2), "%s", choice);
    for (int i = ctr_col + ((int)strlen(choice) / 2) + 1;
         i < ctr_col + (MENU_CHOICE_COLS / 2); i++) {
        mvprintw(ctr_row, i, " ");
    }
    if (selected) attroff(COLOR_PAIR(MENU_SELECTED_COLOR_PAIR));
}

void clear_menu_space(const int rows, const int cols) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            mvprintw(i, j, " ");
        }
    }
}

// TODO
// handle settings and help menu's within this menu method, don't return
int display_menu(const int rows, const int cols) {
    bool selected = false;
    int choice_index = 0;
    do {
        clear_menu_space(rows, cols);
        // menu borders
        attron(COLOR_PAIR(MENU_BORDER_COLOR_PAIR));
        for (int i = (rows / 2) - (MENU_ROWS / 2);
             i < (rows / 2) + (MENU_ROWS / 2); i++) {
            mvprintw(i, (cols / 2) - (MENU_COLS / 2), "%c", MENU_BORDER);
            mvprintw(i, (cols / 2) + (MENU_COLS / 2) - 1, "%c", MENU_BORDER);
        }
        for (int i = (cols / 2) - (MENU_COLS / 2);
             i < (cols / 2) + (MENU_COLS / 2); i++) {
            mvprintw((rows / 2) - (MENU_ROWS / 2), i, "%c", MENU_BORDER);
            mvprintw((rows / 2) + (MENU_ROWS / 2) - 1, i, "%c", MENU_BORDER);
        }
        attroff(COLOR_PAIR(MENU_BORDER_COLOR_PAIR));
        print_game_title((rows / 2) - (MENU_ROWS / 2) + (MENU_ROWS / 6),
                         cols / 2);
        for (int i = 0; i < MENU_CHOICE_COLS; i++) {
            mvprintw((rows / 2) - (MENU_ROWS / 6),
                     (cols / 2) - (MENU_CHOICE_COLS / 2) + i, "~");
        }
        char *options[4];
        options[0] = "P L A Y";
        options[1] = "S E T T I N G S";
        options[2] = "H E L P";
        options[3] = "E X I T";
        for (int i = 0; i < 4; i++) {
            if (i == choice_index) {
                print_menu_choice((rows / 2) - 2 + (i * 3), (cols / 2),
                                  options[i], true);
                continue;
            }
            print_menu_choice((rows / 2) - 2 + (i * 3), (cols / 2), options[i],
                              false);
        }
        switch (getch()) {
            case KEY_UP:
                choice_index--;
                if (choice_index < 0) choice_index = 0;
                break;
            case KEY_DOWN:
                choice_index++;
                if (choice_index > 3) choice_index = 3;
                break;
            case MENU_CHOOSE:
                selected = true;
                break;
            case 'q':
                return MENU_CHOICE_EXIT;
            default:
                break;
        }
    } while (!selected);
    return choice_index;
}
