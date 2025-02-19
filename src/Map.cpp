#include "Map.h"

using namespace std;

// Continent class constructor
Continent::Continent(const string& name) : name(name) {}



// Copy constructor
Territory :: Territory(const Territory& other) {
    name = other.name;
    continentName = other.continentName;
    adjacentTerritories = other.adjacentTerritories;
};


// Class assignment operator
Territory& Territory :: operator=(const Territory& other) {
    if(this == &other) {
        return *this;
    }

    name = other.name;
    continentName = other.continentName;
    adjacentTerritories = other.adjacentTerritories;

    return *this;
};
// Stream insertion operator
std::ostream& operator<<(std::ostream& os, Territory& territory) {
    os << "Territory name: " << territory.GetName() << std::endl;
    os << "Territory continent name: " << territory.GetContinentName() << std::endl;
    os << "Territory adjacent territories: ";
    vector<string> adjacentTerritories;

    for (int i = 0; i < territory.GetAdjacentTerritories().size(); i++) {
        adjacentTerritories.push_back(territory.GetAdjacentTerritories().at(i)->GetName());
    }

    for (string adjacent : adjacentTerritories) {
        os << adjacent << ", ";
    }
    os << std::endl;
    
    os << "Territory number of armies: " << territory.getNbArmies() << std::endl;

    return os;
}

// GetName method
const string& Continent::GetName() const {
    return name;
}

void Continent::addTerritory(Territory* t)
{
    territoryList.push_back(t);
}

vector<Territory*> Continent::getTerritory()
{
    return territoryList;
}

int Continent::getBonusValue()
{
    return bonusValue;
}


// Territory class constructor
Territory::Territory(const string& name, const string& continentName)
    : name(name), continentName(continentName), nbArmies(0), ownerID(-1) {}

//NEW: Territory class constructor (belongs to a player)
Territory::Territory(const string& name, const string& continentName, int playerID) 
    :name(name), continentName(continentName), nbArmies(0), ownerID(playerID) {}

// GetName method
string& Territory::GetName() {
    return name;
}

// GetContinentName method
const string& Territory::GetContinentName() const {
    return continentName;
}

// AddAdjacentTerritory method
void Territory::AddAdjacentTerritory(Territory* adjacent) {
    adjacentTerritories.push_back(adjacent);
}

// GetAdjacentTerritories method
vector<Territory*> Territory::GetAdjacentTerritories() {
    return adjacentTerritories;
}

void Territory::AddAdjacency(Territory* territory1, Territory* territory2) {
    territory1->AddAdjacentTerritory(territory2);
    territory2->AddAdjacentTerritory(territory1);
}

// Return number of armies from the player owning this territory
const int Territory::getNbArmies() const {
    return nbArmies;
}

// Update the number of armies in the territory
void Territory::setNbArmies(int nbArmies) {
    this->nbArmies = nbArmies;
}

// Implement the getOwner and setOwner functions
Player* Territory::getOwner() const {
    return owner;
}

void Territory::setOwner(Player* newOwner) {
    owner = newOwner;
}

//NEW: accessor method + mutator method for attribute ownerID

//returns the ownerID of the territory
int Territory::getOwnerID() {
    return ownerID;
}

//set the ownerID of the territory
void Territory::setOwnerID(int playerID) {
    ownerID = playerID;
}

//NEW: boolean method which returns true if the territory passed is adjacent
bool Territory::isAdjacent(Territory* territory)
{
    vector<string> adjacentTerritories;

    for (int i = 0; i < (*territory).GetAdjacentTerritories().size(); i++) {
        adjacentTerritories.push_back((*territory).GetAdjacentTerritories().at(i)->GetName());
    }
    for (const string adjacentTerritory : adjacentTerritories) 
    {
        //cout << adjacentTerritory << " comparing " << territory->GetName() << endl;;
        if (adjacentTerritory == name)
        {
            return true;
        }
    }    
    return false;
}

// Map class methods


Map::Map(const vector<Continent*> continents, const vector<Territory*> territories) {

    this->continents = continents;
    this->territories = territories;

    for (Territory* territory : territories) {
        adjacencyList[territory->GetName()] = {}; // Initialize with an empty vector
    }

    // Populate the adjacency list based on the adjacency information in territories
    for (Territory* territory : territories) {
        territoryToContinent[territory->GetName()] = territory->GetContinentName();
        for (Territory* adjacent : territory->GetAdjacentTerritories()) {
            adjacencyList[territory->GetName()].push_back(adjacent->GetName());
        }
    }
}

// Copy constructor
Map :: Map (const Map& other) {

}

Map &Map::operator=(const Map &other){
    return *this;
}

vector<Territory*> Map::getTerritories() {
    return territories;
}

// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const Map& map);

bool Map::validate() const {
    
    // Verify if the map is a connected graph
    if (!isConnectedGraph()) {
        cout << "Failed isConnectedGraph() check" << endl;
        return false;
    }
    cout << "Passed isConnectedGraph() check" << endl;

    // Verify if continents are connected subgraphs
    if (!areContinentsConnected()) {
        cout << "Failed areContinentsConnected() check" << endl;
        return false;
    }
    cout << "Passed areContinentsConnected() check" << endl;
    
    // Verify if each country belongs to one and only one continent
    if (!territoriesBelongToOneContinent()) {
        cout << "Failed territoriesBelongToOneContinent() check" << endl;
        return false;
    }
    cout << "Passed territoriesBelongToOneContinent() check" << endl;

    // All checks passed, map is valid
    return true;
}


// isConnectedGraph() method
bool Map::isConnectedGraph() const {
    vector<bool> visited(territories.size(), false);

    // Start DFS from the first territory
    dfs(0, visited);

    // Check if all territories are visited
    for (bool v : visited) {
        if (!v) {
            return false; // If any territory is not visited, the graph is not connected
        }
    }
    
    return true;
}

// Depth-first search
void Map::dfs(size_t territoryIndex, vector<bool>& visited) const {
    stack<size_t> stack;
    stack.push(territoryIndex);
    //actual territories
    /*
    //cout << "-----Territories" << endl;
    for (int i = 0; i < territories.size(); i++) {
        cout << territories.at(i)->GetName() << endl;
    }
    //cout << "-----Adjacency List" << endl;
    for (auto it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
        cout << it->first << endl;
    }*/

    while (!stack.empty()) {
        size_t currentIndex = stack.top();
        stack.pop();

        visited[currentIndex] = true;
        //cout << territories[currentIndex]->GetName() << " is visited" << endl;
        vector<string> adjacents;
        for (auto it = adjacencyList.begin(); it != adjacencyList.end(); ++it) {
            adjacents.push_back(it->first);
        }
        for (const string& adjacent : adjacents) {
            size_t adjIndex = 0;
            for (size_t i = 0; i < territories.size(); ++i) {
                //cout << adjacent << " comparing " << territories[i]->GetName() << endl;
                if (territories[i]->GetName() == adjacent) {
                    //cout << "-----Debug line #1" << endl;
                    adjIndex = i;
                    break;
                }
            }
            if (!visited[adjIndex]) {
                //cout << "-----Debug line #2" << endl;
                stack.push(adjIndex);
            }
        }
    }
}



// areContinentsConnected() method
bool Map::areContinentsConnected() const {
    
    vector<bool> visitedContinents(continents.size(), false);

    
    

    // Start DFS from a territory in each continent
    for (size_t i = 0; i < continents.size(); ++i) {
        
        if (!visitedContinents[i]) {
            dfsContinent(i, visitedContinents);
        }
    }

    // Manual check to see if all elements in visitedContinents are true
    for (size_t i = 0; i < visitedContinents.size(); ++i) {
        if (!visitedContinents[i]) {
            return false; // If any element is false, continents are not connected
        }
    }

    return true;
}



void Map::dfsContinent(size_t continentIndex, vector<bool>& visitedContinents) const {

    
    visitedContinents[continentIndex] = true;
    const string& continentName = continents[continentIndex]->GetName();

    

    // Find the territories belonging to the current continent
    vector<size_t> continentTerritories;
    for (size_t i = 0; i < territories.size(); ++i) {
        if (territories[i]->GetContinentName() == continentName) {
            continentTerritories.push_back(i);
            continents[continentIndex]->addTerritory(territories.at(i));
        }
    }

   

    // Visit adjacent territories within the same continent
    for (size_t i = 0; i < continentTerritories.size(); ++i) {
        size_t territoryIndex = continentTerritories[i];
        const vector<string>& adjacents = adjacencyList.at(territories[territoryIndex]->GetName());

        
        for (const string& adjacent : adjacents) {
            size_t adjIndex = getTerritoryIndex(adjacent);
            if (!visitedContinents[getContinentIndex(territoryToContinent.at(adjacent))] &&
                territories[adjIndex]->GetContinentName() == continentName) {
                dfsContinent(continentIndex, visitedContinents);
            }
        
        }
    }
}


// getTerritoryIndex() method for getting index of territory based on the name
size_t Map::getTerritoryIndex(const string& territoryName) const {
    for (size_t i = 0; i < territories.size(); ++i) {
        if (territories[i]->GetName() == territoryName) {
            return i;
        }
    }
    return static_cast<size_t>(-1); // Territory not found
}

