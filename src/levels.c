#include "levels.h"
#include "portal_chars.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Level 000: Lonely
 *
 * Objective: Nothing. There is nothing to do but shoot portals at walls.
 *
 * Strategy: Die alone.
 */
Node *init_level_000(const int rows, const int cols) {
    Node *nodes;
    nodes = (Node *)malloc(rows * cols * sizeof(Node));
    if (nodes == NULL) {
        fprintf(stderr, "Error: allocating memory\n");
        exit(1);
    }
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Node tmp_node;
            tmp_node.row = i;
            tmp_node.col = j;
            if (i == 0 || j == 0 || i == rows - 1 || j == cols - 1) {
                tmp_node.type = WALL;
            } else if (i == rows / 2 && j == cols / 2) {
                tmp_node.type = PLAYER;
                tmp_node.ch = PLAYER_C;
            } else {
                tmp_node.type = EMPTY;
                tmp_node.ch = EMPTY_C;
            }
            nodes[i + (j % cols)] = tmp_node;
        }
    }
    return nodes;
}

