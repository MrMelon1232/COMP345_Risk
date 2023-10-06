#include "GameEngineDriver.h"
#include <string>

// Allows the user to test the GameEngine's states and transitions in a console-driven application
void testGameStates() {
    initStateAndTransitions();
    
    while (currentState) {
        std::string cmdName;
        std::cin >> cmdName;
        findAndTransition(cmdName);
    }
}