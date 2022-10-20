// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#ifndef NODE_HPP_
#define NODE_HPP_

#define EMPTY_CH ' '
#define PLAYER_CH 'X'
#define BLUE_PORTAL_CH 'B'
#define ORANGE_PORTAL_CH 'O'
#define BLOCK_CH 'K'

enum NODE_TYPE {
    EMPTY,
    PLAYER,
    BLUE_PORTAL,
    ORANGE_PORTAL,
    BLOCK,
};

class Node {
    private:
        int row;
        int column;
        NODE_TYPE type;
        char ch;
    public:
        Node(const int row, const int column, const NODE_TYPE type, const char ch);
};

#endif // NODE_HPP_
