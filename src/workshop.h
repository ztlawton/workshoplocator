#include <string>
#include <vector>

enum class eModuleType
{
    radiator,
    solar,
    greenhouse,
    artificialG,
    stowage,
    recycling,
};

enum class eWorkShopType
{
    agriculture,
    biotech,
    electronics,
    energy,
    heavyindustry,
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

    static void clearModuleCount()
    {
        theModuleCount = 0;
    }
    static int moduleCount()
    {
        return theModuleCount;
    }

protected:
    eModuleType myType;
    cxy myLoc;
    int myHeat;
    static int theModuleCount;
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

    void add( cModule* m );
    void addRadiator();
    int productivity();

    int moduleCount() const
    {
        return myModules.size();
    }
    cxy location() const{
        return myLoc;
    }
    eWorkShopType type() const{
        return myType;
    }

    void asciiArt( std::vector<std::vector<char>>& vgrid );

    std::string text();

private:
    eWorkShopType myType;
    cxy myLoc;
    std::vector<eModuleType> myModuleNeeds;
    std::vector<cModule *> myModules;
    std::vector<int> myPBPmax;          // max PBP contribution from each module type
    int myBaseTemp;
    int myMinTemp;
    int myMaxTemp;
    int myActualTemp;

    float ProductivityBonusPoints();
};

/// @brief Layout of workshops and modules
class cLayout
{
public:
    void setWorkshopMix(
        const std::vector<eWorkShopType> mix);

    void calculateLayout();
    std::string text();
    void displayAsciiArt();

private:
    std::vector<cWorkshop *> myLayout;

    void biotech( cxy& location );
    void biotech2( cxy& location );
    void agriculture( cxy& location );
    void electronics( cxy& location );
    void electronics2( cxy& location );
    void energy( cxy& location );
    void industry( cxy& location );

    int moduleCount();
};

/// Insert character at location in ascii art grid
void insertAscii(
    std::vector<std::vector<char>> &vgrid,
    const cxy &loc,
    char c);
