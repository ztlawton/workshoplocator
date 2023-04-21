#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "workshop.h"

cLayout::cLayout()
    : myMaxModules(48)
{
}

void cLayout::calculateLayout()
{
    cModule::clearModuleCount();

    cxy location(5, 3);
    agriculture2(location);

    location.x = 2;
    location.y += 3;
    biotech2(location);

    location.x = 2;
    location.y += 3;
    electronics2(location);

    location.x = 4;
    location.y += 3;
    energy2(location);

    location.x = 5;
    location.y += 1;
    industry(location);

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

void cLayout::agriculture2(cxy &location)
{
    /// locate agricultural workshops so they share greenhouse modules
    bool first = true;
    bool even = true;
    cModule *mg, *sh1, *sh2, *sh3;
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
            first = false;
        }
        else
        {
            w->add(sh1); // shared greenhouse with previous workshop
        }
        if (even)
        {
            sh2 = new cModule(eModuleType::greenhouse);
            sh2->move(cxy(w->location().x + 1, w->location().y + 1));
            w->add(sh2);
            even = false;
        }
        else
        {
            w->add(sh2);
            even = true;
        }

        mg = new cModule(eModuleType::solar);
        mg->move(cxy(w->location().x - 1, w->location().y + 1));
        w->add(mg);
        mg = new cModule(eModuleType::artificialG);
        mg->move(cxy(w->location().x - 1, w->location().y - 1));
        w->add(mg);
        sh1 = new cModule(eModuleType::greenhouse);
        sh1->move(cxy(w->location().x + 1, w->location().y));
        w->add(sh1);

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
        mg = new cModule(eModuleType::stowage);
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

void cLayout::biotech2(cxy &location)
{
    // https://github.com/JamesBremner/workshoplocator/issues/12#issuecomment-1514917857

    cModule *shared1, *shared2;
    bool first = true;
    bool even = true;

    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::biotech)
            continue;

        w->move(location);

        auto mg = new cModule(eModuleType::artificialG);
        mg->move(cxy(w->location().x, w->location().y + 1));
        w->add(mg);
        mg = new cModule(eModuleType::artificialG);
        mg->move(cxy(w->location().x, w->location().y - 1));
        w->add(mg);

        if (first)
        {
            mg = new cModule(eModuleType::solar);
            mg->move(cxy(w->location().x - 1, w->location().y + 1));
            w->add(mg);
            mg = new cModule(eModuleType::solar);
            mg->move(cxy(w->location().x - 1, w->location().y - 1));
            w->add(mg);
            shared1 = new cModule(eModuleType::greenhouse);
            shared1->move(cxy(w->location().x + 1, w->location().y + 1));
            w->add(shared1);
            shared2 = new cModule(eModuleType::greenhouse);
            shared2->move(cxy(w->location().x + 1, w->location().y - 1));
            w->add(shared2);
            first = false;
            even = false;
        }
        else
        {
            w->add(shared1);
            w->add(shared2);
            if (even)
            {
                shared1 = new cModule(eModuleType::greenhouse);
                shared1->move(cxy(w->location().x + 1, w->location().y + 1));
                w->add(shared1);
                shared2 = new cModule(eModuleType::greenhouse);
                shared2->move(cxy(w->location().x + 1, w->location().y - 1));
                w->add(shared2);
                even = false;
            }
            else
            {
                shared1 = new cModule(eModuleType::solar);
                shared1->move(cxy(w->location().x + 1, w->location().y + 1));
                w->add(shared1);
                shared2 = new cModule(eModuleType::solar);
                shared2->move(cxy(w->location().x + 1, w->location().y - 1));
                w->add(shared2);
                even = true;
            }
        }

        w->CalcActualTemp();

        location.x += 2;
        if (location.x > 15)
        {
            location.x = 2;
            location.y += 3;
            first = true;
            even = true;
        }
    }
}
void cLayout::electronics(cxy &location)
{
    cModule *mg, *shared_module;
    bool first = true;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::electronics)
            continue;

        w->move(location);

        if (first)
        {
            first = false;
            mg = new cModule(eModuleType::recycling);
            mg->move(cxy(w->location().x - 1, w->location().y));
            w->add(mg);
            shared_module = new cModule(eModuleType::recycling);
            shared_module->move(cxy(w->location().x + 1, w->location().y));
            w->add(shared_module);
        }
        else
        {
            w->add(shared_module); // shared module with previous workshop

            shared_module = new cModule(eModuleType::recycling);
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
void cLayout::electronics2(cxy &location)
{
    location.x += 1;

    cModule *mg, *shared1, *shared2, *shared3, *shared4;
    bool first = true;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::electronics)
            continue;

        w->move(location);

        if (first)
        {
            first = false;
            mg = new cModule(eModuleType::stowage);
            mg->move(cxy(w->location().x - 1, w->location().y + 2));
            w->add(mg);
            shared1 = new cModule(eModuleType::stowage);
            shared1->move(cxy(w->location().x + 1, w->location().y + 2));
            w->add(shared1);
            mg = new cModule(eModuleType::solar);
            mg->move(cxy(w->location().x - 1, w->location().y + 1));
            w->add(mg);
            shared2 = new cModule(eModuleType::solar);
            shared2->move(cxy(w->location().x + 1, w->location().y + 1));
            w->add(shared2);
            mg = new cModule(eModuleType::recycling);
            mg->move(cxy(w->location().x - 1, w->location().y));
            w->add(mg);
            shared3 = new cModule(eModuleType::recycling);
            shared3->move(cxy(w->location().x + 1, w->location().y));
            w->add(shared3);
            mg = new cModule(eModuleType::radiator);
            mg->move(cxy(w->location().x - 1, w->location().y - 1));
            w->add(mg);
            shared4 = new cModule(eModuleType::radiator);
            shared4->move(cxy(w->location().x + 1, w->location().y - 1));
            w->add(shared4);
        }
        else
        {
            w->add(shared1);
            shared1 = new cModule(eModuleType::stowage);
            shared1->move(cxy(w->location().x + 1, w->location().y + 2));
            w->add(shared1);
            w->add(shared2);
            shared2 = new cModule(eModuleType::solar);
            shared2->move(cxy(w->location().x + 1, w->location().y + 1));
            w->add(shared2);
            w->add(shared3);
            shared3 = new cModule(eModuleType::recycling);
            shared3->move(cxy(w->location().x + 1, w->location().y));
            w->add(shared3);
            w->add(shared4);
            shared4 = new cModule(eModuleType::radiator);
            shared4->move(cxy(w->location().x + 1, w->location().y - 1));
            w->add(shared4);
        }

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

void cLayout::energy(cxy &location)
{
    cModule *mg, *shared_module;
    bool first = true;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::energy)
            continue;

        w->move(location);

        if (first)
        {
            first = false;
            mg = new cModule(eModuleType::solar);
            mg->move(cxy(w->location().x - 1, w->location().y));
            w->add(mg);
            shared_module = new cModule(eModuleType::solar);
            shared_module->move(cxy(w->location().x + 1, w->location().y));
            w->add(shared_module);
        }
        else
        {
            w->add(shared_module); // shared module with previous workshop

            shared_module = new cModule(eModuleType::solar);
            shared_module->move(cxy(w->location().x + 1, w->location().y));
            w->add(shared_module);
        }

        mg = new cModule(eModuleType::artificialG);
        mg->move(cxy(w->location().x, w->location().y - 1));
        w->add(mg);
        mg = new cModule(eModuleType::stowage);
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
void cLayout::energy2(cxy &location)
{
    cModule *sh[20];
    location.y = cModule::lastLocation().y + 2;
    int count = 0;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::energy)
            continue;

        w->move(location);

        if (count == 0)
        {
            count++;
            w->add(
                eModuleType::artificialG,
                cxy(w->location().x - 2, w->location().y - 1));
            sh[3] = w->add(
                eModuleType::stowage,
                cxy(w->location().x - 2, w->location().y + 1));
            w->add(
                eModuleType::radiator,
                cxy(w->location().x - 1, w->location().y - 1));
            sh[4] = w->add(
                eModuleType::radiator,
                cxy(w->location().x - 1, w->location().y + 1));
            w->add(
                eModuleType::solar,
                cxy(w->location().x, w->location().y - 1));
            sh[5] = w->add(
                eModuleType::solar,
                cxy(w->location().x, w->location().y + 1));
            sh[2] = w->add(
                eModuleType::solar,
                cxy(w->location().x + 1, w->location().y));
            location.x += 1;
            location.y += 1;
        }
        else if (count == 1)
        {
            count++;
            w->add(sh[2]);
            w->add(sh[5]);
            sh[8] = w->add(
                eModuleType::solar,
                cxy(w->location().x, w->location().y + 1));
            w->add(
                eModuleType::radiator,
                cxy(w->location().x + 1, w->location().y - 1));
            sh[9] = w->add(
                eModuleType::radiator,
                cxy(w->location().x + 1, w->location().y + 1));
            w->add(
                eModuleType::artificialG,
                cxy(w->location().x + 2, w->location().y - 1));
            sh[10] = w->add(
                eModuleType::stowage,
                cxy(w->location().x + 2, w->location().y + 1));
            location.x += -1;
            location.y += 1;
        }
        else if (count == 2)
        {
            count++;
            w->add(sh[3]);
            w->add(sh[4]);
            w->add(sh[5]);
            w->add(sh[8]);
            w->add(
                eModuleType::artificialG,
                cxy(w->location().x - 2, w->location().y + 1));
            w->add(
                eModuleType::radiator,
                cxy(w->location().x -1, w->location().y + 1));
            w->add(
                eModuleType::solar,
                cxy(w->location().x, w->location().y + 1));
        }
        else
        {
            throw std::runtime_error(
                "cLayout::energy2 max energy workshops ( 3 ) exceeded"            );
        }

        w->CalcActualTemp();
    }
}
void cLayout::industry(cxy &location)
{
    cModule *mg, *shared_module;
    bool first = true;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::heavyindustry)
            continue;

        w->move(location);

        if (first)
        {
            first = false;
            mg = new cModule(eModuleType::stowage);
            mg->move(cxy(w->location().x - 1, w->location().y));
            w->add(mg);
            shared_module = new cModule(eModuleType::stowage);
            shared_module->move(cxy(w->location().x + 1, w->location().y));
            w->add(shared_module);
        }
        else
        {
            w->add(shared_module); // shared module with previous workshop

            shared_module = new cModule(eModuleType::stowage);
            shared_module->move(cxy(w->location().x + 1, w->location().y));
            w->add(shared_module);
        }

        mg = new cModule(eModuleType::solar);
        mg->move(cxy(w->location().x, w->location().y - 1));
        w->add(mg);
        mg = new cModule(eModuleType::recycling);
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
    int count = cModule::moduleCount();
    if (count > myMaxModules)
        throw std::runtime_error(
            "cLayout::moduleCount module count exceeded with " + std::to_string(count));
    return count;
}

std::string cLayout::text()
{
    float totalProductivity = 0;
    std::vector<int> mix(5);
    std::stringstream ss;
    ss << "\nLayout\n";
    for (auto *w : myLayout)
    {
        ss << w->text();
        totalProductivity += w->productivity();
        mix[(int)w->type()] += 1;
    }
    ss << "\n";

    ss << "Module Count: " << cModule::moduleCount()
       << " ( max " << myMaxModules << " )\n";

    for (int m : mix)
    {
        ss << m << " ";
    }
    ss << " => Total Productivity: " << totalProductivity << "\n";
    return ss.str();
}
