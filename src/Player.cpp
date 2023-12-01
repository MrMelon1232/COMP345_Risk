#include "Player.h"

int Player::nextID = 0;

// Constructor
Player::Player() : orders(new OrdersList), hand(new Hand) {
    // Initialize other members if needed
    playerID = nextID;
    nextID++;
    reinforcementPool = 10;
}

Player::Player(std::string& playerName) : Player() {
    name = playerName;
}

// Copy Constructor
Player::Player(Player& other) {
    // Copy territoriesOwned (assuming Territory objects are not dynamically allocated)
    territoriesOwned = other.territoriesOwned;

    // Copy orders (deep copy)
    if (other.orders != nullptr) {
        orders = new OrdersList(*other.orders); // Assuming OrdersList has a copy constructor
    }
    else {
        orders = nullptr;
    }

    // Copy hand (deep copy)
    if (other.hand != nullptr) {
        hand = new Hand(*other.hand); // Assuming Hand has a copy constructor
    }
    else {
        hand = nullptr;
    }
}

// Class assignment operator
Player& Player:: operator=(Player& other) {
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
    }
    else {
        orders = nullptr;
    }

    // Copy hand (deep copy)
    if (other.hand != nullptr) {
        hand = new Hand(*other.hand); // Assuming Hand has a copy constructor
    }
    else {
        hand = nullptr;
    }

    return *this;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, Player& player) {

    os << "-------------------------------------------------------------\n";
    //displays playerID
    os << "Player" << player.playerID << ": \n";

    os << "Territories Owned: \n";
    for (Territory* territory : player.territoriesOwned) {
        os << *territory;
    }

    if (player.orders != nullptr) {
        os << *player.orders; // Assuming that your OrdersList class has an operator<<
    }
    else {
        os << "None";
    }

    if (player.hand != NULL) {
        os << *player.hand;
    }
    else {
        os << "None";
    }
    os << "-------------------------------------------------------------\n";
    return os;
}

// Function that returns a list of territories that are to be defended
vector<Territory*> Player::toDefend() {
    if (ps) {
        return ps->toDefend(); // Delegate to PlayerStrategy's toDefend()
    }
    return {}; // Return empty vector if no strategy set
}

// Function that returns a list of territories that are to be attacked
vector<Territory*> Player::toAttack() {
   if (ps) {
        return ps->toAttack(); // Delegate to PlayerStrategy's toAttack()
    }
    return {}; // Return empty vector if no strategy set
}

