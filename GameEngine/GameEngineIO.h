#ifndef GAMEENGINEIO_H
#define GAMEENGINEIO_H

#include <string>

#include <fstream>
#include <string>
#include <sstream>

class GameEngine;
class GameEngineIO {
public:
    GameEngineIO(GameEngine* engine);
    ~GameEngineIO();

    void loadGame(std::string fileName);
    void saveGame(std::string fileName);

    void loadPlayers();
    void loadFactories();
    void loadMosaics(Player* players[]);
    void loadStorageArea(Player* players[]);
    void loadBrokenTiles(Player* players[]);
    void loadBag();
    void loadLid();
    void loadSeed();
    
private:
    GameEngine* gameEngine;
    std::string gameInfo[36];
    bool readError;  
};

#endif //GAMEENGINEIO_H