#ifndef PACMAN_H
#define PACMAN_H
#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>


#include <pthread.h>

using namespace std;
using namespace sf;

class Pacman
{
    public:

    Sprite sprite;
    Texture u1,u2,u3,d1,d2,d3,r1,r2,r3,l1,l2,l3;
    Vector2f position;

    int state; // 0 means open mouth , 1 means half closed mouth , 2 means full closed mouth
    int direction; //0 means up , 1 means right , 2 means down , 3 means left
    Clock clk; 
    int score;
    
    
    Pacman()
    {
        u1.loadFromFile("img/pacman-up/1.png");
        u2.loadFromFile("img/pacman-up/2.png");
        u3.loadFromFile("img/pacman-up/3.png");

        d1.loadFromFile("img/pacman-down/1.png");
        d2.loadFromFile("img/pacman-down/2.png");
        d3.loadFromFile("img/pacman-down/3.png");

        r1.loadFromFile("img/pacman-right/1.png");
        r2.loadFromFile("img/pacman-right/2.png");
        r3.loadFromFile("img/pacman-right/3.png");

        l1.loadFromFile("img/pacman-left/1.png");
        l2.loadFromFile("img/pacman-left/2.png");
        l3.loadFromFile("img/pacman-left/3.png");

        sprite.setTexture(l3);
        direction = 3;
        state = 0;
        position.x = 17;
        position.y = 36;
        
        Vector2u size = l3.getSize();
        //sprite.setOrigin(size.x / 2.f, size.y / 2.f);
        sprite.setPosition(position.x * 15 ,  position.y * 15);
        sprite.setScale(1.3,1.5); // DONT CHANGE THE SCALE VALUES , THEY ARE PIXEL PERFECT :") 1.4 x 1.7
        clk.restart();
        score = 0;

    }

    int getInput(char input)
    {
        if(clk.getElapsedTime().asSeconds() > 0.1)
        {
            if(state == 0)
            {
                state = 1;

                if(direction == 0)
                    sprite.setTexture(u2);
                
                else if (direction == 1)
                    sprite.setTexture(r2);
                
                else if (direction == 2)
                    sprite.setTexture(d2);

                else if (direction == 3)
                    sprite.setTexture(l2);
            }

            else if (state == 1)
            {
                state = 2;
                
                if(direction == 0)
                    sprite.setTexture(u3);
                
                else if (direction == 1)
                    sprite.setTexture(r3);
                
                else if (direction == 2)
                    sprite.setTexture(d3);

                else if (direction == 3)
                    sprite.setTexture(l3);
            }

            else if (state == 2)
            {
                state = 0;
                
                 if(direction == 0)
                    sprite.setTexture(u1);
                
                else if (direction == 1)
                    sprite.setTexture(r1);
                
                else if (direction == 2)
                    sprite.setTexture(d1);

                else if (direction == 3)
                    sprite.setTexture(l1);
            }

            clk.restart();
        }   

        if(input == 'w')
        {
            return 0;
        }

        else if(input == 'd')
        {
            return 1;
        }

        else if(input == 's')
        {
           return 2;
        }

        else if(input == 'a')
        {
            return 3;
        }
        return direction;
    }

    void setDirection(int dir)
    {
        direction = dir;
    }
};
#endif