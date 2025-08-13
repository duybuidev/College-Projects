#include "hcmcampaign.h"

////////////////////////////////////////////////////////////////////////
/// STUDENT'S ANSWER BEGINS HERE
////////////////////////////////////////////////////////////////////////

string vehicleTypeToString(VehicleType vt) {
    switch (vt) {
        case TRUCK: return "TRUCK";
        case MORTAR: return "MORTAR";
        case ANTIAIRCRAFT: return "ANTIAIRCRAFT";
        case ARMOREDCAR: return "ARMOREDCAR";
        case APC: return "APC";
        case ARTILLERY: return "ARTILLERY";
        case TANK: return "TANK";
        default: return "UNKNOWN_VEHICLE_TYPE";
    }
}

VehicleType stringToVehicleType(const string& s) {
    if (s == "TRUCK") return TRUCK;
    if (s == "MORTAR") return MORTAR;
    if (s == "ANTIAIRCRAFT") return ANTIAIRCRAFT;
    if (s == "ARMOREDCAR") return ARMOREDCAR;
    if (s == "APC") return APC;
    if (s == "ARTILLERY") return ARTILLERY;
    if (s == "TANK") return TANK;
    throw runtime_error("Unknown VehicleType string: " + s);
}

string infantryTypeToString(InfantryType it) {
    switch (it) {
        case SNIPER: return "SNIPER";
        case ANTIAIRCRAFTSQUAD: return "ANTIAIRCRAFTSQUAD";
        case MORTARSQUAD: return "MORTARSQUAD";
        case ENGINEER: return "ENGINEER";
        case SPECIALFORCES: return "SPECIALFORCES";
        case REGULARINFANTRY: return "REGULARINFANTRY";
        default: return "UNKNOWN_INFANTRY_TYPE";
    }
}

InfantryType stringToInfantryType(const string& s) {
    if (s == "SNIPER") return SNIPER;
    if (s == "ANTIAIRCRAFTSQUAD") return ANTIAIRCRAFTSQUAD;
    if (s == "MORTARSQUAD") return MORTARSQUAD;
    if (s == "ENGINEER") return ENGINEER;
    if (s == "SPECIALFORCES") return SPECIALFORCES;
    if (s == "REGULARINFANTRY") return REGULARINFANTRY;
    throw runtime_error("Unknown InfantryType string: " + s);
}

bool isPerfectSquare(int n) {
    if (n < 0) return false;
    if (n == 0) return true;
    int root = round(sqrt(n));
    return root * root == n;
}

int sumDigits(int n) {
    string s = to_string(n);
    int sum = 0;
    for (char c : s) {
        sum += c - '0';
    }
    return sum;
}

int getPersonalNumber(int num_val, int year_val) {
    int sum_year_digits = 0;
    string year_str = to_string(year_val);
    for (char c : year_str) {
        sum_year_digits += c - '0';
    }

    int total_sum = num_val + sum_year_digits;
    while (total_sum > 9) {
        total_sum = sumDigits(total_sum);
    }
    return total_sum;
}

int getFibonacciNearest(int val) {
    if (val <= 0) return 0;
    if (val == 1) return 1;
    
    int f_prev = 0, f_curr = 1;
    if (val == 0) return 0;

    while(f_curr < val) {
        int temp = f_curr;
        f_curr = f_prev + f_curr;
        f_prev = temp;
        if (f_curr == val) return val;
    }
    
    if (abs(val - f_prev) <= abs(f_curr - val)) {
        if (abs(val - f_prev) == abs(f_curr - val)) return f_curr; 
        return f_prev;
    } else {
        return f_curr;
    }
}

bool isSpecialNumber(int S, int k) {
    if (S < 0) return false;
    if (S == 0) return true; 

    int temp_S = S;
    while (temp_S > 0) {
        if (temp_S % k > 1) return false;
        temp_S /= k;
    }
    return true;
}

int calculateUnitListCapacity(int S) {
    bool special = false;
    if (isSpecialNumber(S, 3) || isSpecialNumber(S, 5) || isSpecialNumber(S, 7)) {
        special = true;
    }
    return special ? 12 : 8;
}

Unit* cloneUnit(const Unit* unit_ptr) {
    if (!unit_ptr) return nullptr;
    return unit_ptr->clone();
}

// Position Method Implementations
Position::Position(int r, int c) : r(r), c(c) {}

Position::Position(const string& str_pos) {
    size_t comma_pos = str_pos.find(',');
    if (str_pos.length() < 5 || str_pos[0] != '(' || str_pos.back() != ')' || comma_pos == string::npos) {
        throw runtime_error("Invalid position string format: " + str_pos);
    }
    string r_str = str_pos.substr(1, comma_pos - 1);
    string c_str = str_pos.substr(comma_pos + 1, str_pos.length() - comma_pos - 2);
    try {
        this->r = stoi(r_str);
        this->c = stoi(c_str);
    } catch (const std::invalid_argument& ia) {
        throw runtime_error("Invalid number in position string: " + str_pos);
    }
}
int Position::getRow() const { return r; }
int Position::getCol() const { return c; }
void Position::setRow(int r_val) { r = r_val; }
void Position::setCol(int c_val) { c = c_val; }
string Position::str() const {
    return "(" + to_string(r) + "," + to_string(c) + ")";
}
double Position::distanceTo(const Position& other) const {
    return sqrt(pow(this->r - other.r, 2) + pow(this->c - other.c, 2));
}
bool Position::operator==(const Position& other) const {
    return r == other.r && c == other.c;
}


// Unit Method Implementations
Unit::Unit(int quantity, int weight, const Position pos)
    : quantity(quantity), weight(weight), pos(pos) {}
Unit::~Unit() {}
Position Unit::getCurrentPosition() const { return pos; }
int Unit::getQuantity() const { return quantity; }
void Unit::setQuantity(int q) { quantity = max(0, q); }
int Unit::getWeight() const { return weight; }
void Unit::setWeight(int w) { weight = max(0, w); }


// Vehicle Method Implementations
Vehicle::Vehicle(int quantity, int weight, const Position pos, VehicleType vehicleType)
    : Unit(quantity, weight, pos), vehicleType(vehicleType) {}
VehicleType Vehicle::getVehicleType() const { return vehicleType; }
int Vehicle::getAttackScore() { // Non-const if it can modify state (e.g. SpecialForces in Infantry)
    double score_val = (double)(static_cast<int>(vehicleType) * 304 + quantity * weight) / 30.0;
    return static_cast<int>(ceil(score_val));
}
// Vehicle Method Implementations
string Vehicle::str() const {
    stringstream ss;
    ss << "Vehicle[vehicleType=" << vehicleTypeToString(vehicleType)
       << ",quantity=" << quantity
       << ",weight=" << weight
       // ***** MODIFICATION HERE *****
       << ",position=" << pos.str() << "]";
    return ss.str();
}
Unit* Vehicle::clone() const { return new Vehicle(*this); }


