#include "GameEngine.h"
#include <algorithm>
#include <cmath>
#include "Map.h"

// Constructor to initialize a state with a name.
State::State(string name) { 
    this->name = name; 
};

// Copy constructor for State class.
State::State(State& state) {
    name = state.name;
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
    // Clean up dynamically allocated memory
    for (Transition* transition : transitions) {
        delete transition;
    }
}

// Constructor to initialize transition with command name and state to transition to.
Transition::Transition(string commandName, State* nextState) {
    this->commandName = commandName;
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
    // Clean up dynamically allocated memory
    delete nextState;
}

// Variable that holds the current state of the game.
State* currentState;

// Function that creates the state and transition objects for the game engine. It also sets the initial state.
void initStateAndTransitions() {
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
    Transition* assignCountries = new Transition("assigncountries", assignReinforcements);
    Transition* issueOrder = new Transition("issueorder", issueOrders);
    Transition* endIssueOrders = new Transition("endissueorders", executeOrders);
    Transition* execOrder = new Transition("execorder", executeOrders);
    Transition* endExecOrders = new Transition("endexecorders", assignReinforcements);
    Transition* winTrans = new Transition("win", win);
    Transition* play = new Transition("play", start);
    Transition* end = new Transition("end", nullptr);

    start->addTransitions({loadMap});
    mapLoaded->addTransitions({loadMap, validateMap});
    mapValidated->addTransitions({addPlayer});
    playersAdded->addTransitions({addPlayer, assignCountries});
    assignReinforcements->addTransitions({issueOrder});
    issueOrders->addTransitions({issueOrder, endIssueOrders});
    executeOrders->addTransitions({execOrder, winTrans, endExecOrders});
    win->addTransitions({play, end});

    currentState = start;
    std::cout << "Current state is " << *currentState << "." << std::endl;
}

// Function that finds transition with given command name. If not found, it displays an error. Otherwise, calls function to perform the state transition.
void findAndTransition(string name) {
    vector<Transition*> transitions = currentState->getTransitions();
    vector<Transition*>::iterator it = std::find_if(transitions.begin(), transitions.end(), 
                                         [&name](Transition* cmd) { return cmd->getCommandName() == name; });
    if (it != transitions.end())
        transition(*it);
    else
        std::cout << "Invalid command for state " << *currentState << "." << std::endl;
}

// Function that sets the current game engine's state to the transition's next state.
void transition(Transition* transition) {
    currentState = transition->getNextState();
    std::cout << "Command `" << transition->getCommandName() << "` has been executed. ";
    if (currentState)
        std::cout << "Current state is now " << *currentState  << "." << std::endl;
    else
        std::cout << "The game cycle has been completed." << std::endl; 
}

//addition for A2: main game loop
void Transition::mainGameLoop() {
    bool gameEnd = false;
    while (!gameEnd) {
        //run game loop
        reinforcementPhase();
        issueOrdersPhase();
        executeOrdersPhase();

        gameEnd =  gameResultCheck();
    }
    cout << "Winner: " << playersList.at(0)->getName() << endl;

}

bool Transition::gameResultCheck() {
    //check if a player has no territories owned, then eliminate him
    auto iterator = playersList.begin();
    while (iterator != playersList.end()) {
        if ((*iterator)->getTerritories().size() < 1) {
            iterator = playersList.erase(iterator);
            continue;
        }
        ++iterator;
    }

    //check if a player owns all the territories
    if (playersList.size() == 1) {
        int numberTerritoriesOwned = playersList.at(0)->getTerritories().size();
        int numberTerritories = 0;
        for (int i = 0; i < gameMap->getContinents().size(); i++) {
            numberTerritories += gameMap->getContinents().at(i)->getTerritory().size();
        }

        if (numberTerritoriesOwned == numberTerritories) {
            return true;
        }
    }

    return false;
}

void Transition::reinforcementPhase() {
    int reinforcement = 0;
    auto iterator = playersList.begin();
    while (iterator != playersList.end()) {
        //number of territories owned by players
        int territoryQuantity = 0; 
        territoryQuantity = (*iterator)->getTerritories().size();
        //reinforcement amount
        reinforcement = floor(territoryQuantity/3);

        int totalTerritories = 0;
        //check if player owns all territories in a continent
        for (int i = 0; i < gameMap->getContinents().size(); i++) {
            //count territories own per continent
            for (int j = 0; j < (*iterator)->getTerritories().size(); j++) {
                if ((*iterator)->getTerritories().at(j)->GetContinentName() == gameMap->getContinents().at(i)->GetName()) {
                    totalTerritories++;
                }
            }
            if (totalTerritories == gameMap->getContinents().at(i)->getTerritory().size()) {
                reinforcement += gameMap->getContinents().at(i)->getBonusValue();
            }
        }

        if (reinforcement < 3) {
            reinforcement = 3;
        }
    }
}

void Transition::issueOrdersPhase() {

}

void Transition::executeOrdersPhase() {

}