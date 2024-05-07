#include "ghost.h"

class Inky : public Ghost
{
    public:
    Inky(void *&arg)
    {
        tex.loadFromFile(".././resources/img/ghosts/inky.png");
        sprite.setTexture(tex);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.6,1.6);
    }
};