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


};