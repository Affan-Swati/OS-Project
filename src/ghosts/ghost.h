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
    Sprite sprite;
    Texture sprSheet;
    Texture texR1 ,texR2,texL1,texL2,texU1,texU2,texD1,texD2 , texB1 , texB2 , texW1 , texW2 , texEL , texER , texEU , texED;
    int mode; // 0 means scatter, 1 means chase, 2 means frightened, 3 means eaten
    int state; // 0 / 1
    int direction; // 0 means up, 1 means right, 2 means down, 3 means left
    SharedVariables *shared;

    Ghost() 
    {
        
    }

    void updateTexture(int direction)
    {
        if(shared->ghostState == 0)
        {
            if(direction == 0)
            {
                sprite.setTexture(texU1);
            }

            else if(direction == 1)
            {
                sprite.setTexture(texR1);
            }

            else if(direction == 2)
            {
                sprite.setTexture(texD1);
            }

            else
            {
                sprite.setTexture(texL1);
            }
        }

        else if(shared->ghostState == 1)
        {
            if(direction == 0)
            {
                sprite.setTexture(texU2);
            }

            else if(direction == 1)
            {
                sprite.setTexture(texR2);
            }

            else if(direction == 2)
            {
                sprite.setTexture(texD2);
            }

            else
            {
                sprite.setTexture(texL2);
            }
        }

    }
};

#endif // GHOST_H
