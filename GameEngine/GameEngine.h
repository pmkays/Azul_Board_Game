#ifndef GAMEENGINE_H
#define GAMEENGINE_H

#include "../Components/headers/LinkedList.h"
#include "../Components/headers/Factory.h"
#include "../Player/headers/Player.h"
#include "GameEngineCallback.h"

#include <fstream>
#include <string>
#include <sstream>

#define NUM_FACTORIES   6
#define NUM_PLAYERS     2
#define MAX_ROWS        5

enum Error_Message{
    INVALID_COMMAND,
    SUCCESS,
    SAVED,
    NO_TILES_IN_CENTRAL,
    COLOUR_NOT_IN_FACTORY,
    INVALID_MOVE
};

class GameEngineIO;
class GameEngine{
public:
    GameEngine();
    GameEngine(const int seed);
    ~GameEngine();
    void commonGameEngine();

    //Gameplay Conditions
    void newGame(const std::string player1Name, const std::string player2Name, int modeSelection);
    void gameplayLoop(bool& eof, bool& continueMenuLoop);
    bool winConditionMet();
    bool endOfRoundConditionMet();
    void endOfRoundPreparations();

    //Player turn moves
    int playerTurn(const std::string playerTurnCommand);
    std::string interpretPlayerTurn(const int result);
    void swapCurrentTurn();
    void calculatePointsPerRound();
    void calculateEndGamePoints();
    void resetGame();

    //Player input validation
    bool checkInput();
    bool checkCommand1(const std::string input, int& factoryNo);
    bool checkCommand2(const std::string input, Type& tileType);
    bool checkCommand3(const std::string input, int& storageRow);
    bool tileExistsInFactory(Type tileType, int factoryNo);
    bool centralFactoryOnlyHasFirstTile();
    bool inputIsInt(const std::string input);
    bool validateColumnPlacement(const std::string input, unsigned int row, unsigned int& column, Player* player);

    //Moving of tiles
    bool moveTilesFromFactory(Player* player, unsigned const int factoryNumber, unsigned const int row, const Type type, const bool toBroken);
    void returnFactoryTilesByColour(Type type);
    void moveTilesToMosaicStorage(Player* player, unsigned const int factoryNumber, unsigned const int row, const Type type);
    void moveTilesToBrokenTiles(Player* player, unsigned const int factoryNumber, const Type type);
    void moveTilesToLid(Player* player);
    void movePlayerTilesToMosaic();
    void moveTilesToMosaicForGreyBoard(Player* player);

    //Component Utilities
    void populateFactories();
    void populateBagAndShuffle(int modeSelection);
    void shuffle(std::vector<std::shared_ptr<Tile>> &bagToShuffle, unsigned const int size);
    void addTilesByColourToBag(const Type type, std::vector<std::shared_ptr<Tile>> &bagToShuffle);
    void clearBoxLid();
    void refillBag();
    bool changeType(Type& tileType, char tileChar);
    
    //Getters
    std::string getCurrentTurn() const;
    std::string getPlayerStartingNextRound() const;
    Player* getCurrentPlayer() const;
    Player* getPlayerOne() const;
    Player* getPlayerTwo() const;
    Factory* getFactory(unsigned const int number) const;
    LinkedList* getTileBag() const;
    LinkedList* getBoxLid() const;
    int getSeed() const;

    //Setters
    void setPlayerOne(std::string player);
    void setPlayerTwo(std::string player);
    void setSeed(int seed);
    void setCurrentTurn(std::string playerName);
    void setPlayerStartingNextRound(std::string playerName);

private:
    Player* playerOne;
    Player* playerTwo;
    Factory* factory[6];
    LinkedList* bag;
    LinkedList* boxLid;
    std::string currentTurn;
    std::string playerStartingNextRound;
    GameEngineCallback* gec;
    int seed;
    std::string gameInfo[36];

    //major enhancement 
    unsigned int dimensions;
    int modeSelection;

    // For testing purposes
    bool testing;

};

#endif // GAMEENGINE_H