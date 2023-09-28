#include "Player.h"
#include "Orders.h"

// Constructor
Player:: Player() {

}

// Copy Constructor
Player:: Player(const Player& other) {

}

// Class assignment operator
Player& Player:: operator=(const Player& other) {
    if(this == &other) {
        return *this;
    }

    return *this;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Player& player) {
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
    if (type == OrderType::Deploy) {
        newOrder = new Deploy();
    } else if (type == OrderType::Advance) {
        newOrder = new Advance();
    } else if (type == OrderType::Bomb) {
        newOrder = new Bomb();
    } else if (type == OrderType::Blockade) {
        newOrder = new Blockade();
    } else if (type == OrderType::Airlift) {
        newOrder = new Airlift();
    } else if (type == OrderType::Negotiate) {
        newOrder = new Negotiate();
    }
     
    orders.push_back(newOrder);
}

Player::~Player() {
    // Destructor, clean up any allocated memory
    for (Order* order : orders) {
        delete order;
    }
}