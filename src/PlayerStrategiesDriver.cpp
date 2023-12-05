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
    france->setNbArmies(40);

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
    humanPlayer->getOrdersList()->getOrder(0)->execute();
    cout << "-----------END of Testing Deploy-------------\n\n";
    // Issue an advance order
    cout << "-----------Testing Advance-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Advance);
    humanPlayer->getOrdersList()->getOrder(1)->execute();
    cout << "-----------END of Testing Advance-------------\n\n";
    // Issue a bomb order
    cout << "-----------Testing Bomb-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Bomb); 
    humanPlayer->getOrdersList()->getOrder(2)->execute();
    cout << "-----------END of Testing Bomb-------------\n\n";
    // Issue a blockade order
    cout << "-----------Testing Blockade-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Blockade); 
    humanPlayer->getOrdersList()->getOrder(3)->execute();
    cout << "-----------END of Testing Blockade-------------\n\n";
    // Issue an airlift order
    cout << "-----------Testing Airlift-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Airlift);
    humanPlayer->getOrdersList()->getOrder(4)->execute();
    cout << "-----------END of Testing Airlift-------------\n\n"; 
    // Issue a negotiate order
    cout << "-----------Testing Negotiate-------------\n";
    humanStrategy->issueOrder(playerList, OrderType::Negotiate);
    humanPlayer->getOrdersList()->getOrder(5)->execute();
    cout << "-----------END of Testing Negotiate-------------\n\n";    
    
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
    Territory* spain2 = new Territory("Spain", "Europe", benevolentPlayer2->getPlayerID());
    benevolentPlayer->addTerritory(germany2);
    benevolentPlayer2->addTerritory(spain2); 
    germany2->setOwner(benevolentPlayer);
    spain2->setOwner(benevolentPlayer2);

    //adding units
    germany2->setNbArmies(5);
    spain2->setNbArmies(3);
    
    cout << "Benevolent player owns France.\n";
    //Player 2 owns France
    Territory* france2 = new Territory("France", "Europe", benevolentPlayer->getPlayerID());
    benevolentPlayer->addTerritory(france2); 
    france2->setOwner(benevolentPlayer);

    france2->setNbArmies(10);

    cout << "Germany and France are adjacent to each other.\n";
    cout << "Spain and France are adjacent to each other.\n"; 

    //Germany is adjacent to France and portugal
    germany2->AddAdjacentTerritory(france2);
    france2->AddAdjacentTerritory(germany2);

    //Spain is adjacent to France
    spain2->AddAdjacentTerritory(france2);
    france2->AddAdjacentTerritory(spain2);

    // Issue a deploy 
    cout << "-----------Testing Deploy-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Deploy);
    benevolentPlayer->getOrdersList()->getOrder(0)->execute();
    cout << "-----------END of Testing Deploy-------------\n\n";
    // Issue an advance order
    cout << "-----------Testing Advance-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Advance);
    benevolentPlayer->getOrdersList()->getOrder(1)->execute();
    cout << "-----------END of Testing Advance-------------\n\n";
    // Issue a bomb order
    cout << "-----------Testing Bomb-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Bomb);
    cout << "-----------END of Testing Bomb-------------\n\n";
     // Issue an airlift order
    cout << "-----------Testing Airlift-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Airlift);
    benevolentPlayer->getOrdersList()->getOrder(2)->execute();
    cout << "-----------END of Testing Airlift-------------\n\n"; 
    // Issue a blockade order
    cout << "-----------Testing Blockade-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Blockade);
    benevolentPlayer->getOrdersList()->getOrder(3)->execute();
    cout << "-----------END of Testing Blockade-------------\n\n";
    // Issue a negotiate order
    cout << "-----------Testing Negotiate-------------\n";
    benevolentPlayer->issueOrder(playerList2, OrderType::Negotiate);
    benevolentPlayer->getOrdersList()->getOrder(4)->execute();
    cout << "-----------END of Testing Negotiate-------------\n\n"; 

    cout << "-----------End of Benevolent Human player -------------\n";

    //--------------------------------------------------------------------------------------------------
    // End of Benevolent Player Strategy
    //--------------------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------------------
    // Cheater Player Strategy
    //--------------------------------------------------------------------------------------------------
    Player * cheaterPlayer = new Player();
    PlayerStrategy* cheaterStrategy = new CheaterPlayerStrategy(cheaterPlayer);
    cheaterPlayer->setStrategy(cheaterStrategy);
    Player* cheaterPlayer2 = new Player();
    cheaterPlayer2->setName("CheaterBot");
    cheaterPlayer2->setStrategy(cheaterStrategy);
    vector<Player*> playerList3;
    playerList3.push_back(cheaterPlayer2);
    cheaterPlayer->setReinforcementPool(1000);

    // Simulate the Cheater player's behavior (automatically conquer all adjacent territories)
    cout << "Testing Cheater Player Strategy...\n";
    Territory* germany3 = new Territory("Germany", "Europe", cheaterPlayer->getPlayerID());
    Territory* spain3 = new Territory("Spain", "Europe", cheaterPlayer->getPlayerID());
    cheaterPlayer->addTerritory(germany3); 
    cheaterPlayer->addTerritory(spain3);   

    // Adding units
    germany3->setNbArmies(5); 
    spain3->setNbArmies(3);   

    cout << "Cheater player 2 owns France.\n";
    // Player 2 owns France
    Territory* france3 = new Territory("France", "Europe", cheaterPlayer2->getPlayerID());
    cheaterPlayer2->addTerritory(france3); 

    cout << "Germany and France are adjacent to each other.\n";
    cout << "Spain and France are adjacent to each other.\n";

    // Germany is adjacent to France 
    germany3->AddAdjacentTerritory(france3); 
    france3->AddAdjacentTerritory(germany3); 

    // Spain is adjacent to France
    spain3->AddAdjacentTerritory(france3);  
    france3->AddAdjacentTerritory(spain3);   

    // No need to issue orders for the Cheater player

    cout << "-----------Testing Orders execution-------------\n";
    OrdersList* orders3 = cheaterPlayer->getOrdersList();
    for (int i = 0; i < orders3->getSize(); i++) {
        orders3->getOrder(i)->execute();
    }
    //--------------------------------------------------------------------------------------------------
    // End of Cheater Player Strategy
    //--------------------------------------------------------------------------------------------------

        //--------------------------------------------------------------------------------------------------
    // Aggressive Player Strategy
    //--------------------------------------------------------------------------------------------------
    cout << "-----------Start of Aggressive player -------------\n";
    Player* aggressivePlayer = new Player();
    PlayerStrategy* aggressiveStrategy = new AggressivePlayerStrategy(aggressivePlayer);
    aggressivePlayer->setStrategy(aggressiveStrategy);
    Player* neutralPlayer3 = new Player();
    PlayerStrategy* neutralStrategy3 = new NeutralPlayerStrategy(neutralPlayer3);
    neutralPlayer3->setName("NeutralDummy3");
    neutralPlayer3->setStrategy(neutralStrategy3);
    vector<Player*> playerList4;
    playerList4.push_back(neutralPlayer3);
    aggressivePlayer->setReinforcementPool(5);
    
    // Simulate issuing different types of orders
    Territory* germany4 = new Territory("Germany", "Europe", aggressivePlayer->getPlayerID());
    Territory* poland4 = new Territory("Germany", "Europe", aggressivePlayer->getPlayerID());
    Territory* spain4 = new Territory("Spain", "Europe", neutralPlayer3->getPlayerID());
    aggressivePlayer->addTerritory(germany4);
    neutralPlayer3->addTerritory(spain4); 
    germany4->setOwner(aggressivePlayer);
    poland4->setOwner(aggressivePlayer);
    spain4->setOwner(neutralPlayer3);

    //adding units
    germany4->setNbArmies(5);
    poland4->setNbArmies(1); 
    spain4->setNbArmies(14);
    
    cout << "Neutral player owns France.\n";
    //Player 2 owns France
    Territory* france4 = new Territory("France", "Europe", aggressivePlayer->getPlayerID());
    aggressivePlayer->addTerritory(france4); 
    france4->setOwner(aggressivePlayer);

    france4->setNbArmies(10);

    cout << "Germany and France are adjacent to each other.\n";
    cout << "Spain and France are adjacent to each other.\n"; 

    //Germany is adjacent to France and portugal
    germany4->AddAdjacentTerritory(france4);
    france4->AddAdjacentTerritory(germany4);

    //Spain is adjacent to France
    spain4->AddAdjacentTerritory(france4);
    france4->AddAdjacentTerritory(spain4);

    // Issue a deploy 
    cout << "-----------Testing Deploy-------------\n";
    aggressivePlayer->issueOrder(playerList4, OrderType::Deploy);
    aggressivePlayer->getOrdersList()->getOrder(0)->execute();
    cout << "-----------END of Testing Deploy-------------\n\n";
    // Issue an advance order
    cout << "-----------Testing Advance-------------\n";
    aggressivePlayer->issueOrder(playerList4, OrderType::Advance);
    aggressivePlayer->getOrdersList()->getOrder(1)->execute();
    cout << "-----------END of Testing Advance-------------\n\n";
    // Issue a bomb order
    cout << "-----------Testing Bomb-------------\n";
    aggressivePlayer->issueOrder(playerList4, OrderType::Bomb);
    cout << "-----------END of Testing Bomb-------------\n\n";
     // Issue an airlift order
    cout << "-----------Testing Airlift-------------\n";
    aggressivePlayer->issueOrder(playerList4, OrderType::Airlift);
    aggressivePlayer->getOrdersList()->getOrder(2)->execute();
    cout << "-----------END of Testing Airlift-------------\n\n"; 
    // Issue a blockade order
    cout << "-----------Testing Blockade-------------\n";
    aggressivePlayer->issueOrder(playerList4, OrderType::Blockade);
    aggressivePlayer->getOrdersList()->getOrder(3)->execute();
    cout << "-----------END of Testing Blockade-------------\n\n";
    // Issue a negotiate order
    cout << "-----------Testing Negotiate-------------\n";
    aggressivePlayer->issueOrder(playerList4, OrderType::Negotiate);
    aggressivePlayer->getOrdersList()->getOrder(4)->execute();
    cout << "-----------END of Testing Negotiate-------------\n\n"; 

    cout << "-----------End of Aggressive player -------------\n";

    //--------------------------------------------------------------------------------------------------
    // End of Aggressive Player Strategy
    //--------------------------------------------------------------------------------------------------

    //--------------------------------------------------------------------------------------------------
    // Neutral Player Strategy
    //--------------------------------------------------------------------------------------------------
    cout << "-----------Start of Neutral player -------------\n";
    Player* neutralPlayer = new Player();
    PlayerStrategy* neutralStrategy = new NeutralPlayerStrategy(neutralPlayer);
    neutralPlayer->setStrategy(neutralStrategy);
    Player* neutralPlayer2 = new Player();
    PlayerStrategy* neutralStrategy2 = new NeutralPlayerStrategy(neutralPlayer2);
    neutralPlayer2->setName("NeutralDummy");
    neutralPlayer2->setStrategy(neutralStrategy2);
    vector<Player*> playerList3;
    playerList3.push_back(neutralPlayer2);
    neutralPlayer->setReinforcementPool(1000);
    
    // Simulate issuing different types of orders
    Territory* germany3 = new Territory("Germany", "Europe", neutralPlayer->getPlayerID());
    Territory* spain3 = new Territory("Spain", "Europe", neutralPlayer2->getPlayerID());
    neutralPlayer->addTerritory(germany3);
    neutralPlayer2->addTerritory(spain3); 
    germany3->setOwner(neutralPlayer);
    spain3->setOwner(neutralPlayer2);

    //adding units
    germany3->setNbArmies(5);
    spain3->setNbArmies(3);
    
    cout << "Neutral player owns France.\n";
    //Player 2 owns France
    Territory* france3 = new Territory("France", "Europe", neutralPlayer->getPlayerID());
    neutralPlayer->addTerritory(france2); 
    france3->setOwner(neutralPlayer);

    france3->setNbArmies(10);

    cout << "Germany and France are adjacent to each other.\n";
    cout << "Spain and France are adjacent to each other.\n"; 

    //Germany is adjacent to France and portugal
    germany3->AddAdjacentTerritory(france3);
    france3->AddAdjacentTerritory(germany3);

    //Spain is adjacent to France
    spain3->AddAdjacentTerritory(france3);
    france3->AddAdjacentTerritory(spain3);

    // Issue a deploy 
    cout << "-----------Testing Deploy-------------\n";
    neutralPlayer->issueOrder(playerList3, OrderType::Deploy);
    neutralPlayer->getOrdersList()->getOrder(0)->execute();
    cout << "-----------END of Testing Deploy-------------\n\n";
    // Issue an advance order
    cout << "-----------Testing Advance-------------\n";
    neutralPlayer->issueOrder(playerList3, OrderType::Advance);
    neutralPlayer->getOrdersList()->getOrder(1)->execute();
    cout << "-----------END of Testing Advance-------------\n\n";
    // Issue a bomb order
    cout << "-----------Testing Bomb-------------\n";
    neutralPlayer->issueOrder(playerList3, OrderType::Bomb);
    cout << "-----------END of Testing Bomb-------------\n\n";
     // Issue an airlift order
    cout << "-----------Testing Airlift-------------\n";
    neutralPlayer->issueOrder(playerList3, OrderType::Airlift);
    neutralPlayer->getOrdersList()->getOrder(2)->execute();
    cout << "-----------END of Testing Airlift-------------\n\n"; 
    // Issue a blockade order
    cout << "-----------Testing Blockade-------------\n";
    neutralPlayer->issueOrder(playerList3, OrderType::Blockade);
    neutralPlayer->getOrdersList()->getOrder(3)->execute();
    cout << "-----------END of Testing Blockade-------------\n\n";
    // Issue a negotiate order
    cout << "-----------Testing Negotiate-------------\n";
    neutralPlayer->issueOrder(playerList3, OrderType::Negotiate);
    neutralPlayer->getOrdersList()->getOrder(4)->execute();
    cout << "-----------END of Testing Negotiate-------------\n\n"; 

    cout << "-----------End of Neutral player -------------\n";

    //--------------------------------------------------------------------------------------------------
    // End of Neutral Player Strategy
    //--------------------------------------------------------------------------------------------------
}

    
}
