#include "CommandProcessing.h"
#include <iostream>
#include <algorithm>
#include <stdexcept>
#include <random>

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
    return *this;
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
CommandProcessor::CommandProcessor(GameEngine* gameEngine) : gameEngine(gameEngine) {
    mapLoader = new MapLoader();
}

// Copy constructor of CommandProcessor.
CommandProcessor::CommandProcessor(CommandProcessor& commandProcessor) {
    this->gameEngine = new GameEngine(*(commandProcessor.gameEngine));
    for (Command* command : commands) {
        Command* newCommand = new Command(*command);
        commands.push_back(newCommand);
    }
    mapLoader = new MapLoader(*(commandProcessor.mapLoader));
}

// Returns a pointer to the newly created command (saved too if not nullprt).
Command* CommandProcessor::getCommand() {
    Command* command = readCommand();
    if (command)
        saveCommand(command);
    return command;
}

// Returns a pointer to the new command created from the console.
Command* CommandProcessor::readCommand() {
    cout << "Please enter a command name:" << endl;
    string cmdName;
    cin >> cmdName;

    Command* command = new Command(cmdName);

    if (cmdName == "loadmap" || cmdName == "addplayer") {
        string arg;
        if (cmdName == "loadmap")
            cout << "Please enter a map file to load." << endl;
        else
            cout << "Please enter the player's name." << endl;
        cin >> arg;
        command->setArg(arg);
    }

    return command;
}

// Saves the command into the CommandProcessor's commands list.
void CommandProcessor::saveCommand(Command* command) {
    commands.push_back(command);
}

// Sets the effect according to the command type. If the command is invalid, it sets an invalid effect.
void CommandProcessor::validate(Command* command) {
    string cmdName = command->getName();
    if (!gameEngine->isCommandValid(cmdName)) {
        command->saveEffect("Command `" + command->getName() + "` is invalid in state: `" + gameEngine->getCurrentState()->getName() + "`.");
        return;
    }

    if (cmdName == "loadmap") {
        command->saveEffect("Loading map.");
    }
    else if (cmdName == "validatemap") {
        command->saveEffect("Validating map.");
    }
    else if (cmdName == "addplayer") {
        command->saveEffect("Adding player.");
    }
    else if (cmdName == "gamestart") {
        command->saveEffect("Ending startup phase. Starting play phase.");
    }
    else if (cmdName == "replay") {
        command->saveEffect("Restarting the game.");
    } else if (cmdName == "quit") {
        command->saveEffect("Quitting the game.");
    } else { // if command behavior undefined, simply transition state.
        command->saveEffect("Transitioning to another state.");
    }
}

// Executes the given command. If command is invalid, it just outputs the effect.
void CommandProcessor::executeCommand(Command* command) {
    validate(command);
    if (command->getEffect().find("invalid") != string::npos) {
        cout << command->getEffect() << endl;
        return;
    }

    string cmdName = command->getName();
    if (cmdName == "loadmap") {
        loadMap(command);
    }
    else if (cmdName == "validatemap") {
        validateMap(command);
    }
    else if (cmdName == "addplayer") {
        addPlayer(command);
    }
    else if (cmdName == "gamestart") {
        gameStart(command);
    }
    else if (cmdName == "replay") {
        replay(command);
    }
    else if (cmdName == "quit") {
        gameEngine->findAndTransition(command->getName());
    }
    else { // if command behavior undefined, simply transition state.
        gameEngine->findAndTransition(command->getName());
    }
}

// Helper function for the `loadmap <fileName>` command.
void CommandProcessor::loadMap(Command* command) {
    try {
        gameEngine->setCurrentMap(mapLoader->LoadMap(command->getArg()));
        gameEngine->findAndTransition(command->getName());
    }
    catch (const runtime_error& error) {
        command->saveEffect("Map file not found.");
        cout << "Could not load map file " + command->getArg() + ". State is still `" + gameEngine->getCurrentState()->getName() + "`." << endl;
    }
}

// Helper function for the `validatemap` command
void CommandProcessor::validateMap(Command* command) {
    if (gameEngine->getCurrentMap()->validate()) {
        cout << "Map loaded successfully." << endl;
        gameEngine->findAndTransition(command->getName());
    }
    else {
        cout << "Invalid map or map file. Try loading a different one." << endl;
    }
}

// Helper function for the `addPlayer <playerName>` command.
void CommandProcessor::addPlayer(Command* command) {

    if (gameEngine->getPlayers().size() >= 6) {
        command->saveEffect("Cannot add more players. Maximum limit reached.");
        cout << "Cannot add more players. Maximum limit reached." << endl;
        return;
    }
    string playerName = command->getArg();
    Player* newPlayer = new Player(playerName);
    gameEngine->addPlayer(newPlayer);
    cout << "Player " << playerName << " added successfully." << endl;
    gameEngine->findAndTransition(command->getName());
}

