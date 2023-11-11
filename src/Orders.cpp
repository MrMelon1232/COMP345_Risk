#include "Orders.h"
#include "Player.h"
#include "Map.h"
#include "Cards.h"
#include <Vector>
#include <iostream>
using namespace std;


//Default constructor for Order (Allows creation of an object even if it is abstract)
Order::Order()
{

}

//Outputting an object of type Order will behave as follows: 
ostream& operator<< (ostream& myOrder, Order& O)
{
    return O.displayOrder(myOrder); 
}

//-------Below are function definitions for classes that inherit from Order---------//


//Default constructor
Deploy:: Deploy() : armyUnits(0)
{
    this -> player = nullptr;
    this -> target = nullptr;
};

//Non default constructor
Deploy::Deploy(int armyUnits, Player* player, Territory* target)
{
    this -> armyUnits = armyUnits;
    this -> player = player;
    this -> target = target;
}    

//Default Destructor
Deploy::~Deploy()
{

}

//Copy constructor
Deploy* Deploy::copy() const
{
    return new Deploy(*this);
}

//Boolean function which returns true if the order is valid
bool Deploy::validate()
{
    if (target->getOwnerID() != player->getPlayerID())
    {
        cout << "Cannot deploy on a territory that is not owned by the player!\n";
        return false;
    }

    //We will always deploy 5 units as reinforcement. This number is arbritary and subject to change. 
    //We will not deploy if we dont have the minimum amount of units
    if(player->getReinforcementPool() < armyUnits)
    {
        cout << "Cannot deploy, player does not have sufficient units!\n";
        return false;
    }
    cout << "Deploy has been validated!\n";
    return true; 
}

//Execute allows the player to deploy, for this function to proceed, it must be validated
void Deploy::execute()
{
    if(validate() == true)
    {
        cout<<"Executing Deploy\n";
        target->setNbArmies(armyUnits + target->getNbArmies());
        player->setReinforcementPool(player->getReinforcementPool() - armyUnits); 
        cout<<armyUnits<<" units have been deployed to "<<target->GetName()<<".\n";
        cout<<target->GetName()<<" now has "<<target->getNbArmies()<<" units.\n";
        notify(*this);

    }
}

Deploy& Deploy::operator=(const Deploy &other)
{
    return *this;
}

//Format for outputting Deploy
ostream& Deploy::displayOrder(ostream &myOrder) const 
{
    myOrder << "Deploy";
    return myOrder;
}


//For now all other suborders follow the same logic as the deploy suborder
//Therefore comments are not provided as only the name of the suborder is changed

Advance::Advance()
{

}

Advance::Advance(int armyUnits, Player* player, Territory* target, Territory* source)
{
    this->armyUnits = armyUnits; 
    this->player = player; 
    this->target = target; 
    this->source = source; 
}

Advance::~Advance()
{

}

Advance* Advance::copy() const
{
    return new Advance(*this);
}

bool Advance::validate()
{
    if (source->getOwnerID() != player->getPlayerID())
    {
        cout << "Cannot advance from a territory that a player does not own!\n";
        cout << "Validation failed!\n";
        return false;
    }

    if(!(target->isAdjacent(source)))
    {
        cout << "Cannot advance to a territory which is not adjacent!\n";
        cout << "Validation failed!\n";
        return false;
    }

    if(player->isAllyPresent(target->getOwnerID()))
    {
        cout << "Cannot advance to a territory owned by a player which is an ally until the end of this turn!\n";
        cout << "Validation failed!\n";
        return false; 
    }

    //If the source and the target belong to the same player, execute will not trigger an attack simulation
    if(source->getOwnerID() == target->getOwnerID())
    {
        cout << "Moving units from source to target...\n";
        int unitsToBeMoved = source->getNbArmies(); 
        target->setNbArmies(unitsToBeMoved); 
        source->setNbArmies(0); 
        return false; 
    }
    cout << "Advance has been validated!\n";
    return true;
}

