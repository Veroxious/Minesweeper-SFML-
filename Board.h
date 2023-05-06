#pragma once
#include <SFML/Graphics.hpp>
#include "Tile.h"
using std::cout;
using std::endl;
using std::string;
using sf::Texture;
using sf::Sprite;
using sf::Vector2f;
using sf::RenderWindow;

class Board
{
private:
    vector<Tile> Tiles;
    unsigned int numCols = 0;
    unsigned int numRows = 0;
    int tileCount = 0;
    int flagCount = 0;
    int mineCount = 0;
    int flippedCount = 0;
    bool gameState = true;
    bool win = false;
    bool debug = false;
public:
    std::map<string, Vector2f> buttons;
    void Initializer();
    void setNeighbors();
    int GetCols();
    int GetRows();
    void Reveal(sf::Vector2i pos);
    void ChainAttack();
    void Flag(sf::Vector2i pos);
    void Renderer(std::map<string, Texture>& resources, RenderWindow& window);
    bool State();
    bool Dub();
    void Reset();
    int TileNumbers();
    void Debug();
    void TestBoard(string fname);
};

void Board::Debug()
{
    if (this->debug == false)
    {
        this->debug = true;
    }
    else
    {
        this->debug = false;
    }
}


int Board::TileNumbers()
{
    return this->tileCount;
}

void Board::Reset()
{
    this->gameState = true;
    this->win = false;
    this->numCols = 0;
    this->numRows = 0;
    this->tileCount = 0;
    this->flagCount = 0;
    this->mineCount = 0;
}

bool Board::State()
{
    return gameState;
}

bool Board::Dub()
{
    return win;
}

void Board::ChainAttack()
{
    unsigned int i;
    for (i = 0; i < this->Tiles.size(); i++)
    {
        if (this->Tiles.at(i).ifBomba() == true)
        {
            if (this->Tiles.at(i).ifFlipped() == false)
            {
                this->Tiles.at(i).Flip();
            }
            else if (this->Tiles.at(i).ifFlagged() == true)
            {
                this->Tiles.at(i).setFlag();
            }
        }
    }
}

void Board::Flag(sf::Vector2i pos)
{
    int currcol = pos.x / 32;
    int currrow = pos.y / 32;
    int i = currrow * this->numCols + currcol;
    if (this->Tiles.at(i).ifFlipped() == false and this->Tiles.at(i).ifFlagged() == false)
    {
        this->Tiles.at(i).Flip();
        this->Tiles.at(i).setFlag();
        this->flagCount -= 1;
    }
    else if (this->Tiles.at(i).ifFlipped() == true and this->Tiles.at(i).ifFlagged() == true)
    {
        this->Tiles.at(i).Flip();
        this->Tiles.at(i).setFlag();
        this->flagCount += 1;
    }
}

void Board::Reveal(sf::Vector2i pos)
{
    int currcol = pos.x / 32;
    int currrow = pos.y / 32;
    int i = currrow * this->numCols + currcol;
    if (this->Tiles.at(i).ifFlipped() == false)
    {
        if (this->Tiles.at(i).ifBomba() == true)
        {
            this->ChainAttack();
        }
        else
        {
            if (this->Tiles.at(i).Scout() == 0)
            {
                this->Tiles.at(i).Flip();
                this->Tiles.at(i).ZeroReveal();
            }
            else
            {
                this->Tiles.at(i).Flip();
            }
        }
    }
}

void Board::TestBoard(string fname)
{
    this->Tiles.clear();
    this->mineCount = 0;
    this->gameState = true;
    this->win = false;
    std::ifstream inBoard;
    inBoard.open(fname);
    if (!inBoard.is_open())
    {
        cout << "Could not open test board" << endl;
    }
    int currrow = 0;
    unsigned int i = 0;
    string row;
    while (!inBoard.eof())
    {
        getline(inBoard, row);
        for (i = 0; i < row.size(); i++)
        {
            Tile currTile;
            if (row.at(i) == '0')
            {
                currTile.SetCoords(i, currrow);
                this->Tiles.push_back(currTile);
            }
            else if (row.at(i) == '1')
            {
                currTile.SetCoords(i, currrow);
                currTile.setBomba();
                this->Tiles.push_back(currTile);
                this->mineCount += 1;
            }
        }
        currrow += 1;
    }
    this->flagCount = mineCount;
    tileCount = i * currrow;
    this->setNeighbors();
    inBoard.close();
}

