#pragma once
#include <vector>
#include <iostream>
using namespace std;
#include "LoggingObserver.h"

//----Order and all subclasses of Order----//
class Order : public ILoggable, public Subject
{
    public:
        Order();
        //virtual functions must be implemented by all subclasses (They will be overriden)  
        virtual Order* copy() const = 0;
        virtual void execute() = 0; 
        virtual bool validate() = 0; 
        virtual ostream& displayOrder(ostream& myOrder) const = 0;
        virtual ~Order() = default;
        virtual string logMessage() const = 0;

    private:
        friend ostream& operator<< (ostream& myOrder, const Order& O);
};

//Deploy is a subclass of Order
class Deploy : public Order
{
    public:
        Deploy();
        ~Deploy() override;
        Deploy* copy() const  override;
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Deploy& operator=(const Deploy& other);
        // Part 2 Logging
        string stringtoLog() override;
};

//Advance is a subclass of Order
class Advance : public Order
{
    public:
        Advance();
        ~Advance() override;
        Advance* copy() const  override;
        void execute() override;
        bool validate() override; 
        ostream& displayOrder(ostream& myOrder) const override;
        Advance& operator=(const Advance& other);
        // Part 2 Logging
        string stringtoLog() override;
};

//Bomb is a subclass of Order
class Bomb : public Order
{
    public:
        Bomb();
        ~Bomb() override;
        Bomb* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Bomb& operator=(const Bomb& other);
        // Part 2 Logging
        string stringtoLog() override;
};

//Blockade is a subclass of Order
class Blockade : public Order
{
    public:
        Blockade();
        ~Blockade() override;
        Blockade* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Blockade& operator=(const Blockade& other);
        // Part 2 Logging
        string stringtoLog() override;
};

//Airlift is a subclass of Order
class Airlift : public Order
{
    public:
        Airlift();
        ~Airlift() override;
        Airlift* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Airlift& operator=(const Airlift& other);
        // Part 2 Logging
        string stringtoLog() override;
};

//Negotiate is a subclass of Order
class Negotiate : public Order
{
    public:
        Negotiate();
        ~Negotiate() override;
        Negotiate* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Negotiate& operator=(const Negotiate& other);
        // Part 2 Logging
        string stringtoLog() override;
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

        // Part 2 Logging
        string stringToLog() override;

    private: 
        std::vector<Order*> listOfOrders;
        friend ostream& operator<<(ostream& myOrderList, OrdersList& Olist);
}; 





