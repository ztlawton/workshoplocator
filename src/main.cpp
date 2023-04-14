#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>
#include "workshop.h"

void cWorkshop::move(const cxy &newLocation)
{
    myLoc = newLocation;
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
    throw std::runtime_error(
        "cLayout::calculateLayout NYI");
}

main()
{
    // get required workshop mix
    const std::vector<eWorkShopType> mix;
    // ... prompt, read input, convert to vetor of workshop types

    cLayout L;
    L.setWorkshopMix(mix);
    L.calculateLayout();

    return 0;
}
