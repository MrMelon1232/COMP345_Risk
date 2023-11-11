// Set ifndef to disable or enable header file
#ifndef PLAYER_H
#define PLAYER_H

// Includes
#include "Orders.h"
#include "Cards.h"
#include "Map.h"
#include <vector>
using namespace std;
#include <iostream> 


// Foward declaration of classes to be used for typing
class Territory;
class OrdersList;
class Hand;
enum class OrderType;

// Declaration of Player class
class Player 
{
    // Players' possessions
    private:
        string name;

        // List of territories owned by the player
        vector<Territory*> territoriesOwned;
        
        // List of orders owned by the player
        OrdersList* orders;

        // Hand of cards owned by the player
        Hand* hand;

        int reinforcement;
    // Player public methods
    public:

        // Constructor
        Player();

        // Copy constructor
        Player(const Player& other);

        // Class assignment operator
        Player& operator=(const Player& other);

        // Stream insertion operator
        friend ostream& operator<<(ostream& os, const Player& player);

        // Returns a list of territories that are to be defended
        vector<Territory*> toDefend();
        
        // Returns a list of territories that are to be attacked
        vector<Territory*> toAttack();

        // Creates order object and adds it to the list of orders
        void issueOrder(OrderType type);

        // Add territory
        void addTerritory(Territory* territory);

        // Destructor to clean up resources
        ~Player();

        //A2
        string getName();

        void setName(string n);
        
        vector<Territory*> getTerritories();

        int getReinforcement();

        void setReinforcement(int army);
};



//test methods
void testPlayers();
#endif
