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
            bool w,a,s,d,p,back;

            
            w = Keyboard::isKeyPressed(Keyboard::Up);
            a = Keyboard::isKeyPressed(Keyboard::Left);
            s = Keyboard::isKeyPressed(Keyboard::Down);
            d = Keyboard::isKeyPressed(Keyboard::Right);
            p = Keyboard::isKeyPressed(Keyboard::P);
            back = Keyboard::isKeyPressed(Keyboard::BackSpace);

               
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

            else if(p)
            {
                pthread_mutex_lock(&shared->mutex);
                if(!shared->gamePaused)
                {
                    shared->gamePaused = true;
                }
        

                input = 'p';
                pthread_mutex_unlock(&shared->mutex);

            }

            else if(back)
            {         
                shared->gamePaused = false;

                sem_post(&shared->gamePaused2);
                sem_post(&shared->gamePaused2);
                sem_post(&shared->gamePaused2);
                sem_post(&shared->gamePaused2);
                sem_post(&shared->gamePaused2);
               
            }
            shared->userInput =  input;  
       } 
    }
};