// Infantry Method Implementations
Infantry::Infantry(int quantity, int weight, const Position pos, InfantryType infantryType)
    : Unit(quantity, weight, pos), infantryType(infantryType) {}
InfantryType Infantry::getInfantryType() const { return infantryType; }
int Infantry::getAttackScore() {
    // Step 1: Calculate score with current quantity (before PN check)
    // This score is used for the Personal Number calculation.
    int score_for_pn_check = static_cast<int>(infantryType) * 56 + this->quantity * this->weight;

    // If it's Special Forces and weight is a perfect square, this initial score_for_pn_check
    // should include the +75 bonus *before* calculating PN, as per "sau khi xem xét trường hợp lính đặc công".
    if (infantryType == SPECIALFORCES && isPerfectSquare(this->weight)) {
        score_for_pn_check += 75;
    }

    // Step 2: Calculate Personal Number
    int pn = getPersonalNumber(score_for_pn_check, 1975);

    // Step 3: Adjust quantity based on Personal Number
    int old_quantity = this->quantity;
    if (pn > 7) {
        // Chi viện thêm 20% số lượng
        this->quantity = static_cast<int>(ceil(this->quantity * 1.20));
    } else if (pn < 3) {
        // 10% số lượng đào ngũ
        this->quantity = static_cast<int>(floor(this->quantity * 0.90));
        if (this->quantity < 0) this->quantity = 0; // Ensure quantity doesn't go negative
    }

    // Step 4: Recalculate the final score using the (potentially) new quantity
    int final_score = static_cast<int>(infantryType) * 56 + this->quantity * this->weight;

    // Step 5: Add Special Forces perfect square weight bonus (if applicable) to the final_score
    // This bonus is applied based on the *original* consideration of being special forces,
    // but its value is added to the score calculated with the *final* (adjusted) quantity.
    if (infantryType == SPECIALFORCES && isPerfectSquare(this->weight)) {
        final_score += 75;
    }

    return final_score;
}
// Infantry Method Implementations
string Infantry::str() const {
    stringstream ss;
    ss << "Infantry[infantryType=" << infantryTypeToString(infantryType)
       << ",quantity=" << quantity
       << ",weight=" << weight
       // ***** MODIFICATION HERE *****
       << ",position=" << pos.str() << "]";
    return ss.str();
}
Unit* Infantry::clone() const { return new Infantry(*this); }


// UnitList Method Implementations
UnitList::Node::Node(Unit* u) : unit(u), next(nullptr) {}
UnitList::Node::~Node() { delete unit; }

UnitList::UnitList(int initial_capacity) : head(nullptr), capacity(initial_capacity), current_size(0), vehicle_count(0), infantry_count(0) {}

UnitList::~UnitList() {
    clear();
}

void UnitList::clear() {
    Node* current = head;
    while (current != nullptr) {
        Node* next_node = current->next;
        delete current; 
        current = next_node;
    }
    head = nullptr;
    current_size = 0;
    vehicle_count = 0;
    infantry_count = 0;
}

bool UnitList::insert(Unit* unit_to_add) {
    if (!unit_to_add) return false;

    // Try to find an existing unit of the *exact same specific type*
    // For vehicles, this means same VehicleType. For infantry, same InfantryType.
    // The PDF is ambiguous if position also needs to match for "đã tồn tại".
    // Let's assume for this hypothesis: if a unit of the same type exists, merge quantities.
    // The first one found of that type gets the quantity boost.

    Node* current = head;
    bool merged = false;

    if (Vehicle* new_v = dynamic_cast<Vehicle*>(unit_to_add)) {
        while(current && !merged) {
            if (Vehicle* existing_v = dynamic_cast<Vehicle*>(current->unit)) {
                if (existing_v->getVehicleType() == new_v->getVehicleType()) {
                    // Found existing vehicle of the same type. Merge quantities.
                    // The system output TRUCK Q=16 suggests the existing unit's properties (like position) are kept.
                    existing_v->setQuantity(existing_v->getQuantity() + new_v->getQuantity());
                    // We might also want to update weight if the problem implies averaging or summing, but PDF says "cập nhật lại số lượng"
                    // Let's assume only quantity is summed.
                    delete unit_to_add; // Merged, so delete the one passed in
                    merged = true;
                }
            }
            current = current->next;
        }
    } else if (Infantry* new_i = dynamic_cast<Infantry*>(unit_to_add)) {
        current = head; // Reset current to search from head for infantry
        while(current && !merged) {
            if (Infantry* existing_i = dynamic_cast<Infantry*>(current->unit)) {
                if (existing_i->getInfantryType() == new_i->getInfantryType()) {
                    existing_i->setQuantity(existing_i->getQuantity() + new_i->getQuantity());
                    delete unit_to_add; // Merged
                    merged = true;
                }
            }
            current = current->next;
        }
    }

    if (merged) {
        this->updateCounts(); // if a merge happened, counts might not change but good to be sure.
        // No need to recalculate army LF/EXP here, Army::updateStats() handles that.
        return true;
    }

    // If not merged, try to add as a new unit if capacity allows
    if (current_size >= capacity) {
        return false;
    }

    Node* new_node = new Node(unit_to_add); // Node takes ownership
    current_size++;
    if (dynamic_cast<Vehicle*>(unit_to_add)) {
        vehicle_count++;
        // Add Vehicle to end
        if (!head) {
            head = new_node;
        } else {
            Node* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = new_node;
        }
    } else if (dynamic_cast<Infantry*>(unit_to_add)) {
        infantry_count++;
        // Add Infantry to head
        new_node->next = head;
        head = new_node;
    } else {
        delete new_node;
        current_size--;
        return false;
    }
    return true;
}
    
bool UnitList::isContain(VehicleType vehicleType) {
    Node* current = head;
    while (current) {
        Vehicle* v = dynamic_cast<Vehicle*>(current->unit);
        if (v && v->getVehicleType() == vehicleType) {
            return true;
        }
        current = current->next;
    }
    return false;
}

bool UnitList::isContain(InfantryType infantryType) {
    Node* current = head;
    while (current) {
        Infantry* i = dynamic_cast<Infantry*>(current->unit);
        if (i && i->getInfantryType() == infantryType) {
            return true;
        }
        current = current->next;
    }
    return false;
}

string UnitList::str() const {
    stringstream ss;
    ss << "UnitList[count_vehicle=" << vehicle_count
       << ";count_infantry=" << infantry_count; // Semicolon always after infantry_count

    Node* current = head;
    if (current != nullptr) { // Only print the next semicolon if there are units to list
        ss << ";"; // This semicolon separates counts from the unit list itself
        bool first_unit = true;
        while (current) {
            if (!first_unit) {
                ss << ",";
            }
            if (current->unit) { // Added null check for safety, though nodes should always have units
                ss << current->unit->str();
            } else {
                ss << "null_unit_in_node"; // Should not happen
            }
            first_unit = false;
            current = current->next;
        }
    }
    // If current was nullptr (empty list), no additional semicolon and no units are printed.
    
    ss << "]";
    return ss.str();
}

