// Project3.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <random>
#include <fstream>
#include <map>
#include "Board.h"


std::map<string, Texture> TextureLoad()
{
    std::map<string, Texture> textures;
    Texture debug;
    debug.loadFromFile("images/debug.png");
    textures.emplace("debug", debug);
    Texture digits;
    digits.loadFromFile("images/digits.png");
    textures.emplace("digits", digits);
    Texture face_happy;
    face_happy.loadFromFile("images/face_happy.png");
    textures.emplace("face_happy", face_happy);
    Texture face_lose;
    face_lose.loadFromFile("images/face_lose.png");
    textures.emplace("face_lose", face_lose);
    Texture face_win;
    face_win.loadFromFile("images/face_win.png");
    textures.emplace("face_win", face_win);
    Texture flag;
    flag.loadFromFile("images/flag.png");
    textures.emplace("flag", flag);
    Texture mine;
    mine.loadFromFile("images/mine.png");
    textures.emplace("mine", mine);
    int i;
    int num = 1;
    for (i = 1; i <= 8; i++)
    {
        Texture i;
        string file = "images/number_";
        i.loadFromFile(file + std::to_string(num) + ".png");
        textures.emplace(std::to_string(num), i);
        num += 1;
    }
    int n;
    num = 1;
    for (n = 1; n <= 3; n++)
    {
        Texture n;
        string file = "images/test_";
        n.loadFromFile(file + std::to_string(num) + ".png");
        textures.emplace("test" + std::to_string(num), n);
        num += 1;
    }
    Texture tile_hidden;
    tile_hidden.loadFromFile("images/tile_hidden.png");
    textures.emplace("tile_hidden", tile_hidden);
    Texture tile_revealed;
    tile_revealed.loadFromFile("images/tile_revealed.png");
    textures.emplace("tile_revealed", tile_revealed);
    return textures;
}

int main()
{
    Board game;
    game.Initializer();
    sf::RenderWindow window(sf::VideoMode(game.GetCols() * 32, (game.GetRows() * 32) + 100), "Minesweeper");
    std::map<string, Texture> assets;
    assets = TextureLoad();
    sf::Vector2i localPosition;
    bool inGame = true;
    while (window.isOpen())
    {
        sf::Event event;
        window.clear(sf::Color::White);
        game.Renderer(assets, window);
        window.display();
        window.setFramerateLimit(60);
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
            case (sf::Event::Closed):
                 window.close();
            case (sf::Event::MouseButtonPressed):
                 if (event.mouseButton.button == sf::Mouse::Left)
                 {
                     localPosition = sf::Mouse::getPosition(window);
                     if (localPosition.x <= game.GetCols() * 32 and localPosition.y < game.GetRows() * 32 and game.State())
                     {
                         window.clear(sf::Color::White);
                         game.Reveal(localPosition);
                         game.Renderer(assets, window);
                         window.display();
                         game.State();
                     }
                     else if (localPosition.x >= game.buttons["face"].x and localPosition.x <= game.buttons["face"].x + 64 and localPosition.y >= game.buttons["face"].y and localPosition.y <= game.buttons["face"].y + 64)
                     {
                         window.clear(sf::Color::White);
                         game.Reset();
                         game.Initializer();
                         game.Renderer(assets, window);
                         window.display();
                     }
                     else if (localPosition.x >= game.buttons["debug"].x and localPosition.x <= game.buttons["debug"].x + 64 and localPosition.y >= game.buttons["debug"].y and localPosition.y <= game.buttons["debug"].y + 64 and game.State())
                     {
                         window.clear(sf::Color::White);
                         game.Debug();
                         game.Renderer(assets, window);
                         window.display();
                     }
                     else if (localPosition.x >= game.buttons["test1"].x and localPosition.x <= game.buttons["test1"].x + 64 and localPosition.y >= game.buttons["test1"].y and localPosition.y <= game.buttons["test1"].y + 64)
                     {
                         window.clear(sf::Color::White);
                         game.TestBoard("boards/testboard1.brd");
                         game.Renderer(assets, window);
                         window.display();
                     }
                     else if (localPosition.x >= game.buttons["test2"].x and localPosition.x <= game.buttons["test2"].x + 64 and localPosition.y >= game.buttons["test2"].y and localPosition.y <= game.buttons["test2"].y + 64)
                     {
                         window.clear(sf::Color::White);
                         game.TestBoard("boards/testboard2.brd");
                         game.Renderer(assets, window);
                         window.display();
                     }
                     else if (localPosition.x >= game.buttons["test3"].x and localPosition.x <= game.buttons["test3"].x + 64 and localPosition.y >= game.buttons["test3"].y and localPosition.y <= game.buttons["test3"].y + 64)
                     {
                         window.clear(sf::Color::White);
                         game.TestBoard("boards/testboard3.brd");
                         game.Renderer(assets, window);
                         window.display();
                     }
                 }
                 else if (event.mouseButton.button == sf::Mouse::Right)
                 {
                     localPosition = sf::Mouse::getPosition(window);
                     if (localPosition.x <= game.GetCols() * 32 and localPosition.y < game.GetRows() * 32 and game.State())
                     {
                         window.clear(sf::Color::White);
                         game.Flag(localPosition);
                         game.Renderer(assets, window);
                         window.display();
                         game.Dub();
                     }
                 }
            }
        }
    }
    return 0;
}