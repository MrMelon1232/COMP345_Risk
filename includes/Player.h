// Set ifndef to disable or enable header file
#ifndef PLAYER_H
#define PLAYER_H

// Includes
#include "Orders.h"
#include "Cards.h"
#include "Map.h"
#include <vector>
//#include <string>
using namespace std;
#include <iostream> 
#include "PlayerStrategies.h"


// Foward declaration of classes to be used for typing
class Territory;
class OrdersList;
class Hand;
class Card;
enum class OrderType;
class PlayerStrategy;

// Declaration of Player class
class Player
{
    // Players' possessions
private:
    // Player name
    string name;

    // List of territories owned by the player
    std::vector<Territory*> territoriesOwned;

    // List of orders owned by the player
    OrdersList* orders;

    // Hand of cards owned by the player
    Hand* hand;

    //NEW: Each player will have a unique ID 
    static int nextID;
    int playerID;
  
    //NEW: Each player will have a reinforcement pool
    int reinforcementPool, tempPool;

    //NEW: each player will have a vector of integers for players which they have an alliance with
    std::vector<int> myAlliesForTheTurn;

    // Member of type PlayerStrategy
    PlayerStrategy* ps; 

    // Player public methods
public:

    // Constructor
    Player();

    // Copy constructor
    Player(Player& other);

    // Class assignment operator
    Player& operator=(Player& other);

    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, Player& player);

    // Returns a list of territories that are to be defended
    std::vector<Territory*> toDefend();

    // Returns a list of territories that are to be attacked
    std::vector<Territory*> toAttack();

    // Creates order object and adds it to the list of orders
    void issueOrder(Player* player, vector<Player*> target, OrderType type);

    // Destructor to clean up resources
    ~Player();
    //NEW methods for a2-part4
    int getPlayerID();
    int getReinforcementPool();
    void setReinforcementPool(int reinforcement);
    bool isAnyAdjacent(Territory* territory);
    void addCardToHand(Card* c);
    void addAlly(int allyID);
    void clearAllies();
    bool isAllyPresent(int allyID);
  
        //A2
        string getName();

        void setName(string n);
        
        vector<Territory*> getTerritories();

        int getTempPool();
        void setTempPool(int count);
        // Add territory
        void addTerritory(Territory* territory);

        Hand* getHand() const {return hand;}
  
        Player(std::string& playerName);

        int getHandSize();

        string getCard(int i);

        OrdersList* getOrdersList();

    //A3 part
    void setStrategy(PlayerStrategy* newStrategy);
};


//test methods
void testPlayers();
#endif
