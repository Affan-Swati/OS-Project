#include "ghost.h"

class Pinky : public Ghost
{
    public:
    Pinky(void *&arg)
    {
        tex.loadFromFile(".././resources/img/ghosts/pinky.png");
        sprite.setTexture(tex);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.6,1.6);
    }

};