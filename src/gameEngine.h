#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <time.h>
#include <string>
#include "graphicsrenderer.h"
#include "pacman.h"
#include "ghosts/blinky.h"
#include "ghosts/pinky.h"
#include "ghosts/inky.h"
#include "ghosts/clyde.h"
#include "fruit.h"
#include "sharedvariables.h"
#include "menu.h"
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
    Fruit *cherry;
    float speed;

    Text text , text2;
    sf::Font font;
    Sprite food , logo;
    Texture tex , tex_logo , ghostSheet;

    Sprite scoreSprite[4];
    Texture scoreTex[4]; // 200 ,400 , 800 , 1600

    Music siren , eat1,eat2  , eatPower , frightenSound ,homeRunningSound , menuMusic;
    Clock frightenClock;
    Clock scatterClock;
    Clock chaseClock;
    Clock clk;
   
    bool frightenStart , frightenEnd;
    vector<pair<int,int>> frightenPallets;
    Clock frightenPalletsClocks[4];
    int ghostEaten; // tracks ghost eaten during frighten state
    int foodEaten;
    string name;

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
        cherry = new Fruit(arg,0,22,26); // 0 means cherry , 1 means strawberry , 2 means mango , 3 means apple

        tex.loadFromFile("../resources/img/other/dot.png");
        tex_logo.loadFromFile("../resources/img/other/logo.png");
        ghostSheet.loadFromFile(".././resources/img/ghosts/ghost_sheet.png");
        scoreTex[0].loadFromImage(ghostSheet.copyToImage(), (IntRect)FloatRect(0 * 15.83  , 4 * 15 + 3,    15.83 , 15));
        scoreTex[1].loadFromImage(ghostSheet.copyToImage(), (IntRect)FloatRect(1 * 15.83 + 1 , 4 * 15 + 3, 17 , 15));
        scoreTex[2].loadFromImage(ghostSheet.copyToImage(), (IntRect)FloatRect(2 * 15.83 + 1 , 4 * 15 + 3, 17 , 15));
        scoreTex[3].loadFromImage(ghostSheet.copyToImage(), (IntRect)FloatRect(3 * 15.83 + 1 , 4 * 15 + 3, 18 , 15));
        siren.openFromFile("../resources/sounds/siren.wav");
        eat2.openFromFile("../resources/sounds/eat.wav");
        eat1.openFromFile("../resources/sounds/PelletEat2.wav");
        frightenSound.openFromFile("../resources/sounds/blue.wav");
        homeRunningSound.openFromFile("../resources/sounds/homerunning.wav");
        menuMusic.openFromFile("../resources/sounds/menu.wav");

        food.setTexture(tex);
        food.setScale(1.5,1.5);
        logo.setTexture(tex_logo);
        logo.setPosition(212,750);
        logo.setScale(0.1,0.1);
        eat1.setVolume(50.f);
        eat2.setVolume(50.f);
        menuMusic.setLoop(true);
        menuMusic.setVolume(70);
        text.setString("Score:0");

        for(int i = 0 ; i < 4 ; i++)
        {
            scoreSprite[i].setTexture(scoreTex[i]);
            scoreSprite[i].setScale(2,2);
        }
        

        this->speed = 1;
        this->initializeFood();
        shared->gameBoard[(int)pacman->position.y][(int)pacman->position.x] = 2;
        this->frightenStart = false;
        this->frightenEnd = false;
        this->foodEaten = 0;
        this->ghostEaten = 0;

    }
    

    void ghostEatenAnimation(RenderWindow &window , int ghost)
    {
        Clock animationClock;
        animationClock.restart();

        shared->animation = true;
        pthread_mutex_lock(&shared->mutex);
        while(animationClock.getElapsedTime().asSeconds() < 0.7)
        {
            window.clear();
            graphicsRenderer->drawMaze(window);
            graphicsRenderer->drawMap(window);
            graphicsRenderer->drawFood(window,frightenPallets);
            
           
            if(ghost != 0)
            {
                graphicsRenderer->drawGhost(window, blinky->sprite,shared->blinkyPos.first.x ,shared->blinkyPos.first.y);
            }

            else
            {
                graphicsRenderer->drawGhost(window,scoreSprite[ghostEaten - 1],shared->blinkyPos.first.x ,shared->blinkyPos.first.y);
            }

            if(ghost != 1)
            {
                graphicsRenderer->drawGhost(window, pinky->sprite,shared->pinkyPos.first.x ,shared->pinkyPos.first.y);
            }

            else
            {
                graphicsRenderer->drawGhost(window,scoreSprite[ghostEaten - 1],shared->pinkyPos.first.x ,shared->pinkyPos.first.y);
            }

            if(ghost != 2)
            {
                graphicsRenderer->drawGhost(window, inky->sprite,shared->inkyPos.first.x ,shared->inkyPos.first.y);
            }

            else
            {
                graphicsRenderer->drawGhost(window,scoreSprite[ghostEaten - 1],shared->inkyPos.first.x ,shared->inkyPos.first.y);
            }

            if(ghost != 3)
            {
                graphicsRenderer->drawGhost(window, clyde->sprite,shared->clydePos.first.x ,shared->clydePos.first.y);
            }

            else
            {
                graphicsRenderer->drawGhost(window,scoreSprite[ghostEaten - 1],shared->clydePos.first.x ,shared->clydePos.first.y);
            }

            graphicsRenderer->drawGhostSpeedBoosts(window);

            
            if(cherry->isActive)
                window.draw(cherry->sprite);           
            graphicsRenderer->drawLives(window,pacman->lives);
            window.draw(text);
            window.draw(text2);
            window.display();  
        }

        shared->animation = false;
        pthread_mutex_unlock(&shared->mutex);
        sem_post(&shared->animation2);
        sem_post(&shared->animation2);
        sem_post(&shared->animation2);
        sem_post(&shared->animation2);
        sem_post(&shared->animation2);
    }

    void startAnimation(RenderWindow &window)
    {
        Clock animationClock;
        Music animationMusic;
        animationMusic.openFromFile("../resources/sounds/intro.wav");

        siren.stop();
        frightenSound.stop();
        homeRunningSound.stop();
        animationMusic.setVolume(100);
        animationMusic.play();
        animationClock.restart();

        for(int i = 0 ; i < 4 ; i++)
            shared->key_perm[i].restart();

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
            graphicsRenderer->drawLives(window,pacman->lives);
            graphicsRenderer->drawGhostSpeedBoosts(window);
            if(cherry->isActive)
                window.draw(cherry->sprite);
            window.draw(text);
            window.draw(text2);
            window.display();  
        }
        animationMusic.stop();
    }

    int displayMenu(RenderWindow &window)
    {
        Music menuAudio;
        Menu menu(window);
        int menu_option = 0;
        int eventRet;
        Clock animationClock;
        Clock opacityClock;
        Sprite dummy;
        dummy.setScale(2,2);

        menuAudio.openFromFile("../resources/sounds/hover.wav");
        RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(Color(0, 0, 0, 170)); // Set the overlay color with alpha (transparency)
        window.setMouseCursorVisible(false);

        while (menu_option != 1 && menu_option != 4) 
        {
            Event event;
            while (window.pollEvent(event)) 
            {
                if (event.type == Event::Closed) 
                {
                    window.close();
                    return 4;
                }

                eventRet = menu.handleEvent(event);
                if(eventRet != 0)
                    menu_option = eventRet;
            }

            graphicsRenderer->drawMaze(window);
            graphicsRenderer->drawMap(window);
            graphicsRenderer->drawFood(window,frightenPallets);
            pacman->getInput('s');
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
            else if(animationClock.getElapsedTime().asSeconds() < 4)
            {
                blinky->updateTexture(0);
                pinky->updateTexture(3);
                inky->updateTexture(0);
                clyde->updateTexture(1);
            }
            else
                animationClock.restart();

            
            window.draw(overlay);

            dummy = pinky->sprite;
            dummy.setScale(2.5,2.5);
            graphicsRenderer->drawGhost(window, dummy, 18 ,37);

            dummy = inky->sprite;
            dummy.setScale(2.5,2.5);
            graphicsRenderer->drawGhost(window, dummy,  21 ,37);

            dummy = blinky->sprite;
            dummy.setScale(2.5,2.5);
            graphicsRenderer->drawGhost(window, dummy,24 ,37);

            dummy = clyde->sprite;
            dummy.setScale(2.5,2.5);
            graphicsRenderer->drawGhost(window, dummy, 27 ,37);

            dummy = pacman->sprite;
            dummy.setScale(2.2,2.2);
            dummy.setPosition(340,600);
            window.draw(dummy);


            menu.draw(window);
            logo.setPosition(200,100);
            window.draw(logo);

            dummy.setPosition(Mouse::getPosition(window).x ,Mouse::getPosition(window).y);
            dummy.setScale(2,2);

            window.draw(dummy);

            window.display();  

            if(menu_option == 1 || menu_option == 4)
            {
                return menu_option;
            }
       }

       return menu_option;

    }

    void getNameInput(RenderWindow &window)
    {
        Event e;   
        sf::Font font;
        font.loadFromFile("../resources/font.ttf");

        Text text3;
        text3.setFont(font);
        text3.setFillColor(Color::Yellow);
        text3.setCharacterSize(40);
        text3.setPosition(150, 350);
        text3.setString("Your name: ");
        text3.setScale(0.5,0.5);

        Clock animationClock;
        Sprite dummy;

        string playerName = "";
        bool isEnteringName = true;

        RectangleShape overlay(Vector2f(window.getSize().x, window.getSize().y));
        overlay.setFillColor(Color(0, 0, 0, 170)); // Set the overlay color with alpha (transparency)

        while (isEnteringName)
        {
            while (window.pollEvent(e))
            {
                if (e.type == Event::Closed)
                {
                    window.close();
                    isEnteringName = false;
                    return;
                }
                else if (e.type == Event::TextEntered)
                {
                    if (e.text.unicode >= 32 && e.text.unicode < 128)
                    {
                        playerName += static_cast<char>(e.text.unicode);
                        text3.setString("Your name: " + playerName);
                    }
                    else if (e.text.unicode == 8 && playerName.size() > 0)
                    {
                        playerName.pop_back();
                        text3.setString("Your name: " + playerName);
                    }
                    else if (e.text.unicode == 13 && playerName.size() > 0)
                    {
                        isEnteringName = false;
                    }
                }
            }

                pacman->getInput('s');
                graphicsRenderer->drawMaze(window);
                graphicsRenderer->drawMap(window);
                graphicsRenderer->drawFood(window,frightenPallets);

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
                else if(animationClock.getElapsedTime().asSeconds() < 4)
                {
                    blinky->updateTexture(0);
                    pinky->updateTexture(3);
                    inky->updateTexture(0);
                    clyde->updateTexture(1);
                }
                else
                    animationClock.restart();

                
                window.draw(overlay);

                dummy = pinky->sprite;
                dummy.setScale(2.5,2.5);
                graphicsRenderer->drawGhost(window, dummy, 18 ,37);

                dummy = inky->sprite;
                dummy.setScale(2.5,2.5);
                graphicsRenderer->drawGhost(window, dummy,  21 ,37);

                dummy = blinky->sprite;
                dummy.setScale(2.5,2.5);
                graphicsRenderer->drawGhost(window, dummy,24 ,37);

                dummy = clyde->sprite;
                dummy.setScale(2.5,2.5);
                graphicsRenderer->drawGhost(window, dummy, 27 ,37);

                dummy = pacman->sprite;
                dummy.setScale(2.2,2.2);
                dummy.setPosition(340,600);
                window.draw(dummy);

 
                window.draw(text3);
                window.display();
            }
            name = playerName;
    }

    void start_game()
    {
        RenderWindow window(VideoMode(695,900), "Pacman OS Project");
        window.setVerticalSyncEnabled(true);
        // window.setFramerateLimit(144);
        font.loadFromFile("../resources/font.ttf");
        text.setFont(font);
        text.setPosition(Vector2f(265,760));
        text.setOutlineColor(Color::White);
        text.setScale(0.7,0.7);
        text2.setFont(font);
        text2.setScale(0.6,0.6);
        text2.setFillColor(Color::Blue);
        
        menuMusic.play();
        if(displayMenu(window) == 4)
        {   
            menuMusic.stop();
            window.close();
            sem_post(&shared->gameStarted);
            sem_post(&shared->gameStarted);
            sem_post(&shared->gameStarted); 
            sem_post(&shared->gameStarted); 
            sem_post(&shared->gameStarted); 
            shared->gameOver = true;
            return;
        }

        window.setMouseCursorVisible(true);
        getNameInput(window);
        text2.setString(name);
        text2.setOrigin(text2.getGlobalBounds().width / 2 , text2.getGlobalBounds().height / 2);
        text2.setPosition(Vector2f(330,850));
        menuMusic.stop();

        startAnimation(window);
        scatterClock.restart();

        // 4 for ghosts , 1 for UI thread
        sem_post(&shared->gameStarted);
        sem_post(&shared->gameStarted);
        sem_post(&shared->gameStarted); 
        sem_post(&shared->gameStarted); 
        sem_post(&shared->gameStarted); 

        float time = 0;        
        siren.play();
        siren.setLoop(true);
        scatterClock.restart();

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
                    frightenSound.stop();
                    homeRunningSound.stop();
                    shared->gameOver = true;
                    window.close();
                    return;
                }
            }

            if(pacman->lives == 0 || foodEaten == 233)
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

            alternateGhostModes();

            updatePacman();

            checkFrightenPallets();

            checkGhostEaten(window);

            pacCollisionWithGhost(window);


            if(isAnyMode(3) && homeRunningSound.getStatus() != SoundSource::Playing)
            {
                homeRunningSound.setVolume(500);
                homeRunningSound.play();
                homeRunningSound.setLoop(true);
            }

            else if(!isAnyMode(3) && homeRunningSound.getStatus() == SoundSource::Playing )
                homeRunningSound.stop();

            text.setString("SCORE:" + to_string(pacman->score));
            graphicsRenderer->drawMaze(window);
            graphicsRenderer->drawMap(window);
            graphicsRenderer->drawFood(window , frightenPallets);
            if(cherry->isActive)
                window.draw(cherry->sprite);

            graphicsRenderer->drawPacMan(window,pacman->sprite,pacman->position.x , pacman->position.y,pacman->direction);
            animateGhosts();
            graphicsRenderer->drawGhost(window, blinky->sprite,shared->blinkyPos.first.x ,shared->blinkyPos.first.y);
            graphicsRenderer->drawGhost(window, pinky->sprite,shared->pinkyPos.first.x , shared->pinkyPos.first.y);
            graphicsRenderer->drawGhost(window, inky->sprite,shared->inkyPos.first.x , shared->inkyPos.first.y);
            graphicsRenderer->drawGhost(window, clyde->sprite,shared->clydePos.first.x , shared->clydePos.first.y);
            
            if(cherry->checkCollision())
                pacman->score = pacman->score + 100;

            window.draw(text);
            window.draw(text2);
            //window.draw(logo);
            graphicsRenderer->drawLives(window,pacman->lives);
            graphicsRenderer->drawGhostSpeedBoosts(window);

            if(shared->gamePaused)
            {
                pauseGame(window);
            }

            window.display();
        }
    }

    private:
   
   void pauseGame(RenderWindow& window)
   {
        Text text3;
        text3.setFont(font);
        text3.setString("GAME PAUSED");
        text3.setFillColor(Color::Red);
        text3.setPosition(210 , 425);
        text3.setScale(0.8,0.8);

        window.draw(text3);
        window.display();
        
        sem_wait(&shared->gamePaused2);
   }
   
    void animateGhosts()
    {
       blinky->updateTexture(determineDirection(shared->blinkyPos));
       pinky->updateTexture(determineDirection(shared->pinkyPos));
       inky->updateTexture(determineDirection(shared->inkyPos));
       clyde->updateTexture(determineDirection(shared->clydePos));
    }

    void checkGhostEaten(RenderWindow &window)
    {
        if(blinky->isEaten(pacman->sprite))
            {
                ghostEaten++;
                ghostEatenAnimation(window,0);
                pacman->score = pacman->score + (pow(2,ghostEaten) * 100);
            }

            if(pinky->isEaten(pacman->sprite))
            {
                ghostEaten++;
                ghostEatenAnimation(window,1);
                pacman->score = pacman->score + (pow(2,ghostEaten) * 100);
            }

            if(inky->isEaten(pacman->sprite))
            {
                ghostEaten++;
                ghostEatenAnimation(window,2);
                pacman->score = pacman->score + (pow(2,ghostEaten) * 100);
            }

            if(clyde->isEaten(pacman->sprite))
            {
                ghostEaten++;
                ghostEatenAnimation(window,3);
                pacman->score = pacman->score + (pow(2,ghostEaten) * 100);
            }
    }
    
    void pacCollisionWithGhost(RenderWindow &window)
    {
        if(blinky->eatsPac(pacman->sprite) || pinky->eatsPac(pacman->sprite) || inky->eatsPac(pacman->sprite) || clyde->eatsPac(pacman->sprite))
        {
            shared->gameReset = true;
            pthread_mutex_lock(&shared->mutex);       
            pacman->lives -= 1;
            int x = shared->pacPos.x;
            int y = shared->pacPos.y;
            siren.stop();
            frightenSound.stop();
            if(homeRunningSound.getStatus() == SoundStream :: Playing)
                homeRunningSound.stop();

            graphicsRenderer->pacDeathAnimation(x,y,window,pacman->lives,frightenPallets);
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
            shared->allowedToLeave[0] = false;shared->allowedToLeave[1] = false;shared->allowedToLeave[2] = false;shared->allowedToLeave[3] = false;
            shared->inHouse[0] = true;shared->inHouse[1] = true;shared->inHouse[2] = true;shared->inHouse[3] = true;
            shared->takenSpeedBoosts[0] = false;shared->takenSpeedBoosts[1] = false;shared->takenSpeedBoosts[2] = false;shared->takenSpeedBoosts[3] = false;
            shared->speedBoosts[0] = true;shared->speedBoosts[1] = true;
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
   
    void alternateGhostModes()
    {
        // check switch between scatter and chase modes
        if(scatterClock.getElapsedTime().asSeconds() > 7 && isAllMode(1))
        {
            chaseClock.restart();
            setAllMode(0);
        }

        if(chaseClock.getElapsedTime().asSeconds() > 20 && isAllMode(0))
        {
            scatterClock.restart();
            setAllMode(1);
        }
    }
    
    void updatePacman()
    {
        int input = pacman->getInput(shared->userInput);

            if (clk.getElapsedTime().asSeconds() > 0.07) // delay for player movement
            {
                if(!validateAndMove(input))
                    validateAndMove(pacman->direction);
                                 
                clk.restart();
            }
    }

    void checkRespawnPallets()
    {
        if(frightenPallets[0].second == -1 && frightenPalletsClocks[0].getElapsedTime().asSeconds() > 25)
        {
            frightenPallets[0].second = 2;
        }

        if(frightenPallets[1].second == -1 && frightenPalletsClocks[1].getElapsedTime().asSeconds() > 25)
        {
            frightenPallets[1].second = 42;
        }

        if(frightenPallets[2].second == -1 && frightenPalletsClocks[2].getElapsedTime().asSeconds() > 25)
        {
            frightenPallets[2].second = 42;
        }

        if(frightenPallets[3].second == -1 && frightenPalletsClocks[3].getElapsedTime().asSeconds() > 25)
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
                    ghostEaten = 0;
                    pacman->score = pacman->score + 50;
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
                    ghostEaten = 0;
                    pacman->score = pacman->score + 50;
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
                    ghostEaten = 0;
                    pacman->score = pacman->score + 50;
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
                    ghostEaten = 0;
                    pacman->score = pacman->score + 50;
                    frightenStart = true;
                    siren.stop();
                    resetGhostClocks();
                    frightenSound.setLoop(true);
                    frightenSound.play();
                    frightenClock.restart();
                }
            }

            if(frightenStart && frightenClock.getElapsedTime().asSeconds() > 6 + ghostEaten && !frightenEnd)
            {
                frightenEnd = true;
                frightenStart = false;
            }

            if(frightenEnd)
            {
                frightenEnd = false;

                //setToOldMode();
                setAllMode(0);
                ghostEaten = 0;
                siren.play();
                siren.setLoop(true);
                frightenSound.stop();
            }

    }

    bool validateAndMove(int input)
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
                pacman->score = pacman->score + 10;
                foodEaten = foodEaten + 1;
            }
            shared->gameBoard[nextY][nextX] = 2; // 0 empty space , 3 means food 
            shared->gameBoard[originalY][originalX] = 0; // 0 empty space , 3 means food 

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

    void setOldState()
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            if(shared->mode[i] == 0 || shared->mode[i] == 1)
                shared->oldMode[i] = shared->mode[i];
        }
    }

    void setToOldMode()
    {
        for(int i = 0 ; i < 4 ; i++)
        {
            if(shared->mode[i] == 0 || shared->mode[i] == 1)
                shared->mode[i] = shared->oldMode[i];
        }
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