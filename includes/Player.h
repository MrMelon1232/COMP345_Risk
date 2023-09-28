// Set ifndef to disable or enable header file
#ifndef PLAYER_H
#define PLAYER_H

// Includes
#include <vector>
using namespace std;
#include <iostream> 


// Foward declaration of classes to be used for typing
class Territory;
class Order;

// Declaration of Player class
class Player 
{
    // Players' possessions
    private:

        // List of territories owned by the player
        std::vector<Territory*> territoriesOwned;

        // List of orders owned by the player
        std::vector<Order*> orders;

    // Player public methods
    public:

        // Constructor
        Player();

        // Copy constructor
        Player(const Player& other);

        // Class assignment operator
        Player& operator=(const Player& other);

        // Stream insertion operator
        friend std::ostream& operator<<(std::ostream& os, const Player& player);

        // Returns a list of territories that are to be defended
        std::vector<Territory*> toDefend();
        
        // Returns a list of territories that are to be attacked
        std::vector<Territory*> toAttack();

        // Creates order object and adds it to the list of orders
        void issueOrder();
};


#endif