// Helper function to the `gameStart` command.
void CommandProcessor::gameStart(Command* command) {

    if (gameEngine->getPlayers().size() < 2) {
        command->saveEffect("Cannot start the game with less than 2 players.");
        cout << "Cannot start the game with less than 2 players." << endl;
        return;
    }

    // 4.a fairly distribute all the territories to the players
    vector<Territory*> allTerritories = gameEngine->getCurrentMap()->territories;

    // Shuffle the territories randomly
    random_device rd;
    default_random_engine rng(rd());
    shuffle(allTerritories.begin(), allTerritories.end(), rng);

    int playerIndex = 0;
    for (Territory* territory : allTerritories) {
        Player* currentPlayer = gameEngine->getPlayers()[playerIndex];
        currentPlayer->addTerritory(territory);
        territory->setOwner(currentPlayer);
        playerIndex = (playerIndex + 1) % gameEngine->getPlayers().size();
    }

    // 4.b determine randomly the order of play of the players in the game
    // Randomize the order of the how the player is accessed by rnadomizing the vector
    shuffle(gameEngine->getPlayers().begin(), gameEngine->getPlayers().end(), rng);

    // 4.c give 50 initial army units to the players, which are placed in their respective reinforcement pool
    for (Player* player : gameEngine->getPlayers()) {
        player->setReinforcementPool(50);
    }

    // 4.d let each player draw 2 initial cards from the deck using the deck’s draw() method
    for (Player* player : gameEngine->getPlayers()) {
        gameEngine->getGameDeck()->draw(player->getHand());
        gameEngine->getGameDeck()->draw(player->getHand());
    }

    // 4.e switch the game to the play phase
    gameEngine->findAndTransition(command->getName());
}

// Helper function to the `replay` command.
void CommandProcessor::replay(Command* command) {
    cout << "replay()" << endl;
    gameEngine->findAndTransition(command->getName()); // cmdName is "replay"
    // TODO: reset GameEngine variables (i.e., current map, players)
}

// Assignment operator of the CommandProcessor.
CommandProcessor& CommandProcessor::operator=(const CommandProcessor& commandProcessor) {
    // Deallocating existing dynamic memory
    for (Command* command : commands)
        delete command;
    commands.clear();

    for (Command* command : commandProcessor.commands) {
        Command* newCommand = new Command(*command);
        commands.push_back(newCommand);
    }
    return *this;
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
    delete mapLoader;

    // We may want to assign a new CommandProcessor on `replay`, so 
    // the GameEngine is responsible of deleting itself.
}

// FileLineReader constructor to initialize with the file to read.
FileLineReader::FileLineReader(string fileName) : fileName(fileName) {
    file.open(fileName);
    if (file.fail())
        throw std::invalid_argument("Cannot open specified file");
}

// Copy constructor of FileLineReader.
FileLineReader::FileLineReader(FileLineReader& flr) {
    this->fileName = flr.fileName;
    this->file.open(fileName); // We assume the file is valid.
}

// Returns a string containing command information from the next line in the file.
string FileLineReader::readLineFromFile() {
    string commandLine;
    getline(file, commandLine, '\n');
    return commandLine;
}

// Indicates whether the eof flag is set in the ifstream object.
bool FileLineReader::isEof() {
    return file.eof();
}

// Assignment operator of FileLineReader.
FileLineReader& FileLineReader::operator=(const FileLineReader& flr) {
    fileName = flr.fileName;
    file.close();
    file.open(fileName); // We assume the file is valid.
    return *this;
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

// FileCommandProcessorAdapter constructor to initialize with the given FileLineReader.
FileCommandProcessorAdapter::FileCommandProcessorAdapter(GameEngine* gameEngine, FileLineReader* flr) : CommandProcessor(gameEngine) {
    this->flr = flr;
}

// Copy constructor of FileCommandProcessorAdapter.
FileCommandProcessorAdapter::FileCommandProcessorAdapter(FileCommandProcessorAdapter& fileCmdProcAdapter) : CommandProcessor(fileCmdProcAdapter) {
    this->flr = new FileLineReader(*(fileCmdProcAdapter.flr));
}

// Returns a pointer to the newly created command by the FileLineReader.
Command* FileCommandProcessorAdapter::readCommand() {
    string commandLine = flr->readLineFromFile();

    if (flr->isEof()) // No more commands to read.
        return nullptr;

    int spaceIndex = commandLine.find(' ');
    string cmdName = commandLine.substr(0, spaceIndex);

    Command* command = new Command(cmdName);
    if (cmdName == "loadmap" || cmdName == "addPlayer")
        command->setArg(commandLine.substr(spaceIndex + 1));
    return command;
}

// Assignment operator of FileCommandProcessorAdapter.
FileCommandProcessorAdapter& FileCommandProcessorAdapter::operator=(const FileCommandProcessorAdapter& fileCmdProcAdapter) {
    CommandProcessor::operator=(fileCmdProcAdapter);
    delete flr;
    flr = new FileLineReader(*(fileCmdProcAdapter.flr));
    return *this;
}

// Stream insertion operator overload of FileCommandProcessorAdapter. Displays the file name the FileLineReader is reading from.
std::ostream& operator<<(std::ostream& output, const FileCommandProcessorAdapter& fileCmdProcAdapter) {
    output << *(fileCmdProcAdapter.flr);
    return output;
}

// FileCommandProcessorAdapter's destructor.
FileCommandProcessorAdapter::~FileCommandProcessorAdapter() {
    delete flr;
}