void UnitList::setCapacity(int cap) { capacity = cap; }
int UnitList::getCapacity() const { return capacity; }
int UnitList::getCurrentSize() const { return current_size; }

vector<Unit*> UnitList::getAllUnits() {
    vector<Unit*> units_vec;
    Node* current = head;
    while (current) {
        units_vec.push_back(current->unit);
        current = current->next;
    }
    return units_vec;
}
    
bool UnitList::removeUnit(Unit* unit_to_remove) {
    if (!head || !unit_to_remove) return false;

    if (head->unit == unit_to_remove) {
        Node* temp = head;
        head = head->next;
        if (dynamic_cast<Vehicle*>(temp->unit)) vehicle_count--;
        else if (dynamic_cast<Infantry*>(temp->unit)) infantry_count--;
        delete temp; 
        current_size--;
        return true;
    }

    Node* current = head;
    while (current->next && current->next->unit != unit_to_remove) {
        current = current->next;
    }

    if (current->next) { 
        Node* temp = current->next;
        current->next = temp->next;
        if (dynamic_cast<Vehicle*>(temp->unit)) vehicle_count--;
        else if (dynamic_cast<Infantry*>(temp->unit)) infantry_count--;
        delete temp; 
        current_size--;
        return true;
    }
    return false; 
}

void UnitList::removeUnitsIf(bool (*predicate)(Unit*)) {
    Node *current = head, *prev = nullptr;
    while (current != nullptr) {
        // Call the function pointer directly
        if (predicate(current->unit)) {
            Node* temp = current;
            if (prev) {
                prev->next = current->next;
            } else {
                head = current->next;
            }
            current = current->next;

            if (dynamic_cast<Vehicle*>(temp->unit)) vehicle_count--;
            else if (dynamic_cast<Infantry*>(temp->unit)) infantry_count--;
            delete temp;
            current_size--;
        } else {
            prev = current;
            current = current->next;
        }
    }
}
void UnitList::updateCounts() {
    vehicle_count = 0;
    infantry_count = 0;
    Node* current = head;
    while (current) {
        if (dynamic_cast<Vehicle*>(current->unit)) vehicle_count++;
        else if (dynamic_cast<Infantry*>(current->unit)) infantry_count++;
        current = current->next;
    }
}

Unit* UnitList::findUnit(VehicleType vt) {
    Node* current = head;
    while (current) {
        Vehicle* v = dynamic_cast<Vehicle*>(current->unit);
        if (v && v->getVehicleType() == vt) return v;
        current = current->next;
    }
    return nullptr;
}
    
Unit* UnitList::findUnit(InfantryType it) {
    Node* current = head;
    while (current) {
        Infantry* i = dynamic_cast<Infantry*>(current->unit);
        if (i && i->getInfantryType() == it) return i;
        current = current->next;
    }
    return nullptr;
}

// TerrainElement and its derived classes
TerrainElement::TerrainElement(const Position& p) : pos(p) {}
TerrainElement::~TerrainElement() {}

Road::Road(const Position& p) : TerrainElement(p) {}
void Road::getEffect(Army* army, const vector<Unit*>& all_army_units) { /* No effect */ }
string Road::getTypeString() const { return "ROAD"; }

Mountain::Mountain(const Position& p) : TerrainElement(p) {}
void Mountain::getEffect(Army* army, const vector<Unit*>& all_army_units) {
    bool isLiberationArmy = (army->getName() == "LiberationArmy");
    double radius_check = isLiberationArmy ? 2.0 : 4.0;
    double exp_factor = isLiberationArmy ? 0.30 : 0.20;
    double lf_factor = isLiberationArmy ? -0.10 : -0.05;

    int total_exp_bonus = 0;
    int total_lf_penalty = 0;

    for (Unit* unit : all_army_units) {
        if (unit->getCurrentPosition().distanceTo(this->pos) <= radius_check) {
            if (Infantry* infantry_unit = dynamic_cast<Infantry*>(unit)) {
                total_exp_bonus += static_cast<int>(round(infantry_unit->getAttackScore() * exp_factor));
            } else if (Vehicle* vehicle_unit = dynamic_cast<Vehicle*>(unit)) {
                total_lf_penalty += static_cast<int>(round(vehicle_unit->getAttackScore() * lf_factor));
            }
        }
    }
    army->setEXP(army->getEXP() + total_exp_bonus);
    army->setLF(army->getLF() + total_lf_penalty);
}
string Mountain::getTypeString() const { return "MOUNTAIN"; }

River::River(const Position& p) : TerrainElement(p) {}
void River::getEffect(Army* army, const vector<Unit*>& all_army_units) {
    int total_attack_score_reduction = 0;
    for (Unit* unit : all_army_units) {
        if (Infantry* infantry_unit = dynamic_cast<Infantry*>(unit)) {
            if (unit->getCurrentPosition().distanceTo(this->pos) <= 2.0) {
                 total_attack_score_reduction += static_cast<int>(round(infantry_unit->getAttackScore() * 0.10));
            }
        }
    }
    army->setEXP(army->getEXP() - total_attack_score_reduction);
}
string River::getTypeString() const { return "RIVER"; }

Urban::Urban(const Position& p) : TerrainElement(p) {}
void Urban::getEffect(Army* army, const vector<Unit*>& all_army_units) {
    bool isLiberationArmy = (army->getName() == "LiberationArmy");
    int total_exp_bonus = 0;

    if (isLiberationArmy) {
        for (Unit* unit : all_army_units) {
            double dist = unit->getCurrentPosition().distanceTo(this->pos);
            if (Infantry* inf_unit = dynamic_cast<Infantry*>(unit)) {
                if ((inf_unit->getInfantryType() == SPECIALFORCES || inf_unit->getInfantryType() == REGULARINFANTRY) && dist <= 5.0 && dist > 0) {
                    total_exp_bonus += static_cast<int>(round((2.0 * inf_unit->getAttackScore()) / dist));
                }
            } else if (Vehicle* veh_unit = dynamic_cast<Vehicle*>(unit)) {
                if (veh_unit->getVehicleType() == ARTILLERY && dist <= 2.0) {
                    army->setLF(army->getLF() - static_cast<int>(round(veh_unit->getAttackScore() * 0.50)));
                }
            }
        }
    } else { // ARVN
        for (Unit* unit : all_army_units) {
            double dist = unit->getCurrentPosition().distanceTo(this->pos);
            if (Infantry* inf_unit = dynamic_cast<Infantry*>(unit)) {
                if (inf_unit->getInfantryType() == REGULARINFANTRY && dist <= 3.0 && dist > 0) {
                    total_exp_bonus += static_cast<int>(round((3.0 * inf_unit->getAttackScore()) / (2.0 * dist)));
                }
            }
        }
    }
    army->setEXP(army->getEXP() + total_exp_bonus);
}
string Urban::getTypeString() const { return "URBAN"; }


