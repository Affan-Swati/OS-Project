#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include "graphicsrenderer.h"
#include "pacman.h"
#include "ghosts/blinky.h"
#include "ghosts/pinky.h"
#include "ghosts/inky.h"
#include "ghosts/clyde.h"
#include "sharedvariables.h"
#include <unistd.h>
#include <pthread.h>


//pthread_mutex_t mut = PTHREAD_MUTEX_INITIALIZER;

using namespace std;
using namespace sf;

class GameEngine
{
    private:
    
    SharedVariables *shared;
    GraphicsRenderer *graphicsRenderer;
    Pacman *pacman;
    Ghost *blinky , *pinky ,*inky ,*clyde; // polymorphism 
    float speed;

    Sprite food , logo;
    Texture tex , tex_logo;

    Music siren , eat1,eat2  , eatPower , frightenSound ,homeRunningSound;
    Clock frightenClock;
   
    bool frightenStart , frightenEnd;
    vector<pair<int,int>> frightenPallets;
    Clock frightenPalletsClocks[4];

    public:

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
        tex.loadFromFile("../resources/img/other/dot.png");
        tex_logo.loadFromFile("../resources/img/other/logo.png");
        food.setTexture(tex);
        food.setScale(1.5,1.5);
        logo.setTexture(tex_logo);
        logo.setPosition(208,750);
        logo.setScale(0.1,0.1);


        shared->gameBoard[(int)pacman->position.y][(int)pacman->position.x] = 2;
        siren.openFromFile("../resources/sounds/siren.wav");
        eat2.openFromFile("../resources/sounds/eat.wav");
        eat1.openFromFile("../resources/sounds/PelletEat2.wav");
        frightenSound.openFromFile("../resources/sounds/blue.wav");
        homeRunningSound.openFromFile("../resources/sounds/homerunning.wav");
        //frightenSound.setVolume(3);

        eat1.setVolume(50.f);
        eat2.setVolume(50.f);

