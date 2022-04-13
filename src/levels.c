#include "levels.h"
#include "portal.h"
#include <stdio.h>
#include <stdlib.h>

void init_level_walls(const int rows, const int cols, Node **map) {
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
}

/*
 * Level 000: Lonely (Testing Purposes)
 *
 * Objective: Nothing. There is nothing to do but shoot portals at walls.
 *
 * Strategy: Die alone.
 */
void init_level_000(const int rows, const int cols, Node **map) {
    init_level_walls(rows, cols, map);

    // set player
    (&map[rows / 2][cols / 2])->type = PLAYER;
    (&map[rows / 2][cols / 2])->ch = PLAYER_C;

    // set end
    (&map[rows - 5][cols - 5])->type = END;
    (&map[rows - 5][cols - 5])->ch = END_C;
}

/*
 * Level 001: The Basics
 *
 * Objective: Reach the end block.
 *
 * Strategy: Walking works, but it's just so far away...
 */
void init_level_001(const int rows, const int cols, Node **map) {
    init_level_walls(rows, cols, map);

    // setting player in top left
    Node *player = &map[10][10];
    player->type = PLAYER;
    player->ch = PLAYER_C;

    // seting end in bottom right;
    Node *end = &map[rows - 3][cols - 3];
    end->type = END;
    end->ch = END_C;
}
