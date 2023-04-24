#include <string>
#include <vector>
#include <set>

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

    bool operator==(const cxy &other) const
    {
        return (x == other.x && y == other.y);
    }
    bool operator<(const cxy &other) const
    {
        return (x < other.x && y < other.y);
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

    std::vector<cModule *>::const_iterator
    begin()
    {
        return myModules.begin();
    }
    std::vector<cModule *>::const_iterator
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
    std::set<std::pair<int, int>> setEdge;
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

    /// @brief Total modules in use
    /// @return count
    int moduleCount();

    /// @brief Share or construct a module
    /// @param type module type
    /// @param loc location
    /// @return pointer to module at location of same type if already exists
    /// @return pointer to new module at location if empty
    /// throws exception if location has module of different type
    cModule *shareOrConstruct(
        eModuleType type,
        const cxy &loc);

    /// @brief Find module at location
    /// @param loc location
    /// @return module pointer if present, otherwise 0
    cModule *find(
        const cxy &loc) const;

    /// @brief Stores unique bidirection edge
    /// @param s source location
    /// @param d destination location
    void insert(const cxy &s, const cxy &d);

    /// @brief Counts unique links
    /// @return count
    int linkCount();
};

class cAsciiArt
{
public:
    cAsciiArt();
    void display(const cLayout &layout);

private:
    std::vector<std::vector<char>> myGrid;
    void insert(const cxy &loc, char c);
    void display();
};
