#include "CommandProcessing.h"
#include "GameEngine.h"
#include "Map.h"
#include <iostream>

using std::cin;
using std::cout;
using std::endl;

// Command constructor to initialize with name.
Command::Command(string cmdName) : cmdName(cmdName) {}

// Copy constructor of Command.
Command::Command(Command& command) {
    cmdName = command.cmdName;
    effect = command.effect;
    arg = command.arg;
}

// Assignment operator of Command.
Command& Command::operator=(const Command& command) {
    cmdName = command.cmdName;
    effect = command.effect;
    arg = command.arg;
}

// Stream insertion operator overload to display Command information.
std::ostream& operator<<(std::ostream& output, const Command& command) {
    cout << "Command name: `" << command.cmdName << "`, effect: `"
        << command.effect << "`, arg: `" << command.arg << "`.";
    return output;
}

// Command's destructor.
Command::~Command() {}

// Default constructor of CommandProcessor.
CommandProcessor::CommandProcessor() {}

// Copy constructor of CommandProcessor.
CommandProcessor::CommandProcessor(CommandProcessor& commandProcessor) {
    for (Command* command : commands) {
        Command* newCommand = new Command(*command);
        commands.push_back(newCommand);
    }
}

// Returns a pointer to the newly created command (validation and save included).
Command* CommandProcessor::getCommand() {
    Command* command = readCommand();
    validate(command);
    saveCommand(command);
    return command;
}

// Returns a pointer to the new command created from the console.
Command* CommandProcessor::readCommand() {
    cout << "Please enter a command name:" << endl;
    string cmdName;
    cin >> cmdName;

    Command* command = new Command(cmdName);

    if (cmdName == "loadmap" || cmdName == "addPlayer") {
        string arg;
        if (cmdName == "loadmap")
            cout << "Please enter a map file to load.";
        else
            cout << "Please enter the player's name.";
        cin >> arg;
        command->setArg(arg);
    }

    return command;
}

// Sets the effect according to the command type. If the command is invalid, it sets an invalid effect.
void CommandProcessor::validate(Command* command) {
    string cmdName = command->getName();
    if (!isCommandValid(cmdName)) {
        command->saveEffect("Command `" + command->getName() + "` is invalid in state: `" + currentState->getName() + "`.");
        return;
    }

    if (cmdName == "loadmap") {
        command->saveEffect("Loading map.");
    } else if (cmdName == "validatemap") {
        command->saveEffect("Validating map.");
    } else if (cmdName == "addplayer") {
        command->saveEffect("Adding player.");
    } else if (cmdName == "gamestart") {
        command->saveEffect("Ending startup phase. Starting play phase.");
    } else if (cmdName == "replay") {
        command->saveEffect("Restarting the game.");
    } else if (cmdName == "quit") {
        command->saveEffect("Quitting the game");
    }
}

// Saves the command into the CommandProcessor's commands list.
void CommandProcessor::saveCommand(Command* command) {
    commands.push_back(command);
}

// Executes the given command. If command is invalid, it just outputs the effect.
void CommandProcessor::executeCommand(Command* command) {
    string effect = command->getEffect();
    cout << effect << endl;
    if (effect.find("invalid") != string::npos) {
        return;
    }

    string cmdName = command->getName();
    
    if (cmdName == "loadmap") {
        loadMap(command);
    } else if (cmdName == "validatemap") {
        validateMap(command);
    } else if (cmdName == "addplayer") {
        addPlayer(command);
    } else if (cmdName == "gamestart") {
        gameStart(command);
    } else if (cmdName == "replay") {
        replay(command);
    } else if (cmdName == "quit") {
        findAndTransition(command->getName());
    }
}

// Helper function for the `loadmap <fileName>` command.
void CommandProcessor::loadMap(Command* command) {
    try {
        currentMap = mapLoader->LoadMap(command->getArg());
        findAndTransition(command->getName());
    } catch (const runtime_error& error) {
        cout << "Could not load map file " + command->getArg() + ". State is still `" + currentState->getName() + "`.";
    }
}

