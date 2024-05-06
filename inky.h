#include "ghost.h"

class Inky : public Ghost
{
    public:
    Inky(void *&arg)
    {
        tex.loadFromFile("img/ghosts/inky.png");
        sprite.setTexture(tex);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.6,1.6);
        prevPosition.x = -1;
        prevPosition.y = -1;
        position.x = 22;
        position.y = 22;
        inHouse = true;
    }

    pair<int, int> calculateTargetTile(int pacmanX, int pacmanY, int direction)
    {
        if(inHouse)
        {
            return make_pair(19,22); // house exit
        }

        int newX = pacmanX;
        int newY = pacmanY;

        if(direction == 0)
        {
            newX-= 4; newY -=4;
        }
        else if(direction == 1)
        {
            newX += 5 ;
        }
        else if(direction == 2)
        {
            newY += 5;
        }
        else 
        {
            newX -=4 ;
        }

        newX = newX - (shared->blinkyPos.x - newX);
        newY = newY - (shared->blinkyPos.y  - newY);

        return make_pair(newX,newY);
    }

};