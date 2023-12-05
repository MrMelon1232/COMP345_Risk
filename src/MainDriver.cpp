
#include "CommandProcessingDriver.h"
#include "LoggingObserverDriver.h"
#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"
#include "PlayerStrategies.h"


void testA1() {
    // Test function for map
    //testLoadMaps();

    // Test function for players
    //testPlayers();

    // Test function for orders
    //testOrdersList();

    // Test function for cards
    //testCards();

    // Test function for game engine
    //testGameStates();
}

void testA2() {
    // Test function for command processor
    //testCommandProcessor();

    // Test function for startup phase
    //testStartupPhase();

    //Test function for main game loop
    //testMainGameLoop();

    // Test function for orders execution
    //testOrderExecution();

    // Test function for logging observer
    testLoggingObserver();
}

int main() {
    //Assignment 1
    //testA1();

    //Assignment 2
    //testA2();

    //Assignment 3
    //testTournament();
    testPlayerStrategies();
}

