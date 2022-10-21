// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#ifndef NODE_HPP_
#define NODE_HPP_

#include <ncurses.h>

#define EMPTY_CH ' '
#define PLAYER_CH 'X'
#define BLUE_PORTAL_CH 'B'
#define ORANGE_PORTAL_CH 'O'
#define BLOCK_CH 'K'
#define WALL_CH 'I'

#define DEFAULT_DIRECTION UP

enum NODE_TYPE {
    EMPTY,
    PLAYER,
    BLUE_PORTAL,
    ORANGE_PORTAL,
    BLOCK,
    WALL,
};

enum NODE_COLOR_PAIR {
    EMPTY_CP = 100,
    PLAYER_CP,
    BLUE_PORTAL_CP,
    ORANGE_PORTAL_CP,
    BLOCK_CP,
    WALL_CP,
};

enum DIRECTION {
    UP,
    DOWN,
    LEFT,
    RIGHT,
};

class Node {
   public:
    Node(const int row, const int col, const NODE_TYPE type);
    static NODE_TYPE getTypeFromChar(const char ch);
    int row;
    int col;
    DIRECTION dir;
    NODE_TYPE getType() { return type; };
    void setType(NODE_TYPE type);
    void print(const int rowOffset, const int colOffset);

   private:
    NODE_TYPE type;
    NODE_COLOR_PAIR colorPair;
    char ch;
};

#endif // NODE_HPP_
