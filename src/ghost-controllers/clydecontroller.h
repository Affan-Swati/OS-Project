#include "ghostcontroller.h"

class ClydeController : public GhostController
{
    public:
    ClydeController(void *&arg)
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
            return make_pair(20,22); // house exit
        }

        int displacement = sqrt((pow(pacmanX - shared->clydePos.first.x,2)) + (pow(pacmanY - shared->clydePos.first.y,2)));

        if(displacement >= 8)
            return make_pair(pacmanX,pacmanY);
        
        else 
            return make_pair(-1,51);
           
    }

    void update() 
    {
        int pacmanX = shared->pacPos.x;
        int pacmanY = shared->pacPos.y;
        int direction = shared->pacDirection;

        vector<pair<int,int>> moves = findMoves(shared->clydePos);

        pair<int,int> target = calculateTargetTile(pacmanX,pacmanY,direction);

        int shortest = INT32_MAX;
        pair<int,int>  newCoordinates = {shared->clydePos.first.x , shared->clydePos.first.y};


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

        shared->clydePos.second.x = shared->clydePos.first.x;
        shared->clydePos.second.y = shared->clydePos.first.y;

        shared->clydePos.first.x = newCoordinates.first;
        shared->clydePos.first.y = newCoordinates.second;

        if(inHouse && shared->clydePos.first.x == 22 && shared->clydePos.first.y == 19)
        {
            inHouse = false;
            releaseKeyPermit();
        }

    }
};