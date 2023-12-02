#include "PlayerStrategies.h"
#include "Map.h"

void testPlayerStrategies() {

    // Set order types
    OrderType Deploy;

    //--------------------------------------------------------------------------------------------------
    // Human Player Strategy
    //--------------------------------------------------------------------------------------------------
    Player* humanPlayer = new Player();
    PlayerStrategy* humanStrategy = new HumanPlayerStrategy(humanPlayer);
    humanPlayer->setStrategy(humanStrategy);
    Player* humanPlayer2 = new Player();
    PlayerStrategy* humanStrategy2 = new HumanPlayerStrategy(humanPlayer2);
    humanPlayer2->setName("Robert");
    humanPlayer2->setStrategy(humanStrategy2);
    vector<Player*> playerList;
    playerList.push_back(humanPlayer2);
    humanPlayer->setReinforcementPool(1000);
    
    // Simulate issuing different types of orders
    cout << "Testing Human Player Strategy...\n";
    Territory* germany = new Territory("Germany", "Europe", humanPlayer->getPlayerID());
    Territory* spain = new Territory("Spain", "Europe", humanPlayer->getPlayerID());
    germany->setOwner(humanPlayer);
    spain->setOwner(humanPlayer);
    humanPlayer->addTerritory(germany);
    humanPlayer->addTerritory(spain); 

    //adding units
    germany->setNbArmies(5);
    spain->setNbArmies(3);
    
    cout << "Human player 2 owns France.\n";
    //Player 2 owns France
    Territory* france = new Territory("France", "Europe", humanPlayer2->getPlayerID());
    humanPlayer2->addTerritory(france);
    france->setOwner(humanPlayer2);

    cout << "Germany and France are adjacent to each other.\n";
    cout << "Spain and France are adjacent to each other.\n"; 

    //Germany is adjacent to France 
    germany->AddAdjacentTerritory(france);
    france->AddAdjacentTerritory(germany);

    //Spain is adjacent to France
    spain->AddAdjacentTerritory(france);
    france->AddAdjacentTerritory(spain);

    // Issue a deploy 
    cout << "-----------Testing Deploy-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Deploy);
    cout << "-----------END of Testing Deploy-------------\n\n";
    // Issue an advance order
    cout << "-----------Testing Advance-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Advance);
    cout << "-----------END of Testing Advance-------------\n\n";
    // Issue a bomb order
    cout << "-----------Testing Bomb-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Bomb); 
    cout << "-----------END of Testing Bomb-------------\n\n";
    // Issue a blockade order
    cout << "-----------Testing Blockade-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Blockade); 
    cout << "-----------END of Testing Blockade-------------\n\n";
    // Issue an airlift order
    cout << "-----------Testing Airlift-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Airlift);
    cout << "-----------END of Testing Airlift-------------\n\n"; 
    // Issue a negotiate order
    cout << "-----------Testing Negotiate-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Negotiate);
    cout << "-----------END of Testing Negotiate-------------\n\n"; 

    cout << "-----------Testing Orders execution-------------\n";
    OrdersList * orders = humanPlayer->getOrdersList();
    for (int i = 0; i < orders->getSize(); i++) {
        orders->getOrder(i)->execute();
    }
    
    cout << "-----------End of Testing Human player -------------\n";

    //--------------------------------------------------------------------------------------------------
    // End of Human Player Strategy
    //--------------------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------------------
    // Benevolent Player Strategy
    //--------------------------------------------------------------------------------------------------
    Player* benevolentPlayer = new Player();
    PlayerStrategy* benevolentStrategy = new BenevolentPlayerStrategy(benevolentPlayer);
    benevolentPlayer->setStrategy(benevolentStrategy);
    Player* benevolentPlayer2 = new Player();
    PlayerStrategy* benevolentStrategy2 = new BenevolentPlayerStrategy(benevolentPlayer2);
    benevolentPlayer2->setName("Pingu");
    benevolentPlayer2->setStrategy(benevolentStrategy2);
    vector<Player*> playerList2;
    playerList2.push_back(benevolentPlayer2);
    benevolentPlayer->setReinforcementPool(1000);
    
    // Simulate issuing different types of orders
    cout << "Testing Benevolent Player Strategy...\n";
    Territory* germany2 = new Territory("Germany", "Europe", benevolentPlayer->getPlayerID());
    Territory* spain2 = new Territory("Spain", "Europe", benevolentPlayer->getPlayerID());
    benevolentPlayer->addTerritory(germany);
    benevolentPlayer->addTerritory(spain); 
    germany2->setOwner(benevolentPlayer);
    spain2->setOwner(benevolentPlayer);

    //adding units
    germany2->setNbArmies(5);
    spain2->setNbArmies(3);
    
    cout << "Benevolent player 2 owns France.\n";
    //Player 2 owns France
    Territory* france2 = new Territory("France", "Europe", benevolentPlayer2->getPlayerID());
    benevolentPlayer2->addTerritory(france); 
    france2->setOwner(benevolentPlayer2);

    cout << "Germany and France are adjacent to each other.\n";
    cout << "Spain and France are adjacent to each other.\n"; 

    //Germany is adjacent to France 
    germany2->AddAdjacentTerritory(france2);
    france2->AddAdjacentTerritory(germany2);

    //Spain is adjacent to France
    spain2->AddAdjacentTerritory(france2);
    france2->AddAdjacentTerritory(spain2);

    // Issue a deploy 
    cout << "-----------Testing Deploy-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Deploy);
    cout << "-----------END of Testing Deploy-------------\n\n";
    // Issue an advance order
    cout << "-----------Testing Advance-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Advance);
    cout << "-----------END of Testing Deploy-------------\n\n";
    // Issue a bomb order
    cout << "-----------Testing Bomb-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Bomb); 
    cout << "-----------END of Testing Deploy-------------\n\n";
    // Issue a blockade order
    cout << "-----------Testing Blockade-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Blockade); 
    cout << "-----------END of Testing Deploy-------------\n\n";
    // Issue an airlift order
    cout << "-----------Testing Airlift-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Airlift);
    cout << "-----------END of Testing Deploy-------------\n\n"; 
    // Issue a negotiate order
    cout << "-----------Testing Negotiate-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Negotiate);
    cout << "-----------END of Testing Deploy-------------\n\n"; 

    cout << "-----------Testing Orders execution-------------\n";
    OrdersList * orders2 = benevolentPlayer->getOrdersList();
    for (int i = 0; i < orders2->getSize(); i++) {
        orders2->getOrder(i)->execute();
    }

    cout << "-----------End of Benevolent Human player -------------\n";

    //--------------------------------------------------------------------------------------------------
    // End of Benevolent Player Strategy
    //--------------------------------------------------------------------------------------------------
}