Fortification::Fortification(const Position& p) : TerrainElement(p) {}
void Fortification::getEffect(Army* army, const vector<Unit*>& all_army_units) {
    bool isLiberationArmy = (army->getName() == "LiberationArmy");
    int total_score_change = 0; 

    if (isLiberationArmy) { 
        for (Unit* unit : all_army_units) {
            if (unit->getCurrentPosition().distanceTo(this->pos) <= 2.0) {
                total_score_change -= static_cast<int>(round(unit->getAttackScore() * 0.20));
            }
        }
    } else { // ARVN
         for (Unit* unit : all_army_units) {
            if (unit->getCurrentPosition().distanceTo(this->pos) <= 2.0) { 
                 total_score_change += static_cast<int>(round(unit->getAttackScore() * 0.20));
            }
        }
    }
    army->setLF(army->getLF() + total_score_change);
}
string Fortification::getTypeString() const { return "FORTIFICATION"; }

SpecialZone::SpecialZone(const Position& p) : TerrainElement(p) {}
void SpecialZone::getEffect(Army* army, const vector<Unit*>& all_army_units) {
    int lf_reduction = 0;
    int exp_reduction = 0;

    for (Unit* unit : all_army_units) {
        if (unit->getCurrentPosition().distanceTo(this->pos) <= 1.0) {
            if (Vehicle* v = dynamic_cast<Vehicle*>(unit)) {
                lf_reduction += v->getAttackScore(); 
            } else if (Infantry* i = dynamic_cast<Infantry*>(unit)) {
                exp_reduction += i->getAttackScore();
            }
        }
    }
    army->setLF(army->getLF() - lf_reduction);
    army->setEXP(army->getEXP() - exp_reduction);
}
string SpecialZone::getTypeString() const { return "SPECIAL_ZONE"; }

// Army Method Implementations
int Army::roundToThreshold(int val, int min_val, int max_val) {
    if (val < min_val) return min_val;
    if (val > max_val) return max_val;
    return val;
}

Army::Army(Unit **unitArray, int size, string army_name, BattleField *bf) // Changed to Unit**
    : LF(0), EXP(0), name(army_name), battleField(bf) {

    vector<Unit*> initial_clones;
    for (int i = 0; i < size; ++i) {
        if (unitArray[i]) {
            // unitArray[i] is Unit*, its clone() method is const
            // so unitArray[i]->clone() is fine.
            initial_clones.push_back(unitArray[i]->clone());
        }
    }

    // ... (rest of the Army constructor logic remains the same) ...
    int temp_LF = 0;
    int temp_EXP = 0;
    for (Unit* u : initial_clones) {
        if (dynamic_cast<Vehicle*>(u)) {
            temp_LF += u->getAttackScore();
        } else if (dynamic_cast<Infantry*>(u)) {
            temp_EXP += u->getAttackScore();
        }
    }
    this->LF = roundToThreshold(temp_LF, 0, 1000);
    this->EXP = roundToThreshold(temp_EXP, 0, 500);

    int capacity = calculateUnitListCapacity(this->LF + this->EXP);
    this->unitList = new UnitList(capacity);

    for (Unit* cloned_u : initial_clones) {
        bool inserted = this->unitList->insert(cloned_u);
        if (!inserted) {
            delete cloned_u;
        }
    }
    updateStats();
}

Army::~Army() {
    delete unitList;
}

void Army::updateStats() {
    int current_LF = 0;
    int current_EXP = 0;
    if (unitList) {
        vector<Unit*> units = unitList->getAllUnits();
        for (Unit* u : units) {
            if (dynamic_cast<Vehicle*>(u)) {
                current_LF += u->getAttackScore(); 
            } else if (dynamic_cast<Infantry*>(u)) {
                current_EXP += u->getAttackScore();
            }
        }
    }
    LF = roundToThreshold(current_LF, 0, 1000);
    EXP = roundToThreshold(current_EXP, 0, 500);
    
    if (unitList) {
        unitList->setCapacity(calculateUnitListCapacity(LF + EXP));
        unitList->updateCounts(); 
    }
}
int Army::getLF() const { return LF; }
int Army::getEXP() const { return EXP; }
void Army::setLF(int val) { LF = roundToThreshold(val, 0, 1000); }
void Army::setEXP(int val) { EXP = roundToThreshold(val, 0, 500); }
string Army::getName() const { return name; }
UnitList* Army::getUnitList() { return unitList; }
BattleField* Army::getBattleField() { return battleField; }

// Forward declare findOptimalSubset
vector<Unit*> findOptimalSubset(const vector<Unit*>& units_pool, int target_score_exclusive, bool minimize_sum);

LiberationArmy::LiberationArmy(Unit** unitArray, int size, string name, BattleField* bf) // Changed to Unit**
    : Army(unitArray, size, name, bf) {} 

