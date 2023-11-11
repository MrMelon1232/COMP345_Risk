#pragma once
#include "Map.h"
#include "CommandProcessing.h"
#include <iostream>
#include <vector>
#include "LoggingObserver.h"

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

class GameEngine : public Subject, public ILoggable {
    public:
        GameEngine();
        GameEngine(string mode);
        GameEngine(vector<State*> states);
        GameEngine(GameEngine& gameEngine);

        // Getters and setters.
        string getMode() { return mode; }
        void setMode(string mode) { this->mode = mode; }
        State* getCurrentState() { return currentState; }
        Map* getCurrentMap() { return currentMap; }
        void setCurrentMap(Map* map) { this->currentMap = map; }
        CommandProcessor* getCommandProcessor() { return commandProcessor; }
        void setCommandProcessor(CommandProcessor* commandProcessor) {this->commandProcessor = commandProcessor; }

        bool isCommandValid(string command);
        void findAndTransition(string name);
        void transition(Transition *transition);
        
        GameEngine& operator=(const GameEngine& gameEngine);
        friend std::ostream& operator<<(std::ostream& output, const GameEngine& gameEngine);

        ~GameEngine();

        //Part 2 Logging
        string stringToLog() const override;
    private:
        void setDefaultGameStates();
        void selectMode();
        void initProcessor();

        vector<State*> states;
        State* currentState;
        Map* currentMap;
        CommandProcessor* commandProcessor;
        string mode;
};