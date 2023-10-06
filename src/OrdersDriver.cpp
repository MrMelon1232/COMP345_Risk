#include "Orders.h"
#include "OrdersDriver.h"
#include <iostream>
using namespace std; 

void testOrdersList()
{
    //Welcome message
    cout<<'\n';
    cout << "--------Testing adding to OrderList----------\n";

    //Creating one of each order
    Deploy* deploy = new Deploy();
    Advance* advance = new Advance();
    Bomb* bomb = new Bomb();
    Blockade* blockade = new Blockade();
    Airlift* airlift = new Airlift();
    Negotiate* negotiate = new Negotiate(); 

    //Creating an OrderList
    OrdersList* list = new OrdersList();

    //Adding each order in the list
    list->add(deploy);
    list->add(advance);
    list->add(bomb);
    list->add(blockade);
    list->add(airlift);
    list->add(negotiate);
    //list size is 6 after adding

    //Display the current list after initial addition
    cout<< *list<< '\n';

    //Removing element in first index of the list
    cout << "--------Testing removing an order from the OrderList (index 1)----------\n";
    list->remove(1);
    cout<< *list<< '\n';

    //Removing element in first index of the list
    cout << "--------Testing exception handling: removing an order from the OrderList (out of bound)----------\n";
    list->remove(10);
    cout<< *list<< '\n'; 

    //Trying to move Order to same index
    cout << "--------Testing exception handling: moving an order to the same position within the list (index 1 and index 1)----------\n";
    list->move(1,1);
    cout<< *list<< '\n';

    //Trying to input invalid indices
    cout << "--------Testing exception handling: moving an order with invalid indices (index -1 and index -2)----------\n";
    list->move(-1,-2);
    cout<< *list<< '\n'; 

    //Swapping between element in first index and elemnt in third index
    cout << "--------Testing moving an order from the OrderList (index 1 and index 3)----------\n";
    list->move(1,3);
    cout<< *list<< '\n'; 

    //Exit message
    cout << "--------Testing completed for OrderList Class!----------\n";
}