void LiberationArmy::fight(Army* enemy_arvn_ptr, bool defense) {
    if (!enemy_arvn_ptr) return;

    this->updateStats();
    enemy_arvn_ptr->updateStats();

    if (!defense) { // LibArmy ATTACKING
        int actual_lf_before_boost = this->LF;
        int actual_exp_before_boost = this->EXP;

        int boosted_LF = roundToThreshold(static_cast<int>(round(actual_lf_before_boost * 1.5)), 0, 1000);
        int boosted_EXP = roundToThreshold(static_cast<int>(round(actual_exp_before_boost * 1.5)), 0, 500);

        vector<Unit*> all_my_units = unitList->getAllUnits();
        vector<Unit*> my_infantry, my_vehicles;
        for(Unit* u : all_my_units) {
            if(dynamic_cast<Infantry*>(u)) my_infantry.push_back(u);
            else if(dynamic_cast<Vehicle*>(u)) my_vehicles.push_back(u);
        }

        vector<Unit*> combo_A_strict = findOptimalSubset(my_infantry, enemy_arvn_ptr->getEXP(), true);
        vector<Unit*> combo_B_strict = findOptimalSubset(my_vehicles, enemy_arvn_ptr->getLF(), true);

        bool won_battle = false;
        vector<Unit*> units_to_remove_from_self;

        bool combo_A_found_strict = !combo_A_strict.empty();
        bool combo_B_found_strict = !combo_B_strict.empty();

        if (combo_A_found_strict && combo_B_found_strict) {
            won_battle = true;
            units_to_remove_from_self.insert(units_to_remove_from_self.end(), combo_A_strict.begin(), combo_A_strict.end());
            units_to_remove_from_self.insert(units_to_remove_from_self.end(), combo_B_strict.begin(), combo_B_strict.end());
        } else if (combo_A_found_strict && !combo_B_found_strict) { // Only Combo A strict found
            if (boosted_LF > enemy_arvn_ptr->getLF()) { // Check overall vehicle superiority
                won_battle = true;
                units_to_remove_from_self.insert(units_to_remove_from_self.end(), combo_A_strict.begin(), combo_A_strict.end());
                units_to_remove_from_self.insert(units_to_remove_from_self.end(), my_vehicles.begin(), my_vehicles.end()); // All vehicles removed
            } else {
                won_battle = false; // Giao tranh không xảy ra path
            }
        } else if (!combo_A_found_strict && combo_B_found_strict) { // Only Combo B strict found
            if (boosted_EXP > enemy_arvn_ptr->getEXP()) { // Check overall infantry superiority
                won_battle = true;
                units_to_remove_from_self.insert(units_to_remove_from_self.end(), combo_B_strict.begin(), combo_B_strict.end());
                units_to_remove_from_self.insert(units_to_remove_from_self.end(), my_infantry.begin(), my_infantry.end()); // All infantry removed
            } else {
                won_battle = false; // Giao tranh không xảy ra path
            }
        } else { // BOTH combo_A_strict AND combo_B_strict are empty
            // YOUR CRUCIAL CLARIFICATION: Check overall boosted stats
            bool overall_lf_superiority = boosted_LF > enemy_arvn_ptr->getLF();
            bool overall_exp_superiority = boosted_EXP > enemy_arvn_ptr->getEXP();

            if (overall_lf_superiority && overall_exp_superiority) {
                won_battle = true;
                // Since no specific smaller combos were sufficient, all units are considered used
                units_to_remove_from_self.insert(units_to_remove_from_self.end(), my_infantry.begin(), my_infantry.end());
                units_to_remove_from_self.insert(units_to_remove_from_self.end(), my_vehicles.begin(), my_vehicles.end());
            } else {
                won_battle = false; // Giao tranh không xảy ra
            }
        }

        // Revert LibArmy's stats to pre-boost values for consistent state before applying consequences
        this->LF = actual_lf_before_boost;
        this->EXP = actual_exp_before_boost;

        if (won_battle) {
            // Make units_to_remove_from_self unique (manual way, as std::unique is not allowed)
            vector<Unit*> final_units_to_remove;
            if (!units_to_remove_from_self.empty()) { // Check if vector is not empty
                 // Simple manual unique for pointers
                for (size_t i = 0; i < units_to_remove_from_self.size(); ++i) {
                    bool already_added = false;
                    for (size_t j = 0; j < final_units_to_remove.size(); ++j) {
                        if (units_to_remove_from_self[i] == final_units_to_remove[j]) {
                            already_added = true;
                            break;
                        }
                    }
                    if (!already_added) {
                        final_units_to_remove.push_back(units_to_remove_from_self[i]);
                    }
                }
            }

            for (Unit* u_rem : final_units_to_remove) {
                this->unitList->removeUnit(u_rem);
            }

            vector<Unit*> enemy_units_to_capture = enemy_arvn_ptr->getUnitList()->getAllUnits();
            for (Unit* enemy_u : enemy_units_to_capture) {
                Unit* cloned_enemy_u = enemy_u->clone();
                bool inserted = this->unitList->insert(cloned_enemy_u);
                if (!inserted) {
                    delete cloned_enemy_u;
                }
            }
            enemy_arvn_ptr->getUnitList()->removeUnitsIf([](Unit* ){ return true; });
            enemy_arvn_ptr->setLF(0);
            enemy_arvn_ptr->setEXP(0);
        } else {
            vector<Unit*> current_units = unitList->getAllUnits();
            for (Unit* u : current_units) {
                u->setWeight(static_cast<int>(floor(u->getWeight() * 0.9)));
                if (u->getWeight() < 0) u->setWeight(0);
            }
        }
        this->updateStats();
        enemy_arvn_ptr->updateStats();

    } else { // LibArmy DEFENDING
        // ... (Defending logic remains the same - it seemed to match TC02 correctly)
        int defending_LF = roundToThreshold(static_cast<int>(round(this->LF * 1.3)), 0, 1000);
        int defending_EXP = roundToThreshold(static_cast<int>(round(this->EXP * 1.3)), 0, 500);

        int enemy_attacking_LF = enemy_arvn_ptr->getLF();
        int enemy_attacking_EXP = enemy_arvn_ptr->getEXP();

        bool lib_army_lf_not_weaker = defending_LF >= enemy_attacking_LF;
        bool lib_army_exp_not_weaker = defending_EXP >= enemy_attacking_EXP;

        if (lib_army_lf_not_weaker && lib_army_exp_not_weaker) {
            // LibArmy wins defense, no changes to its units per PDF for this outcome.
        } else if (!lib_army_lf_not_weaker && !lib_army_exp_not_weaker) { // Both stats weaker
            vector<Unit*> all_units_for_chi_vien = unitList->getAllUnits();
            for (Unit* u : all_units_for_chi_vien) {
                u->setQuantity(getFibonacciNearest(u->getQuantity()));
            }
            this->updateStats(); 

            defending_LF = roundToThreshold(static_cast<int>(round(this->LF * 1.3)), 0, 1000);
            defending_EXP = roundToThreshold(static_cast<int>(round(this->EXP * 1.3)), 0, 500);
            lib_army_lf_not_weaker = defending_LF >= enemy_attacking_LF;
            lib_army_exp_not_weaker = defending_EXP >= enemy_attacking_EXP;

            if (lib_army_lf_not_weaker && lib_army_exp_not_weaker) {
                // Now wins after Chi Vien
            } else { 
                vector<Unit*> all_units_loss = unitList->getAllUnits();
                for (Unit* u : all_units_loss) {
                    u->setQuantity(static_cast<int>(floor(u->getQuantity() * 0.9)));
                    if (u->getQuantity() < 0) u->setQuantity(0);
                }
                this->unitList->removeUnitsIf([](Unit* u){ return u->getQuantity() == 0; });
            }
        } else { // One stat weaker
            vector<Unit*> all_units = unitList->getAllUnits();
            for (Unit* u : all_units) {
                u->setQuantity(static_cast<int>(floor(u->getQuantity() * 0.9)));
                if (u->getQuantity() < 0) u->setQuantity(0);
            }
            this->unitList->removeUnitsIf([](Unit* u){ return u->getQuantity() == 0; });
        }
        this->updateStats(); 
        enemy_arvn_ptr->updateStats();
    }
}
string LiberationArmy::str() const {
    stringstream ss;
    ss << "LiberationArmy["
       << "LF=" << LF
       << ",EXP=" << EXP
       << ",unitList=" << (unitList ? unitList->str() : "")
       << ",battleField=" << (battleField ? battleField->str() : "") << "]";
    return ss.str();
}

ARVN::ARVN(Unit** unitArray, int size, string name, BattleField* bf) // Changed to Unit**
    : Army(unitArray, size, name, bf) {} 