void Board::Initializer()
{
    this->Tiles.clear();
    std::ifstream inBoard;
    inBoard.open("boards/config.cfg");
    if (!inBoard.is_open())
    {
        cout << "Could not open config.cfg" << endl;
    }
    inBoard >> this->numCols;
    inBoard >> this->numRows;
    inBoard >> this->mineCount;
    this->flagCount = mineCount;
    tileCount = numCols * numRows;
    srand((unsigned)time(NULL));
    int bombaPlanted = 0;
    int i;
    int currow;
    int currcol;
    for (i = 0; i < tileCount; i++)
    {
        Tile newTile;
        currow = i / this->numCols;
        currcol = i % this->numCols;
        newTile.SetCoords(currcol, currow);
        Tiles.push_back(newTile);
    }
    size_t scale = Tiles.size() / 7;
    while (bombaPlanted < this->mineCount)
    {
        unsigned int i;
        for (i = 0; i < Tiles.size(); i++)
        {
            if (Tiles.at(i).ifBomba() == false)
            {
                int random = rand();
                if (random % scale == 0)
                {
                    Tiles.at(i).setBomba();
                    bombaPlanted += 1;
                }
            }
            if (bombaPlanted == this->mineCount)
            {
                break;
            }
        }
    }
    this->setNeighbors();
    inBoard.close();
}

void Board::setNeighbors()
{
    size_t i;
    for (i = 0; i < this->Tiles.size(); i++)
    {
        if (i == 0)
        {
            Tiles.at(i).addNeighbors(&(Tiles.at(i + 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols + 1)));
        }
        else if (i == (numCols - 1))
        {
            Tiles.at(i).addNeighbors(&(Tiles.at(i - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols - 1)));
        }
        else if (i == (this->Tiles.size() - numCols))
        {
            Tiles.at(i).addNeighbors(&(Tiles.at(i + 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols + 1)));
        }
        else if (i == (this->Tiles.size() - 1))
        {
            Tiles.at(i).addNeighbors(&(Tiles.at(i - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols - 1)));
        }
        else if (1 <= i and i <= (numCols - 2))
        {
            Tiles.at(i).addNeighbors(&(Tiles.at(i + 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols + 1)));
        }
        else if ((this->Tiles.size() - numCols + 1) <= i and i <= (this->Tiles.size() - 2))
        {
            Tiles.at(i).addNeighbors(&(Tiles.at(i + 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols + 1)));
        }
        else if ((i != 0) and (i != (this->Tiles.size() - numCols + 1)) and (i % numCols == 0))
        {
            Tiles.at(i).addNeighbors(&(Tiles.at(i + 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols + 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols + 1)));
        }
        else if ((i != (numCols - 1)) and (i != (this->Tiles.size() - 1)) and ((i + 1) % numCols == 0))
        {
            Tiles.at(i).addNeighbors(&(Tiles.at(i - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols - 1)));
        }
        else
        {
            Tiles.at(i).addNeighbors(&(Tiles.at(i + 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols - 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i + numCols + 1)));
            Tiles.at(i).addNeighbors(&(Tiles.at(i - numCols + 1)));
        }
    }
}

