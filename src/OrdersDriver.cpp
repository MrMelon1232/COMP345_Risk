
#include "Orders.h"
#include "Map.h"
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
    cout << "\n";
}

//--------------------------------------------------------------------------------------------------------------------------

void testOrderExecution()
{
    cout << "------------Welcome to order execution testing!--------------\n";

    cout << "Creating two players...\n";
    Player* playerONE = new Player(); 
    Player* playerTWO = new Player();

    int playerONE_ID = playerONE->getPlayerID(); 
    int playerTWO_ID = playerTWO->getPlayerID();

    cout << "Player 1 owns Germany and Spain.\n";
    //Player 1 owns Germany and Spain
    Territory* germany = new Territory("Germany", "Europe", playerONE_ID);
    Territory* spain = new Territory("Spain", "Europe", playerONE_ID);
    playerONE->addTerritory(germany); 
    playerONE->addTerritory(spain); 

    //adding units
    germany->setNbArmies(5);
    spain->setNbArmies(3); 

    cout << "Player 2 owns France.\n";
    //Player 2 owns France
    Territory* france = new Territory("France", "Europe", playerTWO_ID);
    playerTWO->addTerritory(france); 

    //adding units
    france->setNbArmies(7);

    cout << "Germany and France are adjacent to each other.\n";
    cout << "Spain and France are adjacent to each other.\n";

    //Germany is adjacent to France 
    germany->AddAdjacentTerritory("France");
    france->AddAdjacentTerritory("Germany");

    //Spain is adjacent to France
    spain->AddAdjacentTerritory("France");
    france->AddAdjacentTerritory("Spain"); 

    cout << "Displaying both players...\n";
    cout<< *playerONE; 
    cout<< *playerTWO; 

    cout << "-----------Testing Deploy-------------\n";
    Deploy* deploy = new Deploy(2,playerONE,germany);
    deploy->execute();
    cout << "-----------END of Testing Deploy-------------\n\n";

    cout << "-----------Testing Advance-------------\n";
    Advance* advance = new Advance(2,playerONE,france,germany);
    advance->execute(); 
    cout << "Displaying both players...\n";
    cout<< *playerONE;
    cout<< *playerTWO;
    cout << "-----------END of Testing Advance-------------\n\n";

    cout << "-----------Testing Bomb-------------\n";
    Bomb* bomb = new Bomb(playerTWO, germany);
    bomb->execute(); 
    cout << "-----------End of Testing Bomb-------------\n\n";

    cout << "-----------Testing Airlift-------------\n";
    Airlift* airlift = new Airlift(playerONE, germany, spain, 2); 
    airlift->execute(); 
    cout << "-----------END of Testing Airlift-------------\n\n";

    cout << "-----------Testing Negotiate-------------\n";
    Negotiate* negotiate = new Negotiate(playerONE,playerTWO);
    negotiate->execute(); 

    cout << "Attempting to attack ally...\n";
    bomb->execute(); 
    cout << "-----------END of Testing Negotiate-------------\n\n";

    cout << "-----------Testing Blockade-------------\n";
    Blockade* blockade = new Blockade(playerONE, spain);
    blockade->execute();
    cout << "-----------END of Testing Blockade-------------\n\n";

}