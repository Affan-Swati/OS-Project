#include "ghost.h"

class Blinky : public Ghost
{
    public:
    Blinky(void *&arg)
    {
        tex.loadFromFile("img/ghosts/blinky.png");
        sprite.setTexture(tex);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.6,1.6);
        prevPosition.x = -1;
        prevPosition.y = -1;
        position.x = 18;
        position.y = 22;
        inHouse = true;
    }

    pair<int, int> calculateTargetTile(int pacmanX, int pacmanY , int direction)
    {
        shared->blinkyPos.x = position.x; 
        shared->blinkyPos.y = position.y;
        if(inHouse)
        {
            return make_pair(19,22); // house exit
        }
        return make_pair(pacmanX ,pacmanY ); // 56 , -1 for scatter phase
    }

};