#include "ghostcontroller.h"

class InkyController : public GhostController
{
    public:
    InkyController(void *&arg)
    {
        shared = (SharedVariables*) arg;
        inHouse = true;
        key = false;
        permit = false;
        key_index = -1;
        permit_index = -1;
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

        newX = newX - (shared->blinkyPos.first.x - newX);
        newY = newY - (shared->blinkyPos.first.y  - newY);

        return make_pair(newX,newY);
    }

    void update() 
    {
        int pacmanX = shared->pacPos.x;
        int pacmanY = shared->pacPos.y;
        int direction = shared->pacDirection;

        vector<pair<int,int>> moves = findMoves(shared->inkyPos);

        pair<int,int> target = calculateTargetTile(pacmanX,pacmanY,direction);

        int shortest = INT32_MAX;
        pair<int,int>  newCoordinates = {shared->inkyPos.first.x , shared->inkyPos.first.y};


        for(int i = 0 ; i < moves.size() ; i++)
        {
            int displacement = sqrt((pow(target.first - moves[i].first,2)) + (pow(target.second - moves[i].second,2)));

            if( displacement < shortest)
            {
                shortest = displacement;
                newCoordinates.first = moves[i].first;
                newCoordinates.second = moves[i].second;
            }
        }

        shared->inkyPos.second.x = shared->inkyPos.first.x;
        shared->inkyPos.second.y = shared->inkyPos.first.y;

        shared->inkyPos.first.x = newCoordinates.first;
        shared->inkyPos.first.y = newCoordinates.second;

        if(inHouse && shared->inkyPos.first.x == 22 && shared->inkyPos.first.y == 19)
        {
            inHouse = false;
            releaseKeyPermit();
        }

    }

};