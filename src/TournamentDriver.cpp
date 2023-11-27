#include "GameEngine.h"

void testTournament() {
    GameEngine* gameEngine = new GameEngine();
    // Enter the tournament command
    Command* cmd = gameEngine->getCommandProcessor()->getCommand();

}