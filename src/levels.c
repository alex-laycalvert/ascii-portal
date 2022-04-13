#include "levels.h"
#include "portal.h"
#include <stdio.h>
#include <stdlib.h>

/*
 * Level 000: Lonely
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
            } else if (i == rows / 2 && j == cols / 2) {
                node.type = PLAYER;
                node.ch = PLAYER_C;
            } else if (i == rows / 4 && j == cols / 2) {
                node.type = F_REFLECTOR;
                node.ch = F_REFLECTOR_C;
            } else {
                node.type = EMPTY;
                node.ch = EMPTY_C;
            }
            map[i][j] = node;
        }
    }
}