// ARVN::fight logic remains the same as previously established for consistency.
void ARVN::fight(Army* enemy_lib_army_ptr, bool defense) {
    if (!enemy_lib_army_ptr) return;

    this->updateStats();
    enemy_lib_army_ptr->updateStats();

    if (!defense) { // ARVN ATTACKING
        vector<Unit*> arvn_units = unitList->getAllUnits();
        for (Unit* u : arvn_units) {
            u->setQuantity(static_cast<int>(floor(u->getQuantity() * 0.8)));
            if (u->getQuantity() < 0) u->setQuantity(0);
        }
        unitList->removeUnitsIf([](Unit* u){ return u->getQuantity() == 1 || u->getQuantity() == 0; });
        this->updateStats();
    } else { // ARVN DEFENDING
        // If ARVN still has units, it means LibArmy's attack was "giao tranh khong xay ra".
        // In this case, ARVN is preserved.
        if (this->unitList->getCurrentSize() > 0) {
            // ARVN preserved, no action needed.
        }
        this->updateStats(); // Will be 0/0 if wiped, or original stats if preserved.
    }
}
string ARVN::str() const {
     stringstream ss;
    ss << "ARVN["
       << "LF=" << LF
       << ",EXP=" << EXP
       << ",unitList=" << (unitList ? unitList->str() : "")
       << ",battleField=" << (battleField ? battleField->str() : "") << "]";
    return ss.str();
}

vector<Unit*> findOptimalSubset(const vector<Unit*>& units_pool, int target_score_exclusive, bool minimize_sum) {
    vector<Unit*> best_subset;
    long long best_subset_score_sum = minimize_sum ? LLONG_MAX : LLONG_MIN; // Use LLONG_MAX for minimizing
    int num_units = units_pool.size();
    if (num_units == 0 || num_units > 20) return best_subset; // Limit complexity for 2^N

    for (int i = 0; i < (1 << num_units); ++i) { 
        vector<Unit*> current_subset;
        long long current_score_sum = 0;
        for (int j = 0; j < num_units; ++j) {
            if ((i >> j) & 1) { 
                current_subset.push_back(units_pool[j]);
                current_score_sum += units_pool[j]->getAttackScore();
            }
        }

        if (current_score_sum > target_score_exclusive) {
            if (minimize_sum) {
                if (current_score_sum < best_subset_score_sum) {
                    best_subset_score_sum = current_score_sum;
                    best_subset = current_subset;
                } else if (current_score_sum == best_subset_score_sum) {
                    if (current_subset.size() < best_subset.size()) {
                         best_subset = current_subset;
                    }
                }
            } else { 
                if (current_score_sum > best_subset_score_sum) {
                    best_subset_score_sum = current_score_sum;
                    best_subset = current_subset;
                }
            }
        }
    }
    return best_subset;
}

BattleField::BattleField(int n_r, int n_c,
            const vector<Position*>& arrayForest, 
            const vector<Position*>& arrayRiver,
            const vector<Position*>& arrayFortification,
            const vector<Position*>& arrayUrban,
            const vector<Position*>& arraySpecialZone)
    : n_rows(n_r), n_cols(n_c) {
    
    terrain_map = new TerrainElement**[n_rows];
    for (int i = 0; i < n_rows; ++i) {
        terrain_map[i] = new TerrainElement*[n_cols];
        for (int j = 0; j < n_cols; ++j) {
            terrain_map[i][j] = new Road(Position(i, j)); 
        }
    }

    auto setupTerrain = [&](const vector<Position*>& positions, const string& type) {
        for (Position* p_ptr : positions) {
            if (p_ptr) {
                int r = p_ptr->getRow();
                int c = p_ptr->getCol();
                if (r >= 0 && r < n_rows && c >= 0 && c < n_cols) {
                    delete terrain_map[r][c]; 
                    if (type == "FOREST") terrain_map[r][c] = new Mountain(*p_ptr);
                    else if (type == "RIVER") terrain_map[r][c] = new River(*p_ptr);
                    else if (type == "FORTIFICATION") terrain_map[r][c] = new Fortification(*p_ptr);
                    else if (type == "URBAN") terrain_map[r][c] = new Urban(*p_ptr);
                    else if (type == "SPECIAL_ZONE") terrain_map[r][c] = new SpecialZone(*p_ptr);
                }
            }
        }
    };

    setupTerrain(arrayForest, "FOREST");
    setupTerrain(arrayRiver, "RIVER");
    setupTerrain(arrayFortification, "FORTIFICATION");
    setupTerrain(arrayUrban, "URBAN");
    setupTerrain(arraySpecialZone, "SPECIAL_ZONE");
}
BattleField::~BattleField() {
    for (int i = 0; i < n_rows; ++i) {
        for (int j = 0; j < n_cols; ++j) {
            delete terrain_map[i][j];
        }
        delete[] terrain_map[i];
    }
    delete[] terrain_map;
}

string BattleField::str() const {
    stringstream ss;
    ss << "BattleField[n_rows=" << n_rows << ",n_cols=" << n_cols << "]";
    return ss.str();
}
    
void BattleField::applyTerrainEffects(Army* army) {
    if (!army) return;
    vector<Unit*> army_units = army->getUnitList()->getAllUnits();
    if (army_units.empty()) return;

    for (int r = 0; r < n_rows; ++r) {
        for (int c = 0; c < n_cols; ++c) {
            if (terrain_map[r][c]) { 
                terrain_map[r][c]->getEffect(army, army_units);
            }
        }
    }
    army->updateStats(); 
}
TerrainElement* BattleField::getTerrainElementAt(const Position& pos) const {
    if (pos.getRow() >= 0 && pos.getRow() < n_rows && pos.getCol() >= 0 && pos.getCol() < n_cols) {
        return terrain_map[pos.getRow()][pos.getCol()];
    }
    return nullptr;
}
int BattleField::getRows() const {return n_rows;}
int BattleField::getCols() const {return n_cols;}
vector<TerrainElement*> BattleField::getAllTerrainElements() const {
    vector<TerrainElement*> elements;
    for(int i=0; i<n_rows; ++i) {
        for (int j=0; j<n_cols; ++j) {
            elements.push_back(terrain_map[i][j]);
        }
    }
    return elements;
}


Configuration::Configuration(const string& filepath) : num_rows(0), num_cols(0), liberationUnits_ptr_array(nullptr), liberationUnits_count(0), ARVNUnits_ptr_array(nullptr), ARVNUnits_count(0), eventCode(0) {
    ifstream file(filepath);
    if (!file.is_open()) {
        throw runtime_error("Cannot open configuration file: " + filepath);
    }
    string line;
    while (getline(file, line)) {
        line.erase(0, line.find_first_not_of(" \t\n\r\f\v"));
        line.erase(line.find_last_not_of(" \t\n\r\f\v") + 1);
        if (line.empty() || line[0] == '#') continue; 

        size_t eq_pos = line.find('=');
        if (eq_pos == string::npos) continue;

        string key = line.substr(0, eq_pos);
        string value = line.substr(eq_pos + 1);

        if (key == "NUM_ROWS") num_rows = stoi(value);
        else if (key == "NUM_COLS") num_cols = stoi(value);
        else if (key == "EVENT_CODE") {
            int ec = stoi(value);
            if (ec > 99) eventCode = ec % 100;
            else if (ec < 0) eventCode = 0; 
            else eventCode = ec;
        }
        else if (key == "ARRAY_FOREST") parsePositions(line, arrayForest);
        else if (key == "ARRAY_RIVER") parsePositions(line, arrayRiver);
        else if (key == "ARRAY_FORTIFICATION") parsePositions(line, arrayFortification);
        else if (key == "ARRAY_URBAN") parsePositions(line, arrayUrban);
        else if (key == "ARRAY_SPECIAL_ZONE") parsePositions(line, arraySpecialZone);
        else if (key == "UNIT_LIST") parseUnitList(line);
    }
    file.close();
}

