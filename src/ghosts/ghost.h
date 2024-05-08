#ifndef GHOST_H
#define GHOST_H

#include <SFML/Graphics.hpp>
#include "../sharedvariables.h"
#include <cmath>

using namespace sf;
using namespace std;

class Ghost 
{   
   public:
   virtual void updateTexture(int direction) = 0;
   virtual bool isEaten() = 0;
   virtual bool eatsPac() = 0;

   public:
    Sprite sprite;
    Texture sprSheet;
    Texture texR1 ,texR2,texL1,texL2,texU1,texU2,texD1,texD2 , texB1 , texB2 , texW1 , texW2 , texEL , texER , texEU , texED;
    int mode; // 0 means scatter, 1 means chase, 2 means frightened, 3 means eaten
    int state; // 0 / 1
    int direction; // 0 means up, 1 means right, 2 means down, 3 means left
    SharedVariables *shared;
    Music ghostEatenSound ;
    Clock blink;

    Ghost() 
    {
        
    }

    void BlinkSprite()
    {
        float time = blink.getElapsedTime().asSeconds() - (int)blink.getElapsedTime().asSeconds();
        time *= 10;

        if(shared->ghostState == 0)
        {
            if((int)time < 4)
                sprite.setTexture(texB1);
            else
                sprite.setTexture(texW1);
                
        }
        else 
        {
            if((int)time < 4)
                sprite.setTexture(texB2);
            else
                sprite.setTexture(texW2);
                
        }
    }
};

#endif // GHOST_H
