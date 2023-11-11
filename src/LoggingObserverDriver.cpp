#include "LoggingObserverDriver.h"


void testLoggingObserver() {
    GameEngine gameEngine;
    CommandProcessor commandProcessor(&gameEngine);
    OrdersList orderList;

    // Create log observer
    LogObserver logObserver("gamelog.txt");

    // Attach observers to the instances
    commandProcessor.attach(&logObserver);
    orderList.attach(&logObserver);
    gameEngine.attach(&logObserver);

    // Test commands
    Command* command = new Command("gamestart");
    commandProcessor.getCommand();

    
    // Test execute of orders:
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
    
    // Test game engine
    State * state = new State("playersadded");
    Transition* transition = new Transition("gamestart", state);
    gameEngine.transition(transition);

    // Detach the LogObserver when done
    commandProcessor.detach(&logObserver);
    orderList.detach(&logObserver);
    gameEngine.detach(&logObserver);
}