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


pthread_mutex_t mut3 = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid1 , tid2 , tid3 , tid4 , tid5 , tid6 , tid7;
// t1 gameEngine , t2 UI , t3 ghost controller , t4 blinky , t5 pinly , t6 inky , t7 clyde

bool t1_active = false;
bool t2_active = false;
bool t3_active = false;
bool t4_active = false;
bool t5_active = false;
bool t6_active = false;
bool t7_active = false;

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
    t1_active = false;
    pthread_exit(NULL);
}

void * UI_thread_function(void * arg)
{
    UI *userInterface = new UI(arg);
    userInterface->getInput();
    t2_active = false;
    pthread_exit(NULL);
}

void * Blinky_thread_function(void * arg)
{
    GhostController *blinkyController = new BlinkyController(arg);
    SharedVariables *shared = (SharedVariables *)arg;
    while(!shared->gameStarted); // wait fro start animation

    float delay = 0.1;
    while(!shared->gameOver)
    {
        if(!shared->gameStarted || shared->gameReset)
        continue;

       if(shared->mode[0] == 2)
       {
            delay = 0.2;
       }

       else if(shared->mode[0] == 3)
       {
            delay = 0.08;
       }
       else
       {
            delay = 0.1;
       }


       if(ghostSync1.getElapsedTime().asSeconds() > delay)
       {
            if(!blinkyController->inHouse || (blinkyController->key && blinkyController->permit))
            {
                blinkyController->update();
            }
            else
            {
                pthread_mutex_lock(&mut3);
                if(!blinkyController->key)
                {
                    blinkyController->grabKey();    
                }
                pthread_mutex_unlock(&mut3);

                pthread_mutex_lock(&mut3);
                if(!blinkyController->permit)
                {
                    blinkyController->grabPermit();
                }
                pthread_mutex_unlock(&mut3);
            }
            ghostSync1.restart();
       }
    }

    t4_active = false;
    pthread_exit(NULL);
}

void * Pinky_thread_function(void * arg)
{
    GhostController *pinkyController = new PinkyController(arg);
    SharedVariables *shared = (SharedVariables *)arg;
    while(!shared->gameStarted); // wait fro start animation

    float delay = 0.1;

    while(!shared->gameOver)
    {
        if(!shared->gameStarted || shared->gameReset)
        continue;

       if(shared->mode[1] == 2)
       {
            delay = 0.2;
       }

       else if(shared->mode[1] == 3)
       {
            delay = 0.08;
       }
       else
       {
            delay = 0.1;
       }


       if(ghostSync2.getElapsedTime().asSeconds() > delay)
       {
            if(!pinkyController->inHouse || (pinkyController->key && pinkyController->permit))
            {
                pinkyController->update();
            }
            else
            {
                pthread_mutex_lock(&mut3);
                if(!pinkyController->key)
                {
                    pinkyController->grabKey();    
                }
                pthread_mutex_unlock(&mut3);

                pthread_mutex_lock(&mut3);
                if(!pinkyController->permit)
                {
                    pinkyController->grabPermit();
                }
                pthread_mutex_unlock(&mut3);
            }
            ghostSync2.restart();
       }
    }

    t5_active = false;
    pthread_exit(NULL);
}

void * Inky_thread_function(void * arg)
{
    GhostController *inkyController = new InkyController(arg);
    SharedVariables *shared = (SharedVariables *)arg;

    while(!shared->gameStarted); // wait fro start animation
    
    float delay = 0.1;
    while(!shared->gameOver)
    {
       if(!shared->gameStarted || shared->gameReset)
            continue;

       if(shared->mode[2] == 2)
       {
            delay = 0.2;
       }

       else if(shared->mode[2] == 3)
       {
            delay = 0.08;
       }

       else 
       {
            delay = 0.1;
       }


       if(ghostSync3.getElapsedTime().asSeconds() > delay)
       {
            if(!inkyController->inHouse || (inkyController->key && inkyController->permit))
            {
                inkyController->update();
            }
            else
            {
                pthread_mutex_lock(&mut3);
                if(!inkyController->key)
                {
                    inkyController->grabKey();    
                }
                pthread_mutex_unlock(&mut3);

                pthread_mutex_lock(&mut3);
                if(!inkyController->permit)
                {
                    inkyController->grabPermit();
                }
                pthread_mutex_unlock(&mut3);
            }
            ghostSync3.restart();
       }
    }

    t6_active = false;
    pthread_exit(NULL);
}

void * Clyde_thread_function(void * arg)
{
    GhostController *clydeController = new ClydeController(arg);

    SharedVariables *shared = (SharedVariables *)arg;

    while(!shared->gameStarted); // wait fro start animation
    
    float delay = 0.1;
    while(!shared->gameOver)
    {
        if(!shared->gameStarted || shared->gameReset)
        continue;

       if(shared->mode[3] == 2)
       {
            delay = 0.2;
       }
       else if(shared->mode[3] == 3)
       {
            delay = 0.08;
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
                pthread_mutex_lock(&mut3);
                if(!clydeController->key)
                {
                    clydeController->grabKey();    
                }
                pthread_mutex_unlock(&mut3);

                pthread_mutex_lock(&mut3);
                if(!clydeController->permit)
                {
                    clydeController->grabPermit();
                }
                pthread_mutex_unlock(&mut3);
            }
            ghostSync4.restart();
       }
    }

    t7_active = false;
    pthread_exit(NULL);
}

void * GhostController_thread_function(void *arg)
{
    SharedVariables *shared = (SharedVariables*) arg;

    t4_active = true;
    pthread_create(&tid4,NULL,Blinky_thread_function,arg); 

    t5_active = true;
    pthread_create(&tid5,NULL,Pinky_thread_function,arg); 

    t6_active = true;
    pthread_create(&tid6,NULL,Inky_thread_function,arg); 

    t7_active = true;
    pthread_create(&tid7,NULL,Clyde_thread_function,arg); 

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
    while(t4_active) sleep(2);
    while(t5_active)sleep(2);
    while(t6_active)sleep(2);
    while(t7_active)sleep(2);

    t3_active = false;
    pthread_exit(NULL);
}

int main()
{

    XInitThreads();
    srand(time(0));
    SharedVariables *shared = new SharedVariables;


    t1_active = true;
    pthread_create(&tid1,NULL,gameEngine_thread_function,(void*)shared);   

    t2_active = true;
    pthread_create(&tid2,NULL,UI_thread_function,(void*)shared);  
    
    t3_active = true;
    pthread_create(&tid3,NULL,GhostController_thread_function,(void*)shared); 

    //pthread_exit(0);/
    while(t1_active);
    while(t2_active);
    while(t3_active);

    pthread_mutex_destroy(&mut);
    pthread_mutex_destroy(&mut2);
    pthread_mutex_destroy(&mut3);


}


