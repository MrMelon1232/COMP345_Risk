#include "GameEngine.h"
#include <algorithm>
#include <set>
#include <cmath>
#include "Map.h"
#include <random>
#include <filesystem>

using namespace std;

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
    // The `transitions` are part of the GameEngine.
    // As the "owner", the GameEngine is responsible of deleting the transitions.
    // Therefore, nothing to do here.
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

// Game Engine default constructor. Creates the state and transition objects for the game. It also sets the initial state.
GameEngine::GameEngine() {
    setDefaultGameStates();
    currentMap = nullptr;
    gameDeck = nullptr;
    selectMode();
    maxTurns = -1;
    std::cout << "Current state is " << *currentState << "." << std::endl;
}

// Game Engine constructor to initialize with given mode. Mainly used for testing.
GameEngine::GameEngine(string mode) {
    setDefaultGameStates();
    currentMap = nullptr;
    gameDeck = nullptr;
    this->mode = mode;
    maxTurns = -1;
    initProcessor();
    std::cout << "Current state is " << *currentState << "." << std::endl;
}

// Game Engine constructor to initialize with states. Mainly used for testing.
GameEngine::GameEngine(vector<State*> states) {
    currentState = states.front();
    gameDeck = nullptr;
    for (State* state : states)
        this->states.push_back(state);
    currentMap = nullptr;
    selectMode();
    maxTurns = -1;
    std::cout << "Current state is " << *currentState << "." << std::endl;
}