// Helper function for the `validatemap` command
void CommandProcessor::validateMap(Command* command) {
    if (currentMap->validate())
        cout << "Invalid map or map file. Try loading a different one.";
    else
        cout << "Map loaded successfully.";
    findAndTransition(command->getName());
}

// Helper function for the `addPlayer <playerName>` command.
void CommandProcessor::addPlayer(Command* command) {

}

// Helper function to the `gameStart` command.
void CommandProcessor::gameStart(Command* command) {

}

// Helper function to the `replay` command.
void CommandProcessor::replay(Command* command) {
    findAndTransition(command->getName()); // cmdName is "replay"
    delete currentMap;
    currentMap = nullptr;
    // TODO: reset other GameEngine variables (i.e., players)
}

// Assignment operator of the CommandProcessor.
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& commandProcessor) {
    // Deallocating existing dynamic memory
    for (Command* command : commands)
        delete command;
    commands.clear();

    for (Command* command : commands) {
        Command* newCommand = new Command(*command);
        commands.push_back(newCommand);
    }
}

// Stream insertion operator overload for the CommandProcessor. Prints all the commands in its list.
std::ostream& operator<<(std::ostream& output, const CommandProcessor& commandProcessor) {
    output << "Here's a list of all commands in the CommandProcessor:" << endl;
    for (Command* command : commandProcessor.commands)
        cout << *command << endl;
    return output;
}

// CommandProcessor's destructor.
CommandProcessor::~CommandProcessor() {
    for (Command* command : commands)
        delete command;
}

// FileLineReader constructor to initialize with the file to read.
FileLineReader::FileLineReader(string fileName) : fileName(fileName) {
    file.open(fileName);
}

// Copy constructor of FileLineReader.
FileLineReader::FileLineReader(FileLineReader& flr) {
    this->fileName = flr.fileName;
    this->file.open(fileName);
}

// Returns a pointer to the newly craeted command from the next line in the file.
Command* FileLineReader::readLineFromFile() {
    string commandLine;
    getline(file, commandLine, '\n');
    // TODO: check if no more lines to read?
    string cmdName = commandLine.substr(0, commandLine.find(' '));

    Command* command = new Command(cmdName);
    if (cmdName == "loadmap" || cmdName == "addPlayer")
        command->setArg(commandLine.substr(1));
    return command;
}

// Assignment operator of FileLineReader.
FileLineReader& FileLineReader::operator=(const FileLineReader& flr) {
    fileName = flr.fileName;
    file.close();
    file.open(fileName);
}

// Stream insertion operator of FileLineReader. Displays the file name it's reading from.
std::ostream& operator<<(std::ostream& output, const FileLineReader& flr) {
    output << "File name: " << flr.fileName;
    return output;
}

// FileLineReader's destructor.
FileLineReader::~FileLineReader() {
    file.close();
}

// FileCommandProcessorAdapter constructor to initialize with the file name to pass to the FileLineReader.
FileCommandProcessorAdapter::FileCommandProcessorAdapter(string fileName) {
    flr = new FileLineReader(fileName);
}

// Copy constructor of FileCommandProcessorAdapter.
FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileCommandProcessorAdapter& fileCmdProcAdapter) : CommandProcessor(fileCmdProcAdapter) {
    this->fileName = fileCmdProcAdapter.fileName;
    this->flr = new FileLineReader(fileName);
}

// Returns a pointer to the newly created command by the FileLineReader.
Command* FileCommandProcessorAdapter::readCommand() {
    // TODO: check if no more lines to read?
    return flr->readLineFromFile();
}

// Assignment operator of FileCommandProcessorAdapter.
FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& fileCmdProcAdapter) {    
    CommandProcessor::operator=(fileCmdProcAdapter);
    fileName = fileCmdProcAdapter.fileName;
    delete flr;
    flr = new FileLineReader(fileCmdProcAdapter.fileName);
}

// Stream insertion operator overload of FileCommandProcessorAdapter. Displays the file name the FileLineReader is reading from.
std::ostream& operator<<(std::ostream& output, const FileCommandProcessorAdapter& fileCmdProcAdapter) {
    output << "File name: " << fileCmdProcAdapter.fileName;
    return output;
}

// FileCommandProcessorAdapter's destructor.
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
    flr = nullptr;
}