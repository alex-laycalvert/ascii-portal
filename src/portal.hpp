// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#ifndef PORTAL_HPP_
#define PORTAL_HPP_

#include <string>

#include "config.hpp"
#include "level.hpp"
#include "node.hpp"

using std::string;

class Portal {
   public:
    static Portal *getPortal();
    void loadConfig(Config config);
    void loadLevel(string filename);
    void play();

   private:
    Portal();
    ~Portal();
    static Portal *instance;
    bool playing;
    Config config;
    Level *level;
    vector<vector<Node *>> board;
    Node *player;
    Node *bluePortal;
    Node *orangePortal;
    bool bluePortalSet;
    bool orangePortalSet;
    void restart();
    void printBoard();
    void printOverlays();
    void handleInput(const int input);
    void movePlayer(DIRECTION direction);
    void shootPortal();
};

#endif // PORTAL_HPP_
