#include "GameEngine.h"
#include "GameEngineIO.h"


GameEngineIO::GameEngineIO(GameEngine* engine, int modeSelection){
    this->gameEngine = engine;
    this->modeSelection = modeSelection;
    if(modeSelection == 2){
        this->dimensions = 6;
    } else{
        this->dimensions = 5;
    }
}

GameEngineIO::~GameEngineIO(){}

void GameEngineIO::loadGame(std::string fileName){
    std::ifstream ifs (fileName);
    std::string currentInfo;

    if(!ifs.is_open()){
        throw "File not found";
    }

    unsigned int i = 0;

    while (ifs.good()) {
        std::getline(ifs, currentInfo);
        gameInformation.push_back(currentInfo);
        ++i;
    }

    ifs.close();

    loadSeed();
    loadModeSelection();
    loadNumberOfCentralFactories();
    loadNumberOfFactories();
    loadNumberOfPlayers();
    loadCurrentTurn();
    loadLid();
    loadBag();
    loadFactories();
    loadPlayers();
}

void GameEngineIO::loadSeed(){
    int seed = convertToInt(gameInformation.front());
    gameEngine->setSeed(seed);
    gameInformation.erase(gameInformation.begin());
}

void GameEngineIO::loadModeSelection(){
    int modeSelection = convertToInt(gameInformation.front());
    gameEngine->setModeSelection(modeSelection);
    gameInformation.erase(gameInformation.begin());
    if(modeSelection == Mode::ORANGE_BOARD){
        gameEngine->setDimensions(6);
    }
}

void GameEngineIO::loadNumberOfCentralFactories(){
    int centralFactories = convertToInt(gameInformation.front());
    gameEngine->setNumberOfCentralFactories(centralFactories);
    gameInformation.erase(gameInformation.begin());
}

void GameEngineIO::loadNumberOfFactories(){
    int totalFactories = convertToInt(gameInformation.front());
    gameEngine->setNumberOfFactories(totalFactories);
    gameInformation.erase(gameInformation.begin());
}

void GameEngineIO::loadNumberOfPlayers(){
    int numberOfPlayers = convertToInt(gameInformation.front());
    gameEngine->setNumberOfPlayers(numberOfPlayers);
    gameInformation.erase(gameInformation.begin());
}

void GameEngineIO::loadCurrentTurn(){
    std::string currentTurn = gameInformation.front();
    gameEngine->setCurrentTurn(currentTurn);
    gameInformation.erase(gameInformation.begin());
}

void GameEngineIO::loadLid(){
    std::string boxLid = removeSpaces(gameInformation.front());
    unsigned int lidLastIndex = boxLid.length() - 1;
    for (int i = lidLastIndex; i >= 0; --i) {

        Type tileType = Type::NONE;
        if (gameEngine->changeType(tileType, boxLid[i])) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            gameEngine->getBoxLid()->addTileToFront(tile);
        }
    }
    gameInformation.erase(gameInformation.begin());
}

void GameEngineIO::loadBag(){
    std::string tileBag = removeSpaces(gameInformation.front());
    unsigned int bagLastIndex = tileBag.length() - 1;
    for (int i = bagLastIndex; i >= 0; --i) {

        Type tileType = Type::NONE;
        if (gameEngine->changeType(tileType, tileBag[i])) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            gameEngine->getTileBag()->addTileToFront(tile);
        } 
    }
    gameInformation.erase(gameInformation.begin());
}

void GameEngineIO::loadFactories(){
    for (int i = 0; i < gameEngine->getNumberOfFactories() ; ++i) {
        std::string factories = removeSpaces(gameInformation.front());
        for(unsigned int j = 0; j< factories.length(); j++){
             Type tileType = Type::NONE;
            if (gameEngine->changeType(tileType, factories[j])) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);

                gameEngine->getFactory(i)->addTile(tile);
            }
        }
        gameInformation.erase(gameInformation.begin());
    }
}

void GameEngineIO::loadPlayers(){
    gameEngine->setPlayerOne(gameInformation.front(), dimensions);
    gameInformation.erase(gameInformation.begin());
    readPlayerDetails(gameEngine->getPlayerOne());

    gameEngine->setPlayerTwo(gameInformation.front(), dimensions);
    gameInformation.erase(gameInformation.begin());
    readPlayerDetails(gameEngine->getPlayerTwo());

    if(modeSelection == Mode::THREE_PLAYER || modeSelection == Mode::FOUR_PLAYER){
        gameEngine->setPlayerThree(gameInformation.front(), dimensions);
        gameInformation.erase(gameInformation.begin());
        readPlayerDetails(gameEngine->getPlayerThree());
    }

    if(modeSelection == Mode::FOUR_PLAYER){
        gameEngine->setPlayerFour(gameInformation.front(), dimensions);
        gameInformation.erase(gameInformation.begin());
        readPlayerDetails(gameEngine->getPlayerFour()); 
    }

    gameEngine->setPlayers();
}

