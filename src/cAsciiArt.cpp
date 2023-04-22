#include <iostream>
#include <sstream>
#include "workshop.h"
cAsciiArt::cAsciiArt()
    : myGrid(17, std::vector<char>(17, '.'))
{
    // std::vector<std::vector<char>> vgrid(17, std::vector<char>(17, '.'));

    for (int kx = 0; kx < 17; kx++)
    {
        for (int ky = 0; ky < 17; ky++)
        {
            if (kx + ky < 4)
            {
                myGrid[kx][ky] = ' ';
            }
            int kxp = 17 - kx;
            if (kxp + ky < 4)
            {
                myGrid[kx][ky] = ' ';
            }
            int kyp = 17 - ky;
            if (kx + kyp < 4)
            {
                myGrid[kx][ky] = ' ';
            }
            if (kxp + kyp < 4)
            {
                myGrid[kx][ky] = ' ';
            }
        }
    }
}

void cAsciiArt::display(const cLayout &layout)
{
    for (auto *w : layout) {
        insert(w->location(), w->ascii());
        for( auto * m : *w )
            insert(m->location(), m->ascii());
    }

    display();
}

void cAsciiArt::insert( const cxy& loc, char c)
{
    myGrid[loc.y][loc.x] = c;
}
void cAsciiArt::display()
{

    // mark central grid cell
    myGrid[8][8] = 'X';

    for (auto &row : myGrid)
    {
        std::stringstream srow;
        for (char c : row)
        {
            srow << c;
        }
        std::cout << srow.str() << "\n";
    }
}