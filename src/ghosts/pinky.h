#include "ghost.h"

class Pinky : public Ghost
{
    public:
    Pinky(void *&arg)
    {
        sprSheet.loadFromFile(".././resources/img/ghosts/ghost_sheet.png");
        assignRow();
        sprite.setTexture(texD2);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.75,1.75);
        state = 0;
    }

    private:
    void assignRow()
    {
        float width = 15.83;
        float height = 15;

        texR1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(0 * width + 1.5 , 1 * height + 1.5 ,width , height));
        texR2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(1 * width + 1.5 , 1 * height + 1.5 ,width , height));
        texL1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(2 * width + 1.5 , 1 * height + 1.5 ,width , height));
        texL2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(3 * width + 1.5 , 1 * height + 1.5 ,width , height));
        texU1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(4 * width + 1.5 , 1 * height + 1.5 ,width , height));
        texU2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(5 * width + 1.5 , 1 * height + 1.5 ,width , height));
        texD1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(6 * width + 1.5 , 1 * height + 1.5 ,width , height));
        texD2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(7 * width + 1.5 , 1 * height + 1.5 ,width  ,height));
    }

};