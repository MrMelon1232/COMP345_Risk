#include "PlayerDriver.h"
#include <iostream>

void testPlayers() {
    Player player1, player2;

    // Add a territory to player1
    Territory * territory1 = new Territory("territory A", "Continent A");
    player1.addTerritory(territory1);

    // Simulate player 1 defending territories
    std::cout << "Player 1 Territories to be defended: \n";
    std::vector<Territory*> player1DefendTerritories = player1.toDefend();
    for(Territory * territory: player1DefendTerritories) {
        std::cout << territory->GetName() << " | ";
        std::cout << territory->GetContinentName() << std::endl;
    }
    
    // Simulate player 2 attacking territories
    std::cout << "Player 2 Territories to be attacked: \n";
    std::vector<Territory*> player2AttackTerritories = player2.toAttack();
    for(Territory * territory: player2AttackTerritories) {
        std::cout << territory->GetName() << " | ";
        std::cout << territory->GetContinentName() << std::endl;
    }

    // Issue orders for players
    std::cout << "\nAdding order of Deploy type to player1 \n";
    player1.issueOrder(OrderType::Deploy);
    std::cout << "Displaying player1 information: \n";
    std::cout << player1;
    std::cout << "\nAdding order of Advance type to player1 \n";
    player1.issueOrder(OrderType::Advance);
    std::cout << "\nDisplaying player1 information: \n";
    std::cout << player1;

    // Display player information using the overloaded stream insertion operator
    std::cout << "\nPlayer 1 Info:\n" << player1 << std::endl;
    std::cout << "\nPlayer 2 Info:\n" << player2 << std::endl;
}
