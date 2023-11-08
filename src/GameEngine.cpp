#include "GameEngine.h"
#include <algorithm>

// Constructor to initialize a state with a name.
State::State(string name) { 
    this->name = name; 
};

// Copy constructor for State class.
State::State(State& state) {
    name = state.name;
    // Since the Transition's `nextState` is deleted by the GameEngine,
    // we don't want to allocate dynamic data for the `nextState` of
    // a newly allocated Transition. Therefore, pointing to the same
    // memory location.
    transitions = state.transitions;
}

// Getter for the State's name.
string State::getName() {
    return name;
}

// Getter for the State's transitions.
vector<Transition*> State::getTransitions() {
    return transitions;
}

// Function that can add multiple transitions for this state.
void State::addTransitions(vector<Transition*> transitions...) {
    this->transitions = transitions;
}

// Assignment operator overload for State class.
State& State::operator=(const State& state) {
    name = state.name;
    transitions = state.transitions;
    return *this;
}

// Output stream operator overload to return the state's name.
std::ostream& operator<<(std::ostream& output, const State& state) {
    output << "`" << state.name << "`";
    return output;
}

// Destructor for State class
State::~State() {
    for(Transition* transition : transitions) {
        if (transition) {
            delete transition;
            transition = nullptr;
        }
    }
}

// Constructor to initialize transition with command name and state to transition to.
Transition::Transition(string commandName, State* nextState) {
    this->commandName = commandName;
    // Since the Transition's `nextState` is deleted by the GameEngine,
    // we don't want to allocate dynamic data for the `nextState` of
    // a newly allocated Transition. Therefore, pointing to the same
    // memory location.
    this->nextState = nextState;
};

// Copy constructor for Transition class.
Transition::Transition(Transition& transition) {
    commandName = transition.commandName;
    nextState = transition.nextState;
}

// Getter for Transition's command name.
string Transition::getCommandName() {
    return commandName;
}

// Getter for Transition's next state.
State* Transition::getNextState() {
    return nextState;
}

// Assignment operator overload for Transition class.
Transition& Transition::operator=(const Transition& transition) {
    commandName = transition.commandName;
    nextState = transition.nextState;
    return *this;
}

// Ouput stream operator overload to return Transition's command name and next state.
std::ostream& operator<<(std::ostream& output, const Transition& transition) {
    output << "Command name: " << transition.commandName << 
            ", Next state: " << transition.nextState->getName();
    return output;
}

// Destructor for Transition class
Transition::~Transition() {
    // The `nextState` is part of the GameEngine's `states`.
    // As the "owner", the GameEngine is responsible of deleting the states.
    // Therefore, nothing to do here.
}

// GameEngine default constructor. Creates the state and transition objects for the game. It also sets the initial state.
GameEngine::GameEngine() {
    setDefaultGameStates();
    currentMap = nullptr;
    selectMode();
    std::cout << "Current state is " << *currentState << "." << std::endl;
}

GameEngine::GameEngine(string mode) {
    setDefaultGameStates();
    currentMap = nullptr;
    this->mode = mode;
    initProcessor();
    std::cout << "Current state is " << *currentState << "." << std::endl;
}

// Game Engine constructor to initialize with states. Mainly used for testing.
GameEngine::GameEngine(vector<State*> states) {
    currentState = states.front();
    for (State* state : states)
        this->states.push_back(state);
    currentMap = nullptr;
    selectMode();
    std::cout << "Current state is " << *currentState << "." << std::endl;
}

// Game Engine copy constructor
GameEngine::GameEngine(GameEngine& gameEngine) {
    this->states = gameEngine.states; // To simplify circular data dependency, reuse same states and transitions.
    currentState = states.front();
    currentMap = new Map(*(gameEngine.currentMap));
    mode = gameEngine.mode;

    FileCommandProcessorAdapter* fileCmdProcAdapter = dynamic_cast<FileCommandProcessorAdapter*>(gameEngine.commandProcessor);
    if (fileCmdProcAdapter) {
        FileLineReader* flr = new FileLineReader(*(fileCmdProcAdapter->getFileLineReader()));
        commandProcessor = new FileCommandProcessorAdapter(this, flr);
    } else {
        commandProcessor = new CommandProcessor(this);
    }
}

// Utility method to use the default game states.
void GameEngine::setDefaultGameStates() {
    State* start = new State("start");
    State* mapLoaded = new State("maploaded");
    State* mapValidated = new State("mapvalidated");
    State* playersAdded = new State("playersadded");
    State* assignReinforcements = new State("assignreinforcement");
    State* issueOrders = new State("issueorders");
    State* executeOrders = new State("executeorders");
    State* win = new State("win");

    Transition* loadMap = new Transition("loadmap", mapLoaded);
    Transition* validateMap = new Transition("validatemap", mapValidated);
    Transition* addPlayer = new Transition("addplayer", playersAdded);
    Transition* gamestart = new Transition("gamestart", assignReinforcements);
    Transition* issueOrder = new Transition("issueorder", issueOrders);
    Transition* issueOrdersEnd = new Transition("issueordersend", executeOrders);
    Transition* execOrder = new Transition("execorder", executeOrders);
    Transition* endExecOrders = new Transition("endexecorders", assignReinforcements);
    Transition* winTrans = new Transition("win", win);
    Transition* play = new Transition("replay", start);
    Transition* quit = new Transition("quit", nullptr);

    start->addTransitions({loadMap});
    mapLoaded->addTransitions({loadMap, validateMap});
    mapValidated->addTransitions({addPlayer});
    playersAdded->addTransitions({addPlayer, gamestart});
    assignReinforcements->addTransitions({issueOrder});
    issueOrders->addTransitions({issueOrder, issueOrdersEnd});
    executeOrders->addTransitions({execOrder, winTrans, endExecOrders});
    win->addTransitions({play, quit});

    states = {start, mapLoaded, mapValidated, playersAdded, assignReinforcements, issueOrders, executeOrders, win};
    currentState = start;
}

