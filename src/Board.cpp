#include <iostream>
#include "Board.h"

Board::Board(Board & b)
{
    this->rect_size = b.rect_size;
    this->height = b.height;
    this->width = b.width;

    cellArray = new int * [width];

    for (int i = 0; i < width; ++i)
    {
        cellArray[i] = new int[height];
        for (int j = 0; j < height; ++j)
        {
            cellArray[i][j] = b.getCell(i, j);
        }
    }
}

Board::Board(float squareSize, int width, int height, sf::Vector2f startCorner)
{
    this->height = height;
    this->width = width;
    this->rect_size = squareSize;

    // initialize the array of cells with zero on the stack
    cellArray = new int * [width];
    for (int i = 0; i < width; ++i)
    {
        cellArray[i] = new int[height];
        for (int j = 0; j < height; ++j)
        {
            cellArray[i] = new int[height];
            cellArray[i][j] = 0;
        }
    }
}

int Board::getCell(int x, int y) const
{
    // if it's negative wrap around to the other side of the board
    if (x < 0)
        x += width;
    if (y < 0)
        y += height;

    return cellArray[x % width][y % height];
}

void Board::setCell(int x, int y, int value)
{
    cellArray[x % width][y % height] = value;
}

sf::Color Board::getColor(int value) const
{
    // assign colors to display from the cell value
    sf::Color color;
    switch(value)
    {
        case 0:
            color = sf::Color(150, 150, 150);
            break;
        case 1:
            color = sf::Color::Black;
            break;
        case 2:
            color = sf::Color::Red;
            break;
        case 3:
            color = sf::Color::Green;
            break;
        case 4:
            color = sf::Color::Blue;
            break;
        default:break;
    }
    return color;
}

void Board::draw(sf::RenderWindow &window) const
{
    // create a rectangle to draw
    sf::RectangleShape rect = sf::RectangleShape(sf::Vector2f(rect_size, rect_size));
    rect.setOrigin(sf::Vector2f(rect_size / 2, rect_size / 2));

    // iterate through the entire board
    for (int i = 0; i < width; ++i)
    {
        // for each cell move the rectangle sprite and draw it
        for (int j = 0; j < height; ++j)
        {
            rect.setFillColor(getColor(getCell(i, j)));
            rect.setPosition((float)((rect_size + 1) * i) + rect_size / 2, (float)((rect_size + 1) * j) + rect_size / 2);
            window.draw(rect);
        }
    }
}

void Board::randomize()
{
    // set all cells to a random value
    for(int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            setCell(i, j, std::rand() % 2);
        }
    }
}

void Board::clear(int value)
{
    // clear the board with a given value otherwise 0
    for(int i = 0; i < width; ++i)
    {
        for (int j = 0; j < height; ++j)
        {
            setCell(i, j, value);
        }
    }
}
