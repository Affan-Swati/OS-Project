#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include "graphicsrenderer.h"
#include "pacman.h"
#include "blinky.h"
#include "pinky.h"
#include "inky.h"
#include "clyde.h"
#include "sharedvariables.h"
#include <unistd.h>
#include <pthread.h>


pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

using namespace std;
using namespace sf;

class GameEngine
{
    public:
    
    SharedVariables *shared;
    GraphicsRenderer *graphicsRenderer;
    Pacman *pacman;;
    Ghost *blinky , *pinky ,*inky ,*clyde; // polymorphism 
    // create 4 ghosts here
    float speed;;

    Sprite food , logo;
    Texture tex , tex_logo;

    Music siren , eat1,eat2;
   
    GameEngine(void *&arg)
    {
        shared = (SharedVariables*)arg;
        graphicsRenderer = new GraphicsRenderer(shared);
        pacman = new Pacman;
        blinky = new Blinky(arg);
        pinky = new Pinky(arg);
        inky = new Inky(arg);
        clyde = new Clyde(arg);

        speed = 1;
        this->initializeFood();
        tex.loadFromFile("img/other/dot.png");
        tex_logo.loadFromFile("img/other/logo.png");
        food.setTexture(tex);
        food.setScale(1.5,1.5);
        logo.setTexture(tex_logo);
        logo.setPosition(208,750);
        logo.setScale(0.1,0.1);


        shared->gameBoard[(int)pacman->position.y][(int)pacman->position.x] = 2;
        siren.openFromFile("sounds/siren.wav");
        eat2.openFromFile("sounds/eat.wav");
        eat1.openFromFile("sounds/PelletEat2.wav");

        eat1.setVolume(50.f);
        eat2.setVolume(50.f);

    }
    
    void start_game()
    {
        RenderWindow window(VideoMode(695,900), "OS PROJECT");

        Text text , text2;
        sf::Font font;
        font.loadFromFile("font.ttf");
        text.setFont(font);
        text.setPosition(Vector2f(295,850));
        text.setOutlineColor(Color::Blue);
        text.setOutlineThickness(2.0f);
        text.setFillColor(Color::Yellow);

        text2.setFont(font);
        text2.setPosition(Vector2f(260,430));
        text2.setString("Adil Nadeem ;D");
        text2.setScale(0.8,0.8);
        text2.setOutlineColor(Color::Yellow);
        text2.setOutlineThickness(3.0f);
        text2.setFillColor(Color::Blue);
        

        Clock clk , clk2;
        float time = 0;        
        siren.play();
        siren.setLoop(true);
        
        while (window.isOpen())
        {

            window.clear();
            Event event;
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    siren.stop();
                    eat1.stop();
                    eat2.stop();
                    shared->gameOver = true;
                    window.close();
                }
            }

            pthread_mutex_lock(&mut);
            int input = pacman->getInput(shared->userInput);
            pthread_mutex_unlock(&mut);


            if (clk.getElapsedTime().asSeconds() > 0.1) // delay for player movement
            {
                if(!validate_move(input))
                    validate_move(pacman->direction);
                                 
                clk.restart();
            }

            if (clk2.getElapsedTime().asSeconds() > 0.125) // delay for player movement
            {
                blinky->update(pacman->position.x , pacman->position.y , pacman->direction);
                pinky->update(pacman->position.x , pacman->position.y , pacman->direction);   
                inky->update(pacman->position.x , pacman->position.y , pacman->direction);     
                clyde->update(pacman->position.x , pacman->position.y , pacman->direction);      

                clk2.restart();
            }


            text.setString("SCORE: " + to_string(pacman->score));
            //graphicsRenderer->drawMaze(window);
            graphicsRenderer->drawMap(window);
            graphicsRenderer->drawFood(window,food);
            graphicsRenderer->drawPacMan(window,pacman->sprite,pacman->position.x , pacman->position.y,pacman->direction);
            graphicsRenderer->drawGhost(window, blinky->sprite,blinky->position.x , blinky->position.y);
            graphicsRenderer->drawGhost(window, pinky->sprite,pinky->position.x , pinky->position.y);
            graphicsRenderer->drawGhost(window, inky->sprite,inky->position.x , inky->position.y);
            graphicsRenderer->drawGhost(window, clyde->sprite,clyde->position.x , clyde->position.y);
            window.draw(text);
            window.draw(text2);
            window.draw(logo);
            window.display();
        }
    }

    private:
    
    bool validate_move(int input)
    {
        int originalX = pacman->position.x;
        int originalY = pacman->position.y;
        int nextY = pacman->position.y;
        int nextX = pacman->position.x;

        bool canMove = true;

        if(originalY == 22 && originalX == 0 && input == 3)
        {
            nextX  = 44;
        }

        else if (originalY == 22 && originalX == 44 && input == 1)
        {
            nextX = 0;
        }

        else
        {
    
            if (input == 0) 
            {
                nextY -= speed;
            } else if (input == 1) 
            {
                nextX += speed;
            } else if (input == 2) 
            {
                nextY += speed;
            } else if (input == 3) 
            {
                nextX -= speed;
            }

            for (int i = 0; i < 4; ++i) 
            {
                int cornerX = nextX;
                int cornerY = nextY;

                if (i == 1 || i == 2) 
                {
                    cornerX += 1; // Adjust for right corners
                }
                if (i == 2 || i == 3) 
                {
                    cornerY += 1; // Adjust for bottom corners
                }

                // Check collision with the next cell for each corner
                if (this->checkCollision(cornerX, cornerY)) 
                {
                    canMove = false;
                    break;
                }
            }
        }

        if (canMove) 
        {
            pacman->setDirection(input);
            pacman->position.x = nextX;
            pacman->position.y = nextY;

            if(shared->gameBoard[nextY][nextX] == 3) 
            {
                eat1.stop();
                eat2.stop();
                eat1.play();
                eat2.play();
                pacman->score = pacman->score + 1;
            }
            shared->gameBoard[nextY][nextX] = 2; // 0 empty space , 3 means food 
            shared->gameBoard[originalY][originalX] = 0; // 0 empty space , 3 means food 

            pacman->sprite.setPosition(pacman->position);


        }
        return canMove;
    }

    void initializeFood()
    {
        for (int i = 0; i < shared->ROWS; i+=2) 
        {
            for (int j = 0; j < shared->COLS; j+=2) 
            {
                if (shared->gameBoard[i][j] == 0) 
                {
                    shared->gameBoard[i][j] = 3; // 3 for food
                }
            }
        }

    }

    // Function to check collision at a given position
    bool checkCollision(int x, int y) 
    {
        // Check if the position is within the boundaries of the game board
        if (x >= 0 && x < shared->COLS && y >= 0 && y < shared->ROWS) 
        {
            if(shared->gameBoard[y][x] == 0 || shared->gameBoard[y][x] == 3 || shared->gameBoard[y][x] == 2 )
                return false;
        }
        // If the position is outside the game board, consider it a collision
        return true;
    }

    bool checkCollisionGhost(int x, int y) 
    {
        // Check if the position is within the boundaries of the game board
        if (x >= 0 && x < shared->COLS && y >= 0 && y < shared->ROWS) 
        {
            if(shared->gameBoard[y][x] != 1 && shared->gameBoard[y][x] != 2)
                return false;
        }
        // If the position is outside the game board, consider it a collision
        return true;
    }

};
#endif