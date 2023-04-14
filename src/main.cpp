#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "workshop.h"

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

void cWorkshop::ConstructModules()
{
    for (auto m : myModuleNeeds)
        switch (m)
        {
        case eModuleType::solar:
            myModules.push_back(new cSolar);
            break;
        case eModuleType::artificalG:
            myModules.push_back(new cArtificialG);
            break;
        case eModuleType::greenhouse:
            myModules.push_back(new cGreenHouse);
            break;
        default:
            throw std::runtime_error(
                "cLayout::calculateLayout unkown module type");
        }

    // locate the modules around the workshop
    std::vector<cxy> around{{1, 0}, {0, 1}, {0, -1}};
    auto it = around.begin();
    for (auto *m : myModules)
    {
        m->move(cxy(myLoc.x + it->x, myLoc.y + it->y));
        it++;
    }
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
        ss << "location: " << myLoc.x << " " << myLoc.y << "\n";
        for (auto *m : myModules)
        {
            ss << "\t" << m->text() << "\n";
        }
        return ss.str();
    }

    cAgriculture::cAgriculture()
        : cWorkshop(
              eWorkShopType::agriculture,
              {eModuleType::artificalG,
               eModuleType::solar,
               eModuleType::greenhouse})
    {
    }
    void cLayout::setWorkshopMix(
        const std::vector<eWorkShopType> mix)
    {

        myLayout.clear();
        for (auto w : mix)
        {
            switch (w)
            {
            case eWorkShopType::agriculture:
                myLayout.push_back(new cAgriculture());
                break;

            default:
                throw std::runtime_error(
                    "cLayout::setWorkshopMix unknown workshop type");
            }
        }
    }
    void cLayout::calculateLayout()
    {
        std::vector<cxy> initLocs{
            {10, 10}, {20, 10}};

        auto itLoc = initLocs.begin();
        /// construct the modules
        for (auto *w : myLayout)
        {
            w->move(*itLoc++);
            w->ConstructModules();
        }
    }

    std::string cLayout::text()
    {
        std::stringstream ss;
        for (auto *w : myLayout)
            ss << w->text();
        return ss.str();
    }

    main()
    {
        // get required workshop mix
        std::vector<eWorkShopType> mix;
        // ... prompt, read input, convert to vetor of workshop types

        // test mix
        std::vector<eWorkShopType> testmix{
            eWorkShopType::agriculture,
            eWorkShopType::agriculture};

        cLayout L;
        L.setWorkshopMix(testmix);
        L.calculateLayout();
        std::cout << L.text();

        return 0;
    }
