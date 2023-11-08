#pragma once
#include "Map.h"
#include "CommandProcessing.h"
#include "Player.h"
#include <iostream>
#include <vector>

using std::vector;
using std::string;

class Transition;

// Represents a state of the game with its valid commands
class State {
public:
    State(string name);
    State(State& state);
    string getName();
    vector<Transition*> getTransitions();
    void addTransitions(vector<Transition*> transitions...);
    State& operator=(const State& state);
    friend std::ostream& operator<<(std::ostream& output, const State& state);
    ~State();
    
private:
    string name;
    vector<Transition*> transitions;
};

// Holds the next state to transition to after executing the command with the given name
class Transition {
public:
    Transition(string commandName, State* nextState);
    Transition(Transition& transition);
    string getCommandName();
    State* getNextState();
    Transition& operator=(const Transition& transition);
    friend std::ostream& operator<<(std::ostream& output, const Transition& transition);
    ~Transition();
private:
    string commandName;
    State* nextState;
};

class CommandProcessor; // forward declaration

class GameEngine {
public:
    GameEngine();
    GameEngine(vector<State*> states);
    GameEngine(GameEngine& gameEngine);
    vector<Player*> players;

    MapLoader* mapLoader;


    vector<State*> getStates() { return states; };
    State* getCurrentState() { return currentState; };
    Map* getCurrentMap() { return currentMap; };
    void setCurrentMap(Map* map) { this->currentMap = map; };
    void setCommandProcessor(CommandProcessor* commandProcessor) { this->commandProcessor = commandProcessor; };
    void addPlayer(Player* player);

    bool isCommandValid(string command);
    void findAndTransition(string name);
    void transition(Transition* transition);

    GameEngine& operator=(const GameEngine& gameEngine);
    friend std::ostream& operator<<(std::ostream& output, const GameEngine& gameEngine);

    ~GameEngine();
    void startupPhase();
    void distributeTerritories(int num);
private:
    vector<State*> states;
    State* currentState;
    Map* currentMap;
    CommandProcessor* commandProcessor;
    vector<Player*> players;
};
