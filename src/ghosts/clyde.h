#include "ghost.h"

class Clyde : public Ghost
{
    public:
    Clyde(void *&arg)
    {
        sprSheet.loadFromFile(".././resources/img/ghosts/ghost_sheet.png");
        assignRow();
        sprite.setTexture(texL2);
        shared = (SharedVariables *)arg;
        sprite.setScale(1.75,1.75);
        state = 0;
    }


    private:
    void assignRow()
    {
        float width = 15.83;
        float height = 15;

        texR1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(0 * width + 1.5, 3 * height + 3 ,width  , height));
        texR2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(1 * width + 1.5, 3 * height + 3 ,width  , height));
        texL1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(2 * width + 1.5, 3 * height + 3 ,width  , height));
        texL2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(3 * width + 1.5, 3 * height + 3 ,width  , height));
        texU1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(4 * width + 1.5, 3 * height + 3 ,width  , height));
        texU2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(5 * width + 1.5, 3 * height + 3 ,width  , height));
        texD1.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(6 * width + 1.5, 3 * height + 3 ,width  , height));
        texD2.loadFromImage(sprSheet.copyToImage(), (IntRect)FloatRect(7 * width + 1.5, 3 * height + 3 ,width   ,height));
    }
};