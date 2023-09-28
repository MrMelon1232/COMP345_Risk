#pragma once
#include <vector>
#include <iostream>
using namespace std;

//----Order and all subclasses of Order----//
class Order
{
    public:
        Order();   
        virtual Order* copy() const = 0;
        virtual void execute() = 0; 
        virtual bool validate() = 0; 
        virtual ostream& displayOrder(ostream& myOrder) const = 0;

    private:
        friend ostream& operator<< (ostream& myOrder, Order& O);
};

class Deploy : public Order
{
    public:
        Deploy();
        Deploy* copy() const  override;
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Deploy& operator=(const Deploy& other);

};

class Advance : public Order
{
    public:
        Advance();
        Advance* copy() const  override;
        void execute() override;
        bool validate() override; 
        ostream& displayOrder(ostream& myOrder) const override;
        Advance& operator=(const Advance& other);

};

class Bomb : public Order
{
    public:
        Bomb();
        Bomb* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Bomb& operator=(const Bomb& other);
};

class Blockade : public Order
{
    public:
        Blockade();
        Blockade* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Blockade& operator=(const Blockade& other);
};

class Airlift : public Order
{
    public:
        Airlift();
        Airlift* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Airlift& operator=(const Airlift& other);
    private:
        int numArmyUnits;
};

class Negotiate : public Order
{
    public:
        Negotiate();
        Negotiate* copy() const  override; 
        void execute() override;
        bool validate() override;
        ostream& displayOrder(ostream& myOrder) const override;
        Negotiate& operator=(const Negotiate& other);
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

class OrdersList
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

    private: 
        std::vector<Order*> listOfOrders;
        friend ostream& operator<<(ostream& myOrderList, OrdersList& Olist);
}; 




