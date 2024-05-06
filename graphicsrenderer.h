#ifndef GR_H
#define GR_H
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "sharedvariables.h"

using namespace std;
using namespace sf;

const int CELLSIZE_X = 15;
const int CELLSIZE_Y = 15;

class GraphicsRenderer
{
    public:

        Sprite sprite;
        Texture tex , dummy_tex;
        SharedVariables *shared;

        GraphicsRenderer(SharedVariables *shared) 
        {
            tex.loadFromFile("img/board/map1.png") ;
            sprite.setTexture(tex);     
            sprite.setScale(1,1);
            this->shared = shared;
        }

        void drawFood(RenderWindow &window , Sprite &sprite)
        {   
            CircleShape food;
            food.setRadius(3);
            food.setFillColor(Color::Yellow);
            

            for (int i = 0; i < shared->ROWS; i++) 
            {
                for (int j = 0; j < shared->COLS; j++) 
                {
                    if (shared->gameBoard[i][j] == 3) 
                    {
                        food.setPosition(j * CELLSIZE_X + 10, i * CELLSIZE_Y + 10);
                        //sprite.setPosition((j * CELLSIZE_X) + 3, (i * CELLSIZE_Y) + 5);
                        window.draw(food);
                    }
                }
            }
        }

         void drawPacMan(RenderWindow &window , Sprite& sprite , int x , int y , int direction)
         {
            int offset_X = 0 , offset_Y = 0;
             if(direction == 0 || direction == 2)
                offset_X = 5;
             else
                offset_Y = 3;

            sprite.setPosition(x * CELLSIZE_X + offset_X, y * CELLSIZE_Y + offset_Y);
            window.draw(sprite);
        }

        void drawMaze(RenderWindow &window) 
        {
            // Draw the maze
            RectangleShape cell(Vector2f(CELLSIZE_X,CELLSIZE_Y));
            cell.setOutlineColor(Color(0,0,0,255));
            cell.setOutlineThickness(-1.0f); // Negative thickness for outline
            for (int i = 0; i < shared->ROWS; ++i) 
            {
                for (int j = 0; j < shared->COLS; ++j) 
                {
                    if (shared->gameBoard[i][j] == 1) 
                    {
                        cell.setPosition(j * CELLSIZE_X, i * CELLSIZE_Y);
                        cell.setFillColor(Color::Red); // Change color as needed
                        window.draw(cell);
                    }
                }
            }
        }

        void drawBlinky(RenderWindow &window , Sprite& sprite , int x , int y)
        {
            sprite.setPosition(x * CELLSIZE_X , y * CELLSIZE_Y);
            window.draw(sprite);            
        }
};
#endif
