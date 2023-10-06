#include "Player.h"
#include "Orders.h"
#include "Cards.h"

// Constructor
Player::Player() : orders(new OrdersList), hand(new Hand) {
    // Initialize other members if needed
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
    os << "Territories Owned: ";
    for (const Territory* territory : player.territoriesOwned) {

    }
    
    os << "\nOrders: ";
    if (player.orders != nullptr) {
        os << *player.orders; // Assuming that your OrdersList class has an operator<<
    } else {
        os << "None";
    }

    os << "\nHand: ";
    if (player.hand != NULL) {
        os << *player.hand; 
    } else {
        os << "None";
    }

    return os;
}

// Function that returns a list of territories that are to be defended
std::vector<Territory*> Player::toDefend() {
    std::vector<Territory*> defendTerritories;
    return defendTerritories;
}

// Function that returns a list of territories that are to be attacked
std::vector<Territory*> Player::toAttack() {
    // Implement toAttack logic, return a list of territories to attack
    std::vector<Territory*> attackTerritories;
    // Populate the list with territories
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

Player::~Player() {
    // Destructor, clean up any allocated memory
    delete orders;
    delete hand;
}