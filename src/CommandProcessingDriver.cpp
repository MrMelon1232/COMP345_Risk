#include "CommandProcessingDriver.h"
#include <iostream>

using std::cout;
using std::cin;
using std::endl;

void testCommandProcessor() {
    string mode;
    cout << "Type -console or -file <filename> to specify how to read the commands";
    cin >> mode;

    State* start = new State("start");
    State* mapLoaded = new State("maploaded");
    State* mapValidated = new State("mapvalidated");
    State* playersAdded = new State("playersadded");
    State* assignReinforcements = new State("assignreinforcement");
    State* win = new State("win");

    Transition* loadMap = new Transition("loadmap", mapLoaded);
    Transition* validateMap = new Transition("validatemap", mapValidated);
    Transition* addPlayer = new Transition("addplayer", playersAdded);
    Transition* gameStart = new Transition("gameStart", assignReinforcements);
    Transition* endGame = new Transition("endGame", win); // Skipping the main game loop to test all commands.
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

    if (mode.find("-console")) {
        gameEngine->setCommandProcessor(new CommandProcessor(gameEngine));

        cout << "Please enter the ";

        string commandLine;
        cin >> commandLine;
        
    } else if (mode.find("-file")) {
        string fileName = mode.substr(6);
        gameEngine->setCommandProcessor(new FileCommandProcessorAdapter(gameEngine, fileName));
    }
}