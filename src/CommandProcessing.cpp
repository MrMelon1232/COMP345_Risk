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


// Command constructor to initialize with name and arg.
Command::Command(string cmdName, string arg) : cmdName(cmdName), arg(arg) {}

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

// Save effect function
void Command::saveEffect(string effect) {
    this->effect = effect; 
    notify(*this);
}

// Command's destructor.
Command::~Command() {}

// Tournament's constructor.
TournamentCommand::TournamentCommand(string arg) : Command("tournament", arg) {}

TournamentCommand& TournamentCommand::operator=(const TournamentCommand& tournamentCommand) {
    Command::operator=(tournamentCommand);

    nbGames = tournamentCommand.nbGames;
    maxTurnsPerGame = tournamentCommand.maxTurnsPerGame;

    mapFiles.clear();
    for (string mapFile : tournamentCommand.mapFiles)
        mapFiles.push_back(mapFile);

    playerStrats.clear();
    for (StrategyType strat : tournamentCommand.playerStrats)
        playerStrats.push_back(strat);

    return *this;
}
std::ostream& operator<<(std::ostream& output, TournamentCommand& command)
{
    /*// TODO: insert return statement here
    output << "Tournament mode:\nM:";
    //map list
    for (int i = 0; i < command.mapFiles.size(); i++) {
        output << command.mapFiles.at(i);
    }
    //player list
    output << "\nP: " << endl;
    for (int i = 0; i < command.playerStrats.size(); i++) {
        //need to convert enum to string
        output << command.ToString(command.playerStrats.at(i));
    }
    //game and turn amount
    output << "\nG: " << command.nbGames << "\nD: " << command.maxTurnsPerGame << endl;
    //result table
    output << "\nResults:" << endl;
    for (int i = 0; i < command.table.size(); i++) {
        //output
        if (i != 0) {
            //Map
            output << "\n" << command.table[i].at(0);
        }
        for (int j = 0; j < command.nbGames; j++) {
            //first line
            if (i == 0 && j == 0) {
                output << "\t";
            }
            else if (i == 0 && j != 0) {
                output << "Game " << j << "\t";
            }
            //result per map
            else if (j != 0) {
                output << "\t" << command.table[i].at(j);
            }
        }
    }*/
    return output;
}


void TournamentCommand::saveResult(string str) {
    this->str = str;
    notify(*this);
}

string TournamentCommand::getMapName(string mapFile)
{
    string map = mapFile.substr(mapFile.rfind("/") + 1);
    return map;
}


const char* TournamentCommand::ToString(StrategyType type)
{
    switch (type)
    {
    case StrategyType::Human:   return "Human"; 
    case StrategyType::Aggressive:   return "Aggressive";
    case StrategyType::Benevolent:   return "Benevolent";
    case StrategyType::Neutral:   return "Neutral";
    case StrategyType::Cheater:   return "Cheater";
    default:      return "[Unknown OS_type]";
    }

}

// Tournament's destructor.
TournamentCommand::~TournamentCommand() {

}

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

    if (cmdName == "tournament") {
        string arg;
        std::getline(std::cin, arg);
        return new TournamentCommand(arg);
    }

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
    notify(*command);
}

// Sets the effect according to the command type. If the command is invalid, it sets an invalid effect.
void CommandProcessor::validate(Command* command) {
    if (!command) { // empty command (i.e., no more commands in file)
        throw std::invalid_argument( "No more commands to execute.");
    }

    string cmdName = command->getName();
    if (!gameEngine->isCommandValid(cmdName)) {
        command->saveEffect("Command `" + command->getName() + "` is invalid in state: `" + gameEngine->getCurrentState()->getName() + "`.");
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
    } else if (cmdName == "tournament") {
        if (validateTournament(command))
            command->saveEffect("Starting tournament.");
        else
            command->saveEffect("The tournament command format is invalid.");
    } else { // if command behavior undefined, simply transition state.
        command->saveEffect("Transitioning to another state.");
    }
}

