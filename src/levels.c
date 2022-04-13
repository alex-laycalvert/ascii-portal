#include "levels.h"
#include "portal.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Level 000: Lonely (Testing Purposes)
 *
 * Objective: Nothing. There is nothing to do but shoot portals at walls.
 *
 * Strategy: Die alone.
 */
void init_level_000(const int rows, const int cols, Node **map) {
    for (int i = 0; i < rows; i++) {
        for (int j = 0; j < cols; j++) {
            Node node;
            node.row = i;
            node.col = j;
            if (i == 0 || j == 0 || i == rows - 1 || j == cols - 1) {
                node.type = WALL;
                node.ch = WALL_C;
            } else {
                node.type = EMPTY;
                node.ch = EMPTY_C;
            }
            node.num_linked_nodes = 0;
            node.linked_nodes = NULL;
            map[i][j] = node;
        }
    }
    // set player
    (&map[rows / 2][cols / 2])->type = PLAYER;
    (&map[rows / 2][cols / 2])->ch = PLAYER_C;

    // set reflector
    (&map[rows / 4][cols / 2])->type = F_REFLECTOR;
    (&map[rows / 4][cols / 2])->ch = F_REFLECTOR_C;
    (&map[rows / 4][3 * (cols / 4)])->type = B_REFLECTOR;
    (&map[rows / 4][3 * (cols / 4)])->ch = B_REFLECTOR_C;

    // set block
    (&map[rows / 3][cols / 2])->type = BLOCK;
    (&map[rows / 3][cols / 2])->ch = BLOCK_C;

    (&map[(rows / 2) - 4][cols / 5])->type = TOGGLE_BLOCK;
    (&map[(rows / 2) - 4][cols / 5])->ch = TOGGLE_BLOCK_C;
    (&map[(rows / 2) - 3][cols / 5])->type = TOGGLE_BLOCK;
    (&map[(rows / 2) - 3][cols / 5])->ch = TOGGLE_BLOCK_C;
    (&map[(rows / 2) - 2][cols / 5])->type = TOGGLE_BLOCK;
    (&map[(rows / 2) - 2][cols / 5])->ch = TOGGLE_BLOCK_C;
    (&map[(rows / 2) - 1][cols / 5])->type = TOGGLE_BLOCK;
    (&map[(rows / 2) - 1][cols / 5])->ch = TOGGLE_BLOCK_C;
    (&map[(rows / 2)][cols / 5])->type = TOGGLE_BLOCK;
    (&map[(rows / 2)][cols / 5])->ch = TOGGLE_BLOCK_C;

    // set hold button
    Node *hold = &map[rows / 2][cols / 4];
    hold->type = HOLD_BUTTON;
    hold->ch = HOLD_BUTTON_C;
    hold->num_linked_nodes = 5;

    hold->linked_nodes = (Node **)malloc(5 * sizeof(Node *));
    for (int i = 0; i < hold->num_linked_nodes; i++) {
        hold->linked_nodes[i] = &map[(rows / 2) - i][cols / 5];
    }

    // set end
    (&map[rows - 5][cols - 5])->type = END;
    (&map[rows - 5][cols - 5])->ch = END_C;
}