void Advance::simulateAttack()
{
    cout <<"Attack simulation has begun!\n";

    //We calculate the amount of units each player will kill
    int playerKills = (source->getNbArmies())*0.6; 
    int enemyKills = (target->getNbArmies())*0.7; 

    //We make adjustments to the number of units on both sides
    source->setNbArmies(source->getNbArmies() - enemyKills);
    target->setNbArmies(target->getNbArmies() - playerKills); 

    int remainingPlayerUnits = source->getNbArmies();
    int remainingEnemyUnits = target->getNbArmies(); 

    //Player has taken the target and has moved units to there
    if(remainingPlayerUnits > 0 && remainingEnemyUnits <=0)
    {
        target->setNbArmies(remainingPlayerUnits); 
        source->setNbArmies(0);
        target->setOwnerID(source->getOwnerID());
        Card* newCard = new Card(); 
        player->addCardToHand(newCard); 
        cout <<"Player has captured territory and has moved units up.\n";
        cout <<"Player has earned a card for conquering a territory.\n";
    }
    cout <<"Attack simulation has ended!\n";
}

void Advance::execute()
{
    if(validate() == true)
    {
        cout <<"Executing Advance\n";
        simulateAttack();
        notify(*this);
    }
}

Advance& Advance::operator=(const Advance &other)
{
    return *this;
}

ostream& Advance::displayOrder(ostream &myOrder) const 
{
    myOrder << "Advance";
    return myOrder;
}


//Function Definition for Bomb
Bomb::Bomb()
{

}

Bomb::Bomb(Player* player, Territory* target)
{
    this->player=player; 
    this->target=target; 
}

Bomb::~Bomb()
{

}

Bomb* Bomb::copy() const
{
    return new Bomb(*this);
}

void Bomb::execute()
{
    if(validate() == true)
    {
        cout<<"Executing Bomb\n";
        target->setNbArmies(0.5*(target->getNbArmies()));
        cout<<"The bomb has dropped on " <<target->GetName()<<".\n";
        cout<<target->GetName()<<" now has "<<target->getNbArmies()<<" units remaining.\n";
        notify(*this);
    }
}

bool Bomb::validate()
{
    if (target->getOwnerID() == player->getPlayerID())
    {
        cout << "Cannot bomb, this territory belongs to the player trying to bomb...\n";
        return false; 
    }

    //Cannot bomb a territory owned by a player which is an ally until the end of the turn
    if(player->isAllyPresent(target->getOwnerID()))
    {
        cout << "Cannot bomb ally!\n";
        cout << "Validation failed!\n";
        return false; 
    }

    //To bomb the target, the player must have at least one territory which is adjacent to the target.
    if(player->isAnyAdjacent(target))
    {
        cout << "Validation successful, ready to bomb!\n";
        return true;
    }
    
    cout << "Territory is not adjacent, cannot bomb; validatation failed!\n";
    return false; 
}

Bomb& Bomb::operator=(const Bomb &other)
{
    return *this;
}

ostream& Bomb::displayOrder(ostream &myOrder) const 
{
    myOrder << "Bomb";
    return myOrder;
}


//Function Definition for Blockade
Blockade::Blockade()
{

}

Blockade::Blockade(Player* player, Territory* target)
{
    this->player = player;
    this->target = target; 
}

//The game requires that one neutral player exists. In our implementation, this player always exists.
static auto* neutralPlayer = new Player();

Blockade::~Blockade()
{

}

Blockade* Blockade::copy() const
{
    return new Blockade(*this);
}

bool Blockade::validate()
{
    if(target->getOwnerID() != player->getPlayerID())
    {
        cout << "Cannot barricade a territory which the player does not own!\n";
        cout << "Validation failed!\n";
        return false;
    }
    cout << "Validation complete, proceeding to blockade!\n";
    return true; 
}

