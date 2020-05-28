#ifndef GAMEENGINEIO_H
#define GAMEENGINEIO_H

#include <string>

#include <fstream>
#include <string>
#include <sstream>

class GameEngine;
class GameEngineIO {
public:
    GameEngineIO(GameEngine* engine, int modeSelection);
    ~GameEngineIO();

    // void loadGame(std::string fileName);
    // void saveGame(std::string fileName);

    void loadPlayers();
    void loadFactories();
    void loadMosaic(Player* player);
    void loadStorageArea(Player* player);
    void loadBrokenTiles(Player* player);
    void loadBag();
    void loadLid();
    void loadSeed();
    void loadModeSelection();
    void loadNumberOfCentralFactories();
    void loadNumberOfFactories();
    void loadNumberOfPlayers();
    void loadCurrentTurn();
    void loadPoints(Player* player);


    void loadGame(std::string fileName);
    void saveGame(std::string fileName);
    int convertToInt(std::string toConvert);
    void readPlayerDetails(Player* player);
    std::string removeSpaces(std::string string);

    
private:
    GameEngine* gameEngine;
    std::string gameInfo[40];
    bool readError;
    unsigned int dimensions;
    int modeSelection;
    std::vector<std::string> gameInformation;
};

#endif //GAMEENGINEIO_H