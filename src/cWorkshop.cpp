#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "workshop.h"

cWorkshop::cWorkshop(
    eWorkShopType type)
    : myType(type)
{
    myPBPmax.resize(6);
    switch (myType)
    {
    case eWorkShopType::agriculture:
        myBaseTemp = 20;
        myMinTemp = 20;
        myMaxTemp = 39;
        myModuleNeeds =
            {eModuleType::artificalG,
             eModuleType::solar,
             eModuleType::greenhouse};
        myPBPmax[ (int) eModuleType::artificalG ] = 6;
        myPBPmax[ (int) eModuleType::solar ] = 3;
        myPBPmax[ (int) eModuleType::artificalG ] = 12;
        break;
    default:
        throw std::runtime_error(
            "cWorkshop::cWorkshop unrecognized type");
    }
}

void cLayout::calculateLayout()
{
    /// locate the workshop and its modules
    const int space = 4;
    cxy location(2, space);
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

int cWorkshop::productivity()
{
    // base productivity
    int productivity = 10;

    // productivity bonus points from connected modules
    productivity += ProductivityBonusPoints();

    // temperature adjust
    if (myMinTemp > myActualTemp || myActualTemp > myMaxTemp)
        productivity *= 0.75;

    return productivity;
}

float cWorkshop::ProductivityBonusPoints()
{
    const std::vector<float> PBPdist{-1, 16.67, 13.33, 1, 6.67, 3.33};
    std::vector<float> PBPtype(5);
    float totalPBP = 0;

    // PBP from each connected module
    for (auto *m : myModules)
    {
        // check module is required
        if (std::find(
                myModuleNeeds.begin(), myModuleNeeds.end(), m->type()) ==
            myModuleNeeds.end())
            continue;

        int dist = myLoc.dist(m->location());
        float PBP = 16.65;
        if (dist < 1)
            throw std::runtime_error(
                "cWorkshop::ModuleStrengthPoints bad distance");
        if (dist > 5)
            PBP = 0;
        else
            PBP = PBPdist[dist];
        PBPtype[(int)m->type()] += PBP;
    }

    // check maximum PBP from each module type
    for (int kmt = 0; kmt < 6; kmt++)
    {
        if (PBPtype[kmt] > myPBPmax[kmt])
            PBPtype[kmt] = myPBPmax[kmt];
    }

    // sum PBPs from all types
    for( float pbp : PBPtype )
    {
        totalPBP += pbp;
    }

    return totalPBP;
}

std::string cWorkshop::text()
{
    std::stringstream ss;
    switch (myType)
    {
    case eWorkShopType::agriculture:
        ss << "Agriculture workshop: ";
        break;
    default:
        ss << "Unkown workshop\n";
    }
    ss << "location: " << myLoc.x << " " << myLoc.y;
    ss << " temp: " << myActualTemp;
    ss << " productivity: " << productivity() << "\n";
    for (auto *m : myModules)
    {
        ss << "\t" << m->text();
    }
    return ss.str();
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