void Blockade::execute()
{
    if(validate() == true)
    {
        cout<<"Executing Blockade\n";

        //doubling units
        target->setNbArmies((target->getNbArmies())*2);

        //transferring ownership to neutral player
        target->setOwnerID(neutralPlayer->getPlayerID());
        neutralPlayer->addTerritory(target); 

        cout << "Units have been transferred to the neutral player\n";
        cout << "Displaying neutral player below...\n";
        cout<< *neutralPlayer; 
        cout << "\n";
        notify(*this);
    }
}

Blockade& Blockade::operator=(const Blockade &other)
{
    return *this; 
}
 
ostream& Blockade::displayOrder(ostream &myOrder) const 
{
    myOrder << "Blockade";
    return myOrder;
}


//Function Definition for Airlift
Airlift::Airlift()
{

}

Airlift::Airlift(Player* player,Territory* target,Territory* source, int movingUnits)
{
    this->player = player; 
    this->target = target;
    this->source = source;
    this->movingUnits = movingUnits;  
}

Airlift::~Airlift()
{

}

Airlift* Airlift::copy() const
{
    return new Airlift(*this);
}

bool Airlift::validate()
{
    //Both territories involved must belong to the same players
    if(target->getOwnerID() != player->getPlayerID() || source->getOwnerID() != player->getPlayerID() )
    {
        cout << "Cannot airlift, player does not have the right to execute this task.!\n";
        return false;
    }

    if(source->getNbArmies() < movingUnits)
    {
        cout << "Cannot airlift, not enough units present to be airlifted...\n";
        return false;
    }
    cout << "Validated, ready to airlift!\n";
    return true; 
}

void Airlift::execute()
{
    if(validate() == true)
    {
        cout<<"After Airlift is completed: \n";
        cout<<source->GetName()<<" has "<<source->getNbArmies()<<" units.\n";
        cout<<target->GetName()<<" has "<<target->getNbArmies()<<" units.\n\n";
        cout<<"Executing Airlift\n";

        //Moving from source to target
        source->setNbArmies(source->getNbArmies() - movingUnits); 
        target->setNbArmies(source->getNbArmies() + movingUnits);

        cout<<"After Airlift is completed: \n";
        cout<<source->GetName()<<" has "<<source->getNbArmies()<<" units.\n";
        cout<<target->GetName()<<" has "<<target->getNbArmies()<<" units.\n\n";
        notify(*this);
    }
}


Airlift& Airlift::operator=(const Airlift &other)
{
    return *this;
}

ostream& Airlift::displayOrder(ostream &myOrder) const 
{
    myOrder << "Airlift";
    return myOrder;
}


//Function Definition for Negotiate
Negotiate::Negotiate()
{
}

Negotiate::Negotiate(Player* player,Player* enemy)
{
    this->player = player;
    this->enemy = enemy;
}

Negotiate::~Negotiate()
{

}

Negotiate* Negotiate::copy() const
{
    return new Negotiate(*this);
}

bool Negotiate::validate()
{
    if(player->getPlayerID() == enemy->getPlayerID())
    {
        cout << "Player cannot negotiate with self, validation failed!\n";
        return false;
    }
    cout << "Negotiation validated!";
    return true;
}

void Negotiate::execute()
{
    if(validate() == true)
    {
        cout<<"Executing Negotiate\n";

        //both players are allies of eachother
        player->addAlly(enemy->getPlayerID());
        enemy->addAlly(player->getPlayerID()); 

        cout<<"Both players can no longer attack each other until end of turn now\n";
        notify(*this);
    }
}

Negotiate& Negotiate::operator=(const Negotiate &other)
{
    return *this;
}

ostream& Negotiate::displayOrder(ostream &myOrder) const 
{
    myOrder << "Negotiate";
    return myOrder;
}

//-------End of function definitions for classes that inherit from Order---------//

//-------OrderList and respective methods definition---------//

/*
The OrdersList class makes a list of Orders and the corresponding subtypes. 
Notably it has the method move() and the method remove().
*/

//Default constructor
OrdersList::OrdersList()
{

}

