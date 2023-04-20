#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "workshop.h"

int cModule::theModuleCount;
cxy cModule::theLastLocation;

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
    theModuleCount++;
}

    void cModule::clearModuleCount()
    {
        theModuleCount = 0;
        theLastLocation.y = 0;
    }

void cModule::move(const cxy &newLocation)
{
    myLoc = newLocation;
    if( myLoc.y > theLastLocation.y )
        theLastLocation.y = myLoc.y;

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
