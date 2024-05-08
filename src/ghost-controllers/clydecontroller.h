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

        if(shared->clydePos.first.x == 22 && shared->clydePos.first.y == 22)
        {
            inHouse = true;
            shared->mode[3] = 0;
        }

        if(shared->mode[3] == 3)
        {
            return make_pair(22,22);
        }

        if(shared->mode[3] == 2) // frighten mode set
        {
            int random = rand() % 4;

            if(random == 0)
            {
                return make_pair(shared->clydePos.first.x,shared->clydePos.first.y - 1);
            }

            else if(random == 1)
            {
                return make_pair(shared->clydePos.first.x + 1,shared->clydePos.first.y );
            }

            else if(random == 2)
            {
                return make_pair(shared->clydePos.first.x,shared->clydePos.first.y + 1);
            }

            else 
            {
                return make_pair(shared->clydePos.first.x - 1,shared->clydePos.first.y);
            }
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

        vector<pair<int,int>> moves = findMoves(shared->clydePos,shared->mode[3]);

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

        // shared->gameBoard[(int)shared->clydePos.second.y][(int)shared->clydePos.second.x] = 0;
        // shared->gameBoard[(int)shared->clydePos.first.y][(int)shared->clydePos.first.x] = 8;

        if(inHouse && shared->clydePos.first.x == 22 && shared->clydePos.first.y == 19)
        {
            inHouse = false;
            releaseKeyPermit();
        }

    }
};