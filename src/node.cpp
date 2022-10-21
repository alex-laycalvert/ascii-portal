// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#include "node.hpp"

Node::Node(const int row, const int col, const NODE_TYPE type) {
    this->row = row;
    this->col = col;
    this->dir = DEFAULT_DIRECTION;
    setType(type);
}

NODE_TYPE Node::getTypeFromChar(const char ch) {
    NODE_TYPE type = EMPTY;
    switch (ch) {
        case EMPTY_CH:
            type = EMPTY;
            break;
        case PLAYER_CH:
            type = PLAYER;
            break;
        case BLUE_PORTAL_CH:
            type = BLUE_PORTAL;
            break;
        case ORANGE_PORTAL_CH:
            type = ORANGE_PORTAL;
            break;
        case BLOCK_CH:
            type = BLOCK;
            break;
        case WALL_CH:
            type = WALL;
            break;
        default:
            break;
    }
    return type;
}

void Node::print(const int rowOffset, const int colOffset) {
    attron(COLOR_PAIR(colorPair));
    mvprintw(row + rowOffset, col + colOffset, "%c", ch);
    attroff(COLOR_PAIR(colorPair));
}

void Node::setType(NODE_TYPE type) {
    this->type = type;
    switch (type) {
        case EMPTY:
            ch = EMPTY_CH;
            colorPair = EMPTY_CP;
            break;
        case PLAYER:
            ch = PLAYER_CH;
            colorPair = PLAYER_CP;
            break;
        case BLUE_PORTAL:
            ch = BLUE_PORTAL_CH;
            colorPair = BLUE_PORTAL_CP;
            break;
        case ORANGE_PORTAL:
            ch = ORANGE_PORTAL_CH;
            colorPair = ORANGE_PORTAL_CP;
            break;
        case BLOCK:
            ch = BLOCK_CH;
            colorPair = BLOCK_CP;
            break;
        case WALL:
            ch = WALL_CH;
            colorPair = WALL_CP;
            break;
        default:
            break;
    }
}
