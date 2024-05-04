#include "gameEngine.h"
#include "UI.h"
#include <pthread.h>

using namespace std;
using namespace sf;


pthread_mutex_t mut;
pthread_t tid1 , tid2;

void * gameEngine_thread_function(void *);
void * UI_thread_function(void *);


// void * gameEngine_thread_function(void * arg)
// {
//     pthread_mutex_lock(&mut);
//     GameEngine *gameEngine = new GameEngine();
//     gameEngine->start_game();
//     pthread_mutex_unlock(&mut);
//     pthread_exit(NULL);
// }

// void * UI_thread_function(void * arg)
// {
//     UI *userInterface = new UI;
//     int input = userInterface->getInput();

// }



int main()
{
    //pthread_create(&tid1,NULL,gameEngine_thread_function,NULL);   
    //pthread_create(&tid2,NULL,UI_thread_function,NULL);  


    //pthread_join(tid1,NULL);
    //pthread_join(tid2,NULL);

    GameEngine *gameEngine = new GameEngine();
    gameEngine->start_game();

}
