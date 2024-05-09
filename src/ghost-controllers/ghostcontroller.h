#pragma once
#include "../sharedvariables.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <time.h>

using namespace std;
using namespace sf;

class GhostController
{
    protected:
    virtual pair<int, int> calculateTargetTile(int pacmanX, int pacmanY, int direction) = 0;

    public:
    virtual void update() = 0;
    
    SharedVariables* shared;
    bool inHouse;
    bool key;
    bool permit;
    int key_index;
    int permit_index;
    
    GhostController()
    {
    }

    vector<pair<int,int>> findMoves(pair<Vector2f,Vector2f> &ghostPosition , int mode) 
    {
        int nextX = ghostPosition.first.x;
        int nextY = ghostPosition.first.y;

        vector<pair<int,int>> moves;

        if(!checkCollisionGhost(nextX + 1, nextY) && !reverse(nextX + 1 , nextY,ghostPosition.second))
        {
            moves.push_back(make_pair(nextX + 1 , nextY));
        }

        if(!checkCollisionGhost(nextX - 1, nextY) && !reverse(nextX - 1 , nextY,ghostPosition.second))
        {
            moves.push_back(make_pair(nextX - 1 , nextY));
        }

        if(!checkCollisionGhost(nextX, nextY + 1) && !reverse(nextX , nextY + 1,ghostPosition.second))
        {
            moves.push_back(make_pair(nextX , nextY + 1));
        }

        if(!checkCollisionGhost(nextX , nextY - 1) && !reverse(nextX , nextY -1,ghostPosition.second))
        {
            moves.push_back(make_pair(nextX , nextY - 1));
        }

        
        return moves;
    }

    void grabKeyPermit(int i)
    {
       int delay = 6;

       if(shared->key_perm[i].getElapsedTime().asSeconds() > delay && shared->key_perm[(i + 1) % 4].getElapsedTime().asSeconds() > delay)
       {
            sem_wait(&shared->key_perm_semaphores[i]);
            key = true;
            sem_wait(&shared->key_perm_semaphores[(i + 1) % 4]);

            pthread_mutex_lock(&shared->key_perm_mutex);
            shared->key_perm[i].restart();
            shared->key_perm[(i + 1) % 4].restart();
            permit = true;
            pthread_mutex_unlock(&shared->key_perm_mutex);

       }
    }


    void releaseKeyPermit(int i)
    {
        pthread_mutex_lock(&shared->key_perm_mutex);
        permit = false;
        key = false;        
        sem_post(&shared->key_perm_semaphores[i]);
        sem_post(&shared->key_perm_semaphores[(i + 1) % 4]);  
        pthread_mutex_unlock(&shared->key_perm_mutex);        
    }
    
    private:
    bool checkCollisionGhost(int x, int y) 
    {
        // Check if the position is within the boundaries of the game board
        if (x >= 0 && x < shared->COLS && y >= 0 && y < shared->ROWS) 
        {
            if(shared->gameBoard[y][x] != 1 &&
               shared->gameBoard[y + 1][x] != 1 &&
               shared->gameBoard[y][x + 1] != 1 &&
               shared->gameBoard[y + 1][x + 1] != 1
             ) 
                return false;
        }
        // If the position is outside the game board, consider it a collision
        return true;
    }

    bool reverse(int x , int y, Vector2f &prevPosition)
    {
        //return false;
        return (x == prevPosition.x && y == prevPosition.y );
    }

    protected:
    void checkReset()
    {
        if(shared->gameReset)
        {
            inHouse = true;
        }
    }

};