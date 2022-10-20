// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#define DEFAULT_CONFIG_FILE "/.config/ascii-portal/ascii-portal.conf"

#define MOVE_UP_KEY_KEY "MoveUpKey"
#define MOVE_DOWN_KEY_KEY "MoveDownKey"
#define MOVE_LEFT_KEY_KEY "MoveLeftKey"
#define MOVE_RIGHT_KEY_KEY "MoveRightKey"
#define RESTART_KEY_KEY "RestartKey"
#define QUIT_KEY_KEY "QuitKey"

#define DEFAULT_MOVE_UP_KEY 'k'
#define DEFAULT_MOVE_DOWN_KEY 'j'
#define DEFAULT_MOVE_LEFT_KEY 'h'
#define DEFAULT_MOVE_RIGHT_KEY 'l'
#define DEFAULT_RESTART_KEY 'r'
#define DEFAULT_QUIT_KEY 'q'

#include <string>

using std::string;

class Configuration {
    public:
        int moveUpKey;
        int moveDownKey;
        int moveLeftKey;
        int moveRightKey;
        int restartKey;
        int quitKey;
        Configuration(string filename);
        void setDefault();
};

#endif // CONFIG_HPP_
