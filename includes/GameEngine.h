#pragma once
#include "Map.h"
#include "CommandProcessing.h"
#include "Player.h"
#include "Cards.h"
#include <iostream>
#include <vector>
#include "LoggingObserver.h"
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
    private:
        string commandName;
        State* nextState; 
};

class CommandProcessor; // forward declaration
class TournamentCommand; // forward declaration

// Manages the game setup and execution.
class GameEngine: public Subject, public ILoggable {
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
    void setPlayer(vector<Player*> player);
    CommandProcessor* getCommandProcessor() { return commandProcessor; }
    void setCommandProcessor(CommandProcessor* commandProcessor) {this->commandProcessor = commandProcessor; }

    void addPlayer(Player* player);
    void startupPhase();
    bool isCommandValid(string command);
    void findAndTransition(string name);
    void transition(Transition* transition);
    vector<Player*> getPlayers();
    void setNumOfPlayers(int num);
    int getNumOfPlayers() const;
    void startTournament(TournamentCommand* tournamentCmd);

    GameEngine& operator=(const GameEngine& gameEngine);
    friend std::ostream& operator<<(std::ostream& output, const GameEngine& gameEngine);
        //A2
        void mainGameLoop();
        bool gameResultCheck();
        void reinforcementPhase();
        void issueOrdersPhase();
        void executeOrdersPhase();
        void gameStart();
        void resetGame();

        //method for demo
        void forceGameWin();

        ~GameEngine();

        //Part 2 Logging
        string stringToLog() override;
    private:
        void setDefaultGameStates();
        void selectMode();
        void initProcessor();
        void clearGame();

        vector<State*> states;
        State* currentState;
        Map* currentMap;
        CommandProcessor* commandProcessor;
        string mode;
        vector<Player*> players;
        int reinforcement;
        int numOfPlayers;
        Deck* gameDeck;
        int maxTurns;
        int nbTurnsPlayed;
};

OrderType getOrderType(string str);

//test methods
void testGameStates();
void testMainGameLoop();
void testStartupPhase();
void testTournament();