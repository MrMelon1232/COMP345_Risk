#pragma once

#include <vector>
#include "Orders.h" // Include necessary headers
#include "Map.h"

class Player; // Forward declaration of Player class
class Territory; // Forward declaration
enum class OrderType; // Forward declaration

class PlayerStrategy {
    public:
        PlayerStrategy();
        PlayerStrategy(Player* player);
        virtual void issueOrder(std::vector<Player*> target, OrderType type) = 0;
        virtual vector<Territory*> toDefend() = 0;
        virtual vector<Territory*> toAttack() = 0;
        // Define other pure virtual methods or common functionalities for strategies
        virtual ~PlayerStrategy() = default;
        virtual string getType() = 0; 
    private:
        Player* p;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    HumanPlayerStrategy(Player* player);
    void issueOrder(std::vector<Player*> target, OrderType type) override;
    vector<Territory*> toDefend() override;
    vector<Territory*> toAttack() override;
    string getType() override;
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    AggressivePlayerStrategy();
    AggressivePlayerStrategy(Player* player);
    AggressivePlayerStrategy(const AggressivePlayerStrategy& copied);
    ~AggressivePlayerStrategy() override;
    void issueOrder(std::vector<Player*> target, OrderType type) override;
    vector<Territory*> toDefend() override;
    vector<Territory*> toAttack() override;
    string getType() override;
private:
    Player* p;
};


class BeneloventPlayerStrategy : public PlayerStrategy {
public:
    BeneloventPlayerStrategy(Player* player);
    void issueOrder(std::vector<Player*> target, OrderType type) override;
    vector<Territory*> toDefend() override;
    vector<Territory*> toAttack() override;
    string getType() override;
};


class NeutralPlayerStrategy : public PlayerStrategy {
public:
    NeutralPlayerStrategy();
    NeutralPlayerStrategy(Player* player);
    NeutralPlayerStrategy(const NeutralPlayerStrategy& copied);
    ~NeutralPlayerStrategy() override;
    void issueOrder(std::vector<Player*> target, OrderType type) override;
    vector<Territory*> toDefend() override;
    vector<Territory*> toAttack() override;
    string getType() override;
private:
    Player* p;
};


class CheaterPlayerStrategy : public PlayerStrategy {
public:
    CheaterPlayerStrategy(Player* player);
    void issueOrder(std::vector<Player*> target, OrderType type) override;
    vector<Territory*> toDefend() override;
    vector<Territory*> toAttack() override;
    string getType() override;
private:
    Player* p;
};

// Enum containing the player strategies.
enum class StrategyType {
    Human,
    Aggressive,
    Benevolent,
    Neutral,
    Cheater,
    None // Represents NULL
};

// Free function to parse the player strategy.
StrategyType getStrategyType(string);
// Free function to create the proper strategy based on the type.
PlayerStrategy* loadStrategy(Player* player, StrategyType strategyType);