// Function that creates an order object and adds it to the list of orders
void Player::issueOrder(Player* player, vector<Player*> target, OrderType type) {
    Order* newOrder = nullptr;
    //variables for issuing orders
    string playerTerritory, targetTerritory, playerList;
    int armyAmount, indexD = 0, indexA = 0;
    //player's territory list
    for (int i = 0; i < player->toDefend().size(); i++) {
        playerTerritory += "(" + to_string(i) + ")" + player->toDefend().at(i)->GetName() + " | ";
    }
    
    // Check which type of order it is
    switch (type) {
        case OrderType::Deploy:
            cout << "Issuing deploy order..." << endl;
            //message and input
            while (true) {
                cout << "Select territory to deploy army [int]: " << playerTerritory << endl;
                cin >> indexD;
                if (indexD >= player->getTerritories().size() || indexD < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            while (true) {
                cout << "Select reinforcement amount to deploy [int] (" + to_string(player->getTempPool()) + " remaining) : " << endl;
                cin >> armyAmount;
                if (armyAmount < 0) {
                    cout << "negative quantity is unavailable. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Deploy(armyAmount, player, player->toDefend().at(indexD));
            setTempPool(getTempPool() - armyAmount);
            break;
        case OrderType::Advance:
            cout << "Issuing advance order..." << endl;
            //message and input
            while (true) {
                cout << "Select territory to move army from [int]: " << playerTerritory << endl;
                cin >> indexD;
                if (indexD >= player->getTerritories().size() || indexD < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            for (int i = 0; i < player->toDefend().at(indexD)->GetAdjacentTerritories().size(); i++) {
                targetTerritory += "(" + to_string(i) + ")" + player->toDefend().at(indexD)->GetAdjacentTerritories().at(i)->GetName() + " | ";
            }
            while (true) {
                cout << "Select territory to move army to [int]: " << targetTerritory << endl;
                cin >> indexA;
                if (indexA >= player->toDefend().at(indexD)->GetAdjacentTerritories().size() || indexA < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            while (true) {
                cout << "Select army amount to move [int] :"/* + to_string(player->toDefend().at(indexD)->getNbArmies()) + " available) : "*/ << endl;
                cin >> armyAmount;
                if (armyAmount < 0) {
                    cout << "negative quantity is unavailable. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Advance(armyAmount, player, player->toDefend().at(indexD)->GetAdjacentTerritories().at(indexA), player->toDefend().at(indexD));
            break;
        case OrderType::Bomb:
            cout << "Issuing bomb order..." << endl;
            //target's territory list
            for (int i = 0; i < player->toAttack().size(); i++) {
                targetTerritory += "(" + to_string(i) + ")" + player->toAttack().at(i)->GetName() + " | ";
            }
            //message and input
            while (true) {
                cout << "Select territory to bomb [int]: " << targetTerritory << endl;
                cin >> indexA;
                if (indexA >= player->getTerritories().size() || indexA < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Bomb(player, player->toAttack().at(indexA));
            break;
        case OrderType::Blockade:
            cout << "Issuing blockade order..." << endl;
            //message and input
            while (true) {
                cout << "Select territory to blockade [int]: " << playerTerritory << endl;
                cin >> indexD;
                if (indexD >= player->getTerritories().size() || indexD < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Blockade(player, player->toDefend().at(indexD));
            break;
        case OrderType::Airlift:
            cout << "Issuing airlift order..." << endl;
            //message and input
            while (true) {
                cout << "Select territory to move army from [int]: " << playerTerritory << endl;
                cin >> indexD;
                if (indexD >= player->getTerritories().size() || indexD < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            while (true) {
                cout << "Select territory to move army to [int]: " << playerTerritory << endl;
                cin >> indexA;
                if (indexD >= player->getTerritories().size() || indexD < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            while (true) {
                cout << "Select army amount to move [int] :"/* + to_string(player->toDefend().at(indexD)->getNbArmies()) + " remaining) : "*/ << endl;
                cin >> armyAmount;
                if (armyAmount < 0) {
                    cout << "negative quantity is unavailable. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Airlift(player, player->toDefend().at(indexD), player->toDefend().at(indexA), armyAmount);
            break;
        case OrderType::Negotiate:
            cout << "Issuing diplomacy order..." << endl;
            //playerList
            for (int i = 0; i < target.size(); i++) {
                playerList += "(" + to_string(i) + ")" + target.at(i)->getName() + " | ";
            }
            //message and input
            while (true) {
                cout << "Select territory to move army from [int]: " << playerList << endl;
                cin >> indexA;
                if (indexA >= target.size() || indexA < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Negotiate(player, target.at(indexA));
            break;
        default:
            break;
        }
    orders->add(newOrder);
}

// Function to add territories to the player's possession
void Player::addTerritory(Territory* territory) {
    territoriesOwned.push_back(territory);
}

// Destructor, clean up any allocated memory
Player::~Player() {
    delete orders;
    delete hand;
    delete ps;
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

int Player::getTempPool()
{
    return tempPool;
}

void Player::setTempPool(int count)
{
    tempPool = count;
}

int Player::getHandSize()
{
    return hand->handSize();
}

string Player::getCard(int i)
{
    return hand->currentHand.at(i)->card;
}

OrdersList* Player::getOrdersList()
{
    return orders;
}


//test methods
void testPlayers();

// A3
void Player::setStrategy(PlayerStrategy* newStrategy) {
    ps = newStrategy;
}

string Player::getStrategy()
{
    return ps->getType(); 
}
