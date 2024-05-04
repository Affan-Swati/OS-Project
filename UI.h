#include <iostream>
#include <cstdlib>
#include <time.h>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

using namespace std;
using namespace sf;


class UI
{
    public:

    UI(){}

    int getInput()
    {
         
        bool w,a,s,d;

        w = Keyboard::isKeyPressed(Keyboard::W);
        a = Keyboard::isKeyPressed(Keyboard::A);
        s = Keyboard::isKeyPressed(Keyboard::S);
        d = Keyboard::isKeyPressed(Keyboard::D);

        if(w)
        {
            return 0;
        }

        else if(d)
        {
            return 1;
        }

        else if(s)
        {
           return 2;
        }

        else if(a)
        {
            return 3;
        }

        else return -1;

    }


};