#include "PlayerDriver.h"
#include "CardsDriver.h"
#include "OrdersDriver.h"
#include "GameEngineDriver.h"
#include "MapDriver.h"
#include "CommandProcessingDriver.h"

int main() {

    // Test function for map
    testLoadMaps();

    // Test function for players
    testPlayers();

    // Test function for orders
    testOrdersList();
    testOrderExecution();

    // Test function for cards
    testCards();

    // Test function for game engine
    testGameStates();
    testStartupPhase();

    // Test function for command processor
    testCommandProcessor();
}