void Board::Renderer(std::map<string, Texture>& resources, RenderWindow& window)
{
    Sprite tile;
    Sprite digit1;
    Sprite digit2;
    Sprite digit3;
    Sprite face;
    Sprite win;
    Sprite loss;
    Sprite debug;
    Sprite test1;
    Sprite test2;
    Sprite test3;
    Sprite revealed;
    Sprite flag;
    Sprite number;
    Sprite mine;
    Sprite sign;
    std::map<string, Vector2f> locations;
    tile.setTexture(resources["tile_hidden"]);
    revealed.setTexture(resources["tile_revealed"]);
    mine.setTexture(resources["mine"]);
    flag.setTexture(resources["flag"]);
    window.draw(tile);
    this->flippedCount = 0;
    int numbers;
    unsigned int i;
    for (i = 0; i < this->Tiles.size(); i++)
    {
        if (this->Tiles.at(i).ifFlipped() == false)
        {
            if (this->debug == true)
            {
                if (this->Tiles.at(i).ifBomba() == true)
                {
                    tile.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                    window.draw(tile);
                    mine.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                    window.draw(mine);
                }
                else
                {
                    tile.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                    window.draw(tile);
                }
            }
            else
            {
                tile.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                window.draw(tile);
            }
        }
        else
        {
            if (this->Tiles.at(i).ifFlagged() == false)
            {
                if (this->Tiles.at(i).ifBomba() == true)
                {
                    revealed.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                    window.draw(revealed);
                    mine.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                    window.draw(mine);
                    this->gameState = false;
                }
                else
                {
                    numbers = this->Tiles.at(i).Scout();
                    this->flippedCount += 1;
                    if (numbers == 0)
                    {
                        revealed.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                        window.draw(revealed);
                    }
                    else
                    {
                        revealed.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                        window.draw(revealed);
                        number.setTexture(resources[std::to_string(numbers)]);
                        number.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                        window.draw(number);
                    }
                }
            }
            else
            {
                if (this->debug == true)
                {
                    if (this->Tiles.at(i).ifBomba() == true)
                    {
                        tile.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                        window.draw(tile);
                        mine.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                        window.draw(mine);
                    }
                    else
                    {
                        tile.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                        window.draw(tile);
                        flag.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                        window.draw(flag);
                    } 
                }
                else
                {
                    tile.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                    window.draw(tile);
                    flag.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                    window.draw(flag);
                }
            }
        }
    }
    if (mineCount == tileCount - flippedCount)
    {
        this->win = true;
        this->gameState = false;
        for (i = 0; i < this->Tiles.size(); i++)
        {
            if (this->Tiles.at(i).ifBomba() == true)
            {
                tile.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                window.draw(tile);
                flag.setPosition(Vector2f(this->Tiles.at(i).col * 32.f, this->Tiles.at(i).row * 32.f));
                window.draw(flag);
            }
            this->flagCount = 0;
        }
    }
    if (this->flagCount >= 0)
    {
        digit1.setPosition(Vector2f(21.f, 32.f * (i / numCols)));
        digit1.setTexture(resources["digits"]);
        digit1.setTextureRect(sf::IntRect(((this->flagCount / 100) % 100) * 21, 0, 21, 32));
        window.draw(digit1);
        digit2.setPosition(Vector2f(21.f * 2, 32.f * (i / numCols)));
        digit2.setTexture(resources["digits"]);
        digit2.setTextureRect(sf::IntRect(((this->flagCount / 10) % 10) * 21, 0, 21, 32));
        window.draw(digit2);
        digit3.setPosition(Vector2f(21.f * 3, 32.f * (i / numCols)));
        digit3.setTexture(resources["digits"]);
        digit3.setTextureRect(sf::IntRect((this->flagCount % 10) * 21, 0, 21, 32));
        window.draw(digit3);
    }
    else
    {
        sign.setPosition(Vector2f(0.f, 32.f * (i / numCols)));
        sign.setTexture(resources["digits"]);
        sign.setTextureRect(sf::IntRect(10 * 21, 0, 21, 32));
        window.draw(sign);
        digit1.setPosition(Vector2f(21.f, 32.f * (i / numCols)));
        digit1.setTexture(resources["digits"]);
        digit1.setTextureRect(sf::IntRect(((-1) * (this->flagCount / 100) % 10), 0, 21, 32));
        window.draw(digit1);
        digit2.setPosition(Vector2f(21.f * 2, 32.f * (i / numCols)));
        digit2.setTexture(resources["digits"]);
        digit2.setTextureRect(sf::IntRect(((-1) * (this->flagCount / 10) % 10) * 21, 0, 21, 32));
        window.draw(digit2);
        digit3.setPosition(Vector2f(21.f * 3, 32.f * (i / numCols)));
        digit3.setTexture(resources["digits"]);
        digit3.setTextureRect(sf::IntRect(((-1) * this->flagCount % 10) * 21, 0, 21, 32));
        window.draw(digit3);
    }
    if (this->gameState == true)
    {
        face.setPosition(Vector2f(32.f * (this->numCols / 2), 32.f * (i / numCols)));
        face.setTexture(resources["face_happy"]);
        window.draw(face);
    }
    else if (this->gameState == false and this->win == true)
    {
        face.setPosition(Vector2f(32.f * (this->numCols / 2), 32.f * (i / numCols)));
        face.setTexture(resources["face_win"]);
        window.draw(face);
    }
    else
    {
        face.setPosition(Vector2f(32.f * (this->numCols / 2), 32.f * (i / numCols)));
        face.setTexture(resources["face_lose"]);
        window.draw(face);
    }
    Vector2f faceLoc = face.getPosition();
    this->buttons.emplace("face", faceLoc);
    debug.setPosition(faceLoc.x + 64.f * 2, 32.f * (i / numCols));
    debug.setTexture(resources["debug"]);
    window.draw(debug);
    Vector2f debugLoc = debug.getPosition();
    this->buttons.emplace("debug", debugLoc);
    test1.setPosition(faceLoc.x + 64.f * 3, 32.f * (i / numCols));
    test1.setTexture(resources["test1"]);
    window.draw(test1);
    Vector2f test1Loc = test1.getPosition();
    this->buttons.emplace("test1", test1Loc);
    test2.setPosition(faceLoc.x + 64.f * 4, 32.f * (i / numCols));
    test2.setTexture(resources["test2"]);
    window.draw(test2);
    Vector2f test2Loc = test2.getPosition();
    this->buttons.emplace("test2", test2Loc);
    test3.setPosition(faceLoc.x + 64.f * 5, 32.f * (i / numCols));
    test3.setTexture(resources["test3"]);
    window.draw(test3);
    Vector2f test3Loc = test3.getPosition();
    this->buttons.emplace("test3", test3Loc);
    locations.emplace("happy", faceLoc);
    locations.emplace("debug", debugLoc);
    locations.emplace("test1", test1Loc);
    locations.emplace("test2", test2Loc);
    locations.emplace("test3", test3Loc);
}

int Board::GetCols()
{
    return this->numCols;
}

int Board::GetRows()
{
    return this->numRows;
}