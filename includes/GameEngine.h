#pragma once
#include <iostream>
#include <vector>
#include <Player.h>

using std::vector;
using std::string;

class Transition;
class Player;

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

        //A2
        void mainGameLoop();
        bool gameResultCheck();
        void reinforcementPhase();
        void issueOrdersPhase();
        void executeOrdersPhase();
    private:
        string commandName;
        State* nextState; 
        vector<Player*> playersList;
        Map* gameMap;
        
};

void initStateAndTransitions();
void findAndTransition(string name);
void transition(Transition *transition);

extern State* currentState;

//test methods
void testGameStates();
void testMainGameLoop();