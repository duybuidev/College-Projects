/*
 * Ho Chi Minh City University of Technology
 * Faculty of Computer Science and Engineering
 * Initial code for Assignment 2
 * Programming Fundamentals Spring 2025
 * Date: 02.02.2025
 */

// The library here is concretely set, students are not allowed to include any other libraries.
#ifndef _H_HCM_CAMPAIGN_H_
#define _H_HCM_CAMPAIGN_H_

#include "main.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
/// Complete the following functions
/// DO NOT modify any parameters in the functions.
////////////////////////////////////////////////////////////////////////

// Forward declaration
class Unit;
class UnitList;
class Army;
class TerrainElement;

class Vehicle;
class Infantry;

class LiberationArmy;
class ARVN;

class Position;

class Road;
class Mountain;
class River;
class Urban;
class Fortification;
class SpecialZone;

class BattleField;

class HCMCampaign;
class Configuration;

enum VehicleType
{
    TRUCK,
    MORTAR,
    ANTIAIRCRAFT,
    ARMOREDCAR,
    APC,
    ARTILLERY,
    TANK
};
enum InfantryType
{
    SNIPER,
    ANTIAIRCRAFTSQUAD,
    MORTARSQUAD,
    ENGINEER,
    SPECIALFORCES,
    REGULARINFANTRY
};

// Helper function declarations
string vehicleTypeToString(VehicleType vt);
VehicleType stringToVehicleType(const string& s);
string infantryTypeToString(InfantryType it);
InfantryType stringToInfantryType(const string& s);
Unit* cloneUnit(const Unit* unit_ptr); // Still takes const Unit* for cloning


class Position
{
private:
    int r, c;

public:
    Position(int r = 0, int c = 0);
    Position(const string &str_pos);
    int getRow() const;
    int getCol() const;
    void setRow(int r_val);
    void setCol(int c_val);
    string str() const;
    double distanceTo(const Position& other) const;
    bool operator==(const Position& other) const;
};

class Unit
{
protected:
    int quantity, weight;
    Position pos;

public:
    Unit(int quantity, int weight, const Position pos);
    virtual ~Unit();
    virtual int getAttackScore() = 0;
    Position getCurrentPosition() const;
    virtual string str() const = 0;

    int getQuantity() const;
    void setQuantity(int q);
    int getWeight() const;
    void setWeight(int w);
    virtual Unit* clone() const = 0; // Cloning still based on const original
};

class Vehicle : public Unit {
private:
    VehicleType vehicleType;
public:
    Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType);
    VehicleType getVehicleType() const;
    int getAttackScore() override;
    string str() const override;
    Unit* clone() const override;
};

class Infantry : public Unit {
private:
    InfantryType infantryType;
public:
    Infantry(int quantity, int weight, const Position pos, InfantryType infantryType);
    InfantryType getInfantryType() const;
    int getAttackScore() override;
    string str() const override;
    Unit* clone() const override;
};

class Unit; // Ensure Unit is declared before UnitList

class UnitList
{
private:
    struct Node {
        Unit* unit;
        Node* next;
        Node(Unit* u);
        ~Node();
    };
    Node* head;
    int capacity;
    int current_size;
    int vehicle_count;
    int infantry_count;

    void clear();

public:
    UnitList(int initial_capacity);
    ~UnitList();
    bool insert(Unit *unit_to_add);
    bool isContain(VehicleType vehicleType);
    bool isContain(InfantryType infantryType);
    string str() const;

    // ***** MODIFICATION HERE: Use a function pointer *****
    void removeUnitsIf(bool (*predicate)(Unit*));

    void setCapacity(int cap);
    int getCapacity() const;
    int getCurrentSize() const;
    vector<Unit*> getAllUnits();
    bool removeUnit(Unit* unit_to_remove);
    // void removeUnitsIf(std::function<bool(Unit*)> predicate); // OLD - REMOVE/REPLACE
    void updateCounts();
    Unit* findUnit(VehicleType vt);
    Unit* findUnit(InfantryType it);
};


class TerrainElement
{
protected:
    Position pos;
public:
    TerrainElement(const Position& p);
    virtual ~TerrainElement();
    virtual void getEffect(Army* army, const vector<Unit*>& all_army_units) = 0;
    virtual string getTypeString() const = 0;
};

class Road : public TerrainElement {
public:
    Road(const Position& p);
    void getEffect(Army* army, const vector<Unit*>& all_army_units) override;
    string getTypeString() const override;
};

