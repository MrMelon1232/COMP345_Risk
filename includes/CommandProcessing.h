#pragma once
#include "GameEngine.h"
#include "Map.h"
#include "Player.h"
#include <iostream>
#include <vector>
#include "LoggingObserver.h"

using std::string;
using std::vector;

// Represents an in-game command.
class Command: public ILoggable, public Subject {
    public:
        Command(string cmdName);
        Command(string cmdName, string arg);
        Command(Command& command);

        // Getter and setters.
        string getEffect() { return effect; }
        void saveEffect(string effect);
        string getName() { return cmdName; }
        void setName(string cmdName) { this->cmdName = cmdName; }
        string getArg() { return arg; };
        void setArg(string arg) { this->arg = arg; }

        Command& operator=(const Command& command);
        friend std::ostream& operator<<(std::ostream& output, const Command& command);

        virtual ~Command();

        //Part 2 Loggable
        string stringToLog() override;
    private:
        string cmdName;
        string effect;
        string arg; // For the tournament command, this represents the whole command line.
};

// Represents a tournament command.
class TournamentCommand : public Command {
    public:
        TournamentCommand(string arg);

        vector<vector<string>> table;
        vector<string> mapFiles;
        vector<StrategyType> playerStrats;
        int nbGames;
        int maxTurnsPerGame;

        const char* ToString(StrategyType type);

        TournamentCommand& operator=(const TournamentCommand& command);
        friend std::ostream& operator<<(std::ostream& output, TournamentCommand& command);

        ~TournamentCommand();

        string stringToLog() override;
        void saveResult(string str);

        string str;
};

class GameEngine; // forward declaration

// Manages game commands including creation, validation and execution.
class CommandProcessor: public ILoggable, public Subject {
    public:
        CommandProcessor(GameEngine* gameEngine);
        CommandProcessor(CommandProcessor& commandProcessor);

        Command* getCommand();
        void validate(Command* command);
        void executeCommand(Command* command);
        vector<Command*> getCommands() { return commands; }
        MapLoader* getMapLoader() { return this->mapLoader; }

        CommandProcessor& operator=(const CommandProcessor& commandProcessor);
        friend std::ostream& operator<<(std::ostream& output, const CommandProcessor& commandProcessor);

        virtual ~CommandProcessor();

        // Part 2 Logging
        string stringToLog() override;

    private:
        virtual Command* readCommand();
        void saveCommand(Command* command);
        // helper validation method.
        bool validateTournament(Command* command);

        // helper methods to manage command execution
        void tournament(Command* command);
        void loadMap(Command* command);
        void validateMap(Command* command);
        void addPlayer(Command* command);
        void gameStart(Command* command);
        void replay(Command* command);

        GameEngine* gameEngine;
        vector<Command*> commands;
        MapLoader* mapLoader;
};

// File reader responsible of reading text-based commands.
class FileLineReader {
    public:
        FileLineReader(string fileName);
        FileLineReader(FileLineReader& flr);

        string getFileName() { return fileName; };
        string readLineFromFile();
        bool isEof();

        FileLineReader& operator=(const FileLineReader& flr);
        friend std::ostream& operator<<(std::ostream& output, const FileLineReader& flr);

        ~FileLineReader();
    private:
        string fileName;
        std::ifstream file;
};

// Adapter of the CommandProcessor to read commands from a file.
class FileCommandProcessorAdapter : public CommandProcessor {
    public:
        FileCommandProcessorAdapter(GameEngine* gameEngine, FileLineReader* flr);
        FileCommandProcessorAdapter(FileCommandProcessorAdapter& fileCmdProcAdapter);

        FileLineReader* getFileLineReader() { return flr; };
        Command* readCommand();

        FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& fileCmdProcAdapter);
        friend std::ostream& operator<<(std::ostream& output, const FileCommandProcessorAdapter& fileCmdProcAdapter);

        ~FileCommandProcessorAdapter();
    private:
        FileLineReader* flr;
};