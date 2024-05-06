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

using namespace std;
using namespace sf;


pthread_mutex_t mut3 = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid1 , tid2;

bool t1_active = false;
bool t2_active = false;

void * gameEngine_thread_function(void *);
void * UI_thread_function(void *);


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



int main()
{

    XInitThreads();

    SharedVariables *shared = new SharedVariables;

    t1_active = true;
    pthread_create(&tid1,NULL,gameEngine_thread_function,(void*)shared);   

    t2_active = true;
    pthread_create(&tid2,NULL,UI_thread_function,(void*)shared);  
    
    //pthread_exit(0);
    while(t1_active);
    while(t2_active);
}


