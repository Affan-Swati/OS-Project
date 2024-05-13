#include "ghostcontroller.h"

class PinkyController : public GhostController
{
    public:
    PinkyController(void *&arg)
    {
        shared = (SharedVariables*) arg;
        shared->inHouse[1] = true;
        key = false;
        permit = false;
        key_index = -1;
        permit_index = -1;
    }

    pair<int, int> calculateTargetTile(int pacmanX, int pacmanY, int direction)
    {
        if(shared->inHouse[1])
        {
            return make_pair(22,18); // house exit
        }

        if(shared->mode[1] == 1) // in scatter mode
        {
            return make_pair(45,0);
        }

        if(shared->pinkyPos.first.x == 22 && shared->pinkyPos.first.y == 22)
        {
            relaseSpeedBoost(1);
            shared->inHouse[1] = true;
            shared->mode[1] = shared->oldMode[1];

        }

        if(shared->mode[1] == 3)
        {
            return make_pair(22,22);
        }

         if(shared->mode[1] == 2) // frighten mode set
        {
            int random = rand() % 4;

            if(random == 0)
            {
                return make_pair(shared->pinkyPos.first.x,shared->pinkyPos.first.y - 1);
            }

            else if(random == 1)
            {
                return make_pair(shared->pinkyPos.first.x + 1,shared->pinkyPos.first.y );
            }

            else if(random == 2)
            {
                return make_pair(shared->pinkyPos.first.x,shared->pinkyPos.first.y + 1);
            }

            else 
            {
                return make_pair(shared->pinkyPos.first.x - 1,shared->pinkyPos.first.y);
            }
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

    void update() 
    {        
        if(!shared->takenSpeedBoosts[1])
            checkSpeedBoost();

        int pacmanX = shared->pacPos.x;
        int pacmanY = shared->pacPos.y;
        int direction = shared->pacDirection;

        vector<pair<int,int>> moves = findMoves(shared->pinkyPos,1);

        pair<int,int> target = calculateTargetTile(pacmanX,pacmanY,direction);

        int shortest = INT32_MAX;
        pair<int,int>  newCoordinates = {shared->pinkyPos.first.x , shared->pinkyPos.first.y};


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

        shared->pinkyPos.second.x = shared->pinkyPos.first.x;
        shared->pinkyPos.second.y = shared->pinkyPos.first.y;

        shared->pinkyPos.first.x = newCoordinates.first;
        shared->pinkyPos.first.y = newCoordinates.second;

        // shared->gameBoard[(int)shared->pinkyPos.second.y][(int)shared->pinkyPos.second.x] = 0;
        // shared->gameBoard[(int)shared->pinkyPos.first.y][(int)shared->pinkyPos.first.x] = 6;

        if(shared->mode[1] == 3 && shared->pinkyPos.first.x == 30 && shared->pinkyPos.first.y == 32)
        {
            shared->pinkyPos.first.x = 30;  
            shared->pinkyPos.first.y = 30;
        }

        if(shared->inHouse[1] && shared->pinkyPos.first.x == 22 && shared->pinkyPos.first.y == 19)
        {
            shared->inHouse[1] = false;
            releaseKeyPermit(1);
        }

    }

    void checkSpeedBoost()
    {
        if(!shared->takenSpeedBoosts[1] && !shared->inHouse[1])
        {
            if(!shared->speedBoosts[0] && !shared->speedBoosts[1])
                return;

            pthread_mutex_lock(&shared->speedBoost_mutex);
            if(shared->speedBoosts[0])
            {
                shared->speedBoosts[0] = false;
                this->speedBoostIndex = 0;
            }

            else
            {
                shared->speedBoosts[1] = false;
                this->speedBoostIndex = 1;
            }   
            
            shared->takenSpeedBoosts[1] = true;
            pthread_mutex_unlock(&shared->speedBoost_mutex);
        }
    }

};