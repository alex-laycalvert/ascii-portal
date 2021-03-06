#ifndef PORTAL_H_
#define PORTAL_H_

#include <stdbool.h>

#define TROW_OFFSET 3
#define TCOL_OFFSET 0

#define MENU_SELECTED_COLOR_PAIR 1
#define MENU_BORDER_COLOR_PAIR 2
#define EMPTY_COLOR_PAIR 3
#define WALL_COLOR_PAIR 4
#define PLAYER_COLOR_PAIR 5
#define BPORTAL_COLOR_PAIR 6
#define OPORTAL_COLOR_PAIR 7
#define HOLD_BUTTON_COLOR_PAIR 8
#define BLOCK_COLOR_PAIR 9
#define LEVER_ON_COLOR_PAIR 10
#define LEVER_OFF_COLOR_PAIR 11
#define KEY_COLOR_PAIR 12
#define END_COLOR_PAIR 13
#define BLOCK_SPAWN_COLOR_PAIR 14
#define BLOCK_LANDING_COLOR_PAIR 15
#define TOGGLE_BLOCK_COLOR_PAIR 16

#define WALL_C 'I'
#define PLAYER_C 'X'
#define EMPTY_C ' '
#define BLUE_PORTAL_C 'B'
#define ORANGE_PORTAL_C 'R'
#define PORTAL_C 'O'
#define HOLD_BUTTON_C 'H'
#define BLOCK_C 'C'
#define LEVER_ON_C 'L'  // TODO
#define LEVER_OFF_C 'l' // TODO
#define KEY_C 'K'       // TODO
#define END_C 'E'
#define F_REFLECTOR_C '/'
#define B_REFLECTOR_C '\\'
#define BLOCK_SPAWN_C 'S'   // TODO
#define BLOCK_LANDING_C 'X' // TODO
#define TOGGLE_BLOCK_C 'T'

#define LOOK_UP '^'
#define LOOK_DOWN 'v'
#define LOOK_LEFT '<'
#define LOOK_RIGHT '>'
#define VERT_BAR '|'
#define HOR_BAR '-'

#define QUIT_KEY 'q'
#define MOVE_UP 'w'
#define MOVE_DOWN 's'
#define MOVE_LEFT 'a'
#define MOVE_RIGHT 'd'
#define SHOOT_PORTAL ' '
#define TOGGLE_PORTAL '\t'

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
    B_REFLECTOR,
    BLOCK_SPAWN,
    BLOCK_LANDING,
    TOGGLE_BLOCK,
} NodeType;
typedef struct node {
    int row;
    int col;
    char ch;
    NodeType type;
    int num_linked_nodes;
    struct node **linked_nodes;
} Node;
typedef enum direction { UP, DOWN, LEFT, RIGHT } Direction;
typedef enum current_portal { BLUE, ORANGE } CurrentPortal;

void init_map(const int rows, const int cols);
void init_level(const int level);
void clean_map();
void destroy_map();
void print_status_bar();
void print_map();
void update();
bool play();
void move_player(Direction dir);
void shoot_portal();

#endif // PORTAL_H_
