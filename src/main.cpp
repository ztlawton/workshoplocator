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
    case eModuleType::artificalG:
        myHeat = 6;
        break;
    case eModuleType::solar:
        myHeat = 10;
        break;
    case eModuleType::greenhouse:
        myHeat = 2;
        break;
    case eModuleType::radiator:
        myHeat = -20;
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
    case eModuleType::solar:
        ss << "Solar module: ";
        break;
    case eModuleType::artificalG:
        ss << "ArtificialG module: ";
        break;
    case eModuleType::greenhouse:
        ss << "GreenHouse module: ";
        break;
    default:
        ss << "Unkown module ";
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
        case eModuleType::artificalG:
        case eModuleType::greenhouse:
        case eModuleType::solar:
        case eModuleType::recycling:
        case eModuleType::stowage:

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

void cLayout::calculateLayout()
{
    const int space = 4;
    cxy location(2, space);
    /// locate agricultural workshops so they share greenhouse modules
    bool first = true;
    cModule * mg;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::agriculture)
            continue;

        w->move(location);
        if (first)
        {
            mg = new cModule(eModuleType::greenhouse);
            mg->move(cxy(w->location().x - 1, w->location().y));
            w->add(mg);
            mg = new cModule(eModuleType::greenhouse);
            mg->move(cxy(w->location().x + 1, w->location().y));
            w->add(mg);
            first = false;
        }
        else
        {
            w->add( mg );   // shared greenhouse
            mg = new cModule(eModuleType::greenhouse);
            mg->move(cxy(w->location().x + 1, w->location().y));
            w->add(mg);
        }

        w->CalcActualTemp();

        location.x += 2;
        if (location.x > 15)
        {
            location.x = 2;
            location.y += space;
            first = true;
        }
    }

    /// locate the other workshops
    for (auto *w : myLayout)
    {
        // check workshop waiting to be located
        if (w->location().x > -1)
            continue;

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

void cLayout::setWorkshopMix(
    const std::vector<eWorkShopType> mix)
{
    myLayout.clear();
    for (auto w : mix)
        myLayout.push_back(new cWorkshop(w));
}

int cLayout::moduleCount()
{
    const int maxModules = 48;
    int count = 0;
    for (auto *w : myLayout)
        count += w->moduleCount();
    if (count > maxModules)
        throw std::runtime_error(
            "cLayout::moduleCount module count exceeded with " + std::to_string(count));
    return count;
}

std::string cLayout::text()
{
    std::stringstream ss;
    ss << "\nLayout\n";
    for (auto *w : myLayout)
        ss << w->text();
    return ss.str();
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
    // ... prompt, read input, convert to vetor of workshop types

    // // test mix
    // std::vector<eWorkShopType> testmix{
    //     eWorkShopType::agriculture,
    //     eWorkShopType::agriculture};

    cLayout L;
    L.setWorkshopMix(mix);
    L.calculateLayout();
    std::cout << L.text();
    L.displayAsciiArt();

    return 0;
}
