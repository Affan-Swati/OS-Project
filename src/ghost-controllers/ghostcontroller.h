#pragma once
#include "../sharedvariables.h"
#include <SFML/Graphics.hpp>
#include <vector>
#include <cmath>

using namespace std;
using namespace sf;

class GhostController
{
    public:
    virtual pair<int, int> calculateTargetTile(int pacmanX, int pacmanY, int direction) = 0;
    virtual void update() = 0;

    public:
    SharedVariables* shared;
    bool inHouse;
    
    GhostController()
    {
    }

    vector<pair<int,int>> findMoves(pair<Vector2f,Vector2f> &ghostPosition) 
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

        if(!checkCollisionGhost(nextX, nextY + 1) && !reverse(nextX  , nextY + 1,ghostPosition.second))
        {
            moves.push_back(make_pair(nextX , nextY + 1));
        }

        if(!checkCollisionGhost(nextX , nextY - 1) && !reverse(nextX , nextY - 1,ghostPosition.second))
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

    bool reverse(int x , int y, Vector2f &prevPosition)
    {
        //return false;
        return (x == prevPosition.x && y == prevPosition.y );
    }

};