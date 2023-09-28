#include "Orders.h"
#include <Vector>
#include <iostream>
using namespace std;


//-------Below are function definitions for classes that inherit from Order---------//


 Order::Order()
 {
 }

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
    if(validate() == true)
    {
        cout<<"Executing Deploy";
    }
}

bool Deploy::validate()
{
    return true; 
}

Deploy& Deploy::operator=(const Deploy &other)
{
    return *this;
}

ostream& Deploy::displayOrder(ostream &myOrder) const 
{
    myOrder << "Deploy";
    return myOrder;
}

ostream& operator<< (ostream& myOrder, Order& O)
{
    return O.displayOrder(myOrder); 
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
    if(validate() == true)
    {
        cout <<"Executing Advance";
    }
}

bool Advance::validate()
{
    return true;
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

Bomb* Bomb::copy() const
{
    return new Bomb(*this);
}

void Bomb::execute()
{
    if(validate() == true)
    {
        cout<<"Executing Bomb";
    }
}

bool Bomb::validate()
{
    return true; 
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

Blockade* Blockade::copy() const
{
    return new Blockade(*this);
}

void Blockade::execute()
{
    if(validate() == true)
    {
        cout<<"Executing Blockade";
    }
}

bool Blockade::validate()
{
    return true; 
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

Airlift* Airlift::copy() const
{
    return new Airlift(*this);
}

void Airlift::execute()
{
    if(validate() == true)
    {
        cout<<"Executing Airlift";
    }
}

bool Airlift::validate()
{
    return true; 
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

Negotiate* Negotiate::copy() const
{
    return new Negotiate(*this);
}

void Negotiate::execute()
{
    if(validate() == true)
    {
        cout<<"Executing Negotiate";
    }
}

bool Negotiate::validate()
{
    return true;
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


//Default constructor
OrdersList::OrdersList()
{

}

//Adds an object of type "Order" to the list
void OrdersList::add(Order* O)
{
    listOfOrders.push_back(O); 
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

    if(listSize = 0)
    {
        cout << "The list for Orders is empty. We cannot remove the order!"<<'\n';
        return; 
    } 

    if ((i < 0) || (i >= this -> getSize()))
    {
        cout << "Index out of bound error, order cannot be removed!"<<'\n';
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

void OrdersList::move(int initial,int final)
{
    int listSize = this -> getSize();

    if(initial < 0 || final < 0 || initial > listSize -1 || final > listSize -1)
    {
        cout << "Invalid indices provided, cannot move order!";
        return;
    }

    if(initial == final)
    {
        cout << "Cannot move order, it is already in destination!";
        return;
    }

    Order* temp = listOfOrders.at(initial);
    listOfOrders.at(initial) = listOfOrders.at(final);
    listOfOrders.at(final) = temp;
}

ostream& OrdersList::displayOrderList(ostream& myOrderList)
{
    //we will display the list numbered and we will increment the variable number after each iteration
    int number = 1; 

    myOrderList << "Orders List: " << '\n';

    //for loop to go over every order in the list
    for (Order* orderObject: listOfOrders) 
    {
        myOrderList << number++ << ". " << *orderObject << '\n';
    }
    
    return myOrderList;
}

ostream& operator << (ostream& myOrderList, OrdersList& Olist)
{
    return Olist.displayOrderList(myOrderList);
}

OrdersList& OrdersList::operator=(const OrdersList &other)
{
    return *this;
} 

OrdersList::~OrdersList()
{

}


    
         
    