// Game Engine copy constructor.
GameEngine::GameEngine(GameEngine& gameEngine) {
    this->states = gameEngine.states; // To simplify circular data dependency, reuse same states and transitions.
    currentState = gameEngine.currentState;
    gameDeck = new Deck(*(gameEngine.gameDeck));
    currentMap = new Map(*(gameEngine.currentMap));
    mode = gameEngine.mode;

    FileCommandProcessorAdapter* fileCmdProcAdapter = dynamic_cast<FileCommandProcessorAdapter*>(gameEngine.commandProcessor);
    if (fileCmdProcAdapter) {
        FileLineReader* flr = new FileLineReader(*(fileCmdProcAdapter->getFileLineReader()));
        commandProcessor = new FileCommandProcessorAdapter(this, flr);
    }
    else {
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

    Transition* tournament = new Transition("tournament", assignReinforcements);
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

    start->addTransitions({ loadMap, tournament });
    mapLoaded->addTransitions({ loadMap, validateMap });
    mapValidated->addTransitions({ addPlayer });
    playersAdded->addTransitions({ addPlayer, gamestart });
    assignReinforcements->addTransitions({ issueOrder });
    issueOrders->addTransitions({ issueOrder, issueOrdersEnd });
    executeOrders->addTransitions({ execOrder, winTrans, endExecOrders });
    win->addTransitions({ play, quit });

    states = { start, mapLoaded, mapValidated, playersAdded, assignReinforcements, issueOrders, executeOrders, win };
    currentState = start;
}

// Utility method to select the game's mode.
void GameEngine::selectMode() {
    string mode;
    do {
        cout << "Type -console or -file to specify how to read the commands." << endl;
        cin >> mode;
    } while (mode != "-console" && mode != "-file");
    this->mode = mode;
    initProcessor();
}

// Utility method to set the correct command processor. Assumes the mode is valid.
void GameEngine::initProcessor() {
    if (mode == "-console") {
        commandProcessor = new CommandProcessor(this);
    } else { // mode is -file
        commandProcessor = nullptr;
        do {
            string fileName;
            cout << "Enter a valid file name to read." << endl;
            cin >> fileName;
            try {
                FileLineReader* flr = new FileLineReader(fileName);
                commandProcessor = new FileCommandProcessorAdapter(this, flr);
            } catch (const std::invalid_argument& e) {
                cout << "Could not open file: " << fileName << endl;
            }
        } while(!commandProcessor);
    }
}

void GameEngine::setPlayer(vector<Player*> player)
{
    players = player;
}
// getter and setter for numOfArmies
int GameEngine::getNumOfPlayers() const {
    return numOfPlayers;
}

void GameEngine::setNumOfPlayers(int num) {
    numOfPlayers = num;
}

vector<Player*> GameEngine::getPlayers() {
    return players;
}

void GameEngine::addPlayer(Player* player) {
    players.push_back(player);
}

void listFilesInDirectory() {
    std::string directoryPath = "../Maps/"; 

    if (filesystem::exists(directoryPath) && std::filesystem::is_directory(directoryPath)) {
        cout << "List of map files in the directory:" << endl;
        for (const auto& entry : filesystem::directory_iterator(directoryPath)) {
            if (filesystem::is_regular_file(entry)) {
                cout << entry.path().string() << endl;
            }
        }
    }
    else {
        cerr << "Directory not found or is not a directory." << endl;
    }
}


void GameEngine::startupPhase() {
    listFilesInDirectory();

    // Load a map
    while (currentState->getName() != "maploaded") {
        Command* loadMapCommand = commandProcessor->getCommand();
        commandProcessor->executeCommand(loadMapCommand);
    }

    // Load another map (optional) and validate the map
    while (currentState->getName() != "mapvalidated") {
        Command* validateMapcommand = commandProcessor->getCommand();
        commandProcessor->executeCommand(validateMapcommand);
    }
    
    // add 2-6 players after which gameStart command will be accepted
    while (currentState->getName() != "assignreinforcement") {
        Command* playerCommand = commandProcessor->getCommand();
        commandProcessor->executeCommand(playerCommand);
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
        std::cout << "Current state is now " << *currentState << "." << std::endl;
    else
        std::cout << "The game cycle has been completed." << std::endl; 
    notify(*this);
}

// Game Engine's assignment operator overload.
GameEngine& GameEngine::operator=(const GameEngine& gameEngine) {
    // Deallocating existing dynamic memory
    std::set<Transition*> transitions; // Retrieving all unique transitions before deleting states to avoid deleting an already deleted transition.
    for (State* state : states) { // Deletes the `currentState` too.
        for (Transition* transition : state->getTransitions())
            transitions.insert(transition);
        delete state;
    }
    states.clear();

    for (Transition* transition : transitions)
        delete transition;

    this->states = gameEngine.states; // To simplify circular data dependency, reuse same states and transitions.
    currentState = gameEngine.currentState;
    currentMap = new Map(*(gameEngine.currentMap));
    mode = gameEngine.mode;
    delete gameDeck;
    gameDeck = new Deck(*(gameEngine.gameDeck));

    for (Player* player : players)
        delete player;
    players.clear();
    for (Player* player : gameEngine.players)
        players.push_back(player);

    FileCommandProcessorAdapter* fileCmdProcAdapter = dynamic_cast<FileCommandProcessorAdapter*>(gameEngine.commandProcessor);
    if (fileCmdProcAdapter) {
        FileLineReader* flr = new FileLineReader(*(fileCmdProcAdapter->getFileLineReader()));
        commandProcessor = new FileCommandProcessorAdapter(this, flr);
    } else {
        commandProcessor = new CommandProcessor(this);
    }
    return *this;
}

// Game Engine's stream operator overload to display the game's mode.
std::ostream& operator<<(std::ostream& output, const GameEngine& gameEngine) {
    output << "Game engine mode: " << gameEngine.mode;
    return output;
}

// Game Engine's destructor.
GameEngine::~GameEngine() {
    std::set<Transition*> transitions; // Retrieving all unique transitions before deleting states to avoid deleting an already deleted transition.
    for (State* state : states) { // Deletes the `currentState` too.
        for (Transition* transition : state->getTransitions())
            transitions.insert(transition);
            delete state;
    }

    for (Transition* transition : transitions)
        delete transition;

    for (Player* player : players)
        delete player;

    delete gameDeck;
    delete currentMap;
    delete commandProcessor;
}

string GameEngine::stringToLog() {
    return "Game state changed to " + currentState->getName();
}
//addition for A2: main game loop
void GameEngine::mainGameLoop() {
    cout << "\n\nentering main game loop" << endl;
    bool gameEnd = false;
    while (!gameEnd) {
        //run game loop
        
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();
        
        int randomValue;
        randomValue = rand() % 2;
        if (randomValue == 1 && nbTurnsPlayed == maxTurns/2) {
            forceGameWin();
        }
        nbTurnsPlayed++;
        gameEnd =  gameResultCheck();
        cout << "testing" << endl;
    }

    if (players.size() > 1) {
        cout << "The game is a draw!" << endl;
    }
    else if (players.size() == 1) {
        cout << "Winner: " << (*players.at(0)).getStrategy() << ((*players.at(0)).getName()) << endl;
    }
}

bool GameEngine::gameResultCheck() {

    cout << "\nVerifying current game result:" << endl;
    //check if a player has no territories owned, then eliminate him

    auto iterator = players.begin();
    while (iterator != players.end()) {
        cout << "checking player" << endl;
        if ((*iterator)->getTerritories().size() < 1) {
            cout << "Player " << (*iterator)->getName() + " ID: " << (*iterator)->getPlayerID() << " is out!" << endl;
            iterator = players.erase(iterator);
            continue;
        }
        ++iterator;
    }
    
    //check if a player owns all the territories
    if (players.size() == 1) {
        int numberTerritoriesOwned = players.at(0)->getTerritories().size();
        int numberTerritories = 0;
        for (int i = 0; i < currentMap->getContinents().size(); i++) {
            cout << "territories owned in continent: " << currentMap->getContinents().at(i)->getTerritory().size() << endl;
            numberTerritories += currentMap->getContinents().at(i)->getTerritory().size();
        }
        if (numberTerritoriesOwned == numberTerritories) {
            return true;
        }
    }
    cout << "Game still in progress, starting new turn." << endl;

    if (maxTurns == nbTurnsPlayed) // The game played doesn't have a limit of turns.
        return true;
    return false;
}

void GameEngine::reinforcementPhase() {
    cout << "\nEntering reinforcement phase." << endl;
    //int reinforcement = 0;
    auto iterator = players.begin();
    
    while (iterator != players.end()) {
        (*iterator)->setTempPool((*iterator)->getReinforcementPool());
        //number of territories owned by players
        int territoryQuantity = 0; 
        territoryQuantity += (*iterator)->getTerritories().size();
        //reinforcement amount
        reinforcement = floor(territoryQuantity/3);

        int totalTerritories = 0;
        //check if player owns all territories in a continent
        for (int i = 0; i < currentMap->getContinents().size(); i++) {
            //count territories own per continent
            for (int j = 0; j < (*iterator)->getTerritories().size(); j++) {
                if ((*iterator)->getTerritories().at(j)->GetContinentName() == currentMap->getContinents().at(i)->GetName()) {
                    totalTerritories++;
                }
            }
            if (totalTerritories == currentMap->getContinents().at(i)->getTerritory().size()) {
                reinforcement += currentMap->getContinents().at(i)->getBonusValue();
            }
        }

        if (reinforcement < 3) {
            reinforcement = 3;
        }
        ++iterator;
    }
}

void GameEngine::issueOrdersPhase() {
    //vector value for round-robin
    vector<int> turn;
    for (int i = 0; i < players.size(); i++) {
        turn.push_back(i);
        //setting reinforcement pool
        players.at(i)->setReinforcementPool(players.at(i)->getReinforcementPool() + reinforcement);
        if (players.at(i)->getStrategy() == "neutral" || players.at(i)->getStrategy() == "cheater") {
            players.at(i)->setTempPool(-1);
        }
        else {
            players.at(i)->setTempPool(players.at(i)->getTempPool() + reinforcement);
        }
    }

    //round-robin loop
    bool trueFalse = true, advance = true, cardUsed = false;
    int iteration = 0;
    while (!turn.empty()) {
        string str, availableOrder;
        cout << "Current player issuing order: " << players.at(turn.at(iteration))->getName() << " ID: " << players.at(turn.at(iteration))->getPlayerID() << " | " << players.at(turn.at(iteration))->getStrategy() << endl;
        //show territories that player can deploy
        //deploying reinforcements
        while (players.at(turn.at(iteration))->getTempPool() > 0) {
            players.at(turn.at(iteration))->issueOrder(players, getOrderType("deploy"));
            if (players.at(turn.at(iteration))->getTempPool() <= 0) {
                trueFalse = false;
                break;
            }
        }
        //chose between advance or a card
        while (trueFalse && advance) {
            //advance for humans
            if (players.at(turn.at(iteration))->getStrategy() == "human") {
                cout << "Would you like to advance armies? Player won't be able to use advance again for this phase after selecting 'no'. [y/n] : " << endl;
                cin >> str;

                char first = str.at(0);
                //remove player from roundrobin
                if (first == 'y') {
                    players.at(turn.at(iteration))->issueOrder(players, getOrderType("advance"));
                    trueFalse = false;
                    break;
                }
                else if (first == 'n') {
                    advance = false;
                    break;
                }
                else {
                    cout << "Cannot understand choice. please enter again." << endl;
                }
            }
            //for cpu orders
            else {
                if (players.at(turn.at(iteration))->getStrategy() == "aggressive" || players.at(turn.at(iteration))->getStrategy() == "benevolent") {
                    players.at(turn.at(iteration))->issueOrder(players, getOrderType("advance"));
                }
                advance = false;
                trueFalse = false;
            }
        }


        //issue order for cards. 1 order per cycle
        for (int i = 0; i < players.at(turn.at(iteration))->getHandSize(); i++) {
            availableOrder += "[" + players.at(turn.at(iteration))->getCard(i) + "]\t";
        }
        while (trueFalse && !cardUsed) {
            //card for humans
            if (players.at(turn.at(iteration))->getStrategy() == "human") {
                cout << "Available Order: " << availableOrder << "\nState the order ton be issued: " << endl;
                cin >> str;

                //check if player has card
                for (int i = 0; i < players.at(turn.at(iteration))->getHandSize(); i++) {
                    if (players.at(turn.at(iteration))->getCard(i) == str) {
                        players.at(turn.at(iteration))->issueOrder(players, getOrderType(str));
                        trueFalse = false;
                        cardUsed = true;
                        break;
                    }
                    else  if (i == players.at(turn.at(iteration))->getHandSize() - 1) {
                        cout << "Invalid order. Please ender a valid order:" << endl;
                    }
                }
            }
            //card for cpu
            else {
                int randomValue;
                randomValue = rand() % (players.at(turn.at(iteration))->getHandSize() + 1);
                if (randomValue < players.at(turn.at(iteration))->getHandSize() && !cardUsed) {
                    if (players.at(turn.at(iteration))->getCard(randomValue) == "reinforcement") {
                        players.at(turn.at(iteration))->issueOrder(players, getOrderType("diplomacy"));
                    }
                    else {
                        players.at(turn.at(iteration))->issueOrder(players, getOrderType(players.at(turn.at(iteration))->getCard(randomValue)));
                    }
                }
                trueFalse = false;
                cardUsed = true;
                break;
            }
        }
        //end turn or not
        while (trueFalse) {
            if (players.at(turn.at(iteration))->getStrategy() == "human") {
                cout << "Will you end your turn? [y/n]" << endl;
                cin >> str;
                char first = str.at(0);
                //remove player from roundrobin
                if (first == 'y') {
                    turn.erase(turn.begin() + iteration);
                    break;
                }
                else if (first == 'n') {
                    break;
                }
                else {
                    cout << "Cannot understand choice. please enter again [y/n]: " << endl;
                }
            }
            else {
                int randomValue;
                randomValue = rand() % 2 + 1;
                if (randomValue == 1) {
                    turn.erase(turn.begin() + iteration);
                    break;
                }
                break;
            }
        }
        //return to first iteration
        trueFalse = true;
        if (iteration >= turn.size()-1) {
            iteration = 0;
        }
        else {
            iteration++;
        }
    }
}

void GameEngine::executeOrdersPhase() {
    //vector value for round-robin
    cout << "Entering Execute Order Phase." << endl;
    vector<int> turn;
    for (int i = 0; i < players.size(); i++) {
        turn.push_back(i);
    }

    //round-robin loop
    int iteration = 0;
    bool advance = false;
    while (!turn.empty()) {
        cout << "Executing for: " << players.at(turn.at(iteration))->getStrategy() << endl;
        bool trueFalse = true;
        //first iteration for deploy orders
        while (players.at(turn.at(iteration))->getReinforcementPool() > 0 && players.at(turn.at(iteration))->getOrdersList()->getSize() > 0) {
            for (int i = 0; i < players.at(turn.at(iteration))->getOrdersList()->getSize(); i++) {
                if (players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->getName() == "Deploy") {
                    players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->execute(); 
                    players.at(turn.at(iteration))->getOrdersList()->remove(i);
                    i--;    
                }
            }
            trueFalse = false;
        }
        //second iteration for remaining orders
        while (trueFalse) {
            for (int i = 0; i < players.at(turn.at(iteration))->getOrdersList()->getSize(); i++) {
                if (players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->getName() == "Advance") {
                    players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->execute();
                    players.at(turn.at(iteration))->getOrdersList()->remove(i);
                    i--;
                }
                else if (players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->getName() == "Bomb") {
                    players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->execute();
                    players.at(turn.at(iteration))->getOrdersList()->remove(i);
                    i--;
                }
                else if (players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->getName() == "Blockade") {
                    players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->execute();
                    players.at(turn.at(iteration))->getOrdersList()->remove(i);
                    i--;
                }
                else if (players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->getName() == "Airlift") {
                    players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->execute();
                    players.at(turn.at(iteration))->getOrdersList()->remove(i);
                    i--;
                }
                else if (players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->getName() == "Negotiate") {
                    players.at(turn.at(iteration))->getOrdersList()->getOrder(i)->execute();
                    players.at(turn.at(iteration))->getOrdersList()->remove(i);
                    i--;
                }
            }
            trueFalse = false;
        }
        //remove player from roundrobin
        if (players.at(turn.at(iteration))->getOrdersList()->getSize() == 0) {
            turn.erase(turn.begin() + iteration);
        }

        //return to first iteration
        if (iteration >= turn.size()-1) {
            iteration = 0;
        }
        else {
            iteration++;
        }
    }
}

OrderType getOrderType(string str) {
    if (str == "deploy") {
        cout << "deploy order type" << endl;
        return OrderType::Deploy;
    }
    else if (str == "advance") {
        cout << "advance order type" << endl;
        return OrderType::Advance;
    }
    else if (str == "bomb") {
        cout << "bomb order type" << endl;
        return OrderType::Bomb;
    }
    else if (str == "blockade") {
        cout << "blockade order type" << endl;
        return OrderType::Blockade;
    }
    else if (str == "airlift") {
        cout << "airlift order type" << endl;
        return OrderType::Airlift;
    }
    else if (str == "diplomacy") {
        cout << "negotiate order type" << endl;
        return OrderType::Negotiate;
    }
}

void GameEngine::forceGameWin() {
    cout << "Erasing a player manually to simulate winning game..." << endl;
    Player* temp;
    int randomValue;
    randomValue = rand() % players.size();
    temp = players.at(randomValue);
    players.clear();
    players.push_back(temp);
    for (int i = 0; i < currentMap->getTerritories().size(); i++) {
        if (currentMap->getTerritories().at(i)->getOwnerID() != players.at(0)->getPlayerID()) {
            currentMap->getTerritories().at(i)->setOwner(players.at(0));
            currentMap->getTerritories().at(i)->setOwnerID(players.at(0)->getPlayerID());
            players.at(0)->addTerritory(currentMap->getTerritories().at(i));
        }
    }
}

void GameEngine::gameStart() {
    // 4.a fairly distribute all the territories to the players
    vector<Territory*> allTerritories = getCurrentMap()->territories;

    // Shuffle the territories randomly
    random_device rd;
    default_random_engine rng(rd());
    shuffle(allTerritories.begin(), allTerritories.end(), rng);

    int playerIndex = 0;
    for (Territory* territory : allTerritories) {
        Player* currentPlayer = getPlayers()[playerIndex];
        currentPlayer->addTerritory(territory);
        territory->setOwner(currentPlayer);
        territory->setOwnerID(currentPlayer->getPlayerID());
        playerIndex = (playerIndex + 1) % getPlayers().size();
    }

    // 4.b determine randomly the order of play of the players in the game
    // Randomize the order of the how the player is accessed by rnadomizing the vector
    vector<Player*> allPlayers = getPlayers();
    shuffle(allPlayers.begin(), allPlayers.end(), rng);

    // 4.c give 50 initial army units to the players, which are placed in their respective reinforcement pool
    for (Player* player : getPlayers()) {
        player->setReinforcementPool(50);
    }

    // 4.d let each player draw 2 initial cards from the deck using the deck’s draw() method
    gameDeck = new Deck();
    for (Player* player : getPlayers()) {
        gameDeck->draw(player->getHand());
        gameDeck->draw(player->getHand());
    }
}

void GameEngine::resetGame() {
    clearGame();
    startupPhase();
    mainGameLoop();
}

void GameEngine::startTournament(TournamentCommand* tournamentCmd) {
    //log for the txt file
    LogObserver logObserver("tournament.txt");
    tournamentCmd->attach(&logObserver);
    vector<string> endGameData;
    MapLoader* mapLoader = getCommandProcessor()->getMapLoader();
    for (string mapFile : tournamentCmd->mapFiles) {
        //add game data (map#)
        endGameData.clear();
        endGameData.push_back("Map");
        for (int j = 0; j < tournamentCmd->nbGames; j++) {
            // Prepare the game.
            try {
                setCurrentMap(mapLoader->LoadMap(mapFile));
            } catch (const runtime_error& error) {
                tournamentCmd->saveEffect("Map file not found.");
                cout << "Could not load map file " + mapFile + ". Skipping this map." << endl;
                continue;
            }

            for (StrategyType type : tournamentCmd->playerStrats) {
                Player* newPlayer = new Player();
                newPlayer->setStrategy(loadStrategy(newPlayer, type));
                addPlayer(newPlayer);
            }

            nbTurnsPlayed = 0;
            maxTurns = tournamentCmd->maxTurnsPerGame;

            // Distribute territories, cards, and reinforcements
            gameStart();
            // Play the game.
            mainGameLoop();
            //add game data (winning player)
            
            if (players.size() == 1) {
                string str = players.at(0)->getStrategy();
                str.resize(7);
                endGameData.push_back(str);
            }
            else {
                endGameData.push_back("Draw");
            }
            // Clear game variables.
            clearGame();
        }
        tournamentCmd->table.push_back(endGameData);
    }
    tournamentCmd->saveResult(tournamentCmd->stringToLog());
    tournamentCmd->detach(&logObserver);
}

void GameEngine::clearGame() {
    for (Player* player : players)
        delete player;
    players.clear();

    delete currentMap;
    currentMap = nullptr;

    delete gameDeck;
    gameDeck = nullptr;
}