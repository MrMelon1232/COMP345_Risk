#include "Player.h"

int Player::nextID = 0;

// Constructor
Player::Player() : orders(new OrdersList), hand(new Hand) {
    // Initialize other members if needed
    playerID = nextID;
    nextID++;
    reinforcementPool = 10;
}

// Copy Constructor
Player:: Player(const Player& other) {
    // Copy territoriesOwned (assuming Territory objects are not dynamically allocated)
    territoriesOwned = other.territoriesOwned;

    // Copy orders (deep copy)
    if (other.orders != nullptr) {
        orders = new OrdersList(*other.orders); // Assuming OrdersList has a copy constructor
    } else {
        orders = nullptr;
    }

    // Copy hand (deep copy)
    if (other.hand != nullptr) {
        hand = new Hand(*other.hand); // Assuming Hand has a copy constructor
    } else {
        hand = nullptr;
    }
}

// Class assignment operator
Player& Player:: operator=(const Player& other) {
   if (this == &other) {
        return *this; // Self-assignment, no need to do anything
    }

    // Deallocate existing memory (if any)
    delete orders;
    delete hand;

    // Copy territoriesOwned (assuming Territory objects are not dynamically allocated)
    territoriesOwned = other.territoriesOwned;

    // Copy orders (deep copy)
    if (other.orders != nullptr) {
        orders = new OrdersList(*other.orders); // Assuming OrdersList has a copy constructor
    } else {
        orders = nullptr;
    }

    // Copy hand (deep copy)
    if (other.hand != nullptr) {
        hand = new Hand(*other.hand); // Assuming Hand has a copy constructor
    } else {
        hand = nullptr;
    }

    return *this;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Player& player) {

    os << "-------------------------------------------------------------\n";
    //displays playerID
    os << "Player" <<player.playerID<<": \n";

    os << "Territories Owned: \n";
    for (const Territory* territory : player.territoriesOwned) {
        os << *territory;
    }
    
    if (player.orders != nullptr) {
        os << *player.orders; // Assuming that your OrdersList class has an operator<<
    } else {
        os << "None";
    }
    
    if (player.hand != NULL) {
        os << *player.hand; 
    } else {
        os << "None";
    }
    os << "-------------------------------------------------------------\n";
    return os;
}

// Function that returns a list of territories that are to be defended
vector<Territory*> Player::toDefend() {
    vector<Territory*> defendTerritories;
    defendTerritories.push_back(new Territory("TerritoryA", "ContinentA")); // Example territory
    defendTerritories.push_back(new Territory("TerritoryB", "ContinentA")); // Example territory
    return defendTerritories;
}

// Function that returns a list of territories that are to be attacked
vector<Territory*> Player::toAttack() {
    // Implement toAttack logic, return a list of territories to attack
    vector<Territory*> attackTerritories;
    // Populate the list with territories
    attackTerritories.push_back(new Territory("TerritoryC", "ContinentB")); // Example territory
    attackTerritories.push_back(new Territory("TerritoryD", "ContinentB")); // Example territory

    return attackTerritories;
}

// Function that creates an order object and adds it to the list of orders
void Player::issueOrder(OrderType type) {
    Order* newOrder = nullptr;

    // Check which type of order it is
    switch (type) {
        case OrderType::Deploy:
            newOrder = new Deploy();
            break;
        case OrderType::Advance:
            newOrder = new Advance();
            break;
        case OrderType::Bomb:
            newOrder = new Bomb();
            break;
        case OrderType::Blockade:
            newOrder = new Blockade();
            break;
        case OrderType::Airlift:
            newOrder = new Airlift();
            break;
        case OrderType::Negotiate:
            newOrder = new Negotiate();
            break;
        default:
            break;
        }
    orders->add(newOrder);
}

// Function to add territories to the player's possession
void Player:: addTerritory(Territory* territory) {
    territoriesOwned.push_back(territory);
}

// Destructor, clean up any allocated memory
Player::~Player() {
    delete orders;
    delete hand;
}

//Accessor method which returns the id of a player
int Player::getPlayerID() {
    return playerID;
}

//Accessor and mutator method for reinforcement pool
int Player::getReinforcementPool()
{
    return reinforcementPool; 
}
void Player::setReinforcementPool(int reinforcement)
{
    reinforcementPool = reinforcement; 
}

//The following method checks if a given territory is adjacent with any of the owned territories of the player
bool Player::isAnyAdjacent(Territory* territory)
{
    for (Territory* ownedTerritory : territoriesOwned) 
    {
        if (ownedTerritory->isAdjacent(territory)) 
        {
            return true; 
        }
    }
    return false; 
}

//adds a card to the hand of the player
void Player::addCardToHand(Card* c)
{
    hand->addHand(c);
}

//adds an ally
void Player::addAlly(int allyID) 
{
    myAlliesForTheTurn.push_back(allyID); 
}

//Player must call this method at the end of each of their turn
void Player::clearAllies() 
{
    myAlliesForTheTurn.clear();
}

//Checks if the player has the passed ally in their vector
bool Player::isAllyPresent(int allyID) 
{
    for (int value : myAlliesForTheTurn) 
    {
        if (value == allyID) 
        {
            return true; 
        }
    }
    return false; 
}

//A2

string Player::getName()
{
    return name;
}

void Player::setName(string n)
{
    name = n;
}

vector<Territory*> Player::getTerritories()
{
    return territoriesOwned;
}

//test methods
void testPlayers();
