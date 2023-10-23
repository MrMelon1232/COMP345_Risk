#pragma once
#include <iostream>
#include <vector>

using std::string;
using std::vector;

// Represents an in-game command.
class Command {
    public:
        Command(string cmdName);
        Command(Command& command);

        // Getter and setters.
        string getEffect() { return effect; };
        void saveEffect(string effect) { this->effect = effect; };
        string getName() { return cmdName; };
        string getArg() { return arg; };
        void setArg(string arg) { this-> arg = arg; };

        Command& operator=(const Command& command);
        friend std::ostream& operator<<(std::ostream& output, const Command& command);

        ~Command();
    private:
        string cmdName;
        string effect;
        string arg;
};

// Manages game commands including creation, validation and execution.
class CommandProcessor {
    public:
        CommandProcessor();
        CommandProcessor(CommandProcessor& commandProcessor);

        Command* getCommand();
        void validate(Command* command);
        void executeCommand(Command* command);

        CommandProcessor& operator=(const CommandProcessor& commandProcessor);
        friend std::ostream& operator<<(std::ostream& output, const CommandProcessor& commandProcessor);

        virtual ~CommandProcessor();
    private:
        virtual Command* readCommand();
        void saveCommand(Command* command);

        // helper methods to manage command execution
        void loadMap(Command* command);
        void validateMap(Command* command);
        void addPlayer(Command* command);
        void gameStart(Command* command);
        void replay(Command* command);

        vector<Command*> commands;
};

// File reader adapted to create commands.
class FileLineReader {
    public:
        FileLineReader(string fileName);
        FileLineReader(FileLineReader& flr);

        Command* readLineFromFile();

        FileLineReader& operator=(const FileLineReader& flr);
        friend std::ostream& operator<<(std::ostream& output, const FileLineReader& flr);

        ~FileLineReader();
    private:
        string fileName;
        std::ifstream file;
};

// Adapter of the CommandProcessor to read commands from a file.
class FileCommandProcessorAdapter : CommandProcessor {
    public:
        FileCommandProcessorAdapter(string fileName);
        FileCommandProcessorAdapter(FileCommandProcessorAdapter& fileCmdProcAdapter);

        Command* readCommand();

        FileCommandProcessorAdapter& operator=(const FileCommandProcessorAdapter& fileCmdProcAdapter);
        friend std::ostream& operator<<(std::ostream& output, const FileCommandProcessorAdapter& fileCmdProcAdapter);

        ~FileCommandProcessorAdapter();
    private:
        string fileName;
        FileLineReader* flr;
};