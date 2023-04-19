#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "workshop.h"

void cLayout::calculateLayout()
{
    cxy location(2, 4);
    agriculture(location);

    location.x = 2;
    location.y += 3;
    biotech(location);

    location.x = 2;
    location.y += 3;
    bool first = true;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::electronics)
            continue;

        w->move(location);
        auto mg = new cModule(eModuleType::solar);
        mg->move(cxy(w->location().x - 1, w->location().y));
        w->add(mg);
        mg = new cModule(eModuleType::stowage);
        mg->move(cxy(w->location().x + 1, w->location().y));
        w->add(mg);
        mg = new cModule(eModuleType::recycling);
        mg->move(cxy(w->location().x, w->location().y + 1));
        w->add(mg);

        w->CalcActualTemp();

        location.x += 3;
        if (location.x > 15)
        {
            location.x = 2;
            location.y += 3;
            first = true;
        }
    }

    location.x = 2;
    location.y += 3;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::energy)
            continue;

        w->move(location);
        auto mg = new cModule(eModuleType::artificialG);
        mg->move(cxy(w->location().x - 1, w->location().y));
        w->add(mg);
        mg = new cModule(eModuleType::solar);
        mg->move(cxy(w->location().x + 1, w->location().y));
        w->add(mg);
        mg = new cModule(eModuleType::stowage);
        mg->move(cxy(w->location().x, w->location().y + 1));
        w->add(mg);

        w->CalcActualTemp();

        location.x += 3;
        if (location.x > 15)
        {
            location.x = 2;
            location.y += 3;
            first = true;
        }
    }

    location.x = 5;
    location.y += 2;
    first = true;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::heavyindustry)
            continue;

        w->move(location);
        auto mg = new cModule(eModuleType::solar);
        mg->move(cxy(w->location().x - 1, w->location().y));
        w->add(mg);
        mg = new cModule(eModuleType::stowage);
        mg->move(cxy(w->location().x + 1, w->location().y));
        w->add(mg);
        mg = new cModule(eModuleType::recycling);
        mg->move(cxy(w->location().x, w->location().y + 1));
        w->add(mg);

        w->CalcActualTemp();

        location.x += 3;
        if (location.x > 15)
        {
            location.x = 2;
            location.y += 3;
            first = true;
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

void cLayout::agriculture(cxy &location)
{
    /// locate agricultural workshops so they share greenhouse modules
    bool first = true;
    cModule *mg;
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
            w->add(mg); // shared greenhouse with previous workshop

            mg = new cModule(eModuleType::greenhouse);
            mg->move(cxy(w->location().x + 1, w->location().y));
            w->add(mg);
        }

        mg = new cModule(eModuleType::solar);
        mg->move(cxy(w->location().x, w->location().y + 1));
        w->add(mg);
        mg = new cModule(eModuleType::artificialG);
        mg->move(cxy(w->location().x, w->location().y - 1));
        w->add(mg);

        w->CalcActualTemp();

        location.x += 2;
        if (location.x > 15)
        {
            location.x = 2;
            location.y += 3;
            first = true;
        }
    }
}

void cLayout::biotech(cxy &location)
{
    cModule *mg, *shared_module;
    bool first = true;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::biotech)
            continue;

        w->move(location);

        if (first)
        {
            first = false;
            mg = new cModule(eModuleType::artificialG);
            mg->move(cxy(w->location().x - 1, w->location().y));
            w->add(mg);
            shared_module = new cModule(eModuleType::artificialG);
            shared_module->move(cxy(w->location().x + 1, w->location().y));
            w->add(shared_module);
        }
        else
        {
            w->add(shared_module); // shared module with previous workshop

            shared_module = new cModule(eModuleType::artificialG);
            shared_module->move(cxy(w->location().x + 1, w->location().y));
            w->add(shared_module);
        }

        mg = new cModule(eModuleType::solar);
        mg->move(cxy(w->location().x, w->location().y - 1));
        w->add(mg);
        mg = new cModule(eModuleType::greenhouse);
        mg->move(cxy(w->location().x, w->location().y + 1));
        w->add(mg);

        w->CalcActualTemp();

        location.x += 2;
        if (location.x > 15)
        {
            location.x = 2;
            location.y += 3;
            first = true;
        }
    }
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
    float totalProductivity = 0;
    std::stringstream ss;
    ss << "\nLayout\n";
    for (auto *w : myLayout)
    {
        ss << w->text();
        totalProductivity += w->productivity();
    }
    ss << "\nTotal Productivity: " << totalProductivity << "\n";
    return ss.str();
}