void GameEngineIO::readPlayerDetails(Player* player){
    loadPoints(player);
    loadMosaic(player);
    loadStorageArea(player);
    loadBrokenTiles(player);
}

void GameEngineIO::loadPoints(Player* player){
    player->setPoints(convertToInt(gameInformation.front()));
    gameInformation.erase(gameInformation.begin());
}

void GameEngineIO::loadMosaic(Player* player){
    for (unsigned int row = 0; row < dimensions; ++row) {
        std::string mosaicRow = removeSpaces(gameInformation.front());

        for (unsigned int col = 0; col < mosaicRow.length(); ++col) {
            Type tileType = Type::NONE;
            if (gameEngine->changeType(tileType, mosaicRow[col])) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                if (player->getMosaicStorage()->getMosaic()->addTile(tile, row, col)) {
                    tile = nullptr;
                }
            }
        }
        gameInformation.erase(gameInformation.begin());
    }
    player->getMosaicStorage()->getMosaic()->resetPoints();
}

void GameEngineIO::loadStorageArea(Player* player){
    for (unsigned int row = 0; row < dimensions; ++row) {
        std::string storageRow = removeSpaces(gameInformation.front());
        for (unsigned int col = 0; col < storageRow.length(); ++col) {
            Type tileType = Type::NONE;
            if (gameEngine->changeType(tileType, storageRow[col])) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                player->getMosaicStorage()->addTile(tile, row);
                    tile = nullptr;
            }
        }
        gameInformation.erase(gameInformation.begin());
    }
}

void GameEngineIO::loadBrokenTiles(Player* player){
    Type tileType = Type::NONE;
    std::string brokenTiles = removeSpaces(gameInformation.front());
    for(unsigned int i = 0; i < brokenTiles.length(); i++){
         if (gameEngine->changeType(tileType, brokenTiles[i])) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            if(tile->getType() == Type::FIRST_PLAYER){
                gameEngine->setPlayerStartingNextRound(player->getName());
            }
            player->getMosaicStorage()->getBrokenTiles()->addTile(tile);
        }
    }
    gameInformation.erase(gameInformation.begin());
}

int GameEngineIO::convertToInt(std::string toConvert){
    int toReturn;
    std::stringstream number(toConvert);
    number >> toReturn;
    return toReturn;
}

std::string GameEngineIO::removeSpaces(std::string string) { 
    std::string toConvert = string;
    for(unsigned int i=0; i<toConvert.length(); i++){
        if(toConvert[i] == ' '){
            toConvert.erase(i,1);
        }
    } 
    return toConvert;
}

void GameEngineIO::saveGame(std::string fileName){

    int numberOfPlayers = gameEngine->getNumberOfPlayers();
    int numberOfFactories = gameEngine->getNumberOfFactories();
    Player** players = gameEngine->getPlayers();
    std::ofstream outFile;
    outFile.open(fileName);

    if(outFile.good()){

        /*1. save game setup utilities
        * 2. save game components
        * 3. save current turn
        * 4. save player-related information
        */

        outFile << gameEngine->getSeed() << std::endl;

        outFile << gameEngine->getModeSelection() << std::endl;

        outFile << gameEngine->getNumberOfCentralFactories() << std::endl;

        outFile << gameEngine->getNumberOfFactories() << std::endl;

        outFile << gameEngine->getNumberOfPlayers() << std::endl;

        outFile << gameEngine->getCurrentTurn() << std::endl; 

        outFile << gameEngine->getBoxLid()->toSave() <<std::endl;

        outFile << gameEngine->getTileBag()->toSave() << std::endl;

        for(int i = 0; i < numberOfFactories; i++){
            outFile << gameEngine->getFactory(i)->toSave() << std::endl;
        }

        for(int i = 0; i < numberOfPlayers; i++){
            Player* player = players[i];
            outFile << player->getName() <<std::endl;
            outFile << player->getPoints() << std::endl;
            for(unsigned int j = 0; j < dimensions; j++){
                outFile << player->getMosaicStorage()->getMosaic()->rowToSaveEnhancements(j) << std::endl;
            }
            for(unsigned int j = 0; j < dimensions; j++){
                outFile << player->getMosaicStorage()->rowToSave(j) << std::endl;
            }
            outFile << player->getMosaicStorage()->getBrokenTiles()->toSave() << std::endl;
        }
    }
    outFile.close();

}