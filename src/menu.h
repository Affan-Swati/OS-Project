#pragma once
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>

using namespace sf;
using namespace std;

class Menu 
{
private:
    vector<Text> options;
    sf::Font font;
    Color textColor;
    Color hoverColor;
    int selectedOption;
    RenderWindow &window;


public:
    Menu(RenderWindow &win) : window(win)
    { 
        font.loadFromFile("../resources/font.ttf");
        selectedOption = -1;
        textColor = Color::Yellow;
        hoverColor = Color::Blue; // Color when option is hovered
        initializeOptions();
    }

    void initializeOptions() 
    {
        addOption("START GAME", 330);
        addOption("EXIT", 400);
    }

    void addOption(const string& text, float yPos) 
    {
        Text option(text, font, 30);
        option.setFillColor(textColor);
        option.setPosition(695 / 2 - option.getGlobalBounds().width / 2, yPos);
        options.push_back(option);
    }

    int handleEvent(Event& event) 
    {
        if (event.type == Event::MouseMoved) 
        {
            Vector2i mousePosition = Mouse::getPosition(window);
            bool hovering = false;
            for (int i = 0; i < options.size(); ++i) 
            {
                FloatRect optionBounds = options[i].getGlobalBounds();
                if (optionBounds.intersects(FloatRect(mousePosition.x, mousePosition.y, 10, 10))) 
                {
                    options[i].setFillColor(hoverColor); // Change color when hovered
                    options[i].setScale(1.1f, 1.1f); // Increase size when hovered
                    selectedOption = i;
                    hovering = true;
                } 
                else 
                {
                    options[i].setFillColor(textColor);
                    options[i].setScale(1.0f, 1.0f); // Reset size when not hovered
                }
            }
        }

        if (event.type == Event::MouseButtonPressed) 
        {
            if (event.mouseButton.button == Mouse::Left && selectedOption != -1) 
            {
                if (selectedOption == 0) 
                {
                    return 1;
                } 
                else if (selectedOption == 1) 
                {
                    return 2;
                } 

                else
                    return 0;
            }
        }
        
        return 0;
    }

    void draw(RenderWindow &window) 
    {
        for (const auto& option : options) 
        {
            window.draw(option);
        }
    }
};

