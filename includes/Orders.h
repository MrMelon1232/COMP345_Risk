#pragma once
#include <vector>
#include <iostream>
#include "Player.h"
#include "LoggingObserver.h"
using namespace std;
class Player;
class Territory;
class ILoggable;
class Subject;
//----Order and all subclasses of Order----//
class Order: public ILoggable, public Subject
{
    public:
        Order();
        //virtual functions must be implemented by all subclasses (They will be overriden)  
        virtual Order* copy() const = 0;
        virtual void execute() = 0; 
        virtual bool validate() = 0; 
        virtual ostream& displayOrder(ostream& myOrder) const = 0;
        virtual ~Order() = default;
        virtual string getName() const = 0; // Getter for name

    private:
        friend ostream& operator<< (ostream& myOrder, Order& O);
};

//Deploy is a subclass of Order
class Deploy : public Order
{
    public:
        Deploy();
        Deploy(int armyUnits, Player* player, Territory* target);
        ~Deploy() override;
        Deploy* copy() const  override;
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Deploy& operator=(const Deploy& other);
        string stringToLog() const override;        
        string getName() const override { return "Deploy"; }

    private:
        int armyUnits; 
        Player* player; 
        Territory* target; 
};

//Advance is a subclass of Order
class Advance : public Order
{
    public:
        Advance();
        Advance(int armyUnits,Player* player,Territory* target, Territory* source);
        ~Advance() override;
        Advance* copy() const  override;
        void execute() override;
        bool validate() override; 
        ostream& displayOrder(ostream& myOrder) const override;
        Advance& operator=(const Advance& other);
        void simulateAttack(); 
        string stringToLog() const override;
        string getName() const override { return "Advance"; }
    
    private:
        int armyUnits;
        Player* player;
        Territory* target; 
        Territory* source;
};

//Bomb is a subclass of Order
class Bomb : public Order
{
    public:
        Bomb();
        Bomb(Player* player,Territory* target); 
        ~Bomb() override;
        Bomb* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Bomb& operator=(const Bomb& other);
        string stringToLog() const override;
        string getName() const override { return "Bomb"; }
    
    private:
        Player* player;
        Territory* target; 
};

//Blockade is a subclass of Order
class Blockade : public Order
{
    public:
        Blockade();
        Blockade(Player* player, Territory* target);
        ~Blockade() override;
        Blockade* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Blockade& operator=(const Blockade& other);
        string stringToLog() const override;
        string getName() const override { return "Blockade"; }
    
    private:
        Player* player;
        Territory* target; 
};

//Airlift is a subclass of Order
class Airlift : public Order
{
    public:
        Airlift();
        Airlift(Player* player,Territory* target,Territory* source, int movingUnits); 
        ~Airlift() override;
        Airlift* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Airlift& operator=(const Airlift& other);
        string stringToLog() const override;
        string getName() const override { return "Airlift"; }
    
    private:
        Player* player;
        Territory* target; 
        Territory* source;
        int movingUnits; 
};

//Negotiate is a subclass of Order
class Negotiate : public Order
{
    public:
        Negotiate();
        Negotiate(Player* player,Player* enemy);
        ~Negotiate() override;
        Negotiate* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Negotiate& operator=(const Negotiate& other);
        string stringToLog() const override;
        string getName() const override { return "Negotiate"; }
    
    private:
        Player* player;
        Player* enemy; 
};

// Enum for order types
enum class OrderType {
    Deploy,
    Advance,
    Bomb,
    Blockade,
    Airlift,
    Negotiate 
};

//----End of Order and all subclasses of Order----//

//----OrderList-----//

class OrdersList: public ILoggable, public Subject
{
    public:
        OrdersList();
        void add(Order* O);
        int getSize();
        void remove(int i);
        void move(int initial, int final);
        ostream& displayOrderList(ostream& myOrderList);
        OrdersList& operator=(const OrdersList& other);
        ~OrdersList(); 
        Order* getOrder(int i);
        string stringToLog() const override;
                
    private: 
        std::vector<Order*> listOfOrders;
        friend ostream& operator<<(ostream& myOrderList, OrdersList& Olist);
}; 

//test methods
void testOrdersList();
void testOrderExecution();