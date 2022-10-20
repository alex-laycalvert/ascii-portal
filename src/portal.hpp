// alex-laycalvert
// https://github.com/alex-laycalvert/ascii-portal

#ifndef PORTAL_HPP_
#define PORTAL_HPP_

#include <string>

#include "level.hpp"

using std::string;

class Portal {
   public:
    static Portal *getPortal();
    void loadLevel(string filename);

   private:
    Portal();
    ~Portal();
    static Portal *instance;
};

#endif // PORTAL_HPP_
