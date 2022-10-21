// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#include "portal.hpp"

#include <iostream>

Portal *Portal::instance = 0;

Portal::Portal() {}

Portal *Portal::getPortal() {
    if (!instance) {
        instance = new Portal();
    }
    instance->level = nullptr;
    return instance;
}

void Portal::loadConfig(Config config) { this->config = config; }

void Portal::loadLevel(string filename) {
    level = new Level(filename);
    restart();
}

void Portal::play() {
    playing = true;
    while (playing) {
        printBoard();
        int input = getch();
        handleInput(input);
    }
}

void Portal::restart() {
    board = level->getBoard();
    player = board.at(level->getPlayerRow()).at(level->getPlayerCol());
}

void Portal::printBoard() {
    for (int i = 0; i < (int)board.size(); i++) {
        for (int j = 0; j < (int)board.at(i).size(); j++) {
            board.at(i).at(j)->print(0, 0);
        }
    }
    printOverlays();
}

void Portal::printOverlays() {
    switch (player->dir) {
        case UP:
            mvprintw(player->row - 1, player->col, "^");
            break;
        case DOWN:
            mvprintw(player->row + 1, player->col, "v");
            break;
        case LEFT:
            mvprintw(player->row, player->col - 1, "<");
            break;
        case RIGHT:
            mvprintw(player->row, player->col + 1, ">");
            break;
        default:
            break;
    }
}

void Portal::handleInput(const int input) {
    if (input == config.moveUpKey) {
        movePlayer(UP);
    } else if (input == config.moveDownKey) {
        movePlayer(DOWN);
    } else if (input == config.moveLeftKey) {
        movePlayer(LEFT);
    } else if (input == config.moveRightKey) {
        movePlayer(RIGHT);
    } else if (input == config.changeDirUpKey) {
        player->dir = UP;
    } else if (input == config.changeDirDownKey) {
        player->dir = DOWN;
    } else if (input == config.changeDirLeftKey) {
        player->dir = LEFT;
    } else if (input == config.changeDirRightKey) {
        player->dir = RIGHT;
    } else if (input == config.shootPortalKey) {
        shootPortal();
    } else if (input == config.restartKey) {
        restart();
    } else if (input == config.quitKey) {
        endwin();
        exit(0);
    }
}

void Portal::movePlayer(DIRECTION direction) {
    int playerRowOffset = 0;
    int playerColOffset = 0;
    switch (direction) {
        case UP:
            playerRowOffset = -1;
            break;
        case DOWN:
            playerRowOffset = 1;
            break;
        case LEFT:
            playerColOffset = -1;
            break;
        case RIGHT:
            playerColOffset = 1;
            break;
    }
    DIRECTION currentDirection = player->dir;
    Node *nextNode = board.at(player->row + playerRowOffset)
                         .at(player->col + playerColOffset);
    Node *doubleNextNode = board.at(player->row + playerRowOffset * 2)
                               .at(player->col + playerColOffset * 2);
    switch (nextNode->getType()) {
        case EMPTY:
            player->setType(EMPTY);
            nextNode->setType(PLAYER);
            player = nextNode;
            break;
        case BLOCK:
            if (doubleNextNode->getType() != EMPTY) {
                break;
            }
            player->setType(EMPTY);
            nextNode->setType(PLAYER);
            doubleNextNode->setType(BLOCK);
            player = nextNode;
            break;
        case BLUE_PORTAL:
            break;
        case ORANGE_PORTAL:
            break;
        default:
            break;
    }
    player->dir = currentDirection;
}

void Portal::shootPortal() {
    switch (player->dir) {
        case UP:
            break;
        case DOWN:
            break;
        case LEFT:
            break;
        case RIGHT:
            break;
        default:
            break;
    }
}
