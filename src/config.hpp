// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#ifndef CONFIG_HPP_
#define CONFIG_HPP_
#define DEFAULT_CONFIG_FILE "/.config/ascii-portal/ascii-portal.conf"

#define MOVE_UP_KEY_KEY "MoveUpKey"
#define MOVE_DOWN_KEY_KEY "MoveDownKey"
#define MOVE_LEFT_KEY_KEY "MoveLeftKey"
#define MOVE_RIGHT_KEY_KEY "MoveRightKey"
#define CHANGE_DIR_UP_KEY_KEY "ChangeDirectionUpKey"
#define CHANGE_DIR_DOWN_KEY_KEY "ChangeDirectionDownKey"
#define CHANGE_DIR_LEFT_KEY_KEY "ChangeDirectionLeftKey"
#define CHANGE_DIR_RIGHT_KEY_KEY "ChangeDirectionRightKey"
#define SHOOT_PORTAL_KEY_KEY "ShootPortalKey"
#define RESTART_KEY_KEY "RestartKey"
#define QUIT_KEY_KEY "QuitKey"

#define DEFAULT_MOVE_UP_KEY 'w'
#define DEFAULT_MOVE_DOWN_KEY 's'
#define DEFAULT_MOVE_LEFT_KEY 'a'
#define DEFAULT_MOVE_RIGHT_KEY 'd'
#define DEFAULT_CHANGE_DIR_UP_KEY 'k'
#define DEFAULT_CHANGE_DIR_DOWN_KEY 'j'
#define DEFAULT_CHANGE_DIR_LEFT_KEY 'h'
#define DEFAULT_CHANGE_DIR_RIGHT_KEY 'l'
#define DEFAULT_SHOOT_PORTAL_KEY ' '
#define DEFAULT_RESTART_KEY 'r'
#define DEFAULT_QUIT_KEY 'q'

#include <string>

using std::string;

typedef struct config_t {
    int moveUpKey;
    int moveDownKey;
    int moveLeftKey;
    int moveRightKey;
    int changeDirUpKey;
    int changeDirDownKey;
    int changeDirLeftKey;
    int changeDirRightKey;
    int shootPortalKey;
    int restartKey;
    int quitKey;
} Config;

void setDefault(Config *config);
Config loadConfig(const string filename);

#endif // CONFIG_HPP_
