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
            tex.loadFromFile("../resources/img/board/map1.png") ;
            sprite.setTexture(tex);     
            sprite.setScale(1,1);
            this->shared = shared;
        }

        void drawFood(RenderWindow &window , Sprite &sprite)
        {   
            RectangleShape food;
            food.setSize(Vector2f(4,4));
            food.setFillColor(Color(255,229,180,255));
        
            for (int i = 0; i < shared->ROWS; i++) 
            {
                for (int j = 0; j < shared->COLS; j++) 
                {
                    if (shared->gameBoard[i][j] == 3) 
                    {
                        food.setPosition(j * CELLSIZE_X + 12, i * CELLSIZE_Y + 12);
                        //sprite.setPosition((j * CELLSIZE_X) + 3, (i * CELLSIZE_Y) + 5);
                        window.draw(food);
                    }
                }
            }


            CircleShape circle;
            circle.setRadius(10);
            circle.setFillColor(Color(255,229,180,255));

            int xOffset = 5;
            int yOffset = 0;

            for(int i = 0 ; i < shared->frightenPallets.size(); i++)
            {
                if(shared->frightenPallets[i].first == -1)
                    continue;

                circle.setPosition(shared->frightenPallets[i].second * CELLSIZE_X + xOffset , shared->frightenPallets[i].first * CELLSIZE_Y + 4);
                window.draw(circle);
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
            cell.setFillColor(Color(0,0,255,100));

            int opacity = 210;
            bool dec = true;

            for (int i = 0; i < shared->ROWS; ++i) 
            {   
                if(dec)
                {
                    if(opacity <= 50)
                        dec = false;
                    else
                        opacity -= 10;
                }

                else
                {
                    if(opacity >= 200)
                        dec = true;
                    else
                        opacity += 10;

                }
                cell.setFillColor(Color(0,0,255,opacity));

                for (int j = 0; j < shared->COLS; ++j) 
                {
                    if (shared->gameBoard[i][j] == 1) 
                    {
                        cell.setPosition(j * CELLSIZE_X, i * CELLSIZE_Y);
                         // Change color as needed
                        window.draw(cell);
                    }
                }
            }
        }

        void drawGhost(RenderWindow &window , Sprite& sprite , int x , int y)
        {
            //int currentX = 
            sprite.setPosition(x * CELLSIZE_X , y * CELLSIZE_Y);
            window.draw(sprite);            
        }

        void drawMap(RenderWindow &window )
        {
            window.draw(sprite);
        }

};
#endif