bool CommandProcessor::validateTournament(Command* command) {
    TournamentCommand* tournamentCmd = dynamic_cast<TournamentCommand*>(command);
    if (tournamentCmd == nullptr) { // shouldn't happen if the readCommand was implemented properly...
        cout << "Something went wrong." << endl;
        return false;
    }

    string cmdLine = tournamentCmd->getArg();
    if (cmdLine == "") {
        cout << "Invalid command: no arguments provided for the tournament." << endl;
        return false;
    }

    // Retrive number of turns.
    int nbTurns;
    int turnsIndex = cmdLine.find("-D");
    try {
        string nbTurnsStr = cmdLine.substr(turnsIndex+3);
        nbTurns = std::stoi(nbTurnsStr);
    } catch (const std::invalid_argument& e) {
        cout << "Invalid number of turns per game. " << 
            "Please use `tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>`" << endl;
        return false;
    }

    if (nbTurns < 10 || nbTurns > 50) {
         cout << "Number of turns should be between 10 and 50 inclusively. " << endl;
        return false;
    }
    
    // Retrieve number of games.
    int nbGames;
    int gamesIndex = cmdLine.find("-G");
    try {
        string nbGamesStr = cmdLine.substr(gamesIndex+3, turnsIndex);
        nbGames = std::stoi(nbGamesStr);
    } catch (const std::invalid_argument& e) {
        cout << "Invalid number of games. " << 
            "Please use `tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>`" << endl;
        return false;
    }

    if (nbGames < 1 || nbGames > 5) {
         cout << "Number of games should be between 1 and 5 inclusively. " << endl;
        return false;
    }
    //


    // Retrieve player strats
    int stratIndex = cmdLine.find("-P");
    string stratsStr = cmdLine.substr(stratIndex+3, gamesIndex-stratIndex-3);
    vector<StrategyType> playerStrats;
    int index = 0;
    while ((index = stratsStr.find(" ")) != std::string::npos) {
        string strat = stratsStr.substr(0, index);

        StrategyType playerStrat = getStrategyType(strat);
        if (StrategyType::None != playerStrat && StrategyType::Human != playerStrat)
            playerStrats.push_back(playerStrat);
        else
            cout << "Strat `" << strat << "` is an invalid player strat, skipping." << endl;

        stratsStr.erase(0, index + 1);
    }

    if (playerStrats.size() == 0) {
        cout << "Invalid player strategies. " << 
            "Please use `tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>`" << endl;
        return false;
    } else if (playerStrats.size() < 2 || playerStrats.size() > 4) {
        cout << "Number of strats should be between 2 and 4 inclusively and Human strategy is not allowed in tournament mode." << endl;
        return false;
    }

    // Retrieve list of maps
    int mapIndex = cmdLine.find("-M");
    string mapsStr = cmdLine.substr(mapIndex+3, stratIndex-mapIndex-3);
    vector<string> mapFiles;
    index = 0;
    while ((index = mapsStr.find(" ")) != std::string::npos) {
        string mapFile = mapsStr.substr(0, index);
        mapFiles.push_back(mapFile);
        mapsStr.erase(0, index + 1);
    }

    // Since map files could be removed during execution, file paths will be validated during execution.
    if (mapFiles.size() == 0) {
        cout << "Invalid map files." <<
            "Please use `tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>`" << endl;
        return false;
    } else if (mapFiles.size() > 5) {
        cout << "Number of maps should be between 1 and 5 inclusively and Human strategy is not allowed in tournament mode." << endl;
        return false;
    }

    tournamentCmd->mapFiles = mapFiles;
    tournamentCmd->playerStrats = playerStrats;
    tournamentCmd->nbGames = nbGames;
    tournamentCmd->maxTurnsPerGame = nbTurns;
    return true;
}

// Executes the given command. If command is invalid, it just outputs the effect.
void CommandProcessor::executeCommand(Command* command) {
    validate(command);
    if (command->getEffect().find("invalid") != string::npos) {
        cout << command->getEffect() << endl;
        return;
    }

    string cmdName = command->getName();

    if (cmdName == "tournament") {
        tournament(command);
    } else if (cmdName == "loadmap") {
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
        gameEngine->findAndTransition(command->getName());
    } else { // if command behavior undefined, simply transition state.
        gameEngine->findAndTransition(command->getName());
    }
}

// Helper function for the `tournament -M <listofmapfiles> -P <listofplayerstrategies> -G <numberofgames> -D <maxnumberofturns>` command.
void CommandProcessor::tournament(Command* command) {
    TournamentCommand* tournamentCmd = dynamic_cast<TournamentCommand*>(command);
    if (tournamentCmd == nullptr) { // shouldn't happen if the readCommand was implemented properly...
        cout << "Something went wrong. Finishing tournament." << endl;
        return;
    }

    gameEngine->findAndTransition(tournamentCmd->getName());
    gameEngine->startTournament(tournamentCmd);
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

    // 4 a to d
    gameEngine->gameStart();

    // 4.e switch the game to the play phase
    gameEngine->findAndTransition(command->getName());
}

// Helper function to the `replay` command.
void CommandProcessor::replay(Command* command) {
    gameEngine->findAndTransition(command->getName()); // cmdName is "replay"
    gameEngine->resetGame();
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

    // If command is tournament, create instance of TournamentCommand
    if (cmdName == "tournament") {
        string arg = commandLine.substr(10);
        return new TournamentCommand(arg);
    }

    Command* command = new Command(cmdName);
    if (cmdName == "loadmap" || cmdName == "addplayer")
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

// Part 2
string CommandProcessor::stringToLog() {
    return "test";
}

string TournamentCommand::stringToLog()
{
    string str;
    str = "Tournament mode:\nM: ";
    //map list
    for (int i = 0; i < mapFiles.size(); i++) {
        str += getMapName(mapFiles.at(i));
        str += ",\t";
    }
    //player list
    str += "\nP: ";
    for (int i = 0; i < playerStrats.size(); i++) {
        //need to convert enum to string
        str += ToString(playerStrats.at(i));
        str += ",\t";
    }
    //game and turn amount
    str += "\nG: " + to_string(nbGames) + "\nD: " + to_string(maxTurnsPerGame) + "\n";
    //result table
    str += "\nResults:\n\t";
    for (int i = 1; i <= nbGames; i++) {
        str += "Game " + to_string(i) + "\t";
    }
    
    for (int i = 0; i < table.size(); i++) {
        //map
        str += "\n" + table[i].at(0) + " " + to_string(i+1);
        for (int j = 0; j < table[i].size(); j++) {
            cout << table[i].at(j) << "\t";
            if (j != 0) {
                str += "\t" + table[i].at(j);
            }
        }
    }
    str += "\n\n";

    return str;
}

string Command::stringToLog() {
    return "Command: " + cmdName + ", Effect: " + effect;
}
