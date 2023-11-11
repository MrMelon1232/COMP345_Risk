#include "GameEngineDriver.h"
#include <string>

// Allows the user to test the GameEngine's states and transitions in a console-driven application
void testGameStates() {
    GameEngine* gameEngine = new GameEngine("-console");

    while (gameEngine->getCurrentState()) {
        std::string cmdName;
        std::cin >> cmdName;
        gameEngine->findAndTransition(cmdName);
    }

    delete gameEngine;
}


// Allows the user to test the GameEngine's states and transitions in a console-driven application
void testGameStates() {
    GameEngine* gameEngine = new GameEngine("-console");

    while (gameEngine->getCurrentState()) {
        std::string cmdName;
        std::cin >> cmdName;
        gameEngine->findAndTransition(cmdName);
    }

    delete gameEngine;
}
