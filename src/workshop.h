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
public:
    cxy()
        : x(0), y(0)
    {
    }
    cxy(int X, int Y)
        : x(X), y(Y)
    {
    }
    int dist( const cxy& other )
    {
        return abs(x-other.x) + abs(y-other.y);
    }
    int x;
    int y;
};

class cModule
{
public:
    cModule(eModuleType type, int heat)
        : myType(type),
          myHeat(heat)
    {
    }
    void move(const cxy &newLocation);
    cxy location() const
    {
        return myLoc;
    }
    int heat() const{
        return myHeat;
    }
    std::string text();

protected:
    eModuleType myType;
    cxy myLoc;
    int myHeat;
};

class cArtificialG : public cModule
{
public:
    cArtificialG()
        : cModule(eModuleType::artificalG, 6)
    {
    }
};
class cSolar : public cModule
{
public:
    cSolar()
        : cModule(eModuleType::solar, 10)
    {
    }
};
class cGreenHouse : public cModule
{
public:
    cGreenHouse()
        : cModule(eModuleType::greenhouse, 2)
    {
    }
};
class cWorkshop
{
public:
    cWorkshop(eWorkShopType type,
              const std::vector<eModuleType> &needs);
    void move(const cxy &newLocation);
    void ConstructModules();
    void CalcActualTemp();
    std::string text();

private:
    eWorkShopType myType;
    cxy myLoc;
    std::vector<eModuleType> myModuleNeeds;
    std::vector<cModule *> myModules;
    int myBaseTemp;
    int myMinTemp;
    int myMaxTemp;
    int myActualTemp;
};

class cAgriculture : public cWorkshop
{
public:
    cAgriculture();
};

/// @brief Layout of workshops and modules
class cLayout
{
public:
    void setWorkshopMix(
        const std::vector<eWorkShopType> mix);

    void calculateLayout();
    std::string text();

private:
    std::vector<cWorkshop *> myLayout;
};
