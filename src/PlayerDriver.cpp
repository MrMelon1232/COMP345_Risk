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
    player1.issueOrder(OrderType::Deploy);
    player2.issueOrder(OrderType::Advance);

    // Display player information using the overloaded stream insertion operator
    std::cout << "Player 1 Info:\n" << player1 << std::endl;
    std::cout << "Player 2 Info:\n" << player2 << std::endl;

    test
}
