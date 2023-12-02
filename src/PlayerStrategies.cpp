#include "PlayerStrategies.h"

PlayerStrategy::PlayerStrategy(Player* player) {
    this->p = player;
}

PlayerStrategy::PlayerStrategy(const PlayerStrategy& copied) {
    this->p = copied.p;
}
    
//--------------------------------------------------------------------------------------------------
// Human Player Strategy
//--------------------------------------------------------------------------------------------------

// Constructor
HumanPlayerStrategy::HumanPlayerStrategy(Player *player) : PlayerStrategy(player) {}

// Issue order function
void HumanPlayerStrategy::issueOrder(std::vector<Player*> target, OrderType type) {
    Order* newOrder = nullptr;
    //variables for issuing orders
    string playerTerritory, targetTerritory, playerList;
    int armyAmount, indexD = 0, indexA = 0;
    //player's territory list
    for (int i = 0; i < p->toDefend().size(); i++) {
        playerTerritory += "(" + to_string(i) + ")" + p->toDefend().at(i)->GetName() + " | ";
    }
    
    // Check which type of order it is
    switch (type) {
        case OrderType::Deploy:
            cout << "Issuing deploy order..." << endl;
            //message and input
            while (true) {
                cout << "Select territory to deploy army [int]: " << playerTerritory << endl;
                cin >> indexD;
                if (indexD >= p->getTerritories().size() || indexD < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            while (true) {
                cout << "Select reinforcement amount to deploy [int] (" + to_string(p->getReinforcementPool()) + " remaining) : " << endl;
                cin >> armyAmount;
                if (armyAmount < 0) {
                    cout << "negative quantity is unavailable. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Deploy(armyAmount, p, p->toDefend().at(indexD));
            p->setTempPool(p->getReinforcementPool() - armyAmount);
            break;
        case OrderType::Advance:
            cout << "Issuing advance order..." << endl;
            //message and input
            while (true) {
                cout << "Select territory to move army from [int]: " << playerTerritory << endl;
                cin >> indexD;
                if (indexD >= p->getTerritories().size() || indexD < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            for (int i = 0; i < p->toDefend().at(indexD)->GetAdjacentTerritories().size(); i++) {
                targetTerritory += "(" + to_string(i) + ")" + p->toDefend().at(indexD)->GetAdjacentTerritories().at(i)->GetName() + " | ";
            }
            while (true) {
                cout << "Select territory to move army to [int]: " << targetTerritory << endl;
                cin >> indexA;
                if (indexA >= p->toDefend().at(indexD)->GetAdjacentTerritories().size() || indexA < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            while (true) {
                cout << "Select army amount to move [int] :"/* + to_string(player->toDefend().at(indexD)->getNbArmies()) + " available) : "*/ << endl;
                cin >> armyAmount;
                if (armyAmount < 0) {
                    cout << "negative quantity is unavailable. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Advance(armyAmount, p, p->toDefend().at(indexD)->GetAdjacentTerritories().at(indexA), p->toDefend().at(indexD));
            break;
        case OrderType::Bomb:
            cout << "Issuing bomb order..." << endl;
            //target's territory list
            for (int i = 0; i < p->toAttack().size(); i++) {
                targetTerritory += "(" + to_string(i) + ")" + p->toAttack().at(i)->GetName() + " | ";
            }
            //message and input
            while (true) {
                cout << "Select territory to bomb [int]: " << targetTerritory << endl;
                cin >> indexA;
                if (indexA >= p->getTerritories().size() || indexA < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Bomb(p, p->toAttack().at(indexA));
            break;
        case OrderType::Blockade:
            cout << "Issuing blockade order..." << endl;
            //message and input
            while (true) {
                cout << "Select territory to blockade [int]: " << playerTerritory << endl;
                cin >> indexD;
                if (indexD >= p->getTerritories().size() || indexD < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Blockade(p, p->toDefend().at(indexD));
            break;
        case OrderType::Airlift:
            cout << "Issuing airlift order..." << endl;
            //message and input
            while (true) {
                cout << "Select territory to move army from [int]: " << playerTerritory << endl;
                cin >> indexD;
                if (indexD >= p->getTerritories().size() || indexD < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            while (true) {
                cout << "Select territory to move army to [int]: " << playerTerritory << endl;
                cin >> indexA;
                if (indexA >= p->getTerritories().size() || indexA < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            while (true) {
                cout << "Select army amount to move [int] :"/* + to_string(player->toDefend().at(indexD)->getNbArmies()) + " remaining) : "*/ << endl;
                cin >> armyAmount;
                if (armyAmount < 0) {
                    cout << "negative quantity is unavailable. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Airlift(p, p->toDefend().at(indexD), p->toDefend().at(indexA), armyAmount);
            break;
        case OrderType::Negotiate:
            cout << "Issuing diplomacy order..." << endl;
            //playerList
            for (int i = 0; i < target.size(); i++) {
                playerList += "(" + to_string(i) + ")" + target.at(i)->getName() + " | ";
            }
            //message and input
            while (true) {
                cout << "Select territory to move army from [int]: " << playerList << endl;
                cin >> indexA;
                if (indexA >= target.size() || indexA < 0) {
                    cout << "index out of range. try again." << endl;
                }
                else {
                    break;
                }
            }
            newOrder = new Negotiate(p, target.at(indexA));
            break;
        default:
            break;
        }
    // Add the created order to the player's list of orders
    p->getOrdersList()->add(newOrder);
}

// To defend function
vector<Territory*> HumanPlayerStrategy::toDefend() {
    return p->getTerritories();
}

// To attack function
vector<Territory*> HumanPlayerStrategy::toAttack() {
    vector<Territory*> territoriesToAttack;

    for (Territory* ownedTerritory : p->getTerritories()) {
        for (Territory* adjacentTerritory : ownedTerritory->GetAdjacentTerritories()) {
            if (adjacentTerritory->getOwnerID() != p->getPlayerID() &&
                find(territoriesToAttack.begin(), territoriesToAttack.end(), adjacentTerritory) == territoriesToAttack.end()) {
                territoriesToAttack.push_back(adjacentTerritory);
            }
        }
    }
    return territoriesToAttack;
}


string HumanPlayerStrategy::getType() {
    return "human";
}


//--------------------------------------------------------------------------------------------------
// End of Human Player Strategy
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Agressive Player Strategy
//--------------------------------------------------------------------------------------------------

// Non-default constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(Player *player) : PlayerStrategy(player) {}

// Copy constructor
AggressivePlayerStrategy::AggressivePlayerStrategy(const AggressivePlayerStrategy &copied) : PlayerStrategy(copied) {}

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
                find(territoriesToAttack.begin(), territoriesToAttack.end(), adjacentTerritory) == territoriesToAttack.end()) 
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
    Territory* toBomb = toAttack().front();
    Territory* strongest = toDefend().front();
    Territory* weakest = toDefend().back();

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
            armyAmount = p->getReinforcementPool(); 
            newOrder = new Deploy(armyAmount, p, strongest);
            p->setReinforcementPool(p->getReinforcementPool() - armyAmount);
            cout<< armyAmount <<" units have been deployed to " <<strongest->GetName()<<"."<< endl;; 
            break;

        case OrderType::Advance:

            cout << "Issuing advance order from an AGGRESSIVE player..." << endl;

            armyAmount = strongest->getNbArmies();

            for (Territory* toAdvance : strongest->GetAdjacentTerritories()) 
            {
                if (toAdvance->getOwnerID() != p->getPlayerID()) 
                {
                    cout<<"An agressive player is advancing towards player with ID:  "<< toAdvance->getOwnerID()<<"."<< endl; 
                    newOrder = new Advance(armyAmount, p, toAdvance, strongest);
                    break;
                }
            }
            break;

        case OrderType::Bomb:

            cout << "Issuing bomb order from an AGGRESSIVE player..." << endl;
            cout<<"An agressive player is bombing a player with ID:  "<< toBomb->getOwnerID()<<"."<< endl;
            newOrder = new Bomb(p, toBomb);
            break;

        case OrderType::Blockade:

            cout << "Issuing blockade order from an AGGRESSIVE player..." << endl;
            //Agressive player will blockade the weakest territory
            cout << "Aggressive player using blockade on weakest territory: " <<weakest->GetName()<<endl;
            newOrder = new Blockade(p, weakest);
            break;

        case OrderType::Airlift:

            cout << "Issuing airlift order from an AGRESSIVE player..." << endl;
            //Aggressive player will move forward players from weak territory to the strongest one
            armyAmount = weakest->getNbArmies(); 
            cout << "Aggressive player about to airlift units to its strongest territory!"<<endl;
            newOrder = new Airlift(p, strongest, weakest, armyAmount);
            break;

        case OrderType::Negotiate:
            cout << "Aggressive player REFUSES to negotiate!" << endl;
            break;
        

        default:
            break;
    }
    //adding new order to list 
    p->getOrdersList()->add(newOrder);      
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
// Benevolent Player Strategy
//--------------------------------------------------------------------------------------------------
BenevolentPlayerStrategy::BenevolentPlayerStrategy(Player *player) : PlayerStrategy(player) {}

void BenevolentPlayerStrategy::issueOrder(std::vector<Player*> target, OrderType type) {
    Order* newOrder = nullptr; 
    int armyAmount = 0;

    // Get the weakest territories to defend
    vector<Territory*> weakestTerritories = p->toDefend();

    // Check which type of order it is
    switch (type) {
        case OrderType::Deploy:
            cout << "Issuing deploy order for weakest territories..." << endl;
            armyAmount = p->getReinforcementPool(); // Deploy all available armies to the weakest territories
            for (Territory* territory : weakestTerritories) {
                newOrder = new Deploy(armyAmount, p, territory);
                 //adding new order to list 
                p->getOrdersList()->add(newOrder);
                p->setTempPool(p->getTempPool() - armyAmount);
            }
            break;

        case OrderType::Advance:
            cout << "Issuing advance order for weakest territories..." << endl;
            for (Territory* territory : weakestTerritories) {
                // Find an adjacent territory to the weakest owned territory
                Territory* adjacentToWeakest = nullptr;
                for (Territory* adjacentTerritory : territory->GetAdjacentTerritories()) {
                    if (adjacentTerritory->getOwner() == p) {
                        adjacentToWeakest = adjacentTerritory;
                        break;
                    }
                }

                if (adjacentToWeakest != nullptr) {
                    armyAmount = adjacentToWeakest->getNbArmies() / 2;
                    newOrder = new Advance(armyAmount, p, territory, adjacentToWeakest);
                    //adding new order to list 
                    p->getOrdersList()->add(newOrder);   
                }
            }
            break;

        case OrderType::Bomb:
            cout << "Issuing bomb order (no action)..." << endl;
            // Benevolent player doesn't perform bombing
            break;

        case OrderType::Blockade:
            cout << "Issuing blockade order..." << endl;
            for (Territory* territory : weakestTerritories) {
                // Create a blockade order for each weakest territory owned by the player
                newOrder = new Blockade(p, territory);
                //adding new order to list 
                p->getOrdersList()->add(newOrder);   
            }
            break;

        case OrderType::Airlift:
            cout << "Issuing airlift order to reinforce weakest territories..." << endl;
            for (Territory* weakestOwnedTerritory : weakestTerritories) {
                // Find a source territory (owned by the player and not the weakest)
                vector<Territory*> eligibleTerritories;
                for (Territory* territory : p->getTerritories()) {
                    if (territory != weakestOwnedTerritory && territory->getOwner() == p && territory->getNbArmies() > 0) {
                        eligibleTerritories.push_back(territory);
                    }
                }
                if (!eligibleTerritories.empty()) {
                    // Randomly select a territory from the eligible ones
                    int randomIndex = rand() % eligibleTerritories.size();
                    Territory* sourceTerritory = eligibleTerritories[randomIndex];
                    // Choose the weakest territory as the destination
                    armyAmount = sourceTerritory->getNbArmies() / 2;
                    newOrder = new Airlift(p, weakestOwnedTerritory, sourceTerritory, armyAmount);
                    //adding new order to list 
                    p->getOrdersList()->add(newOrder);   
                }
            }
            break;

        case OrderType::Negotiate:
            cout << "Issuing diplomacy order..." << endl;
            for (Player* enemyPlayer : target) {
                if (enemyPlayer != p) {
                    newOrder = new Negotiate(p, enemyPlayer);
                    //adding new order to list 
                    p->getOrdersList()->add(newOrder);   
                }
            }
            break;
        default:
            break;
    } 
}

// To defend function
vector<Territory*> BenevolentPlayerStrategy::toDefend() {
    std::vector<Territory*> playerTerritories = p->getTerritories();
    std::vector<Territory*> weakestTerritories;

    int minArmies = INT_MAX; // Start with a high number for comparison

    // Find the minimum number of armies among the player's territories
    for (Territory* territory : playerTerritories) {
        if (territory->getNbArmies() < minArmies) {
            minArmies = territory->getNbArmies();
        }
    }

    // Find territories with the minimum number of armies (weakest territories)
    for (Territory* territory : playerTerritories) {
        if (territory->getNbArmies() == minArmies) {
            weakestTerritories.push_back(territory);
        }
    }

    return weakestTerritories;
}

// To attack function
vector<Territory*> BenevolentPlayerStrategy::toAttack() {
    vector<Territory*> notAttacking;
    cout<<"Benevolent player is returning an empty list for attacking...";
    return notAttacking;
}


string BenevolentPlayerStrategy::getType() {
    return "benevolent";
}

//--------------------------------------------------------------------------------------------------
// End of Benevolent Player Strategy
//--------------------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
// Neutral Player Strategy
//--------------------------------------------------------------------------------------------------

NeutralPlayerStrategy::NeutralPlayerStrategy(Player *player) : PlayerStrategy(player) {}

NeutralPlayerStrategy::NeutralPlayerStrategy(const NeutralPlayerStrategy &copied): PlayerStrategy(copied) {}

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

CheaterPlayerStrategy::CheaterPlayerStrategy(Player* player) : PlayerStrategy(player) {}

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

StrategyType getStrategyType(string str) {
    if (str == "Human" || str == "human")
        return StrategyType::Human;
    else if (str == "Aggressive" || str == "aggressive")
        return StrategyType::Aggressive;
    else if (str == "Benevolent" || str == "benevolent")
        return StrategyType::Benevolent;
    else if (str == "Neutral" || str == "neutral")
        return StrategyType::Neutral;
    else if (str == "Cheater" || str == "cheater")
        return StrategyType::Cheater;
    return StrategyType::None;
}

PlayerStrategy* loadStrategy(Player* player, StrategyType strategyType) {
    if (strategyType == StrategyType::Human)
        return new HumanPlayerStrategy(player);
    else if (strategyType == StrategyType::Aggressive)
        return new AggressivePlayerStrategy(player);
    else if (strategyType == StrategyType::Benevolent)
        return new BenevolentPlayerStrategy(player);
    else if (strategyType == StrategyType::Neutral)
        return new NeutralPlayerStrategy(player);
    else if (strategyType == StrategyType::Cheater)
        return new CheaterPlayerStrategy(player);
    return nullptr; // StrategyType::None, shouldn't happen if validation was done properly.
}