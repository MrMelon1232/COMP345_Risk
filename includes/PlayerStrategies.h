#pragma once

#include <vector>
#include "Orders.h" // Include necessary headers

class Player; // Forward declaration of Player class

class PlayerStrategy {
public:
    
    virtual void issueOrder(Player* player, std::vector<Player*> target, OrderType type) = 0;
    virtual void toDefend() = 0;
    virtual void toAttack() = 0;
    // Define other pure virtual methods or common functionalities for strategies
    virtual ~PlayerStrategy() = default;
};

class HumanPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player, std::vector<Player*> target, OrderType type) override;
    void toDefend() override;
    void toAttack() override;
};

class AggressivePlayerStrategy PlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player, std::vector<Player*> target, OrderType type) override;
    void toDefend() override;
    void toAttack() override;
};


class BeneloventPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player, std::vector<Player*> target, OrderType type) override;
    void toDefend() override;
    void toAttack() override;
};


class NeutralPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player, std::vector<Player*> target, OrderType type) override;
    void toDefend() override;
    void toAttack() override;
};


class CheaterPlayerStrategy : public PlayerStrategy {
public:
    void issueOrder(Player* player, std::vector<Player*> target, OrderType type) override;
    void toDefend() override;
    void toAttack() override;
};



