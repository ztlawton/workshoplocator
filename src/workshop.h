#include <string>
#include <vector>

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

/// @brief Layout of workshops and modules
class cLayout
{
public:
    void setWorkshopMix(
        const std::vector<eWorkShopType> mix);

    void calculateLayout();

private:
    std::vector<cWorkshop *> myLayout;
};

