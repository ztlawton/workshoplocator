#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "workshop.h"

cWorkshop::cWorkshop(
    eWorkShopType type)
    : myType(type), myLoc(-1, -1)
{
    myPBPmax.resize(6);
    switch (myType)
    {
    case eWorkShopType::agriculture:
        myBaseTemp = 20;
        myMinTemp = 20;
        myMaxTemp = 39;
        myModuleNeeds =
            {eModuleType::artificialG,
             eModuleType::solar,
             eModuleType::greenhouse};
        myPBPmax[(int)eModuleType::artificialG] = 20;
        myPBPmax[(int)eModuleType::solar] = 10;
        myPBPmax[(int)eModuleType::greenhouse] = 40;
        break;
    case eWorkShopType::biotech:
        myBaseTemp = 5;
        myMinTemp = 30;
        myMaxTemp = 39;
        myModuleNeeds =
            {eModuleType::artificialG,
             eModuleType::solar,
             eModuleType::greenhouse};
        myPBPmax[(int)eModuleType::artificialG] = 30;
        myPBPmax[(int)eModuleType::solar] = 20;
        myPBPmax[(int)eModuleType::greenhouse] = 20;
        break;
    case eWorkShopType::electronics:
        myBaseTemp = 5;
        myMinTemp = 0;
        myMaxTemp = 9;
        myModuleNeeds =
            {eModuleType::solar,
             eModuleType::stowage,
             eModuleType::recycling};
        myPBPmax[(int)eModuleType::solar] = 20;
        myPBPmax[(int)eModuleType::stowage] = 20;
        myPBPmax[(int)eModuleType::recycling] = 30;
        break;
    case eWorkShopType::energy:
        myBaseTemp = 30;
        myMinTemp = 20;
        myMaxTemp = 29;
        myModuleNeeds =
            {eModuleType::artificialG,
             eModuleType::solar,
             eModuleType::stowage};
        myPBPmax[(int)eModuleType::artificialG] = 10;
        myPBPmax[(int)eModuleType::solar] = 50;
        myPBPmax[(int)eModuleType::stowage] = 10;
        break;
    case eWorkShopType::heavyindustry:
        myBaseTemp = 30;
        myMinTemp = 20;
        myMaxTemp = 29;
        myModuleNeeds =
            {eModuleType::solar,
             eModuleType::stowage,
             eModuleType::recycling};
        myPBPmax[(int)eModuleType::solar] = 20;
        myPBPmax[(int)eModuleType::stowage] = 30;
        myPBPmax[(int)eModuleType::recycling] = 20;
        break;
    default:
        throw std::runtime_error(
            "cWorkshop::cWorkshop unrecognized type");
    }
}


void cWorkshop::move(const cxy &newLocation)
{
    myLoc = newLocation;
}

void cWorkshop::ConstructModules()
{
    for (auto m : myModuleNeeds)
        myModules.push_back(new cModule(m));

    // locate the modules around the workshop
    std::vector<cxy> around{{1, 0}, {0, 1}, {0, -1}};
    auto it = around.begin();
    for (auto *m : myModules)
    {
        m->move(cxy(myLoc.x + it->x, myLoc.y + it->y));
        it++;
    }
}

void cWorkshop::add(cModule *m)
{
    myModules.push_back(m);
}

cModule *cWorkshop::add(
    eModuleType type,
    const cxy &loc)
{
    auto mg = new cModule(type);
    mg->move(loc);
    add(mg);
    return mg;
}

void cWorkshop::addRadiator()
{
    myModules.push_back(new cModule(eModuleType::radiator));
    myModules.back()->move(cxy(myLoc.x - 1, myLoc.y));
    CalcActualTemp();
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
    const std::vector<float> PBPdist{-1, 16.67, 13.33, 10, 6.67, 3.33};
    std::vector<float> PBPtype(6);
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
    for (float pbp : PBPtype)
    {
        totalPBP += pbp;
    }

    return totalPBP;
}

bool cWorkshop::CalcActualTemp()
{
    // reset to base temp
    myActualTemp = myBaseTemp;

    // std::cout << "base temp " << myActualTemp << "\n";

    // calculate heat change from each module, modified by distance
    for (auto *m : myModules)
    {
        switch (m->type())
        {
        case eModuleType::solar:
        case eModuleType::greenhouse:
        case eModuleType::artificialG:
        case eModuleType::stowage:
        case eModuleType::recycling:

            // heating module type
            {
                int delta = 0;
                switch (myLoc.dist(m->location()))
                {
                case 0:
                    throw std::runtime_error(
                        "cWorkshop::CalcActualTemp bad location");
                    break;
                case 1:
                    delta = m->heat();
                    break;
                case 2:
                    delta = m->heat() - 2;
                    break;
                case 3:
                    delta = m->heat() - 4;
                    break;
                case 4:
                    delta = m->heat() - 6;
                    break;
                case 5:
                    delta = m->heat() - 8;
                    break;
                default:
                    break;
                }
                // disregard heat changes that are negative,
                if (delta < 0)
                    delta = 0;
                myActualTemp += delta;

                // std::cout << " after m type " << (int)m->type() <<" " << myActualTemp << "\n";
            }
            break;

        case eModuleType::radiator:
        {
            // cooling module type

            int delta = m->heat();
            delta += 2 * (myLoc.dist(m->location()) - 1);
            // disregard heat changes that are positive
            if (delta > 0)
                delta = 0;
            myActualTemp += delta;
        }
        break;

        default:
            throw std::runtime_error(
                "cWorkshop::CalcActualTemp unrecognized module type");
        }
    }

    // check workshop temperature inside limits
    if (myMinTemp > myActualTemp || myActualTemp > myMaxTemp)
        return false;

    return true;
}

std::string cWorkshop::text()
{
    std::stringstream ss;
    switch (myType)
    {
    case eWorkShopType::agriculture:
        ss << "Agriculture Chambers (Workshop): ";
        break;
    case eWorkShopType::biotech:
        ss << "Biotech Complex (Workshop): ";
        break;
    case eWorkShopType::electronics:
        ss << "Electronics Cleanroom (Workshop): ";
        break;
    case eWorkShopType::energy:
        ss << "Energy Cluster (Workshop): ";
        break;
    case eWorkShopType::heavyindustry:
        ss << "Heavy Industry Hall (Workshop): ";
        break;
    default:
        ss << "Unknown workshop\n";
    }
    ss << "location: " << myLoc.x << " " << myLoc.y;
    ss << " temp: " << myActualTemp;
    if (myActualTemp > myMaxTemp)
        ss << " !!!! ";
    ss << " productivity: " << productivity() << "\n";
    for (auto *m : myModules)
    {
        ss << "\t" << m->text();
    }
    return ss.str();
}

void insertAscii(
    std::vector<std::vector<char>> &vgrid,
    const cxy &loc,
    char c)
{
    if (0 > loc.x || loc.x >= 17 ||
        0 > loc.y || loc.y >= 17)
    {
        std::cout << "WARNING: location outside grid\n";
        return;
    }
    vgrid[loc.y][loc.x] = c;
}
char cWorkshop::ascii() const
{
    char c;
    switch (myType)
    {
    case eWorkShopType::agriculture:
        c = 'A';
        break;
    case eWorkShopType::biotech:
        c = 'B';
        break;
    case eWorkShopType::electronics:
        c = 'E';
        break;
    case eWorkShopType::energy:
        c = 'N';
        break;
    case eWorkShopType::heavyindustry:
        c = 'H';
        break;
    default:
        throw std::runtime_error(
            "cWorkshop::ascii unkown workshop type");
    }
    return c;
}