//Adds an object of type "Order" to the list sequentially
void OrdersList::add(Order* O)
{
    listOfOrders.push_back(O); 
    notify(*this);
}

//Returns the size of the list
int OrdersList::getSize()
{
    return listOfOrders.size(); 
}

//Removes an Order at an index "i"
void OrdersList::remove(int i)
{
    int listSize = this -> getSize();

    if(listSize == 0)
    {
        cout << "ERROR: The list for Orders is empty. We cannot remove the order!\n";
        return; 
    } 

    if ((i < 0) || (i >= this -> getSize()))
    {
        cout << "ERROR: Index out of bound error, order cannot be removed!\n";
        return; 
    }

    //using an iterator to reach the index 
    auto myIterator = listOfOrders.begin(); 
    advance(myIterator, i);

    //deleting the content of the pointer
    delete *myIterator;

    //erasing the pointer
    listOfOrders.erase(myIterator); 
}

//Method which allows to move an order from an index (initial) to another index (final)
void OrdersList::move(int initial,int final)
{
    int listSize = this -> getSize();

    if(initial < 0 || final < 0 || initial > listSize -1 || final > listSize -1)
    {
        cout << "Invalid indices provided, cannot move order!\n";
        return;
    }

    if(initial == final)
    {
        cout << "Cannot move order, it is already in destination!\n";
        return;
    }

    Order* temp = listOfOrders.at(initial);
    listOfOrders.at(initial) = listOfOrders.at(final);
    listOfOrders.at(final) = temp;
}

//Format of outputting an OrdersList
ostream& OrdersList::displayOrderList(ostream& myOrderList)
{
    //we will display the list numbered and we will increment the variable number after each iteration
    int number = 1; 

    myOrderList << "Orders List: \n";

    //for loop to go over every order in the list
    for (Order* orderObject: listOfOrders) 
    {
        myOrderList << number++ << ". " << *orderObject << '\n';
    }
    
    return myOrderList;
}

//Passing a list in the output stream will call "displayOrderList(ostream& myOrderList)"" 
ostream& operator << (ostream& myOrderList, OrdersList& Olist)
{
    return Olist.displayOrderList(myOrderList);
}

OrdersList& OrdersList::operator=(const OrdersList &other)
{
    return *this;
} 

//Destructor (NOT DEFAULT)
OrdersList::~OrdersList()
{
    //Clearing memory for each Order 'o' in the list
    for (int o = 0; o < (this -> getSize()); o++)
        delete listOfOrders[o];
}

Order* OrdersList::getOrder(int i)
{
    return listOfOrders.at(i);
}

// Part 2: Logging
string Deploy::stringToLog() const {
    return "Deploy order: " + to_string(armyUnits) + " units have been deployed to " + target->GetName();
}

string Advance::stringToLog() const {
    return "Advance order: target: " + target->GetName() + " is now owned by player " +
                         std::to_string(target->getOwnerID()) + " and has " +
                         std::to_string(target->getNbArmies()) + " units.";
}

string Bomb::stringToLog() const {
    return "Bomb order: " + target->GetName() + " now has " + std::to_string(target->getNbArmies()) + " units remaining after bomb has been dropped on.";
}

string Blockade::stringToLog() const {
    return "Blockade order: neutral player: " + std::to_string(neutralPlayer->getPlayerID()) + " has received " + std::to_string(target->getNbArmies());
}

string Airlift::stringToLog() const {
    return "Airlift order: " + target->GetName() + " has " + std::to_string(target->getNbArmies()) +
       " units after " + std::to_string(movingUnits) +
       " units have been moved from " + std::to_string(source->getNbArmies());
}

string Negotiate::stringToLog() const {
    return "Negotiate order: player " + std::to_string(player->getPlayerID()) +
       " is now allies with player " + std::to_string(enemy->getPlayerID());
}

string OrdersList::stringToLog() const {
    return "Add function: Order " + listOfOrders.back()->getName() + " has been added to the player's list of orders.";
}

    
         
    