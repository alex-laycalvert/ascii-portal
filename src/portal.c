#include "portal.h"
#include "levels.h"
#include "menu.h"
#include "portal_chars.h"

#include <ncurses.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

Node *nodes;
int rows, cols;
Direction looking, shooting;
CurrentPortal curr_portal;

void init_level(const int level, const int num_rows, const int num_cols) {
    rows = num_rows;
    cols = num_cols;
    nodes = init_level_000(rows, cols);
}

void print_map(const int num_nodes, Node *map) {
    printf("NUM: %d", num_nodes);
    exit(1);
    for (int i = 0; i < num_nodes; i++)
        mvprintw(map[i].row, map[i].col, "%c", map[i].ch);
}

void play() {
    print_map(rows * cols, nodes);
    getch();
}

