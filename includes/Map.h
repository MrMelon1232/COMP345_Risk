#ifndef MAP_H
#define MAP_H

#include <vector>
#include <string>
#include <map>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stack>

using namespace std; 

// Continent Class
class Continent {
private:
    string name;
    vector<Territory*> territoryList;
public:
    Continent(const string& name);
    const string& GetName() const;
    int bonusValue;
};

// Territory Class
class Territory {
private:
    string name;
    int nbArmies;
    string continentName;
    vector<string> adjacentTerritories;

public:
    Territory(const string& name, const string& continentName);
    const string& GetName() const;
    const string& GetContinentName() const;
    void AddAdjacentTerritory(const string& adjacent);
    const vector<string>& GetAdjacentTerritories() const;
    const int getNbArmies() const;
    void setNbArmies(int nbArmies);

    // Copy constructor
    Territory(const Territory& other);
    // Class assignment operator
    Territory& operator=(const Territory& other);
    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Territory& territory);
};

// Map Class
class Map {
public:
    Map(const vector<Continent*> continents, const vector<Territory*> territories);
    bool validate() const;

    vector<Continent*> continents;
    vector<Territory*> territories;
    map<string, string> territoryToContinent;
    map<string, vector<string>> adjacencyList;

    bool isConnectedGraph() const;
    void dfs(size_t territoryIndex, std::vector<bool>& visited) const;
    bool areContinentsConnected() const;
    void dfsContinent(size_t continentIndex, vector<bool>& visitedContinents) const;
    size_t getTerritoryIndex(const string& territoryName) const;
    size_t getContinentIndex(const string& continentName) const;
    bool territoriesBelongToOneContinent() const;
    vector<Continent*> getContinents();

    // Copy constructor
    Map(const Map& other);
    // Class assignment operator
    Map& operator=(const Map& other);
    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const Map& map);

    // Deconstructor
    ~Map();
};


// Functions to parse map file and create and return continent object and territory object
vector<Continent*> ParseContinents(ifstream& mapFile);
vector<Territory*> ParseTerritories(ifstream& mapFile, const vector<Continent>& continents);

// Map Loader Class
class MapLoader {
public:
    MapLoader();
    Map* LoadMap(const string& mapFileName);

    // Copy constructor
    MapLoader(const MapLoader& other);
    // Class assignment operator
    MapLoader& operator=(const MapLoader& other);
    // Stream insertion operator
    friend std::ostream& operator<<(std::ostream& os, const MapLoader& mapLoader);
    
};

#endif // MAP_H

//test methods
void testLoadMaps();