Configuration::~Configuration() {
    for (Position* p : arrayForest) delete p;
    for (Position* p : arrayRiver) delete p;
    for (Position* p : arrayFortification) delete p;
    for (Position* p : arrayUrban) delete p;
    for (Position* p : arraySpecialZone) delete p;

    for (Unit* u : liberationUnits_store) delete u; 
    for (Unit* u : ARVNUnits_store) delete u;
    
    delete[] liberationUnits_ptr_array; 
    delete[] ARVNUnits_ptr_array;
}
    
string Configuration::str() const {
    stringstream ss;

    ss << "["; // Start with the bracket as per system output for the whole configuration
    ss << "num_rows=" << num_rows;
    ss << ",num_cols=" << num_cols;

    // Lambda to format a vector of Position pointers into a string like "name=[(r1,c1),(r2,c2),...]"
    auto posVectorToStrSingleLine = [&](const string& name, const vector<Position*>& vec) {
        ss << "," << name << "=["; // Comma before the attribute name, then name=[
        for (size_t i = 0; i < vec.size(); ++i) {
            if (vec[i]) { // Check if the pointer is not null
                ss << vec[i]->str(); // This already returns "(r,c)"
            } else {
                ss << "null";
            }
            if (i < vec.size() - 1) {
                ss << ","; // Comma between positions
            }
        }
        ss << "]"; // Closing bracket for the list of positions
    };

    posVectorToStrSingleLine("arrayForest", arrayForest);
    posVectorToStrSingleLine("arrayRiver", arrayRiver);
    posVectorToStrSingleLine("arrayFortification", arrayFortification);
    posVectorToStrSingleLine("arrayUrban", arrayUrban);
    posVectorToStrSingleLine("arraySpecialZone", arraySpecialZone);

    // Lambda to format a vector of Unit pointers
    auto unitVectorToStrSingleLine = [&](const string& name, const vector<Unit*>& vec) {
        ss << "," << name << "=["; // Comma before attribute name
        for (size_t i = 0; i < vec.size(); ++i) {
            if (vec[i]) {
                ss << vec[i]->str(); // Unit::str() returns "ClassName[attr=val,...]"
            } else {
                ss << "null";
            }
            if (i < vec.size() - 1) {
                ss << ",";
            }
        }
        ss << "]";
    };

    unitVectorToStrSingleLine("liberationUnits", liberationUnits_store);
    unitVectorToStrSingleLine("ARVNUnits", ARVNUnits_store);

    ss << ",eventCode=" << eventCode;
    ss << "]"; // End with the bracket for the whole configuration
    return ss.str();
}
void Configuration::parsePositions(const string& line, vector<Position*>& target_vector) {
    size_t start_bracket = line.find('[');
    size_t end_bracket = line.find(']');
    if (start_bracket == string::npos || end_bracket == string::npos || start_bracket >= end_bracket) return;

    string content = line.substr(start_bracket + 1, end_bracket - start_bracket - 1);
    if (content.empty()) return;
    
    string current_pos_str;
    for (char ch : content) {
        if (ch == '(') {
            current_pos_str.clear();
            current_pos_str += ch;
        } else if (ch == ')') {
            current_pos_str += ch;
            if (!current_pos_str.empty()) {
                 try {
                    target_vector.push_back(new Position(current_pos_str));
                } catch (const std::runtime_error& e) {
                    cerr << "Warning: Could not parse position: " << current_pos_str << " due to " << e.what() << endl;
                }
                current_pos_str.clear();
            }
        } else if (!current_pos_str.empty()) {
            current_pos_str += ch;
        }
    }
}

void Configuration::parseUnitList(const string& line) {
    size_t start_bracket = line.find('[');
    size_t end_bracket = line.find(']');
    if (start_bracket == string::npos || end_bracket == string::npos || start_bracket >= end_bracket) return;
    string content = line.substr(start_bracket + 1, end_bracket - start_bracket - 1);
    if (content.empty()) return;

    string current_unit_str;
    int parentheses_balance = 0;
    for (char ch : content) {
        current_unit_str += ch;
        if (ch == '(') parentheses_balance++;
        else if (ch == ')') parentheses_balance--;

        if (ch == ')' && parentheses_balance == 0 && !current_unit_str.empty()) { 
            size_t first_char = current_unit_str.find_first_not_of(" ,");
            if (first_char != string::npos) current_unit_str = current_unit_str.substr(first_char);
            
            size_t p_open = current_unit_str.find('(');
            size_t p_close = current_unit_str.rfind(')'); 
            if (p_open != string::npos && p_close != string::npos && p_open < p_close) {
                string unit_name_str = current_unit_str.substr(0, p_open);
                string args_str = current_unit_str.substr(p_open + 1, p_close - p_open - 1);
                
                stringstream arg_ss(args_str);
                string q_str, w_str, army_belong_str;
                string pos_full_str; // To capture "(r,c)"

                getline(arg_ss, q_str, ',');
                getline(arg_ss, w_str, ',');
                
                // Custom logic to read position string like "(r,c)"
                char temp_char;
                int pos_paren_count = 0;
                bool in_pos = false;
                while(arg_ss.get(temp_char)){
                    if(temp_char == '(' && !in_pos) {
                        in_pos = true;
                        pos_paren_count++;
                        pos_full_str += temp_char;
                    } else if (in_pos) {
                        pos_full_str += temp_char;
                        if (temp_char == '(') pos_paren_count++;
                        else if (temp_char == ')') pos_paren_count--;
                        if (pos_paren_count == 0) break; // Position string fully captured
                    } else if (temp_char == ',') {
                        // This comma should be the one AFTER position string
                        break; 
                    }
                }
                // The character after position (should be a comma if army_belong follows) might have been consumed
                // Or if it's the end, arg_ss will be at eof.
                // The next getline will handle army_belong_str correctly.
                
                getline(arg_ss, army_belong_str); // Reads the rest (should be army_belong)
                // Trim army_belong_str if it picked up a leading comma
                if (!army_belong_str.empty() && army_belong_str[0] == ',') {
                    army_belong_str = army_belong_str.substr(1);
                }


                try {
                    int q = stoi(q_str);
                    int w = stoi(w_str);
                    Position pos_obj(pos_full_str);
                    int army_belong = stoi(army_belong_str);

                    Unit* new_unit = nullptr;
                    try {
                        VehicleType vt = stringToVehicleType(unit_name_str);
                        new_unit = new Vehicle(q,w,pos_obj,vt);
                    } catch (const runtime_error&) {
                        try {
                            InfantryType it = stringToInfantryType(unit_name_str);
                            new_unit = new Infantry(q,w,pos_obj,it);
                        } catch (const runtime_error&) {
                             cerr << "Warning: Unknown unit name during parsing: " << unit_name_str << endl;
                        }
                    }
                    
                    if (new_unit) {
                        if (army_belong == 0) liberationUnits_store.push_back(new_unit);
                        else ARVNUnits_store.push_back(new_unit);
                    }
                } catch (const std::exception& e) {
                    cerr << "Failed to parse unit arguments for " << unit_name_str << ": \"" << args_str << "\" Error: " << e.what() << endl;
                     cerr << " Parsed q_str: " << q_str << ", w_str: " << w_str << ", pos_full_str: " << pos_full_str << ", army_belong_str: " << army_belong_str << endl;

                }
            }
            current_unit_str.clear(); 
        }
    }
    liberationUnits_count = liberationUnits_store.size();
    liberationUnits_ptr_array = new Unit*[liberationUnits_count > 0 ? liberationUnits_count : 1]; 
    for(size_t i=0; i < liberationUnits_store.size(); ++i) liberationUnits_ptr_array[i] = liberationUnits_store[i];

    ARVNUnits_count = ARVNUnits_store.size();
    ARVNUnits_ptr_array = new Unit*[ARVNUnits_count > 0 ? ARVNUnits_count : 1];
    for(size_t i=0; i < ARVNUnits_store.size(); ++i) ARVNUnits_ptr_array[i] = ARVNUnits_store[i];
}