        frightenStart = false;
        frightenEnd = false;

    }
    
    void startAnimation(RenderWindow &window)
    {
        Clock animationClock;
        Music animationMusic;
        animationMusic.openFromFile("../resources/sounds/intro.wav");
        animationClock.restart();

        animationMusic.setVolume(100);
        animationMusic.play();
        animationClock.restart();
        while(animationClock.getElapsedTime().asSeconds() < 4)
        {
            window.clear();
            graphicsRenderer->drawMaze(window);
            graphicsRenderer->drawMap(window);
            graphicsRenderer->drawFood(window,frightenPallets);
            pacman->getInput('s');
            graphicsRenderer->drawPacMan(window,pacman->sprite,pacman->position.x , pacman->position.y,pacman->direction);

            if(animationClock.getElapsedTime().asSeconds() < 1)
            {
                blinky->updateTexture(3);
                pinky->updateTexture(0);
                inky->updateTexture(1);
                clyde->updateTexture(2);
            }
            else if(animationClock.getElapsedTime().asSeconds() < 2)
            {
                blinky->updateTexture(2);
                pinky->updateTexture(1);
                inky->updateTexture(2);
                clyde->updateTexture(3);
            }
            else if(animationClock.getElapsedTime().asSeconds() < 3)
            {
                blinky->updateTexture(1);
                pinky->updateTexture(2);
                inky->updateTexture(3);
                clyde->updateTexture(0);
            } 
            else
            {
                blinky->updateTexture(0);
                pinky->updateTexture(3);
                inky->updateTexture(0);
                clyde->updateTexture(1);
            }

            graphicsRenderer->drawGhost(window, blinky->sprite,shared->blinkyPos.first.x ,shared->blinkyPos.first.y);
            graphicsRenderer->drawGhost(window, pinky->sprite,shared->pinkyPos.first.x , shared->pinkyPos.first.y);
            graphicsRenderer->drawGhost(window, inky->sprite,shared->inkyPos.first.x , shared->inkyPos.first.y);
            graphicsRenderer->drawGhost(window, clyde->sprite,shared->clydePos.first.x , shared->clydePos.first.y); 
            window.draw(logo);
            graphicsRenderer->drawLives(window,pacman->lives);
            window.display();  
        }
        animationMusic.stop();
    }

    void start_game()
    {
        RenderWindow window(VideoMode(695,900), "OS PROJECT");

        Text text , text2;
        sf::Font font;
        font.loadFromFile("../resources/font.ttf");
        text.setFont(font);
        text.setPosition(Vector2f(295,850));
        text.setOutlineColor(Color::Blue);
        text.setOutlineThickness(2.0f);
        text.setFillColor(Color::Yellow);

        text2.setFont(font);
        text2.setPosition(Vector2f(240,430));
        text2.setString("WHAT THE SIGMA");
        text2.setScale(0.8,0.8);
        text2.setOutlineColor(Color::Yellow);
        text2.setOutlineThickness(3.0f);
        text2.setFillColor(Color::Blue);
        
        startAnimation(window);

        // 4 for ghosts , 1 for UI thread
        sem_post(&shared->gameStarted);
        sem_post(&shared->gameStarted);
        sem_post(&shared->gameStarted); 
        sem_post(&shared->gameStarted); 
        sem_post(&shared->gameStarted); 

        Clock clk;
        float time = 0;        
        siren.play();
        siren.setLoop(true);
        
        while (window.isOpen())
        {
            window.clear();
            Event event;

            if(pacman->lives == 0)
            {
                siren.stop();
                eat1.stop();
                eat2.stop();
                frightenSound.stop();
                homeRunningSound.stop();
                eatPower.stop();
                shared->gameOver = true;
                window.close();
            }
            while (window.pollEvent(event))
            {
                if (event.type == Event::Closed)
                {
                    siren.stop();
                    eat1.stop();
                    eat2.stop();
                    frightenSound.stop();
                    homeRunningSound.stop();
                    shared->gameOver = true;
                    window.close();
                    return;
                }
            }

            int input = pacman->getInput(shared->userInput);

            if (clk.getElapsedTime().asSeconds() > 0.08) // delay for player movement
            {
                if(!validate_move(input))
                    validate_move(pacman->direction);
                                 
                clk.restart();
            }

            blinky->isEaten(pacman->sprite);
            pinky->isEaten(pacman->sprite);
            inky->isEaten(pacman->sprite);
            clyde->isEaten(pacman->sprite);

            if(isAnyMode(3) && homeRunningSound.getStatus() != SoundSource::Playing)
            {
                homeRunningSound.setVolume(500);
                homeRunningSound.play();
                homeRunningSound.setLoop(true);
            }

            else if(!isAnyMode(3) && homeRunningSound.getStatus() == SoundSource::Playing )
                homeRunningSound.stop();

            pacCollisionWithGhost(window);
            text.setString("SCORE: " + to_string(pacman->score));
            graphicsRenderer->drawMaze(window);
            graphicsRenderer->drawMap(window);
            graphicsRenderer->drawFood(window , frightenPallets);
            graphicsRenderer->drawPacMan(window,pacman->sprite,pacman->position.x , pacman->position.y,pacman->direction);
            animateGhosts();
            graphicsRenderer->drawGhost(window, blinky->sprite,shared->blinkyPos.first.x ,shared->blinkyPos.first.y);
            graphicsRenderer->drawGhost(window, pinky->sprite,shared->pinkyPos.first.x , shared->pinkyPos.first.y);
            graphicsRenderer->drawGhost(window, inky->sprite,shared->inkyPos.first.x , shared->inkyPos.first.y);
            graphicsRenderer->drawGhost(window, clyde->sprite,shared->clydePos.first.x , shared->clydePos.first.y);
            window.draw(text);
            //window.draw(text2);
            window.draw(logo);
            graphicsRenderer->drawLives(window,pacman->lives);
            window.display();
        }
    }

    private:

    void animateGhosts()
    {
       blinky->updateTexture(determineDirection(shared->blinkyPos));
       pinky->updateTexture(determineDirection(shared->pinkyPos));
       inky->updateTexture(determineDirection(shared->inkyPos));
       clyde->updateTexture(determineDirection(shared->clydePos));
    }

    void pacCollisionWithGhost(RenderWindow &window)
    {
        if(blinky->eatsPac(pacman->sprite) || pinky->eatsPac(pacman->sprite) || inky->eatsPac(pacman->sprite) || clyde->eatsPac(pacman->sprite))
        {
            pthread_mutex_lock(&shared->mutex);
            shared->gameReset = true;
            pacman->lives -= 1;
            int x = shared->pacPos.x;
            int y = shared->pacPos.y;
            siren.stop();
            if(homeRunningSound.getStatus() == SoundStream :: Playing)
                homeRunningSound.stop();

            graphicsRenderer->pacDeathAnimation(x,y,window,logo,pacman->lives,frightenPallets);
            shared->gameBoard[(int)shared->pacPos.y][(int)shared->pacPos.x] = 0;
            shared->pacDirection  = 3;
            pacman->setDirection(3);
            shared->pacPos = Vector2f(17,36);
            pacman->position = Vector2f(17,36);
            shared->gameReset = true;
            // first is currentPos , second is previousPos
            shared->blinkyPos.first = Vector2f(18,22); shared->blinkyPos.second = Vector2f(18,22); 
            shared->pinkyPos.first = Vector2f(20,22); shared->pinkyPos.second = Vector2f(20,22);
            shared->inkyPos.first  = Vector2f(24,22); shared->inkyPos.second = Vector2f(24,22);
            shared->clydePos.first = Vector2f(26,22); shared->clydePos.second = Vector2f(26,22);
            shared->ghostState = 0; // 0 or 1
            shared->mode[0] = 0;  shared->mode[1] = 0; shared->mode[2] = 0; shared->mode[3] = 0;// 0 chase , 1 scatter , 2 frighten , 3 eaten
            pthread_mutex_unlock(&shared->mutex);

            if(pacman->lives == 0)

                shared->gameOver = true;
            
            else
            {
                startAnimation(window);
                siren.play();
                siren.setLoop(true);
            }

            shared->gameReset = false;

            // 4 for ghosts , 1 for UI thread
            sem_post(&shared->gameReset2);
            sem_post(&shared->gameReset2); 
            sem_post(&shared->gameReset2); 
            sem_post(&shared->gameReset2); 
            sem_post(&shared->gameReset2); 
        }
    }

    int determineDirection(pair<Vector2f,Vector2f> &position)
    {
        if(position.first.x < position.second.x)
            return 3;
        else if(position.first.x > position.second.x)
            return 1;
        else if(position.first.y > position.second.y)
            return 2;
        else 
            return 0;
    }

    void checkRespawnPallets()
    {
        if(frightenPallets[0].second == -1 && frightenPalletsClocks[0].getElapsedTime().asSeconds() > 20)
        {
            frightenPallets[0].second = 2;
        }

        if(frightenPallets[1].second == -1 && frightenPalletsClocks[1].getElapsedTime().asSeconds() > 20)
        {
            frightenPallets[1].second = 42;
        }

        if(frightenPallets[2].second == -1 && frightenPalletsClocks[2].getElapsedTime().asSeconds() > 20)
        {
            frightenPallets[2].second = 42;
        }

        if(frightenPallets[3].second == -1 && frightenPalletsClocks[3].getElapsedTime().asSeconds() > 20)
        {
            frightenPallets[3].second = 2;
        }
    }

    void checkFrightenPallets()
    {
        checkRespawnPallets();
        int originalY = shared->pacPos.y;
        int originalX = shared->pacPos.x;

        if(originalY == 5)
            {
                if(originalX == frightenPallets[1].second)
                {
                    frightenPallets[1].second = -1;
                    frightenPalletsClocks[1].restart();
                    setAllMode(2);
                    frightenStart = true;
                    siren.stop();
                    resetGhostClocks();
                    frightenSound.setLoop(true);
                    frightenSound.play();
                    frightenClock.restart();
                }

                else if(originalX == frightenPallets[0].second)
                {
                    frightenPallets[0].second = -1;
                    frightenPalletsClocks[0].restart();
                    setAllMode(2);
                    frightenStart = true;
                    siren.stop();
                    resetGhostClocks();
                    frightenSound.setLoop(true);
                    frightenSound.play();
                    frightenClock.restart();
                }
            }

            else if(originalY == 34)
            {
                if(originalX== frightenPallets[2].second)
                {
                    frightenPallets[2].second = -1;
                    frightenPalletsClocks[2].restart();
                    setAllMode(2);
                    frightenStart = true;
                    siren.stop();
                    resetGhostClocks();
                    frightenSound.setLoop(true);
                    frightenSound.play();
                    frightenClock.restart();
                }

                else if(originalX == frightenPallets[3].second)
                {
                    frightenPallets[3].second = -1;
                    frightenPalletsClocks[3].restart();
                    setAllMode(2);
                    frightenStart = true;
                    siren.stop();
                    resetGhostClocks();
                    frightenSound.setLoop(true);
                    frightenSound.play();
                    frightenClock.restart();
                }
            }

            if(frightenStart && frightenClock.getElapsedTime().asSeconds() > 6 && !frightenEnd)
            {
                frightenEnd = true;
                frightenStart = false;
            }

            if(frightenEnd)
            {
                frightenEnd = false;
                setAllMode(0);
                siren.play();
                siren.setLoop(true);
                frightenSound.stop();
            }

    }

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
            pthread_mutex_lock(&shared->mutex);
            pacman->setDirection(input);
            shared->pacDirection = input;
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

            checkFrightenPallets();

            pacman->sprite.setPosition(pacman->position);
            shared->pacPos = pacman->position;
            pthread_mutex_unlock(&shared->mutex);
        }
        return canMove;
    }

    void initializeFood()
    {
        for (int i = 0; i < shared->ROWS; i+=2) 
        {
            for (int j = 0; j < shared->COLS; j+=2) 
            {
                 if((i == 4 || i == 5 || i == 6) && (j == 2 || j == 42 ))
                        continue;

                if (shared->gameBoard[i][j] == 0) 
                {
                    shared->gameBoard[i][j] = 3; // 3 for food
                }
            }
        }

        //233 total food
        frightenPallets.push_back(make_pair(5,2));
        frightenPallets.push_back(make_pair(5,42));
        frightenPallets.push_back(make_pair(34,42));
        frightenPallets.push_back(make_pair(34,2));
    }

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

    void setAllMode(int mode)
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            if(shared->mode[i] != 3)
            shared->mode[i] = mode;
        }
    }

    bool isAllMode(int mode)
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            if(shared->mode[i] != mode)
                return false;
        }

        return true;
    }

    bool isAnyMode(int mode)
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            if(shared->mode[i] == mode)
                return true;
        }

        return false;
    }

    void resetGhostClocks()
    {
        blinky->blink.restart();
        pinky->blink.restart();
        inky->blink.restart();
        clyde->blink.restart();
    }
};
#endif