#ifndef GHOST_H
#define GHOST_H

#include <SFML/Graphics.hpp>
#include "../sharedvariables.h"
#include <cmath>

using namespace sf;
using namespace std;

class Ghost 
{
   
   public:
    Sprite sprite;
    Texture tex;
    int mode; // 0 means scatter, 1 means chase, 2 means frightened, 3 means eaten
    int state; // 0 means open normal eyes, 1 means right eyes, 2 means down eyes, 3 means left eyes
    int direction; // 0 means up, 1 means right, 2 means down, 3 means left
    SharedVariables *shared;

    Ghost() 
    {
    
    }

    

};

#endif // GHOST_H