class Mountain : public TerrainElement {
public:
    Mountain(const Position& p);
    void getEffect(Army* army, const vector<Unit*>& all_army_units) override;
    string getTypeString() const override;
};

class River : public TerrainElement {
public:
    River(const Position& p);
    void getEffect(Army* army, const vector<Unit*>& all_army_units) override;
    string getTypeString() const override;
};

class Urban : public TerrainElement {
public:
    Urban(const Position& p);
    void getEffect(Army* army, const vector<Unit*>& all_army_units) override;
    string getTypeString() const override;
};

class Fortification : public TerrainElement {
public:
    Fortification(const Position& p);
    void getEffect(Army* army, const vector<Unit*>& all_army_units) override;
    string getTypeString() const override;
};

class SpecialZone : public TerrainElement {
public:
    SpecialZone(const Position& p);
    void getEffect(Army* army, const vector<Unit*>& all_army_units) override;
    string getTypeString() const override;
};


class Army
{
protected:
    int LF, EXP;
    string name;
    UnitList *unitList;
    BattleField *battleField;

    static int roundToThreshold(int val, int min_val, int max_val);

public:
    // ***** MODIFICATION HERE *****
    Army(Unit **unitArray, int size, string army_name, BattleField *bf); // Changed to Unit**
    virtual ~Army();
    virtual void fight(Army *enemy, bool defense = false) = 0;
    virtual string str() const = 0;

    void updateStats();
    int getLF() const;
    int getEXP() const;
    void setLF(int val);
    void setEXP(int val);
    string getName() const;
    UnitList* getUnitList();
    BattleField* getBattleField();
};

class LiberationArmy : public Army {
public:
    // ***** MODIFICATION HERE *****
    LiberationArmy(Unit** unitArray, int size, string name, BattleField* bf); // Changed to Unit**
    void fight(Army* enemy_arvn, bool defense = false) override;
    string str() const override;
};

class ARVN : public Army {
public:
    // ***** MODIFICATION HERE *****
    ARVN(Unit** unitArray, int size, string name, BattleField* bf); // Changed to Unit**
    void fight(Army* enemy_lib_army, bool defense = false) override;
    string str() const override;
};


class BattleField
{
private:
    int n_rows, n_cols;
    TerrainElement*** terrain_map;

public:
    BattleField(int n_r, int n_c,
                const vector<Position *> &arrayForest,
                const vector<Position *> &arrayRiver,
                const vector<Position *> &arrayFortification,
                const vector<Position *> &arrayUrban,
                const vector<Position *> &arraySpecialZone);
    ~BattleField();
    string str() const;
    void applyTerrainEffects(Army* army);
    TerrainElement* getTerrainElementAt(const Position& pos) const;
    int getRows() const;
    int getCols() const;
    vector<TerrainElement*> getAllTerrainElements() const;
};


class Configuration {
private:
    int num_rows, num_cols;
    vector<Position*> arrayForest, arrayRiver, arrayFortification, arrayUrban, arraySpecialZone;
    vector<Unit*> liberationUnits_store;
    vector<Unit*> ARVNUnits_store;

    Unit** liberationUnits_ptr_array;
    int liberationUnits_count;
    Unit** ARVNUnits_ptr_array;
    int ARVNUnits_count;

    int eventCode;

    void parsePositions(const string& line, vector<Position*>& target_vector);
    void parseUnitList(const string& line);


public:
    Configuration(const string& filepath);
    ~Configuration();
    string str() const;

    int getNumRows() const;
    int getNumCols() const;
    const vector<Position*>& getArrayForest() const;
    const vector<Position*>& getArrayRiver() const;
    const vector<Position*>& getArrayFortification() const;
    const vector<Position*>& getArrayUrban() const;
    const vector<Position*>& getArraySpecialZone() const;

    // ***** MODIFICATION HERE (Return type) *****
    Unit** getLiberationUnitsPtrArray() const; // Changed to return Unit**
    int getLiberationUnitsCount() const;
    Unit** getARVNUnitsPtrArray() const;       // Changed to return Unit**
    int getARVNUnitsCount() const;

    int getEventCode() const;
};


class HCMCampaign
{
private:
    Configuration *config;
    BattleField *battleField;
    LiberationArmy *liberationArmy;
    ARVN *arvnArmy;

public:
    HCMCampaign(const string &config_file_path);
    ~HCMCampaign();
    void run();
    string printResult() const;
};

#endif
