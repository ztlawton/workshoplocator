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
    :x( 0 ), y( 0 )
    {}
    cxy( int X, int Y )
        : x( X ), y( Y )
        {}
    int x;
    int y;
};

class cModule
{
public:
    cModule( eModuleType type )
    : myType( type )
    {}
    void move(const cxy &newLocation);
    std::string text();
protected:
    eModuleType myType;
    cxy myLoc;
};

class cArtificialG : public cModule
{
    public:
    cArtificialG()
    : cModule( eModuleType::artificalG)
    {}
};
class cSolar : public cModule
{
    public:
    cSolar()
    : cModule( eModuleType::solar)
    {}
};
class cGreenHouse : public cModule
{
        public:
    cGreenHouse()
    : cModule( eModuleType::greenhouse)
    {}
};
class cWorkshop
{
public:
    cWorkshop(
        eWorkShopType type,
        const std::vector<eModuleType> &needs)
        : myType(type),
          myModuleNeeds(needs)
    {
    }
    void move(const cxy &newLocation);
    void ConstructModules();
    std::string text();

private:
    eWorkShopType myType;
    cxy myLoc;
    std::vector<eModuleType> myModuleNeeds;
    std::vector<cModule *> myModules;
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
