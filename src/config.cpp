// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#include "config.hpp"

#include <fstream>
#include <string>

using std::getline;
using std::ifstream;
using std::string;

Configuration::Configuration(string filename) {
    this->setDefault();
    ifstream config_file(filename);
    if (!config_file.is_open()) {
        return;
    }
    string line = "";
    string key = "";
    char value;
    bool getting_value = false;
    while (getline(config_file, line)) {
        for (char c : line) {
            if (c == '=') {
                getting_value = true;
                continue;
            }
            if (getting_value) {
                value = c;
                break;
            }
            key += c;
        }
        if (key == MOVE_UP_KEY_KEY) {
            this->moveUpKey = value;
        } else if (key == MOVE_DOWN_KEY_KEY) {
            this->moveDownKey = value;
        } else if (key == MOVE_LEFT_KEY_KEY) {
            this->moveLeftKey = value;
        } else if (key == MOVE_RIGHT_KEY_KEY) {
            this->moveRightKey = value;
        } else if (key == RESTART_KEY_KEY) {
            this->restartKey = value;
        } else if (key == QUIT_KEY_KEY) {
            this->quitKey = value;
        }
    }
}

void Configuration::setDefault() {
    this->moveUpKey = DEFAULT_MOVE_UP_KEY;
    this->moveDownKey = DEFAULT_MOVE_DOWN_KEY;
    this->moveLeftKey = DEFAULT_MOVE_LEFT_KEY;
    this->moveRightKey = DEFAULT_MOVE_RIGHT_KEY;
    this->restartKey = DEFAULT_RESTART_KEY;
    this->quitKey = DEFAULT_QUIT_KEY;
}
