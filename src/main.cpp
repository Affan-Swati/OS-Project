#include <X11/Xlib.h>

#undef Drawable
#undef Window
#undef Font
#undef Status
#undef None
#undef Default

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <pthread.h>


#include "gameEngine.h"
#include "UI.h"
#include "ghost-controllers/ghostcontroller.h"
#include "ghost-controllers/blinkycontroller.h"
#include "ghost-controllers/pinkycontroller.h"
#include "ghost-controllers/inkycontroller.h"
#include "ghost-controllers/clydecontroller.h"

using namespace std;
using namespace sf;


//pthread_mutex_t mut3 = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid[7];

sem_t thread_active[7];

Clock ghostSync1 , ghostSync2 ,ghostSync3 ,ghostSync4 , stateSync;

void * gameEngine_thread_function(void *);
void * UI_thread_function(void *);
void * GhostController_thread_function(void *);

void * Blinky_thread_function(void *);
void * Pinky_thread_function(void *);
void * Inky_thread_function(void *);
void * Clyde_thread_function(void *);


void * gameEngine_thread_function(void * arg)
{
    GameEngine *gameEngine = new GameEngine(arg);
    gameEngine->start_game();
    sem_post(&thread_active[0]);
    pthread_exit(NULL);
}

void * UI_thread_function(void * arg)
{
    UI *userInterface = new UI(arg);
    userInterface->getInput();
    sem_post(&thread_active[1]);
    pthread_exit(NULL);
}

void * Blinky_thread_function(void * arg)
{
    GhostController *blinkyController = new BlinkyController(arg);
    SharedVariables *shared = (SharedVariables *)arg;
    sem_wait(&shared->gameStarted); // wait fro start animation

    float delay = 0.1;
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

        if(shared->mode[0] == 2)
        {
            delay = 0.2;
        }

        else if(shared->mode[0] == 3)
        {
            delay = 0.05;
        }
        else
        {
            delay = 0.1;
        }


        if(ghostSync1.getElapsedTime().asSeconds() > delay)
        {
                if(!blinkyController->inHouse || (blinkyController->key && blinkyController->permit))
                {
                    pthread_mutex_lock(&shared->mutex);
                    blinkyController->update();
                    pthread_mutex_unlock(&shared->mutex);

                }
                else
                {
                    blinkyController->grabKeyPermit(0);
                }
                ghostSync1.restart();
        }
    }

        sem_post(&thread_active[3]);
        pthread_exit(NULL);
}

void * Pinky_thread_function(void * arg)
{
    GhostController *pinkyController = new PinkyController(arg);
    SharedVariables *shared = (SharedVariables *)arg;
    sem_wait(&shared->gameStarted); // wait fro start animation


    float delay = 0.1;

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


       if(shared->mode[1] == 2)
       {
            delay = 0.2;
       }

       else if(shared->mode[1] == 3)
       {
            delay = 0.05;
       }
       else
       {
            delay = 0.1;
       }


       if(ghostSync2.getElapsedTime().asSeconds() > delay)
       {
            if(!pinkyController->inHouse || (pinkyController->key && pinkyController->permit))
            {
                pthread_mutex_lock(&shared->mutex);
                pinkyController->update();
                pthread_mutex_unlock(&shared->mutex);
            }
            else
            {
                pinkyController->grabKeyPermit(1);
            }
            ghostSync2.restart();
       }
    }

    sem_post(&thread_active[4]);
    pthread_exit(NULL);
}

void * Inky_thread_function(void * arg)
{
    GhostController *inkyController = new InkyController(arg);
    SharedVariables *shared = (SharedVariables *)arg;
    
    sem_wait(&shared->gameStarted); // wait fro start animation
    
    float delay = 0.1;
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


       if(shared->mode[2] == 2)
       {
            delay = 0.2;
       }

       else if(shared->mode[2] == 3)
       {
            delay = 0.05;
       }

       else 
       {
            delay = 0.1;
       }


       if(ghostSync3.getElapsedTime().asSeconds() > delay)
       {
            if(!inkyController->inHouse || (inkyController->key && inkyController->permit))
            {
                pthread_mutex_lock(&shared->mutex);
                inkyController->update();
                pthread_mutex_unlock(&shared->mutex);
            }
            else
            {
               inkyController->grabKeyPermit(2);
            }
            ghostSync3.restart();
       }
    }

    sem_post(&thread_active[5]);
    pthread_exit(NULL);
}

void * Clyde_thread_function(void * arg)
{
    GhostController *clydeController = new ClydeController(arg);

    SharedVariables *shared = (SharedVariables *)arg;

    sem_wait(&shared->gameStarted); // wait fro start animation
    
    float delay = 0.1;
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


       if(shared->mode[3] == 2)
       {
            delay = 0.2;
       }
       else if(shared->mode[3] == 3)
       {
            delay = 0.05;
       }
       else
       {
            delay = 0.1;
       }


       if(ghostSync4.getElapsedTime().asSeconds() > delay)
       {
            if(!clydeController->inHouse || (clydeController->key && clydeController->permit))
            {
                clydeController->update();
            }
            else
            {
                pthread_mutex_lock(&shared->mutex);
                clydeController->grabKeyPermit(3);
                pthread_mutex_unlock(&shared->mutex);

            }
            ghostSync4.restart();
       }
    }

    sem_post(&thread_active[6]);
    pthread_exit(NULL);
}

void * GhostController_thread_function(void *arg)
{
    SharedVariables *shared = (SharedVariables*) arg;

    pthread_create(&tid[3],NULL,Blinky_thread_function,arg); 

    pthread_create(&tid[4],NULL,Pinky_thread_function,arg); 

    pthread_create(&tid[5],NULL,Inky_thread_function,arg); 

    pthread_create(&tid[6],NULL,Clyde_thread_function,arg); 

    while(!shared->gameOver)
    {
        if(stateSync.getElapsedTime().asSeconds() > 0.1)
        {
            if(shared->ghostState == 0)
                shared->ghostState = 1;
            else 
                shared->ghostState = 0;

                stateSync.restart();
        }

    }
    
    sem_wait(&thread_active[3]);
    sem_wait(&thread_active[4]);
    sem_wait(&thread_active[5]);
    sem_wait(&thread_active[6]);

    sem_post(&thread_active[2]);
    pthread_exit(NULL);
}

void initialize_semaphores(SharedVariables *&shared)
{
     for(int i = 0; i < 4 ; i++)
        sem_init(&shared->key_perm_semaphores[i] ,0 ,1);
    
    for(int i = 0 ; i < 7 ; i++)
        sem_init(&thread_active[i] ,0 ,0);


    sem_init(&shared->gameReset2 ,0 ,0);
    sem_init(&shared->gameStarted ,0 ,0);
    sem_init(&shared->animation2,0,0);
}

int main()
{

    XInitThreads();
    srand(time(0));
    SharedVariables *shared = new SharedVariables;

    initialize_semaphores(shared);

    pthread_create(&tid[0],NULL,gameEngine_thread_function,(void*)shared);   

    pthread_create(&tid[1],NULL,UI_thread_function,(void*)shared);  

    pthread_create(&tid[2],NULL,GhostController_thread_function,(void*)shared); 

    sem_wait(&thread_active[0]);
    sem_wait(&thread_active[1]);
    sem_wait(&thread_active[2]);
    pthread_mutex_destroy(&shared->mutex);
    pthread_mutex_destroy(&shared->key_perm_mutex);



}


