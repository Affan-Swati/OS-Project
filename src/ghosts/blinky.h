#include "ghost.h"

class Blinky : public Ghost
{
    public:
    Blinky(void *&arg)
    {
        tex.loadFromFile(".././resources/img/ghosts/blinky.png");
        sprite.setTexture(tex);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.6,1.6);
    }

};