#include "Orders.h"
#include <Vector>
#include <iostream>
using namespace std;

class OrderList
{
    private: 
        std::vector<Order*> ordersList;

    public:
    //Default Constructor
        OrderList()
        {
      
        }      

        OrderList(OrderList &O)
        {
                
        }

};


//-------Below are function definitions for classes that inherit from Order---------//

//Function Definition for Deploy
Deploy::Deploy()
{

}

Deploy* Deploy::copy() const
{
    return new Deploy(*this);
}

void Deploy::execute()
{

}

bool Deploy::validate()
{

}

void Deploy::orderExecuted()
{
    cout << "DEPLOY order has been executed \n";
}

//Function Definition for Advance
Advance::Advance()
{

}

Advance* Advance::copy() const
{
    return new Advance(*this);
}

void Advance::execute()
{

}

bool Advance::validate()
{

}

void Advance::orderExecuted()
{
    cout << "ADVANCE order has been executed \n";
}

//Function Definition for Bomb
Bomb::Bomb()
{

}

Bomb* Bomb::copy() const
{
    return new Bomb(*this);
}

void Bomb::execute()
{

}

bool Bomb::validate()
{

}

void Bomb::orderExecuted()
{
    cout << "BOMB order has been executed \n";
}

//Function Definition for Blockade
Blockade::Blockade()
{

}

Blockade* Blockade::copy() const
{
    return new Blockade(*this);
}

void Blockade::execute()
{

}

bool Blockade::validate()
{

}

void Blockade::orderExecuted()
{
    cout << "BLOCKADE order has been executed \n";
}

//Function Definition for Airlift
Airlift::Airlift()
{

}

Airlift* Airlift::copy() const
{
    return new Airlift(*this);
}

void Airlift::execute()
{

}

bool Airlift::validate()
{

}

void Airlift::orderExecuted()
{
    cout << "AIRLIFT order has been executed \n";
}

//Function Definition for Negotiate
Negotiate::Negotiate()
{

}

Negotiate* Negotiate::copy() const
{
    return new Negotiate(*this);
}

void Negotiate::execute()
{

}

bool Negotiate::validate()
{

}

void Negotiate::orderExecuted()
{
    cout << "NEGOTIATE order has been executed \n";
}






    
         
    