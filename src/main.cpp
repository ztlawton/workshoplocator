#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <algorithm>

enum class eModuleType
{
    artificalG,
    solar,
    greenhouse,
};

enum class eWorkShopType
{
    agriculture,
};

class cxy
{
    int x;
    int y;
};

class cModule
{
public:
private:
    cxy myLoc;
};

class cArificialG : public cModule
{
};
class cSolar : public cModule
{
};
class cGreenHouse : public cModule
{
};
class cWorkshop
{
public:
    cWorkshop(const std::vector<eModuleType> &needs)
        : myModuleNeeds(needs)
    {
    }
    virtual void move(const cxy &newLocation);

private:
    cxy myLoc;
    std::vector<eModuleType> myModuleNeeds;
};

class cAgriculture : public cWorkshop
{
public:
    cAgriculture()
        : cWorkshop({eModuleType::artificalG,
                     eModuleType::solar,
                     eModuleType::greenhouse})
    {
    }
};

class cLayout
{
public:
    void setWorkshopMix(
        const std::vector<eWorkShopType> mix);

    void calculateLayout();

private:
    std::vector<cWorkshop *> myLayout;
};

void cLayout::setWorkshopMix(
    const std::vector<eWorkShopType> mix)
{
    throw std::runtime_error(
        "cLayout::setWorkshopMix NYI"    );
}
void cLayout::calculateLayout()
{
     throw std::runtime_error(
        "cLayout::calculateLayout NYI"    );   
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
