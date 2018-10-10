#include <SFML/Graphics.hpp>
#include <iostream>
#include "Board.h"
#include <algorithm>
#include <fstream>
#include <sstream>

bool running = false;
int steps = 0;
std::vector<int> born;
std::vector<int> survive;
std::vector<sf::Vector2i> selectedCells;
std::map <std::string, std::vector<std::vector<int>> > clipboard;

typedef std::vector<std::vector<int>> Vec2D;

void update(sf::RenderWindow & window, Board & board);
void draw(sf::RenderWindow & window, Board & board);
int livingNeighbours(const Board & board, int x, int y);
void handleInput(sf::RenderWindow & window, Board & board);
void pasteToBoard(std::vector<std::vector<int>> cells, int start_x, int start_y, Board & board);
Vec2D readFile(const std::string &filename);

int main(int argc, char ** argv)
{
    sf::RenderWindow window(sf::VideoMode(990, 990), "Game Of Life", sf::Style::Close);
    window.setKeyRepeatEnabled(false); // only one keydown event is generated

    // example template
    Vec2D acorn = {
            {0, 1, 0, 0, 0, 0, 0},
            {0, 0, 0, 1, 0, 0, 0},
            {1, 1, 0, 0, 1, 1, 1}
    };
    // set templates
    clipboard["acorn"] = acorn;

    // read one from file
    Vec2D test = readFile("gliderGun.life");
    clipboard["gliderGun"] = test;

    float rect_size = 10.0f;

    // take command line arguments
    for (int i = 1; i < argc; ++i)
    {
        std::string arg = argv[i];

        if (arg == "-s" || arg == "--size")
            rect_size = (float)atof(argv[i + 1]);

        if (arg == "--rule")
        {
            std::string rule = argv[i + i];
            bool s = false;
            for(auto c : rule)
            {
                // # of neighbours to survive and be born seperated by /
                if (c == '/')
                    s = true;
                else
                {
                    // add to the survive or born rule
                    if (s)
                        survive.push_back(c - '0');
                    else
                        born.push_back(c - '0');
                }
            }
        }

    }

    Board board(rect_size, 90, 90);

    // if no rules are given assume standard game of life
    if (survive.empty() && born.empty())
    {
        survive = {2, 3};
        born = {3};
    }

    // main game loop
    while (window.isOpen())
    {
        handleInput(window, board);
        draw(window, board);

        if (running)
        {
            update(window, board);
        }
        else
        {
            if (steps > 0)
                {
                    steps--;
                    update(window, board);
                }
        }
    }

    return 0;
}

void handleInput(sf::RenderWindow & window, Board & board)
{
    sf::Event event{};
    static bool mouseDown = false;

    while (window.pollEvent(event))
    {
        switch (event.type)
        {
            case sf::Event::Closed:
                std::cout << "window closed" << std::endl;
                window.close();
                break;

            case sf::Event::MouseButtonPressed:
            {
                mouseDown = true;

                break;
            }

            case sf::Event::MouseButtonReleased:
            {
                // keep track of cells the mouse goes over and reverse their value
                sf::Vector2i cell;
                mouseDown = false;
                while (!selectedCells.empty())
                {
                    // get the nex cell
                    cell = selectedCells.back();
                    selectedCells.pop_back();

                    // swap the cell
                    if (board.getCell(cell.x, cell.y) == 1)
                        board.setCell(cell.x, cell.y, 0);
                    else if (board.getCell(cell.x, cell.y) == 0)
                        board.setCell(cell.x, cell.y, 1);

                }
                break;
            }

            case sf::Event::KeyPressed:
                if (event.key.code == sf::Keyboard::S)
                    steps++;
                else if (event.key.code == sf::Keyboard::Space)
                    running ^= 1;
                else if (event.key.code == sf::Keyboard::R)
                    board.randomize();
                else if (event.key.code == sf::Keyboard::C)
                    board.clear();
                else if (event.key.code == sf::Keyboard::P)
                {
                    int start_x = static_cast<int>(sf::Mouse::getPosition(window).x / (board.rect_size + 1));
                    int start_y = static_cast<int>(sf::Mouse::getPosition(window).y / (board.rect_size + 1));
                    pasteToBoard(clipboard["gliderGun"], start_x, start_y, board);
                }
                break;

            default: break;
        }
    }
    if (mouseDown)
    {
        // get mouse pos
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        // get the cell that was clicked
        int square_x = static_cast<int>(mousePos.x / (board.rect_size + 1));
        int square_y = static_cast<int>(mousePos.y / (board.rect_size + 1));
        sf::Vector2i cell = sf::Vector2i(square_x, square_y);

        if (std::find(selectedCells.begin(), selectedCells.end(), cell) == selectedCells.end())
            selectedCells.push_back(cell);
    }
}

void update(sf::RenderWindow & window, Board & board)
{
    Board new_board(board);
    int neighbours;

    for (int i = 0; i < board.width; ++i)
    {
        for (int j = 0; j < board.height; ++j)
        {
            neighbours = livingNeighbours(board, i, j);

            if (std::find(born.begin(), born.end(), neighbours) != born.end())
                new_board.setCell(i, j, 1);
            else if (std::find(survive.begin(), survive.end(), neighbours) != survive.end())
                new_board.setCell(i, j, board.getCell(i, j));
            else
                new_board.setCell(i, j, 0);
        }
    }
    board = new_board;
}

void draw(sf::RenderWindow & window, Board & board)
{
    window.clear();
    board.draw(window);
    window.display();
}

// returns the number of living neighbours directly around a cell
int livingNeighbours(const Board & board, int x, int y)
{
    int neighbours = 0;

    for (int i = -1; i < 2; ++i)
    {
        for (int j = -1; j < 2; ++j)
        {
            // don't check the middle cell
            if (!(i == 0 && j == 0))
            {
                // check the cell offset by i and j
                if (board.getCell( (x + i), (y + j) ) == 1)

                {
                    neighbours++;
                }
            }
        }
    }
    return neighbours;
}

void pasteToBoard(std::vector<std::vector<int>> cells, int start_x, int start_y, Board & board)
{
    for (std::vector<int>::size_type i = 0; i < cells.size(); ++i)
    {
        for (std::vector<int>::size_type j = 0; j < cells[i].size(); ++j)
        {
            board.setCell((int) j + start_x, (int) i + start_y, cells[i][j]);
        }
    }
}

Vec2D readFile(const std::string & filename)
{
    std::ifstream file;
    std::stringstream fileStream;
    std::string fileStr;
    Vec2D cells;

    // read the data from the file
    try{
        file.open(filename);
        fileStream << file.rdbuf();
        file.close();

    }
    catch(std::ifstream::failure & e)
    {
        std::cerr << "error failed to read file" << std::endl;
    }

    fileStr = fileStream.str();
    std::istringstream iss(fileStr);
    int i = 0;

    // for each line
    for(std::string line; std::getline(iss, line);)
    {
        cells.push_back(std::vector<int>());
        // for each character add the list
        for (char c : line)
        {
            cells[i].push_back(atoi(&c));
        }
        i++;
    }
    return cells;
}

