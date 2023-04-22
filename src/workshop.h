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

    char ascii();

    static void clearModuleCount();

    static int moduleCount()
    {
        return theModuleCount;
    }
    static cxy lastLocation()
    {
        return theLastLocation;
    }

protected:
    eModuleType myType;
    cxy myLoc;
    int myHeat;

    static int theModuleCount;
    static cxy theLastLocation;
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

    void add(cModule *m);

    /// @brief Construct and add a module
    /// @param type of module
    /// @param loc location
    /// @return pointer to new module
    cModule *add(
        eModuleType type,
        const cxy &loc);

    void addRadiator();
    int productivity();

    int moduleCount() const
    {
        return myModules.size();
    }
    cxy location() const
    {
        return myLoc;
    }
    eWorkShopType type() const
    {
        return myType;
    }

    char ascii() const;

    std::string text();

    std::vector<cModule*>::const_iterator
    begin()
    {
        return myModules.begin();
    }
    std::vector<cModule*>::const_iterator
    end()
    {
        return myModules.end();
    }

private:
    eWorkShopType myType;
    cxy myLoc;
    std::vector<eModuleType> myModuleNeeds;
    std::vector<cModule *> myModules;
    std::vector<int> myPBPmax; // max PBP contribution from each module type
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
    cLayout();
    void setWorkshopMix(
        const std::vector<eWorkShopType> mix);

    void calculateLayout();
    std::string text();
    //void displayAsciiArt();

    std::vector<cWorkshop *>::const_iterator
    begin() const
    {
        return myLayout.begin();
    }
    std::vector<cWorkshop *>::const_iterator
    end() const
    {
        return myLayout.end();
    }

private:
    std::vector<cWorkshop *> myLayout;
    int myMaxModules;

    void biotech(cxy &location);
    void biotech2(cxy &location);
    void agriculture(cxy &location);
    void agriculture2(cxy &location);
    void electronics(cxy &location);
    void electronics2(cxy &location);
    void energy(cxy &location);
    void energy2(cxy &location);
    void industry(cxy &location);

    int moduleCount();
};

class cAsciiArt
{
public:
    cAsciiArt();
    void display( const cLayout& layout );

private:
    std::vector<std::vector<char>> myGrid;
    void insert( const cxy& loc, char c );
    void display();
};

