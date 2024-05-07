#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include "sharedvariables.h"

using namespace std;
using namespace sf;

pthread_mutex_t mut2 = PTHREAD_MUTEX_INITIALIZER;


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
       while(!shared->gameOver)
       {
            char input = '*';  // * means user pressed soemthing other than w a s d
            bool w,a,s,d;

            w = Keyboard::isKeyPressed(Keyboard::W);
            a = Keyboard::isKeyPressed(Keyboard::A);
            s = Keyboard::isKeyPressed(Keyboard::S);
            d = Keyboard::isKeyPressed(Keyboard::D);

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
            pthread_mutex_lock(&mut2);
            shared->userInput =  input;  
            pthread_mutex_unlock(&mut2);
       } 
    }


};