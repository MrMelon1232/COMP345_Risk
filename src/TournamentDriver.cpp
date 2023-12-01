#include <iostream>
#include "GameEngine.h"

void testTournament() {
    GameEngine* gameEngine = new GameEngine();
    // Enter the tournament command
    
    Command* cmd = nullptr;
    do {
        std::cout << "Please enter the tournament command to test the tournament mode" << std::endl;
        cmd = gameEngine->getCommandProcessor()->getCommand();
    } while (cmd->getName() != "tournament");

    gameEngine->getCommandProcessor()->executeCommand(cmd);

    delete gameEngine;
}