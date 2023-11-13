#include "CommandProcessing.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

void testCommandProcessor() {
    State* start = new State("start");
    State* mapLoaded = new State("maploaded");
    State* mapValidated = new State("mapvalidated");
    State* playersAdded = new State("playersadded");
    State* assignReinforcements = new State("assignreinforcement");
    State* win = new State("win");

    Transition* loadMap = new Transition("loadmap", mapLoaded);
    Transition* validateMap = new Transition("validatemap", mapValidated);
    Transition* addPlayer = new Transition("addplayer", playersAdded);
    Transition* gameStart = new Transition("gamestart", assignReinforcements);
    Transition* endGame = new Transition("endgame", win); // Skipping the main game loop to test all commands.
    Transition* replay = new Transition("replay", start);
    Transition* quit = new Transition("quit", nullptr);

    start->addTransitions({loadMap});
    mapLoaded->addTransitions({loadMap, validateMap});
    mapValidated->addTransitions({addPlayer});
    playersAdded->addTransitions({addPlayer, gameStart});
    assignReinforcements->addTransitions({endGame}); // Skipping the main game loop to test all commands.
    win->addTransitions({replay, quit});

    vector<State*> states = {start, mapLoaded, mapValidated, playersAdded, assignReinforcements, win};
    GameEngine* gameEngine = new GameEngine(states);

    if (gameEngine->getMode() == "-console") {
        int nbCommands;
        cout << "How many commands would you like to add?" << endl;
        cin >> nbCommands;

        for (int i = 0; i < nbCommands; i++)
            gameEngine->getCommandProcessor()->getCommand();
    } else { // Mode is -file.
        Command* command = nullptr;
        do {
            command = gameEngine->getCommandProcessor()->getCommand();
        } while (command);
    }
    
    cout << "-------------------------------" << endl;
    cout << "PROCEEDING TO COMMANDS EXECUTION" << endl;

    CommandProcessor* commandProcessor = gameEngine->getCommandProcessor();
    for (Command* command : commandProcessor->getCommands()) {
        cout << "-------------------------------" << endl;
        try {    
            commandProcessor->executeCommand(command);
            cout << *command << endl;
        } catch (const std::invalid_argument& e) {
		    cout << "Cannot replay during testing. Ending test." << endl;
	    }
    }
}