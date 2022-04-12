#ifndef PORTAL_H_
#define PORTAL_H_

#include <stdbool.h>

typedef enum node_type {
    PLAYER,
    BLUE_PORTAL,
    ORANGE_PORTAL,
    WALL,
    EMPTY,
    END,
    BLOCK,
    HOLD_BUTTON,
    LEVER_ON,
    LEVER_OFF,
    KEY,
    F_REFLECTOR,
    B_REFLECTOR
} NodeType;
typedef struct node {
    int row;
    int col;
    char ch;
    NodeType type;
} Node;
typedef enum direction { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum current_portal { BLUE, ORANGE } CurrentPortal;

void init_level(const int level, const int rows, const int cols);
void print_map(const int num_nodes, Node *map);
void play();

#endif // PORTAL_H_
