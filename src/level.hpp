// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#ifndef LEVEL_HPP
#define LEVEL_HPP

#include <string>
#include <vector>

#include "node.hpp"

using std::string;
using std::vector;

class Level {
   public:
    Level(string filename);

   private:
    int number;
    string name;
    string author;
    string description;
    vector<vector<Node>> board;
};

#endif // LEVEL_HPP
