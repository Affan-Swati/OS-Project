#include "ghost.h"

class Pinky : public Ghost
{
    public:
    Pinky(void *&arg)
    {
        tex.loadFromFile("img/ghosts/pinky.png");
        sprite.setTexture(tex);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.6,1.6);
        prevPosition.x = -1;
        prevPosition.y = -1;
        position.x = 20;
        position.y = 22;
        inHouse = true;
    }

    pair<int, int> calculateTargetTile(int pacmanX, int pacmanY, int direction)
    {
        if(inHouse)
        {
            return make_pair(19,22); // house exit
        }

        if(direction == 0)
            return make_pair(pacmanX - 4 ,pacmanY - 4 ); // 46 , -1 for scatter phase
        else if(direction == 1)
            return make_pair(pacmanX + 5 ,pacmanY); 
        else if(direction == 2)
            return make_pair(pacmanX ,pacmanY + 5); 
        else 
            return make_pair(pacmanX - 4 ,pacmanY); 

    }

};