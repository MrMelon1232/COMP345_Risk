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
            //using unique playerID and ownerID to determine to whom the territory belongs to
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

 
void AggressivePlayerStrategy::issueOrder(std::vector<Player*> target, OrderType type)
{
    Order* newOrder = nullptr;
    //variables for issuing orders
    string playerTerritory, targetTerritory, playerList;
    int armyAmount, indexD = 0, indexA = 0;
    //player's territory list
    for (int i = 0; i < p->toDefend().size(); i++) {
        playerTerritory += "(" + to_string(i) + ")" + p->toDefend().at(i)->GetName() + " | ";
    }
    
    // Check which type of order it is
    switch (type) 
    {
        case OrderType::Deploy:

            cout << "Issuing deploy order from an AGGRESSIVE player..." << endl;

            //The aggressive player will always deploy to the strongest territory it has
            Territory* strongest = toDefend().front();
            armyAmount = p->getReinforcementPool(); 
            newOrder = new Deploy(armyAmount, p, strongest);
            p->setReinforcementPool(p->getReinforcementPool() - armyAmount);
            cout<< armyAmount <<" units have been deployed to " <<strongest->GetName()<<"."<< endl;; 
            break;

        case OrderType::Advance:

            cout << "Issuing advance order from an AGGRESSIVE player..." << endl;

            Territory* strongest = toDefend().front();
            armyAmount = strongest->getNbArmies();

            for (Territory* target : strongest->GetAdjacentTerritories()) 
            {
                if (target->getOwnerID() != p->getPlayerID()) 
                {
                    cout<<"An agressive player is advancing towards player with ID:  "<< target->getOwnerID()<<"."<< endl; 
                    newOrder = new Advance(armyAmount, p, target, strongest);
                    break;
                }
            }
            break;

        case OrderType::Bomb:

            cout << "Issuing bomb order from an AGGRESSIVE player..." << endl;
            Territory *target = toAttack().front();
            cout<<"An agressive player is bombing a player with ID:  "<< target->getOwnerID()<<"."<< endl;
            newOrder = new Bomb(p, target);
            break;

        case OrderType::Blockade:

            cout << "Issuing blockade order from an AGGRESSIVE player..." << endl;
            //Agressive player will blockade the weakest territory
            Territory* weakest = toDefend().back();
            cout << "Aggressive player using blockade on weakest territory: " <<weakest->GetName()<<endl;
            newOrder = new Blockade(p, weakest);
            break;

        case OrderType::Airlift:

            cout << "Issuing airlift order from an AGRESSIVE player..." << endl;
            //Aggressive player will move forward players from weak territory to the strongest one

            Territory* strongest = toDefend().front();
            Territory* weakest = toDefend().back();
            armyAmount = weakest->getNbArmies(); 
            cout << "Aggressive player about to airlift units to its strongest territory!"<<endl;
            newOrder = new Airlift(p, strongest, weakest, armyAmount);
            break;

        case OrderType::Negotiate:
            cout << "Aggressive player REFUSES to negotiate!" << endl;
            break;
        
        //adding new order to list 
        p->getOrdersList()->add(newOrder);      

        default:
            break;
    }
}

//returns the type of the strategy
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

//The neutral player is not looking to attack
vector<Territory*> NeutralPlayerStrategy::toAttack()
{
    vector<Territory*> notAttacking;
    cout<<"Neutral player is returning an empty list for attacking...";
    return notAttacking;
}

//The neutral player is not defending
vector<Territory*> NeutralPlayerStrategy::toDefend()
{
    vector<Territory*> notDefending;
    cout<<"Neutral player is returning an empty list for defending...";
    return notDefending;
}

void NeutralPlayerStrategy::issueOrder(std::vector<Player*> target, OrderType type)
{
    cout<<"This is player is using the NeutralPlayerStrategy, it cannot issue orders of any type!"; 
}

//returns the type of the strategy
string NeutralPlayerStrategy::getType()
{
    return "neutral"; 
}

//--------------------------------------------------------------------------------------------------
// End of Neutral Player Strategy
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Cheater Player Strategy
//--------------------------------------------------------------------------------------------------

void CheaterPlayerStrategy::issueOrder(std::vector<Player*> target, OrderType type)
{
    cout << "Cheater player does not manually issue orders.\n";
}

vector<Territory*> CheaterPlayerStrategy::toDefend()
{
    vector<Territory*> notDefending;
    cout << "Cheater player does not defend.\n";
    return notDefending;
}

vector<Territory*> CheaterPlayerStrategy::toAttack()
{
    // Automatically conquers all territories adjacent to its own territories
    vector<Territory*> territoriesToAttack;

    for (Territory* ownTerritory : p->getTerritories())
    {
        for (Territory* adjacentTerritory : ownTerritory->GetAdjacentTerritories())
        {
            if (adjacentTerritory->getOwnerID() != p->getPlayerID())
            {
                territoriesToAttack.push_back(adjacentTerritory);
                cout << "Cheater player conquers " << adjacentTerritory->GetName() << ".\n";
                adjacentTerritory->setOwnerID(p->getPlayerID());
            }
        }
    }

    return territoriesToAttack;
}

string CheaterPlayerStrategy::getType()
{
    return "cheater";
}

//--------------------------------------------------------------------------------------------------
// End of Cheater Player Strategy
//--------------------------------------------------------------------------------------------------