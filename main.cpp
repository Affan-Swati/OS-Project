#include <X11/Xlib.h>

#undef Drawable
#undef Window
#undef Font
#undef Status
#undef None
#undef Default

#include "gameEngine.h"
#include "UI.h"
#include <pthread.h>

using namespace std;
using namespace sf;


pthread_mutex_t mut3 = PTHREAD_MUTEX_INITIALIZER;
pthread_t tid1 , tid2;

void * gameEngine_thread_function(void *);
void * UI_thread_function(void *);


void * gameEngine_thread_function(void * arg)
{
    GameEngine *gameEngine = new GameEngine(arg);
    gameEngine->start_game();
    pthread_exit(NULL);
}

void * UI_thread_function(void * arg)
{
    UI *userInterface = new UI(arg);
    userInterface->getInput();
    pthread_exit(NULL);
}



int main()
{

    SharedVariables *shared = new SharedVariables;
    XInitThreads();
    pthread_create(&tid1,NULL,gameEngine_thread_function,(void*)shared);   
    pthread_create(&tid2,NULL,UI_thread_function,(void*)shared);  


    pthread_join(tid1,NULL);
    pthread_join(tid2,NULL);
}


