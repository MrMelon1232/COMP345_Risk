#include "GameEngine.h"
#include <string>

// Allows the user to test the GameEngine's states and transitions in a console-driven application
void testGameStates() {
    GameEngine* gameEngine = new GameEngine("-console");

    while (gameEngine->getCurrentState()) {
        std::string cmdName;
        std::cin >> cmdName;
        gameEngine->findAndTransition(cmdName);
    }

    delete gameEngine;
}

void testMainGameLoop() {
	cout << "\n\n---- TEST FOR MAIN GAME LOOP ----\n" << endl;

	vector<Player*> players;
	vector<Continent*> continents;
	vector<Territory*> territories;

	cout << "Creating necessary dummy values for test" << endl;
	//create new players
	Player* p1 = new Player();
	Player* p2 = new Player();
	Player* p3 = new Player();
	players.push_back(p1);
	players.push_back(p2);
	players.push_back(p3);
	//create new continents
	Continent* c1 = new Continent("c1");
	Continent* c2 = new Continent("c2");
	Continent* c3 = new Continent("c3");
	continents.push_back(c1);
	continents.push_back(c2);
	continents.push_back(c3);
	//create new territories
	Territory* t1 = new Territory("t1", "c1", p1->getPlayerID());
	Territory* t2 = new Territory("t2", "c1", p1->getPlayerID());
	Territory* t3 = new Territory("t3", "c1", p1->getPlayerID());
	Territory* t4 = new Territory("t4", "c2", p1->getPlayerID());
	Territory* t5 = new Territory("t5", "c2", p2->getPlayerID());
	Territory* t6 = new Territory("t6", "c2", p2->getPlayerID());
	Territory* t7 = new Territory("t7", "c3", p2->getPlayerID());
	Territory* t8 = new Territory("t8", "c3", p3->getPlayerID());
	Territory* t9 = new Territory("t9", "c3", p3->getPlayerID());
	Territory* t10 = new Territory("t10", "c3", p3->getPlayerID());
	territories.push_back(t1);
	territories.push_back(t2);
	territories.push_back(t3);
	territories.push_back(t4);
	territories.push_back(t5);
	territories.push_back(t6);
	territories.push_back(t7);
	territories.push_back(t8);
	territories.push_back(t9);
	territories.push_back(t10);
	//assign territories to continents
	for (int i = 0; i < territories.size(); i++) {
		if (i < 4) {
			c1->addTerritory(territories.at(i));
			territories.at(i)->AddAdjacency(territories.at(i), t2);
			territories.at(i)->AddAdjacency(territories.at(i), t5);
			territories.at(i)->AddAdjacency(territories.at(i), t7);
		}
		else if (i > 3 && i < territories.size()) {
			c2->addTerritory(territories.at(i));
		}
		else {
			c3->addTerritory(territories.at(i));
		}
	}
	//assign territories to players
	for (int i = 0; i < territories.size(); i++) {
		if (i < 4) {
			players.at(0)->addTerritory(territories.at(i));
		}
		else {
			players.at(1)->addTerritory(territories.at(i));
		}
	}
	Map* map = new Map(continents, territories);


	cout << "creating game engine" << endl;
	//create game engine
	GameEngine* game = new GameEngine();
	//
	Card* c = new Card("ADVANCE");
	p1->getHand()->addHand(c);
	//setup
	game->setPlayer(players);
	game->setCurrentMap(map);
	game->gameResultCheck();
	game->mainGameLoop();
}

void testStartupPhase() {
    GameEngine* gameEngine = new GameEngine();

    // Call the startup phase
    std::cout << "Testing startup phase..." << std::endl;

	try {
		gameEngine->startupPhase();
	} catch (const std::invalid_argument& e) {
		cout << "No more commands to read. Ending test." << endl;
	}

    delete gameEngine;
}