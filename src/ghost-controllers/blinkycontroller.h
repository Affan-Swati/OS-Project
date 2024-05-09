#include "ghostcontroller.h"

using namespace std;

class BlinkyController : public GhostController
{
    public:
    BlinkyController(void *&arg)
    {
        shared = (SharedVariables*) arg;
        inHouse = true;
        key = false;
        permit = false;
        key_index = -1;
        permit_index = -1;
    }

    pair<int, int> calculateTargetTile(int pacmanX, int pacmanY , int direction)
    {
        if(inHouse)
        {
            return make_pair(22,19); // house exit
        }

        if(shared->blinkyPos.first.x == 22 && shared->blinkyPos.first.y == 22)
        {
            inHouse = true;
            shared->mode[0] = 0;
        }

        if(shared->mode[0] == 3)
        {
            return make_pair(22,22);
        }
    
        if(shared->mode[0] == 2) // frighten mode set
        {
            int random = rand() % 4;

            if(random == 0)
            {
                return make_pair(shared->blinkyPos.first.x,shared->blinkyPos.first.y - 1);
            }

            else if(random == 1)
            {
                return make_pair(shared->blinkyPos.first.x + 1,shared->blinkyPos.first.y );
            }

            else if(random == 2)
            {
                return make_pair(shared->blinkyPos.first.x,shared->blinkyPos.first.y + 1);
            }

            else 
            {
                return make_pair(shared->blinkyPos.first.x - 1,shared->blinkyPos.first.y);
            }
        }

        return make_pair(pacmanX ,pacmanY ); // 56 , -1 for scatter phase
    }

    void update() 
    {
        checkReset();

        int pacmanX = shared->pacPos.x;
        int pacmanY = shared->pacPos.y;
        int direction = shared->pacDirection;

        vector<pair<int,int>> moves = findMoves(shared->blinkyPos,shared->mode[0]);

        pair<int,int> target = calculateTargetTile(pacmanX,pacmanY,direction);

        int shortest = INT32_MAX;
        pair<int,int>  newCoordinates = {shared->blinkyPos.first.x , shared->blinkyPos.first.y};


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

        shared->blinkyPos.second.x = shared->blinkyPos.first.x;
        shared->blinkyPos.second.y = shared->blinkyPos.first.y;

        shared->blinkyPos.first.x = newCoordinates.first;
        shared->blinkyPos.first.y = newCoordinates.second;

        // shared->gameBoard[(int)shared->blinkyPos.second.y][(int)shared->blinkyPos.second.x] = 0;
        // shared->gameBoard[(int)shared->blinkyPos.first.y][(int)shared->blinkyPos.first.x] = 5;

        if(shared->mode[0] == 3 && shared->blinkyPos.first.x == 30 && shared->blinkyPos.first.y == 32)
        {
            shared->blinkyPos.first.x = 30;  
            shared->blinkyPos.first.y = 30;
        }

        if(inHouse && shared->blinkyPos.first.x == 22 && shared->blinkyPos.first.y == 19)
        {
            inHouse = false;
            releaseKeyPermit(0);
        }

    }


};