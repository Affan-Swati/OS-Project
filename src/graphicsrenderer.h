#ifndef GR_H
#define GR_H
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include <cmath>
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
        Sprite blinky,pinky,inky,clyde;
        Texture tex , blinky_tex,pinky_tex,inky_tex,clyde_tex;
        Texture tex_ , sprite_sheet;
        Sprite  live;
        Text text , text2;
        sf::Font font;

        SharedVariables *shared;
        Clock time;

        GraphicsRenderer(SharedVariables *shared) 
        {
            tex.loadFromFile("../resources/img/board/map1.png") ;
            sprite_sheet.loadFromFile("../resources/img/other/Pacman.png");
            tex_.loadFromImage(sprite_sheet.copyToImage(), (IntRect)FloatRect(1 * 15.91 , 1 * 13.8 ,15.91 , 13.8 + 2));
            live.setTexture(tex_);
            live.setScale(2,2);

            blinky_tex.loadFromFile("../resources/img/ghosts/blinky.png");
            blinky.setTexture(blinky_tex);
            pinky_tex.loadFromFile("../resources/img/ghosts/pinky.png");
            pinky.setTexture(pinky_tex);
            inky_tex.loadFromFile("../resources/img/ghosts/inky.png");
            inky.setTexture(inky_tex);
            clyde_tex.loadFromFile("../resources/img/ghosts/clyde.png");
            clyde.setTexture(clyde_tex);

            font.loadFromFile("../resources/font.ttf");
            text.setFont(font);
            text2.setFont(font);

            text.setString("Speed Boost");
            text.setPosition(520,800);
            text.setScale(0.5,0.5);
            text2.setString("Lives");
            text2.setPosition(30,800);
            text2.setScale(0.5,0.5);


            blinky.setScale(2,2);
            pinky.setScale(2,2);
            inky.setScale(2,2);
            clyde.setScale(2,2);

            sprite.setTexture(tex);     
            sprite.setScale(1,1);
            this->shared = shared;
            time.restart();
        }

        void drawFood(RenderWindow &window , vector<pair<int,int>> &frightenPallets)
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

            for(int i = 0 ; i < frightenPallets.size(); i++)
            {
                if(frightenPallets[i].second == -1 || (time.getElapsedTime().asSeconds() < 0.5))
                {
                    continue;
                }

                circle.setPosition(frightenPallets[i].second * CELLSIZE_X + xOffset , frightenPallets[i].first * CELLSIZE_Y + 4);
                window.draw(circle);
            }

            if(time.getElapsedTime().asSeconds() > 0.7)
            time.restart();

        }

        void drawPacMan(RenderWindow &window , Sprite& sprite , int x , int y , int direction)
         {
            int offset_X = 0 , offset_Y = 0;
             if(direction == 0 || direction == 2)
                offset_X = 3; 
             else
                offset_Y = 2;

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
                        window.draw(cell);
                    }
                }
            }
        }

        void drawMazeWhite(RenderWindow &window) 
        {
            // Draw the maze
            RectangleShape cell(Vector2f(CELLSIZE_X,CELLSIZE_Y));
            cell.setOutlineColor(Color(0,0,0,255));
            cell.setOutlineThickness(-1.0f); // Negative thickness for outline
            cell.setFillColor(Color(255,255,255,150));

            for (int i = 0; i < shared->ROWS; ++i) 
            {   
                for (int j = 0; j < shared->COLS; ++j) 
                {
                    if (shared->gameBoard[i][j] == 1) 
                    {
                        cell.setPosition(j * CELLSIZE_X, i * CELLSIZE_Y);
                        window.draw(cell);
                    }
                }
            }
        }

        void drawGhost(RenderWindow &window , Sprite& sprite , int x , int y)
        {     
            sprite.setPosition(x * CELLSIZE_X , y * CELLSIZE_Y);

            window.draw(sprite);            
        }

        void pacDeathAnimation(int x , int y , RenderWindow &window ,int lives , vector<pair<int,int>> &frightenPallets)
        {
            Texture tex , death_sheet;
            Sprite sprite , dummy;
            Clock clk;
            Music dead;

            dead.openFromFile("../resources/sounds/pacmanDeath.wav");
            death_sheet.loadFromFile("../resources/img/other/Pacman.png");

            dummy = this->sprite;
            dummy.setColor(Color(255,255,255,255));

            float height = 13.8;
            float width = 15.91;
            clk.restart();
            sprite.setScale(2,2.2);
            dead.play();

            int j = 0;

            for(int i = 0 ; i < 14 ; i++)
            {   
                while(clk.getElapsedTime().asSeconds() < 0.1);
                clk.restart();

                window.clear();

                if(j < 2)
                {
                    drawMap(window);
                    drawMaze(window);

                }

                else
                {
                    window.draw(dummy);
                    drawMazeWhite(window);
                }

                drawFood(window,frightenPallets);

                int off = i;
                if(i == 12 || i == 13)
                {
                    off = 11;
                }

                if(j == 3)
                    j = 0;
                else
                    j++;

                if(i != 12)
                tex.loadFromImage(death_sheet.copyToImage(), (IntRect)FloatRect(off * width  , 4 * height ,width , height));

                else
                tex.loadFromImage(death_sheet.copyToImage(), (IntRect)FloatRect(off * width  , 3 * height ,width , height));

                sprite.setTexture(tex);
                sprite.setPosition(x * CELLSIZE_X , y * CELLSIZE_Y);
                window.draw(sprite);
                this->drawGhostSpeedBoosts(window);
                this->drawLives(window,lives);
                window.display();
            }
        }

        void drawMap(RenderWindow &window )
        {
            window.draw(sprite);
        }

        void drawLives( RenderWindow &window, int lives)
        {       
            float offset = 10;
            for(int i = 0 ; i < lives ; i++)
            {   
                live.setPosition(offset,760);
                window.draw(live);
                offset += 28;
            }

            window.draw(text2);
        }

        void drawGhostSpeedBoosts(RenderWindow &window)
        {
            if(!shared->takenSpeedBoosts[0])
                blinky.setColor(Color(255,255,255,100));
            else
                blinky.setColor(Color(255,255,255,255));

            if(!shared->takenSpeedBoosts[1])
                pinky.setColor(Color(255,255,255,100));
            else
                pinky.setColor(Color(255,255,255,255));

            if(!shared->takenSpeedBoosts[2])
                inky.setColor(Color(255,255,255,100));
            else
                inky.setColor(Color(255,255,255,255));

            if(!shared->takenSpeedBoosts[3])
                clyde.setColor(Color(255,255,255,100));
            else
                clyde.setColor(Color(255,255,255,255));

            
            blinky.setPosition(530,755);
            pinky.setPosition(570,755);
            inky.setPosition(610,755);
            clyde.setPosition(650,755);

            window.draw(text);
            window.draw(blinky);
            window.draw(pinky);
            window.draw(inky);
            window.draw(clyde);

        }

};
#endif