// Configuration Getters
int Configuration::getNumRows() const { return num_rows; }
int Configuration::getNumCols() const { return num_cols; }
const vector<Position*>& Configuration::getArrayForest() const { return arrayForest; }
const vector<Position*>& Configuration::getArrayRiver() const { return arrayRiver; }
const vector<Position*>& Configuration::getArrayFortification() const { return arrayFortification; }
const vector<Position*>& Configuration::getArrayUrban() const { return arrayUrban; }
const vector<Position*>& Configuration::getArraySpecialZone() const { return arraySpecialZone; }
Unit** Configuration::getLiberationUnitsPtrArray() const { return liberationUnits_ptr_array; } // Returns Unit**
int Configuration::getLiberationUnitsCount() const { return liberationUnits_count; }
Unit** Configuration::getARVNUnitsPtrArray() const { return ARVNUnits_ptr_array; }             // Returns Unit**
int Configuration::getARVNUnitsCount() const { return ARVNUnits_count; }
int Configuration::getEventCode() const { return eventCode; }

// HCMCampaign constructor
HCMCampaign::HCMCampaign(const string& config_file_path) : config(nullptr), battleField(nullptr), liberationArmy(nullptr), arvnArmy(nullptr) {
    try {
        config = new Configuration(config_file_path);

        battleField = new BattleField(
            config->getNumRows(), config->getNumCols(),
            config->getArrayForest(), config->getArrayRiver(),
            config->getArrayFortification(), config->getArrayUrban(),
            config->getArraySpecialZone()
        );

        // Now the types match, no const_cast needed
        liberationArmy = new LiberationArmy(
            config->getLiberationUnitsPtrArray(), // This now returns Unit**
            config->getLiberationUnitsCount(),
            "LiberationArmy",
            battleField
        );

        arvnArmy = new ARVN(
            config->getARVNUnitsPtrArray(), // This now returns Unit**
            config->getARVNUnitsCount(),
            "ARVN",
            battleField
        );

    } catch (const std::exception& e) {
        cerr << "Error during HCMCampaign setup: " << e.what() << endl;
        delete liberationArmy; liberationArmy = nullptr;
        delete arvnArmy; arvnArmy = nullptr;
        delete battleField; battleField = nullptr;
        delete config; config = nullptr;
        throw;
    }
}

HCMCampaign::~HCMCampaign() {
    delete liberationArmy;
    delete arvnArmy; // Use ARVNArmy here
    delete battleField;
    delete config;
}

void HCMCampaign::run() {
    if (!liberationArmy || !arvnArmy || !battleField || !config) {
        cerr << "Campaign not properly initialized. Cannot run." << endl;
        return;
    }

    // 1. Apply terrain effects BEFORE any combat
    battleField->applyTerrainEffects(liberationArmy);
    battleField->applyTerrainEffects(arvnArmy);

    liberationArmy->updateStats(); // Update stats after terrain
    arvnArmy->updateStats();

    int ec = config->getEventCode();

    if (ec < 75) {
        // Quân Giải phóng tấn công, Quân đội Sài Gòn phòng thủ
        liberationArmy->fight(arvnArmy, false); // Lib Army attacks. This might wipe out ARVN.
        // After LibArmy's attack, ARVN's state is updated (possibly to 0 units).
        // Then, ARVN "defends" (responds to the attack results).
        // If ARVN was wiped out, arvnArmy->fight(true) will operate on an empty army.
        // If LibArmy's attack was "giao tranh khong xay ra", ARVN is preserved, and fight(true) should reflect that.
        arvnArmy->fight(liberationArmy, true);   // ARVN processes its defensive stance/outcome.
    } else {
        // Quân đội Sài Gòn tấn công
        arvnArmy->fight(liberationArmy, false); // ARVN attacks. LibArmy state changes.
        // Quân Giải phóng phòng thủ
        liberationArmy->fight(arvnArmy, true);   // LibArmy processes its defensive stance.
        // Quân Giải phóng phản công ngay lập tức
        liberationArmy->fight(arvnArmy, false); // LibArmy counter-attacks. ARVN state changes.
        // Quân đội Sài Gòn phòng thủ (again)
        arvnArmy->fight(liberationArmy, true);   // ARVN processes defense against counter.
    }

    // 3. Post-combat cleanup: remove units with attackScore <= 5
    auto cleanup_units = [](Army* army_ptr) {
        if (army_ptr && army_ptr->getUnitList()) {
            army_ptr->getUnitList()->removeUnitsIf([](Unit* u){ return u->getAttackScore() <= 5; });
            army_ptr->updateStats();
        }
    };
    cleanup_units(liberationArmy);
    cleanup_units(arvnArmy);

    // Final stat update might be good practice
    liberationArmy->updateStats();
    arvnArmy->updateStats();
}

string HCMCampaign::printResult() const {
    if (!liberationArmy || !arvnArmy) {
        return "LIBERATIONARMY[LF=0,EXP=0]-ARVN[LF=0,EXP=0]"; 
    }
    stringstream ss;
    ss << "LIBERATIONARMY[LF=" << liberationArmy->getLF() << ",EXP=" << liberationArmy->getEXP() << "]-"
       << "ARVN[LF=" << arvnArmy->getLF() << ",EXP=" << arvnArmy->getEXP() << "]";
    return ss.str();
}

////////////////////////////////////////////////
/// END OF STUDENT'S ANSWER
////////////////////////////////////////////////
