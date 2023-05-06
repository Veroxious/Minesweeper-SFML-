#pragma once
#include <vector>
using std::vector;

class Tile
{
private:
    bool isBomba = false;
    bool isFlipped = false;
    bool isFlagged = false;
    int nearBomba = 0;
    vector<Tile*> neighbors;
public:
    void setBomba();
    bool ifBomba();
    void addNeighbors(Tile*);
    bool ifFlipped();
    bool ifFlagged();
    void SetCoords(int x, int y);
    void Flip();
    int Scout();
    void ZeroReveal();
    void setFlag();
    int col = 0;
    int row = 0;
};

void Tile::setFlag()
{
    if (this->isFlagged == false)
    {
        this->isFlagged = true;
    }
    else
    {
        this->isFlagged = false;
    }
}

void Tile::ZeroReveal()
{
    unsigned int i;
    for (i = 0; i < this->neighbors.size(); i++)
    {
        if (neighbors.at(i)->ifFlipped() == false)
        {
            if (neighbors.at(i)->Scout() == 0)
            {
                neighbors.at(i)->Flip();
                neighbors.at(i)->ZeroReveal();
            }
            else
            {
                neighbors.at(i)->Flip();
            }
        }
    }
}

int Tile::Scout()
{
    return this->nearBomba;
}


void Tile::Flip()
{
    if (this->isFlipped == false)
    {
        this->isFlipped = true;
    }
    else
    {
        this->isFlipped = false;
    }
}

void Tile::SetCoords(int x, int y)
{
    this->col = x;
    this->row = y;
}

bool Tile::ifFlipped()
{
    return isFlipped;
}

bool Tile::ifBomba()
{
    return this->isBomba;
}

bool Tile::ifFlagged()
{
    return this->isFlagged;
}

void Tile::setBomba()
{
    this->isBomba = true;
}

void Tile::addNeighbors(Tile* currptr)
{
    this->neighbors.push_back(currptr);
    if ((*currptr).ifBomba() == true)
    {
        this->nearBomba += 1;
    }
}