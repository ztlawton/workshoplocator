#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "workshop.h"

void cLayout::calculateLayout()
{
    /// locate the workshop and its modules
    const int space = 4;
    cxy location(2,space);
    for (auto *w : myLayout)
    {
        w->move(location);
        w->ConstructModules();

        if (!w->CalcActualTemp())
            w->addRadiator();

        // increment location for next workshop
        location.x += space;
        if (location.x > 15)
        {
            location.x = 2;
            location.y += space;
        }
    }
    moduleCount();
}

void cWorkshop::asciiArt(std::vector<std::vector<char>> &vgrid)
{
    vgrid[myLoc.x][myLoc.y] = 'W';
    for (auto *m : myModules)
    {
        vgrid[m->location().x][m->location().y] = 'm';
    }
}

void cLayout::displayAsciiArt()
{
    std::vector<std::vector<char>> vgrid(17, std::vector<char>(17, '.'));

    for (int kx = 0; kx < 17; kx++)
    {
        for (int ky = 0; ky < 17; ky++)
        {
            if (kx + ky < 4)
            {
                vgrid[kx][ky] = ' ';
            }
            int kxp = 17 - kx;
            if (kxp + ky < 4)
            {
                vgrid[kx][ky] = ' ';
            }
            int kyp = 17 - ky;
            if (kx + kyp < 4)
            {
                vgrid[kx][ky] = ' ';
            }
            if (kxp + kyp < 4)
            {
                vgrid[kx][ky] = ' ';
            }
        }
    }

    for (auto *w : myLayout)
        w->asciiArt(vgrid);

    for (auto &row : vgrid)
    {
        std::stringstream srow;
        for (char c : row)
        {
            srow << c;
        }
        std::cout << srow.str() << "\n";
    }
}