// Utility method to select the game's mode.
void GameEngine::selectMode() {
    string mode;
    do {
        cout << "Type -console or -file to specify how to read the commands." << endl;
        cin >> mode;
    } while(mode != "-console" && mode != "-file");
    this->mode = mode;
    initProcessor();
}

// Utility method to set the correct command processor. Assumes the mode is valid.
void GameEngine::initProcessor() {
    if (mode == "-console") {
        commandProcessor = new CommandProcessor(this);
    } else { // mode is -file
        string fileName;
        cout << "Enter the file name to read." << endl;
        cin >> fileName;

        FileLineReader* flr = new FileLineReader(fileName);
        commandProcessor = new FileCommandProcessorAdapter(this, flr);
    }
}

// Function that indicates if the command is valid in the current state game.
bool GameEngine::isCommandValid(string command) {
    vector<Transition*> transitions = currentState->getTransitions();
    vector<Transition*>::iterator it = std::find_if(transitions.begin(), transitions.end(), 
                                         [&command](Transition* cmd) { return cmd->getCommandName() == command; });
    return it != transitions.end();
}

// Function that finds transition with given command name. If not found, it displays an error. Otherwise, calls function to perform the state transition.
void GameEngine::findAndTransition(string name) {
    vector<Transition*> transitions = currentState->getTransitions();
    vector<Transition*>::iterator it = std::find_if(transitions.begin(), transitions.end(), 
                                         [&name](Transition* cmd) { return cmd->getCommandName() == name; });
    if (it != transitions.end())
        transition(*it);
    else
        std::cout << "Invalid command for state " << *currentState << "." << std::endl;
}

// Function that sets the current game engine's state to the transition's next state.
void GameEngine::transition(Transition* transition) {
    currentState = transition->getNextState();
    std::cout << "Command `" << transition->getCommandName() << "` has been executed. ";
    if (currentState)
        std::cout << "Current state is now " << *currentState  << "." << std::endl;
    else
        std::cout << "The game cycle has been completed." << std::endl; 
}

GameEngine::~GameEngine() {
    for (State* state : states) // deletes the `currentState` too.
        delete state;
    delete currentMap;
    delete commandProcessor;
}




// FOR ASSIGNMENT 2

void GameEngine::addPlayer(Player* player) {
    players.push_back(player);
}




void listFilesInDirectory() {
    std::string directoryPath = "Maps";  // Use "Maps" as the directory path

    if (fs::exists(directoryPath) && fs::is_directory(directoryPath)) {
        std::cout << "List of map files in the directory:" << std::endl;
        for (const auto& entry : fs::directory_iterator(directoryPath)) {
            if (fs::is_regular_file(entry)) {
                std::cout  << entry.path().filename() << std::endl;
            }
        }
    }
    else {
        std::cerr << "Directory not found or is not a directory." << std::endl;
    }
}



void GameEngine::distributeTerritories(int numPlayers) {

    // Create a list of all territories
    std::vector<Territory*> allTerritories = currentMap->getTerritories();

    // Shuffle the territories randomly
    std::random_device rd;
    std::default_random_engine rng(rd());
    std::shuffle(allTerritories.begin(), allTerritories.end(), rng);

    int playerIndex = 0;
    for (Territory* territory : allTerritories) {
        Player* currentPlayer = players[playerIndex];
        currentPlayer->addTerritory(territory);
        territory->setOwner(currentPlayer);
        playerIndex = (playerIndex + 1) % numPlayers;
    }
}

void GameEngine::startupPhase() {

    listFilesInDirectory();

    // Create a CommandProcessor for console input
    CommandProcessor consoleCommandProcessor(this);

    // Load a map
    Command* loadMapCommand = consoleCommandProcessor.getCommand();
    while (loadMapCommand->getName() != "loadmap") {
        cout << "Invalid command. Please enter 'loadmap <filename>' to load the map." << endl;
        delete loadMapCommand;
        loadMapCommand = consoleCommandProcessor.getCommand();
    }


    string mapFileName = loadMapCommand->getArg();
    delete loadMapCommand;

    if (!mapLoader->LoadMap(mapFileName)) {
        cout << "Failed to load the map. Exiting startup phase." << endl;
        return;
    }

    // Validate the map
    Command* validateMapCommand = consoleCommandProcessor.getCommand();
    while (validateMapCommand->getName() != "validatemap") {
        cout << "Invalid command. Please enter 'validatemap' to validate the map." << endl;
        delete validateMapCommand;
        validateMapCommand = consoleCommandProcessor.getCommand();
    }

    // Execute the validate map command using CommandProcessor
    consoleCommandProcessor.executeCommand(validateMapCommand);
    delete validateMapCommand;


    // Create a CommandProcessor for player input
    CommandProcessor playerCommandProcessor(this);

    // add Players
    int numPlayers;
    do {
        cout << "Enter the number of players (2-6): ";
        cin >> numPlayers;
    } while (numPlayers < 2 || numPlayers > 6);

    for (int i = 1; i <= numPlayers; i++) {
        cout << "Enter the name of Player " << i << ": ";
        string playerName;
        cin >> playerName;

        // Construct the "addPlayer" command in the correct format
        string addPlayerCommand = "addplayer " + playerName;

        // Create and execute the command
        Command* addPlayerCmd = new Command(addPlayerCommand);
        playerCommandProcessor.executeCommand(addPlayerCmd);
        delete addPlayerCmd; // Clean up the command object
    }
}
