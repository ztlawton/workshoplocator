#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <set>
#include <unordered_set>
#include <algorithm>
#include "workshop.h"

cLayout::cLayout()
    : myMaxModules(48)
{
}

void cLayout::calculateLayout()
{
    cModule::clearModuleCount();

    cxy location(5, 2);
    agriculture2(location);

    location.x = 2;
    location.y += 3;
    biotech2(location);

    location.x = 2;
    // location.y += 3;
    electronics2(location);

    location.x = 4;
    location.y += 3;
    energy2(location);

    location.x = 5;
    location.y += 1;
    industry(location);

    moduleCount();
    linkCount();
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
    location.y = cModule::lastLocation().y + 2;

    cModule *mg, *shared1, *shared2, *shared3, *shared4;
    bool first = true;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::electronics)
            continue;

        // check for involvement with the central grid square TID11
        if (7 <= location.y && location.y <= 10 &&
            7 < location.x && location.x < 10)
        {
            location.x = 10;
            first = true;
        }

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
    location.y = cModule::lastLocation().y + 2;
    int count = 0;
    for (auto *w : myLayout)
    {
        if (w->type() != eWorkShopType::energy)
            continue;

        w->move(location);

        if (count == 0)
        {

            w->add(shareOrConstruct(
                eModuleType::artificialG,
                cxy(location.x - 2, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::stowage,
                cxy(location.x - 2, location.y + 1)));
            w->add(shareOrConstruct(
                eModuleType::radiator,
                cxy(location.x - 1, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::radiator,
                cxy(location.x - 1, location.y + 1)));
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x, location.y + 1)));
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x + 1, location.y)));
            location.x += 1;
            location.y += 1;
            count = 1;
        }
        else if (count == 1)
        {
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x - 1, location.y)));
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x, location.y + 1)));
            w->add(shareOrConstruct(
                eModuleType::radiator,
                cxy(location.x + 1, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::artificialG,
                cxy(location.x + 2, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::radiator,
                cxy(location.x + 1, location.y + 1)));
            w->add(shareOrConstruct(
                eModuleType::stowage,
                cxy(location.x + 2, location.y + 1)));

            count = 2;
            location.x += -1;
            location.y += 1;
        }
        else if (count == 2)
        {
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x + 1, location.y)));
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x, location.y + 1)));
            w->add(shareOrConstruct(
                eModuleType::stowage,
                cxy(location.x - 2, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::radiator,
                cxy(location.x - 1, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::artificialG,
                cxy(location.x - 2, location.y + 1)));
            w->add(shareOrConstruct(
                eModuleType::radiator,
                cxy(location.x - 1, location.y + 1)));

            count = 3;
            location.x += 1;
            location.y += 1;
        }
        else if (count == 3)
        {
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x - 1, location.y)));
            w->add(shareOrConstruct(
                eModuleType::solar,
                cxy(location.x, location.y + 1)));
            w->add(shareOrConstruct(
                eModuleType::radiator,
                cxy(location.x + 1, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::stowage,
                cxy(location.x + 2, location.y - 1)));
            w->add(shareOrConstruct(
                eModuleType::radiator,
                cxy(location.x + 1, location.y + 1)));
            w->add(shareOrConstruct(
                eModuleType::artificialG,
                cxy(location.x + 2, location.y + 1)));

            count = 0;
            location.x += -1;
            location.y += 1;
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

void cLayout::insert(const cxy &s, const cxy &d)
{
    int si = 17 * s.y + s.x;
    int di = 17 * d.y + d.x;
    if (di < si)
    {
        int temp = si;
        si = di;
        di = temp;
    }
    setEdge.insert(std::make_pair(si, di));
}

int cLayout::linkCount()
{
    setEdge.clear();

    for (auto *w : myLayout)
    {
        int wx = w->location().x;
        int wy = w->location().y;
        for (auto *m : *w)
        {
            int mx = m->location().x;
            int my = m->location().y;
            int dy = wy - my;
            if (dy)
            {
                int inc = 1;
                if (dy < 0)
                    inc = -1;
                cxy prev(mx, my);
                for (int y = my + inc; y != wy; y += inc)
                    insert(prev, cxy(mx, y));
                insert(prev, cxy(mx, wy));
            }
            int dx = wx - mx;
            if (dx)
            {
                int inc = 1;
                if (dx < 0)
                    inc = -1;
                cxy prev(mx, wy);
                for (int x = mx + inc; x != wx; x += inc)
                    insert(prev, cxy(x, wy));
                insert(prev, w->location());
            }
        }
    }
    return setEdge.size();
}

cModule *cLayout::find(const cxy &loc) const
{
    for (auto *w : myLayout)
        for (auto *m : *w)
            if (loc == m->location())
                return m;
    return 0;
}

cModule *cLayout::shareOrConstruct(
    eModuleType type,
    const cxy &loc)
{
    auto *m = find(loc);
    if (!m)
    {
        m = new cModule(type);
        m->move(loc);
        return m;
    }
    if (m->type() != type)
        throw std::runtime_error(
            "cLayout::share Two different modules on same location");
    return m;
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
       << " ( max " << myMaxModules << " )";
    ss << "   Link Count: " << setEdge.size() << "\n";

    for (int m : mix)
    {
        ss << m << " ";
    }
    ss << " => Total Productivity: " << totalProductivity << "\n";
    return ss.str();
}
