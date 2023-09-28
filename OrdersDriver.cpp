#include "Orders.h"
#include "OrdersDriver.h"
#include <iostream>
using namespace std; 

void testOrdersList()
{
    cout << "--------Testing adding to OrderList----------"<< '\n';

    //Creating one of each order
    Deploy* deploy = new Deploy();
    Advance* advance = new Advance();
    Bomb* bomb = new Bomb();
    Blockade* bloackade = new Blockade();
    Airlift* airlift = new Airlift();
    Negotiate* negotiate = new Negotiate(); 

    //Creating an OrderList
    OrdersList* list = new OrdersList();

    //Adding each order in the list
    list->add(deploy);
    list->add(advance);
    list->add(bomb);
    list->add(bloackade);
    list->add(airlift);
    list->add(negotiate);


    //Display the current list after initial addition
    cout<< *list<< '\n';

    //Removing element in first index of the list
    cout << "--------Testing removing an order from the OrderList (index 1)----------"<< '\n';
    list->remove(1);
    cout<< *list<< '\n';

    //Removing element in first index of the list
    cout << "--------Testing exception handling when removing an order from the OrderList (out of bound)----------"<< '\n';
    list->remove(10);
    cout<< *list<< '\n';  

    //Swapping between element in first index and elemnt in third index
    cout << "--------Testing moving an order from the OrderList (index 1 and index 3)----------"<< '\n';
    list->move(1,3);
    cout<< *list<< '\n'; 

}