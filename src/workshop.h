#include <string>
#include <vector>

enum class eModuleType
{
    artificalG,
    solar,
    greenhouse,
    radiator,
    stowage,
    recycling,
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
    int dist(const cxy &other)
    {
        return abs(x - other.x) + abs(y - other.y);
    }
    int x;
    int y;
};

class cModule
{
public:
    cModule(eModuleType type);

    void move(const cxy &newLocation);

    eModuleType type() const
    {
        return myType;
    }
    cxy location() const
    {
        return myLoc;
    }
    int heat() const
    {
        return myHeat;
    }
    std::string text();

protected:
    eModuleType myType;
    cxy myLoc;
    int myHeat;
};

class cWorkshop
{
public:
    cWorkshop(eWorkShopType type);
    void move(const cxy &newLocation);
    void ConstructModules();

    /// @brief calc temperature from heating/cooling effects of modules
    /// @return true if temperature within limits
    bool CalcActualTemp();

    void addRadiator();
    
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
