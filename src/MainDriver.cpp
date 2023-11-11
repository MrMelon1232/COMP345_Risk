
#include "CommandProcessing.h"
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"


void testA1() {
    // Test function for map
    testLoadMaps();

    // Test function for players
    testPlayers();

    // Test function for orders
    testOrdersList();

    // Test function for cards
    testCards();

    // Test function for game engine
    testGameStates();
}

void testA2() {
    // Test function for command processor
    //testCommandProcessor();

    testStartupPhase();
    //Test function for main game loop
    testMainGameLoop();

    //testOrderExecution();
}

int main() {
    //Assignment 1
    //testA1();

    //Assignment 2
    testA2();
}