// getContinentIndex() method for getting index of continent based on the name
size_t Map::getContinentIndex(const string& continentName) const {
    for (size_t i = 0; i < continents.size(); ++i) {
        if (continents[i]->GetName() == continentName) {
            return i;
        }
    }
    return static_cast<size_t>(-1); // Continent not found
}

// territoriesBelongToOneContinent() method
bool Map::territoriesBelongToOneContinent() const {

   

    for (Territory* territory : territories) {
        string& territoryName = territory->GetName();
        const string& continentName = territory->GetContinentName();

        // Check if the territory exists in the territoryToContinent map
        if (territoryToContinent.find(territoryName) == territoryToContinent.end()) {
            return false; // Territory not found in the map
        }

        // Check if the stored continent name matches the actual continent name
        if (territoryToContinent.at(territoryName) != continentName) {
            return false; // Territory belongs to a different continent
        }
    }

    return true; // All territories belong to one and only one continent
}

vector<Continent*> Map::getContinents()
{
    return continents;
}

// Parse function to return a list of Continent objects by reading a .map file
vector<Continent*> ParseContinents(ifstream& mapFile) {
    vector<Continent*> continents; 

    string line;
    bool foundContinentsSection = false;

    while (getline(mapFile, line)) {
        if (line.empty()) {
            continue;
        }

        if (!foundContinentsSection) {
            if (line == "[Continents]") {
                foundContinentsSection = true;
            }
        }
        else if (line == "[Territories]") {
            break;
        }
        else {
            size_t equalsPos = line.find('=');
            if (equalsPos != string::npos) {
                string continentName = line.substr(0, equalsPos);
                continents.push_back(new Continent(continentName));
            }
        }
    }

    return continents;
}

// Parse function to return a list of Territory objects by reading a .map file
vector<Territory*> ParseTerritories(ifstream& mapFile, const vector<Continent*>& continents) {

   
    vector<Territory*> territories; 
    bool parsingTerritories = false;
    //territory with a string list of maps to use when connecting adjacency
    map<Territory*, vector<string>> adjacencyLists;

    string line;
    // while loop to create all list of territories
    while (getline(mapFile, line)) {
        if (line.empty()) {
            continue; 
        }

        if (line == "[Territories]") {
            parsingTerritories = true;
            continue; 
        }

        if (parsingTerritories) {
            //not involve [Territories] line
            if (line[0] == '[') {
                break;
            }

            vector<string> tokens;
            vector<string> adjacencyList;
            stringstream ss(line);
            string token;
            while (getline(ss, token, ',')) {
                tokens.push_back(token);
            }

            if (tokens.size() >= 4) { // It atleast contains a territory name and a continent name
                string territoryName = tokens[0];
                string continentName = tokens[3]; 

                
                Territory* territory = new Territory(territoryName, continentName);
                territories.push_back(territory);

                if (tokens.size() > 4) {
                    for (size_t i = 4; i < tokens.size(); ++i) {
                        adjacencyList.push_back(tokens.at(i));
                    }
                } 
                adjacencyLists.insert(pair<Territory*, vector<string>>(territory, adjacencyList));
            }
        }
    }
    // second loop connects adjacent territories
    map<Territory*, vector<string>>::iterator it;
    for (it = adjacencyLists.begin(); it != adjacencyLists.end(); ++it) {
        for (int j = 0; j < it->second.size(); j++) {
            for (int i = 0; i < territories.size(); i++) {
                if (territories[i]->GetName() == it->second.at(j)) {
                    it->first->AddAdjacentTerritory(territories[j]);
                }
            }
        }
    }

    return territories;
}

// Deconstructor for Map class
Map::~Map() {

    // Clean up dynamically allocated memory
    for (Continent* continent : continents) {
        delete continent;
    }
    for (Territory* territory : territories) {
        delete territory;
    }
}

// MapLoader class
MapLoader::MapLoader() = default;

// Copy constructor
MapLoader :: MapLoader(const MapLoader& other) = default;
// Class assignment operator
MapLoader& MapLoader :: operator=(const MapLoader& other) = default;
// Stream insertion operator
std::ostream& operator<<(std::ostream& os, const MapLoader& mapLoader) {
    return os;
}

Map* MapLoader::LoadMap(const string& mapFileName) {

    ifstream mapFile(mapFileName);

    if (!mapFile.is_open()) {
        throw runtime_error("Failed to open map file.");
    }

    // Parse continents
    vector<Continent*> continents = ParseContinents(mapFile);

    

    mapFile.clear();
    mapFile.seekg(0);

    // Parse territories
    vector<Territory*> territories = ParseTerritories(mapFile, continents);
    

    // Create and return a Map object
    return new Map(continents, territories);
} 

