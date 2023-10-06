#pragma once
#include <vector>
#include <iostream>
using namespace std;

//----Order and all subclasses of Order----//
class Order
{
    public:
        Order();
        //virtual functions must be implemented by all subclasses (They will be overriden)  
        virtual Order* copy() const = 0;
        virtual void execute() = 0; 
        virtual bool validate() = 0; 
        virtual ostream& displayOrder(ostream& myOrder) const = 0;
        virtual ~Order() = default;

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





