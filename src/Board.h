#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

class Board
{
public:
    float rect_size;
    int height, width;

    Board(Board & b);
    Board(float squareSize, int height, int width, sf::Vector2f startCorner = sf::Vector2f(0.0f, 0.0f));

//    sf::RectangleShape & index(int x, int y);
    int getCell(int x, int y) const;
    void setCell(int x, int y, int value);

    void draw(sf::RenderWindow & window) const;
    void randomize();
    void clear(int value=0);

private:
    sf::Color getColor(int value) const;
//    int cellArray[board_width][board_height];
    int ** cellArray;
};


