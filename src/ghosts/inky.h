#include "ghost.h"

class Inky : public Ghost
{
    public:
    Inky(void *&arg)
    {
        sprSheet.loadFromFile(".././resources/img/ghosts/ghost_sheet.png");
        assignRow();
        sprite.setTexture(texD2);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.75,1.75);
        state = 0;
        ghostEatenSound.openFromFile(".././resources/sounds/monsterEat.wav");

    }


    private:
    void assignRow()
    {
        float width = 15.83;
        float height = 15;

        texR1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(0 * width + 1.5 , 2 * height + 2 ,width , height ));
        texR2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(1 * width + 1.5 , 2 * height + 2 ,width , height ));
        texL1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(2 * width + 1.5 , 2 * height + 2 ,width , height ));
        texL2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(3 * width + 1.5 , 2 * height + 2 ,width , height ));
        texU1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(4 * width + 1.5 , 2 * height + 2 ,width , height ));
        texU2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(5 * width + 1.5 , 2 * height + 2 ,width , height ));
        texD1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(6 * width + 1.5 , 2 * height + 2 ,width , height ));
        texD2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(7 * width + 1.5 , 2 * height + 2 ,width  ,height ));

        texB1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(8  * width + 1.5 , 0 * height ,width  ,height));
        texB2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(9  * width + 1.5 , 0 * height ,width  ,height));
        texW1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(10 * width + 1.5 , 0 * height ,width  ,height));
        texW2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(11 * width + 1.5 , 0 * height ,width  ,height));
        texER.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(8  * width + 1.5 , 1 * height + 1.5 ,width  ,height));
        texEL.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(9  * width + 1.5 , 1 * height + 1.5 ,width  ,height));
        texEU.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(10  * width + 1.5 , 1 * height + 1.5 ,width  ,height));
        texED.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(11 * width + 1.5 , 1 * height + 1.5 ,width  ,height));
    }

    void updateTexture(int direction)
    {
        // if(shared->mode == 2 && !clock_restarted)
        // {
        //     clk.restart();
        //     clock_restarted = true;
        // }

        // if(clk.getElapsedTime().asSeconds() > 10)
        // {
        //     clock_restarted = false;
        // }

        if(shared->mode[2] == 3)
        {
            if(direction == 0)
            {
                sprite.setTexture(texEU);
            }

            else if(direction == 1)
            {
                sprite.setTexture(texER);
            }

            else if(direction == 2)
            {
                sprite.setTexture(texED);
            }

            else
            {
                sprite.setTexture(texEL);
            }

        }

        else if(shared->ghostState == 0)
        {
            if(shared->mode[2] == 2)
            {
                 if((blink.getElapsedTime().asSeconds() > 3))
                    BlinkSprite();
                else
                sprite.setTexture(texB1);
            }

            else if(direction == 0)
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
           if(shared->mode[2] == 2)
            {
                if((blink.getElapsedTime().asSeconds() > 3))
                    BlinkSprite();
                else
                sprite.setTexture(texB2);
            }

            else if(direction == 0)
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

    bool isEaten()
    {
        if(shared->mode[2] != 2)
            return false;

        if(shared->inkyPos.first.x == shared->pacPos.x && shared->inkyPos.second.y == shared->pacPos.y)
        {
            ghostEatenSound.stop();
            ghostEatenSound.play();
            shared->mode[2] = 3;
            return true;
        }

        return false;
    }

    bool eatsPac()
    {
        if(shared->mode[2] != 1 && shared->mode[2] != 0)
        {
            return false;
        }

        if(shared->inkyPos.first.x == shared->pacPos.x && shared->inkyPos.second.y == shared->pacPos.y)
        {
            return true;
        }

        return false;
    }
};