#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include "sharedvariables.h"

using namespace std;
using namespace sf;

class UI
{
    public:
    SharedVariables *shared;

    UI(void *&arg)
    {
        shared = (SharedVariables*) arg;
    }

    void getInput()
    {
       sem_wait(&shared->gameStarted);
       while(!shared->gameOver)
       {
            if(shared->gameReset)
            {
                sem_wait(&shared->gameReset2);
            }

            if(shared->animation)
            {
                sem_wait(&shared->animation2);
            }

        
            char input = '*';  // * means user pressed soemthing other than w a s d
            bool w,a,s,d;

            w = Keyboard::isKeyPressed(Keyboard::Up);
            a = Keyboard::isKeyPressed(Keyboard::Left);
            s = Keyboard::isKeyPressed(Keyboard::Down);
            d = Keyboard::isKeyPressed(Keyboard::Right);

            if(w)
            {
                input = 'w';
            }

            else if(d)
            {
                input = 'd';
            }

            else if(s)
            {
            input= 's';
            }

            else if(a)
            {
                input = 'a';
            }
            shared->userInput =  input;  
       } 
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
                live.setPosition(offset,760);
                window.draw(live);
                offset += 28;
            }



        }



};