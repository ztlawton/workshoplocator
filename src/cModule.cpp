#include <sstream>
#include <iostream>
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
    // check location is within grid
    // TODO: check for cut off corners

    const int max = 17;
    if (0 > newLocation.x || newLocation.x >= max ||
        0 > newLocation.y || newLocation.y >= max)
        throw std::runtime_error(
            "cModule::move Outside grid");

    myLoc = newLocation;

    // record maximum y located module
    if (myLoc.y > theLastLocation.y)
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

char cModule::ascii()
{
    char c;
    switch (myType)
    {
    case eModuleType::radiator:
        c = 'r';
        break;
    case eModuleType::solar:
        c = 's';
        break;
    case eModuleType::greenhouse:
        c = 'g';
        break;
    case eModuleType::artificialG:
        c = 'a';
        break;
    case eModuleType::stowage:
        c = 't';
        break;
    case eModuleType::recycling:
        c = 'c';
        break;
    default:
        throw std::runtime_error(
            " cModule::ascii unrecognized module type");
    }

    return c;
}
