// Set ifndef to disable or enable header file
#ifndef PLAYER_H
#define PLAYER_H

// Includes
#include <vector>
using namespace std;
#include <iostream> 


// Foward declaration of classes to be used for typing
class Territory;
class OrdersList;
class Hand;
class Card;
enum class OrderType;

// Declaration of Player class
class Player
{
    // Players' possessions
private:

    // List of territories owned by the player
    std::vector<Territory*> territoriesOwned;

    // List of orders owned by the player
    OrdersList* orders;

    // Hand of cards owned by the player
    Hand* hand;

    //NEW: Each player will have a unique ID 
    static int nextID;
    int playerID;

    // Player name
    string name;

    //NEW: Each player will have a reinforcement pool
    int reinforcementPool;

    //NEW: each player will have a vector of integers for players which they have an alliance with
    std::vector<int> myAlliesForTheTurn;


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
    void issueOrder(OrderType type);

    // Add territory
    void addTerritory(Territory* territory);

    Hand* getHand() const {return hand;}

    // Destructor to clean up resources
    ~Player();

    Player(const std::string& playerName);

    //NEW methods for a2-part4
    int getPlayerID();
    int getReinforcementPool();
    void setReinforcementPool(int reinforcement);
    bool isAnyAdjacent(Territory* territory);
    void addCardToHand(Card* c);
    void addAlly(int allyID);
    void clearAllies();
    bool isAllyPresent(int allyID);
};



#endif
