#pragma once

#include <vector>
#include "Orders.h" // Include necessary headers
#include "Map.h"

class Player; // Forward declaration of Player class
class Territory; // Forward declaration

class PlayerStrategy {
    public:
        virtual void issueOrder(std::vector<Player*> target, OrderType type) = 0;
        virtual vector<Territory*> toDefend() = 0;
        virtual vector<Territory*> toAttack() = 0;
        // Define other pure virtual methods or common functionalities for strategies
        virtual ~PlayerStrategy() = default;
    private:
        Player* p;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(std::vector<Player*> target, OrderType type) override;
    vector<Territory*> toDefend() override;
    vector<Territory*> toAttack() override;
};

class AggressivePlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(std::vector<Player*> target, OrderType type) override;
    vector<Territory*> toDefend() override;
    vector<Territory*> toAttack() override;
};


class BeneloventPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(std::vector<Player*> target, OrderType type) override;
    vector<Territory*> toDefend() override;
    vector<Territory*> toAttack() override;
};


class NeutralPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(std::vector<Player*> target, OrderType type) override;
    vector<Territory*> toDefend() override;
    vector<Territory*> toAttack() override;
};


class CheaterPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(std::vector<Player*> target, OrderType type) override;
    vector<Territory*> toDefend() override;
    vector<Territory*> toAttack() override;
};