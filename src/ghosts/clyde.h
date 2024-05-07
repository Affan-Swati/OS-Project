#include "ghost.h"

class Clyde : public Ghost
{
    public:
    Clyde(void *&arg)
    {
        tex.loadFromFile(".././resources/img/ghosts/clyde.png");
        sprite.setTexture(tex);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.6,1.6);
    }
};