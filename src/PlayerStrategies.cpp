#pragma once
#include "PlayerStrategies.h"

//--------------------------------------------------------------------------------------------------
// Agressive Player Strategy
//--------------------------------------------------------------------------------------------------

// Default constructor
AggressivePlayerStrategy::AggressivePlayerStrategy()
{
    this->p = nullptr; 
}

// Non-default constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player)
{
    this->p = player;
}

// Copy constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &copied)
{
    this->p = copied.p;
}

// Destructor
AggressivePlayerStrategy::~AggressivePlayerStrategy()
{
    
}

// The aggressive player must know which territories to attack
vector<Territory*> AggressivePlayerStrategy::toAttack()
{
    vector<Territory*> territoriesToAttack;

    for(Territory* allOwnedTerritory: this->p->getTerritories()) 
    {
        for (Territory* adjacentTerritory: allOwnedTerritory->GetAdjacentTerritories()) 
        {
            if (adjacentTerritory->getOwnerID() != this->p->getPlayerID() && 
                find(territoriesToAttack.begin(), territoriesToAttack.end(), adjacentTerritory->getOwnerID()) == territoriesToAttack.end()) 
                {
                territoriesToAttack.push_back(adjacentTerritory);
                }
        }
    }
    return territoriesToAttack;
}

// The aggressive player must know which territories it must defend
vector<Territory*> AggressivePlayerStrategy::toDefend()
{
    vector<Territory*> adjacentToEnemy;

    // Looking for territories that are adjacent to enemy territory
    for (Territory* territories : this->p->getTerritories()) 
    {
        for (Territory* adjacent : territories->GetAdjacentTerritories()) 
        {
 
            if (this->p->getPlayerID() != adjacent->getOwnerID()) 
            {
                adjacentToEnemy.push_back(territories);
                break;
            }
        }
    }
    Territory* listOfStrongestTerritories = adjacentToEnemy.front();

    // Looking for strongest territories
    for (Territory* territories : adjacentToEnemy) {
        if (territories->getNbArmies() > listOfStrongestTerritories->getNbArmies()) {
            listOfStrongestTerritories = territories;
        }
    }

    cout<<"Returning a list containing the strongest territories..."; 
    return vector<Territory*>({listOfStrongestTerritories});
}

//to be completed 
void AggressivePlayerStrategy::issueOrder(std::vector<Player*> target, OrderType type)
{
    return; 
}

string AggressivePlayerStrategy::getType()
{
    return "aggressive"; 
}

//--------------------------------------------------------------------------------------------------
// End of Agressive Player Strategy
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Neutral Player Strategy
//--------------------------------------------------------------------------------------------------

NeutralPlayerStrategy::NeutralPlayerStrategy()
{
    this->p = nullptr;
}


NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player)
{
    this->p = player;
}

NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy &copied)
{
    this->p = copied.p;
}

NeutralPlayerStrategy::~NeutralPlayerStrategy()
{

}

vector<Territory*> NeutralPlayerStrategy::toAttack()
{
    vector<Territory*> notAttacking;
    cout<<"Neutral player is returning an empty list for attacking...";
    return notAttacking;
}

vector<Territory*> NeutralPlayerStrategy::toDefend()
{
    vector<Territory*> notDefending;
    cout<<"Neutral player is returning an empty list for defending...";
    return notDefending;
}

// to complete
void NeutralPlayerStrategy::issueOrder(std::vector<Player*> target, OrderType type)
{
    cout<<"This is player is using the NeutralPlayerStrategy, it cannot issue orders of any type!"; 
}

string NeutralPlayerStrategy::getType()
{
    return "neutral"; 
}

//--------------------------------------------------------------------------------------------------
// End of Neutral Player Strategy
//--------------------------------------------------------------------------------------------------