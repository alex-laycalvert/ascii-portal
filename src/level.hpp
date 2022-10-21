// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <vector>

#include "node.hpp"

#define ASSIGNMENT_CH '='
#define NAME "NAME"
#define DESC "DESC"
#define AUTHOR "AUTHOR"

using std::string;
using std::vector;

class Level {
   public:
    Level(string filename);
    int getPlayerRow() { return initPlayerRow; };
    int getPlayerCol() { return initPlayerCol; };
    string getName() { return name; }
    string getDescription() { return description; }
    string getAuthor() { return author; }
    vector<vector<Node *>> getBoard() { return board; }

   private:
    string name;
    string author;
    string description;
    vector<vector<Node *>> board;
    int initPlayerRow;
    int initPlayerCol;
    void setDefaults();
    void parseFile(string filename);
    void parseStringBoard(vector<string> stringBoard);
};

#endif // LEVEL_HPP
