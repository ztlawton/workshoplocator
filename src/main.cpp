#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "workshop.h"

cModule::cModule(eModuleType type)
    : myType(type)

{
    switch (myType)
    {
    case eModuleType::radiator:
        myHeat = -20;
        break;
    case eModuleType::solar:
        myHeat = 10;
        break;
    case eModuleType::greenhouse:
        myHeat = 2;
        break;
    case eModuleType::artificialG:
        myHeat = 6;
        break;
    case eModuleType::stowage:
        myHeat = 6;
        break;
    case eModuleType::recycling:
        myHeat = 8;
        break;
    default:
        throw std::runtime_error(
            "cModule unrecognised type");
    }
}

void cModule::move(const cxy &newLocation)
{
    myLoc = newLocation;
}

std::string cModule::text()
{
    std::stringstream ss;
    switch (myType)
    {
    case eModuleType::radiator:
        ss << "Radiator (Module): ";
        break;
    case eModuleType::solar:
        ss << "Solar Cells (Module): ";
        break;
    case eModuleType::greenhouse:
        ss << "Greenhouse (Module): ";
        break;
    case eModuleType::artificialG:
        ss << "Artificial-G Lab (Module): ";
        break;
    case eModuleType::stowage:
        ss << "Stowage Platform (Module): ";
        break;
    case eModuleType::recycling:
        ss << "Recycling System (Module): ";
        break;
    default:
        ss << "Unknown Module ";
    }
    ss << "location: " << myLoc.x << " " << myLoc.y << "\n";
    return ss.str();
}

void cWorkshop::move(const cxy &newLocation)
{
    myLoc = newLocation;
}

bool cWorkshop::CalcActualTemp()
{
    // reset to base temp
    myActualTemp = myBaseTemp;

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


std::vector<eWorkShopType>
workshopMixFromUser()
{
    std::cout << "Input Workshop Mix\n"
                 "Enter number of each workshop type in order\n"
                 "Agriculture Biotech Electronics Energy HeavyIndustry\n";

    std::vector<eWorkShopType> ret;
    int count;
    for (int k = 0; k < 5; k++)
    {
        std::cin >> count;
        for (int w = 0; w < count; w++)
            ret.push_back((eWorkShopType)k);
    }
    return ret;
}
main()
{
    // get required workshop mix
    std::vector<eWorkShopType> mix = workshopMixFromUser();

    cLayout L;
    L.setWorkshopMix(mix);
    L.calculateLayout();
    std::cout << L.text();
    L.displayAsciiArt();

    return 0;
}
