#pragma once
#include <iostream>
#include <vector>

//----Order and all subclasses of Order----//
class Order
{
        public: 
        virtual Order* copy() const = 0;
        virtual void execute() = 0; 
        virtual bool validate() = 0; 
        virtual void orderExecuted() = 0;
};

class Deploy : public Order
{
    public:
        Deploy();
        Deploy* copy() const  override;
        void execute() override;
        bool validate() override;
        void orderExecuted() override;

    private:
        int numArmyUnits; 
};

class Advance : public Order
{
    public:
        Advance();
        Advance* copy() const  override;
        void execute() override;
        bool validate() override;
        void orderExecuted() override; 
    
    private:
        int numArmyUnits; 
};

class Bomb : public Order
{
    public:
        Bomb();
        Bomb* copy() const  override; 
        void execute() override;
        bool validate() override;
        void orderExecuted() override; 
};

class Blockade : public Order
{
    public:
        Blockade();
        Blockade* copy() const  override; 
        void execute() override;
        bool validate() override;
        void orderExecuted() override; 
};

class Airlift : public Order
{
    public:
        Airlift();
        Airlift* copy() const  override; 
        void execute() override;
        bool validate() override;
        void orderExecuted() override; 
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
        void orderExecuted() override; 
};

//----End of Order and all subclasses of Order----//

//----OrderList-----//

class OrderList
{
    public:
        OrderList();
        void add(Order* O);
        int getSize();
        void remove(int i);
        void move(int initial, int final);

    private: 
        std::vector<Order*> listOfOrders;
}; 




