#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "sharedvariables.h"
#include <string>
#include <cmath>

using namespace sf;
using namespace std;

class Fruit 
{   
  
   public:
    Sprite sprite;
    Texture tex;
    bool isActive;
    Clock appearClock;
    SharedVariables *shared;
    Music eatenSound ;
    Texture spriteSheet;

    Fruit(void *&arg ,int count,float x , float y) 
    {
        shared = (SharedVariables*) arg;
        
        spriteSheet.loadFromFile("../resources/img/other/Fruits.png");

        tex.loadFromImage(spriteSheet.copyToImage(), (IntRect)FloatRect(count * 15.125  , 0,  15.125 , 13));
        sprite.setTexture(tex);
        sprite.setPosition(Vector2f(x * 15,y * 15));
        sprite.setScale(2,2);
        eatenSound.openFromFile("../resources/sounds/eatFruit.wav");
        isActive = false;
        appearClock.restart();

    }

    bool checkCollision()
    {
        checkActive();
        if(this->isActive && sprite.getGlobalBounds().intersects(FloatRect(shared->pacPos.x * 15 , shared->pacPos.y * 15 , 16 , 16)))
        {
            this->isActive = false;
            appearClock.restart();
            eatenSound.stop();
            eatenSound.play();
            return true;
        }

        return false;
    }

    void checkActive()
    {
        if(!this->isActive && appearClock.getElapsedTime().asSeconds() > 20)
        {
            this->isActive = true;
        }
    }



};
