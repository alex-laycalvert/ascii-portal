// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#include "config.hpp"

#include <fstream>
#include <string>

using std::getline;
using std::ifstream;
using std::string;

Config loadConfig(const string filename) {
    Config config;
    setDefault(&config);
    ifstream config_file(filename);
    if (!config_file.is_open()) {
        return config;
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
            config.moveUpKey = value;
        } else if (key == MOVE_DOWN_KEY_KEY) {
            config.moveDownKey = value;
        } else if (key == MOVE_LEFT_KEY_KEY) {
            config.moveLeftKey = value;
        } else if (key == MOVE_RIGHT_KEY_KEY) {
            config.moveRightKey = value;
        } else if (key == CHANGE_DIR_UP_KEY_KEY) {
            config.changeDirUpKey = value;
        } else if (key == CHANGE_DIR_DOWN_KEY_KEY) {
            config.changeDirDownKey = value;
        } else if (key == CHANGE_DIR_LEFT_KEY_KEY) {
            config.changeDirLeftKey = value;
        } else if (key == CHANGE_DIR_RIGHT_KEY_KEY) {
            config.changeDirRightKey = value;
        } else if (key == SHOOT_PORTAL_KEY_KEY) {
            config.shootPortalKey = value;
        } else if (key == RESTART_KEY_KEY) {
            config.restartKey = value;
        } else if (key == QUIT_KEY_KEY) {
            config.quitKey = value;
        }
    }
    return config;
}

void setDefault(Config *config) {
    config->moveUpKey = DEFAULT_MOVE_UP_KEY;
    config->moveDownKey = DEFAULT_MOVE_DOWN_KEY;
    config->moveLeftKey = DEFAULT_MOVE_LEFT_KEY;
    config->moveRightKey = DEFAULT_MOVE_RIGHT_KEY;
    config->changeDirUpKey = DEFAULT_CHANGE_DIR_UP_KEY;
    config->changeDirDownKey = DEFAULT_CHANGE_DIR_DOWN_KEY;
    config->changeDirLeftKey = DEFAULT_CHANGE_DIR_LEFT_KEY;
    config->changeDirRightKey = DEFAULT_CHANGE_DIR_RIGHT_KEY;
    config->shootPortalKey = DEFAULT_SHOOT_PORTAL_KEY;
    config->restartKey = DEFAULT_RESTART_KEY;
    config->quitKey = DEFAULT_QUIT_KEY;
}
