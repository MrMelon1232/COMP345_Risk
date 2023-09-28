#include "Player.h"

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
    // Implement the stream insertion operator to output player information
    os << "Player territories: ";
    for (Territory* territory : player.territoriesOwned) {
        os << territory->territoriesOwned() << " ";
    }
    os << "\nPlayer orders: ";
    for (Order* order : player.orders) {
        os << order->getDescription() << " ";
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
void Player::issueOrder() {
    Order* newOrder = new Order();
    orders.push_back(newOrder);
}