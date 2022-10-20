// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#include "portal.hpp"

Portal *Portal::instance = 0;

Portal *Portal::getPortal() {
    if (!instance) {
        instance = new Portal();
    }
    return instance;
}

void Portal::loadLevel(string filename) {}
