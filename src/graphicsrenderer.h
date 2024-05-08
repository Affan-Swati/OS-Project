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
        Clock time;

        GraphicsRenderer(SharedVariables *shared) 
        {
            tex.loadFromFile("../resources/img/board/map1.png") ;
            sprite.setTexture(tex);     
            sprite.setScale(1,1);
            this->shared = shared;
            time.restart();
        }

        void drawFood(RenderWindow &window)
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
                if(shared->frightenPallets[i].second == -1 || (time.getElapsedTime().asSeconds() < 0.5))
                {
                    continue;
                }

                circle.setPosition(shared->frightenPallets[i].second * CELLSIZE_X + xOffset , shared->frightenPallets[i].first * CELLSIZE_Y + 4);
                window.draw(circle);
            }

            if(time.getElapsedTime().asSeconds() > 0.7)
            time.restart();

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

            // float change = 1;
            // if(sprite.getPosition().x < x * CELLSIZE_X)
            //     sprite.setPosition(x * CELLSIZE_X + offset_X + change, y * CELLSIZE_Y + offset_Y);

            
            // else if(sprite.getPosition().x > x * CELLSIZE_X)
            //      sprite.setPosition(x * CELLSIZE_X + offset_X - change, y * CELLSIZE_Y + offset_Y);
            
            // else if(sprite.getPosition().y < y *CELLSIZE_Y)
            //      sprite.setPosition(x * CELLSIZE_X + offset_X, y * CELLSIZE_Y + offset_Y + change);
            
            // else if(sprite.getPosition().y > y * CELLSIZE_Y)
            //      sprite.setPosition(x * CELLSIZE_X + offset_X , y * CELLSIZE_Y + offset_Y - change);
            
            // else
            // sprite.setPosition(x * CELLSIZE_X + offset_X , y * CELLSIZE_Y + offset_Y);

            // window.draw(sprite); 
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
            // float change = 0;
            // if(sprite.getPosition().x < x * CELLSIZE_X)
            //     sprite.setPosition(x * CELLSIZE_X + change, y * CELLSIZE_Y );
            
            // else if(sprite.getPosition().x > x * CELLSIZE_X)
            //     sprite.setPosition(x * CELLSIZE_X - change ,y * CELLSIZE_Y);
            
            // else if(sprite.getPosition().y < y *CELLSIZE_Y)
            //     sprite.setPosition(x * CELLSIZE_X,y * CELLSIZE_Y + change);
            
            // else if(sprite.getPosition().y > y * CELLSIZE_Y)
            //     sprite.setPosition(x * CELLSIZE_X ,y * CELLSIZE_Y - change);
            
            // else
            sprite.setPosition(x * CELLSIZE_X , y * CELLSIZE_Y);

            window.draw(sprite);            
        }

        void pacDeathAnimation(int x , int y , RenderWindow &window)
        {
            Texture tex , death_sheet;
            Sprite sprite;
            Clock clk;
            Music dead;

            dead.openFromFile("../resources/sounds/pacmanDeath.wav");
            death_sheet.loadFromFile("../resources/img/other/Pacman.png");

            float height = 13.8;
            float width = 15.91;
            clk.restart();
            sprite.setScale(2,2);
            dead.play();
            for(int i = 0 ; i < 14 ; i++)
            {   
                while(clk.getElapsedTime().asSeconds() < 0.1);
                clk.restart();

                window.clear();
                drawMap(window);
                drawFood(window);
                drawMaze(window);

                int off = i;
                if(i == 12 || i == 13)
                {
                    off = 11;
                }

                if(i != 12)
                tex.loadFromImage(death_sheet.copyToImage(), (IntRect)FloatRect(off * width  , 4 * height ,width , height));

                else
                tex.loadFromImage(death_sheet.copyToImage(), (IntRect)FloatRect(off * width  , 3 * height ,width , height));

                sprite.setTexture(tex);
                sprite.setPosition(x * CELLSIZE_X , y * CELLSIZE_Y);
                window.draw(sprite);
                window.display();
            }
        }

        void drawMap(RenderWindow &window )
        {
            window.draw(sprite);
        }

        void drawLives( RenderWindow &window, int lives)
        {
            Texture tex , sprite_sheet;
            Sprite  live;

            sprite_sheet.loadFromFile("../resources/img/other/Pacman.png");
            
            float height = 13.8;
            float width = 15.91;
            tex.loadFromImage(sprite_sheet.copyToImage(), (IntRect)FloatRect(1 * width , 1 * height ,width , height + 2));
            live.setTexture(tex);
            live.setPosition(0,750);
            live.setScale(2,2);
            float offset = 10;
            for(int i = 0 ; i < lives ; i++)
            {   
                live.setPosition(offset,750);
                window.draw(live);
                offset += 28;
            }



        }

};
#endif
