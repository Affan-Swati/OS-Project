#ifndef GHOST_H
#define GHOST_H

#include <vector>
#include <SFML/Graphics.hpp>
#include "sharedvariables.h"
#include <cmath>

using namespace sf;
using namespace std;

class Ghost 
{
   protected:
    virtual pair<int, int> calculateTargetTile(int pacmanX, int pacmanY, int direction) = 0;

   public:
    Vector2f position , prevPosition , scatter;
    Sprite sprite;
    Texture tex;
    bool inHouse;
    int mode; // 0 means scatter, 1 means chase, 2 means frightened, 3 means eaten
    int state; // 0 means open normal eyes, 1 means right eyes, 2 means down eyes, 3 means left eyes
    int direction; // 0 means up, 1 means right, 2 means down, 3 means left
    Clock clk;
    SharedVariables *shared;

    Ghost() 
    {
    
    }

    void update(int pacmanX, int pacmanY , int direction) 
    {
        vector<pair<int,int>> moves = findMoves();

        pair<int,int> target = calculateTargetTile(pacmanX,pacmanY,direction);

        int shortest = INT32_MAX;
        pair<int,int>  newCoordinates = {position.x , position.y};


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

        prevPosition.x = position.x;
        prevPosition.y = position.y;

        position.x = newCoordinates.first;
        position.y = newCoordinates.second;

        if(inHouse && position.x == 22 && position.y == 19)
            inHouse = false;

    }

    vector<pair<int,int>> findMoves() 
    {
        int nextX = position.x;
        int nextY = position.y;

        vector<pair<int,int>> moves;

        if(!checkCollisionGhost(nextX + 1, nextY) && !reverse(nextX + 1 , nextY))
        {
            moves.push_back(make_pair(nextX + 1 , nextY));
        }

        if(!checkCollisionGhost(nextX - 1, nextY) && !reverse(nextX - 1 , nextY))
        {
            moves.push_back(make_pair(nextX - 1 , nextY));
        }

        if(!checkCollisionGhost(nextX, nextY + 1) && !reverse(nextX  , nextY + 1))
        {
            moves.push_back(make_pair(nextX , nextY + 1));
        }

        if(!checkCollisionGhost(nextX , nextY - 1) && !reverse(nextX , nextY - 1))
        {
            moves.push_back(make_pair(nextX , nextY - 1));
        }

        return moves;
    }


    private:
    bool checkCollisionGhost(int x, int y) 
    {
        // Check if the position is within the boundaries of the game board
        if (x >= 0 && x < shared->COLS && y >= 0 && y < shared->ROWS) 
        {
            if(shared->gameBoard[y][x] != 1 && shared->gameBoard[y][x] != 2 &&
               shared->gameBoard[y + 1][x] != 1 && shared->gameBoard[y + 1][x] != 2 &&
               shared->gameBoard[y][x + 1] != 1 && shared->gameBoard[y][x + 1] != 2 &&
               shared->gameBoard[y + 1][x + 1] != 1 && shared->gameBoard[y + 1][x + 1] != 2
             ) 
                return false;
        }
        // If the position is outside the game board, consider it a collision
        return true;
    }

    bool reverse(int x , int y)
    {
        //return false;
        return (x == prevPosition.x && y == prevPosition.y );
    }

};

#endif // GHOST_H
