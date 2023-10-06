#include "Map.h"
#include <iostream>

using namespace std;

// testLoadMaps() method to run a list of maps and output the valid ones and reject the invalid ones
void testLoadMaps() {
    vector<string> mapFiles = {
        "Africa.map",
        "Americas 1792.map",
        "Invalid.map",
        "Invalid2.map",
        "Invalid3.map",
        
    };

    MapLoader mapLoader;

    for (const string& mapFile : mapFiles) {
        cout << "Loading map: " << mapFile << endl;

        try {
            Map gameMap = mapLoader.LoadMap("C:/Users/Admin/source/repos/Map/" + mapFile);

            if (gameMap.validate()) {
                cout << "Map loaded successfully: " << mapFile << endl;
                cout << endl;
            }
            else {
                cout << "Invalid map file: " << mapFile << endl;
                cout << endl;
            }
        }
        catch (const runtime_error& e) {
            cout << "Error loading map from file: " << mapFile << endl;
            cout << "Error message: " << e.what() << endl;
        }
    }
}
