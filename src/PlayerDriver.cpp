#include "PlayerDriver.h"
#include <iostream>

void testPlayers() {
    Player player1, player2;

    // Simulate player 1 defending territories
    std::cout << "Player 1 Defending Territories: \n";
    std::vector<Territory*> player1DefendTerritories = player1.toDefend();

    // Simulate player 2 attacking territories
    std::cout << "Player 2 Attacking Territories: \n";
    std::vector<Territory*> player2AttackTerritories = player2.toAttack();

    // Issue orders for players
    std::cout << "Adding order of Deploy type to player1 \n";
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
