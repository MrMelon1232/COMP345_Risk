#include "Map.h"
#include "Player.h"
#include "Orders.h"
#include "Cards.h"
#include "GameEngine.h"

int main() {

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