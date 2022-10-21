// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#include "level.hpp"

#include <fstream>
#include <iostream>

using std::ifstream;

Level::Level(string filename) {
    setDefaults();
    parseFile(filename);
}

void Level::setDefaults() {
    name = "NO NAME";
    description = "NO DESCRIPTION";
    author = "UNKNOWN";
}

void Level::parseFile(string filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Error: failed to open " << filename << "\n";
        exit(1);
    }
    string line;
    vector<string> stringBoard;
    while (getline(file, line)) {
        size_t delimPos = line.find(ASSIGNMENT_CH);
        if (delimPos != line.npos) {
            string key = line.substr(0, delimPos);
            string value = line.substr(delimPos + 1, line.npos);
            if (key == NAME) {
                name = value;
            } else if (key == DESC) {
                description = value;
            } else if (key == AUTHOR) {
                author = value;
            }
        } else {
            stringBoard.push_back(line);
        }
    }
    parseStringBoard(stringBoard);
}

void Level::parseStringBoard(vector<string> stringBoard) {
    for (int r = 0; r < (int)stringBoard.size(); r++) {
        vector<Node *> boardRow;
        for (int c = 0; c < (int)stringBoard.at(r).length(); c++) {
            char ch = stringBoard.at(r).at(c);
            NODE_TYPE type = Node::getTypeFromChar(ch);
            if (type == PLAYER) {
                initPlayerRow = r;
                initPlayerCol = c;
            }
            Node *node = new Node(r, c, type);
            boardRow.push_back(node);
        }
        board.push_back(boardRow);
    }
}
