#include "ghost.h"

class Clyde : public Ghost
{
    public:
    Clyde(void *&arg)
    {
        tex.loadFromFile("img/ghosts/clyde.png");
        sprite.setTexture(tex);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.6,1.6);
        prevPosition.x = -1;
        prevPosition.y = -1;
        position.x = 24;
        position.y = 22;
        inHouse = true;
    }

    pair<int, int> calculateTargetTile(int pacmanX, int pacmanY, int direction)
    {
        if(inHouse)
        {
            return make_pair(20,22); // house exit
        }

        
        int displacement = sqrt((pow(pacmanX - position.x,2)) + (pow(pacmanY - position.y,2)));

        if(displacement >= 8)
            return make_pair(pacmanX,pacmanY);
        
        else 
            return make_pair(-1,51);
            
    }

};