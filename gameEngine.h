#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include "gameboard.h"
#include "pacman.h"
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
    GameBoard *gameboard;
    Pacman *pacman = new Pacman;
    // create 4 ghosts here
    float speed = 1;

    Sprite food;
    Texture tex;

    Music siren , eat1,eat2;
   
    GameEngine(void *&arg)
    {
        shared = (SharedVariables*)arg;
        gameboard = new GameBoard;
        tex.loadFromFile("img/other/dot.png");
        food.setTexture(tex);
        food.setScale(1.5,1.5);
        gameBoard[(int)pacman->position.y][(int)pacman->position.x] = 2;
        siren.openFromFile("sounds/siren.wav");
        eat2.openFromFile("sounds/eat.wav");
        eat1.openFromFile("sounds/PelletEat2.wav");

        eat1.setVolume(50.f);
        eat2.setVolume(50.f);

    }
    
    void start_game()
    {
        RenderWindow window(VideoMode(695,900), "OS PROJECT");

        Text text;
        sf::Font font;
        font.loadFromFile("font.ttf");
        text.setFont(font);
        text.setPosition(Vector2f(270,850));

        Clock clk;
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


            if (clk.getElapsedTime().asSeconds() > 0.15) // delay for player movement
            {
                if(!validate_move(input))
                    validate_move(pacman->direction);
                 
                clk.restart();

            }

            text.setString("SCORE: " + to_string(pacman->score));
            gameboard->drawMaze(window);
            window.draw(gameboard->sprite);
            gameboard->drawFood(window,food);
            gameboard->drawPacMan(window,pacman->sprite,pacman->position.x , pacman->position.y);
            window.draw(text);
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
                if (gameboard->checkCollision(cornerX, cornerY)) 
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

            if(gameBoard[nextY][nextX] == 3) 
            {
                eat1.stop();
                eat2.stop();
                eat1.play();
                eat2.play();
                pacman->score = pacman->score + 1;
            }
            gameBoard[nextY][nextX] = 2; // 0 empty space , 3 means food 
            gameBoard[originalY][originalX] = 0; // 0 empty space , 3 means food 
           
            pacman->sprite.setPosition(pacman->position);

        }
        return canMove;
    }

};
#endif