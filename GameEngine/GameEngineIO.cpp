#include "GameEngine.h"
#include "GameEngineIO.h"


GameEngineIO::GameEngineIO(GameEngine* engine, int modeSelection){
    this->gameEngine = engine;
    this->readError = false;
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
    std::cout<<"Loaded seed: " << gameEngine->getSeed() << std::endl;
}

void GameEngineIO::loadModeSelection(){
    int modeSelection = convertToInt(gameInformation.front());
    gameEngine->setModeSelection(modeSelection);
    gameInformation.erase(gameInformation.begin());
    if(modeSelection == Mode::ORANGE_BOARD){
        gameEngine->setDimensions(6);
    }
    std::cout<<"Loaded mode Selection: " << gameEngine->getModeSelection() << std::endl;
}

void GameEngineIO::loadNumberOfCentralFactories(){
    int centralFactories = convertToInt(gameInformation.front());
    gameEngine->setNumberOfCentralFactories(centralFactories);
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded amount of central factories: " << gameEngine->getNumberOfCentralFactories() << std::endl;
}

void GameEngineIO::loadNumberOfFactories(){
    int totalFactories = convertToInt(gameInformation.front());
    gameEngine->setNumberOfFactories(totalFactories);
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded amount of factories: " << gameEngine->getNumberOfFactories() << std::endl;
}

void GameEngineIO::loadNumberOfPlayers(){
    int numberOfPlayers = convertToInt(gameInformation.front());
    gameEngine->setNumberOfPlayers(numberOfPlayers);
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded number of players: " << gameEngine->getNumberOfPlayers() << std::endl;
}

void GameEngineIO::loadCurrentTurn(){
    std::string currentTurn = gameInformation.front();
    gameEngine->setCurrentTurn(currentTurn);
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded current turn: " << gameEngine->getCurrentTurn() << std::endl;
}

void GameEngineIO::loadLid(){
    std::string boxLid = removeSpaces(gameInformation.front());
    std::cout<<"Box lid raw:" << boxLid << std::endl; 
    unsigned int lidLastIndex = boxLid.length() - 1;
    for (int i = lidLastIndex; i >= 0; --i) {

        Type tileType = Type::NONE;
        if (gameEngine->changeType(tileType, boxLid[i])) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            gameEngine->getBoxLid()->addTileToFront(tile);
        } else {
            readError = true;
        }
    }
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded box lid: " << gameEngine->getBoxLid()->toSave() << std::endl;
}

void GameEngineIO::loadBag(){
    std::string tileBag = removeSpaces(gameInformation.front());
    std::cout<<"Tilebag raw:" << tileBag << std::endl; 
    unsigned int bagLastIndex = tileBag.length() - 1;
    for (int i = bagLastIndex; i >= 0; --i) {

        Type tileType = Type::NONE;
        if (gameEngine->changeType(tileType, tileBag[i])) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            gameEngine->getTileBag()->addTileToFront(tile);
        } else {
            readError = true;
        }
    }
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded tile bag: " << gameEngine->getTileBag()->toSave() << std::endl;
}

void GameEngineIO::loadFactories(){
    for (int i = 0; i < gameEngine->getNumberOfFactories() ; ++i) {
        std::string factories = removeSpaces(gameInformation.front());
        std::cout<<"Raw factory:" << i << ": " << factories << std::endl;
        for(unsigned int j = 0; j< factories.length(); j++){
             Type tileType = Type::NONE;
            if (gameEngine->changeType(tileType, factories[j])) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);

                gameEngine->getFactory(i)->addTile(tile);
            } else {
                readError = true;
            }
        }
        gameInformation.erase(gameInformation.begin());
    }

    for(int i = 0; i < gameEngine->getNumberOfFactories(); i++){
        std::cout << "Factory " << i << ": " << gameEngine->getFactory(i)->toSave() << std::endl;
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

    Player** players = gameEngine->getPlayers();
    for(int i = 0; i < gameEngine->getNumberOfPlayers(); i++){
        Player* player = players[i];
        std::cout <<"Player name: " << player->getName() <<std::endl;
        std::cout <<"Player points: " << player->getPoints() <<std::endl;
        for(unsigned int j = 0; j < dimensions; j++){
            std::cout <<"Mosaic: " << player->getMosaicStorage()->getMosaic()->rowToSaveEnhancements(j) << std::endl;
        }
        for(unsigned int j = 0; j < dimensions; j++){
            std::cout <<"storage area: " << player->getMosaicStorage()->rowToSave(j) << std::endl;
        }
        std::cout <<"Broken tiles: " << player->getMosaicStorage()->getBrokenTiles()->toSave() << std::endl;
    }   
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
    std::cout <<"Player points: " << player->getPoints() <<std::endl;
}

void GameEngineIO::loadMosaic(Player* player){
    for (unsigned int row = 0; row < dimensions; ++row) {
        std::string mosaicRow = removeSpaces(gameInformation.front());
        std::cout<<"Raw mosaic:" << row << ": " << mosaicRow << std::endl;

        for (unsigned int col = 0; col < mosaicRow.length(); ++col) {
            Type tileType = Type::NONE;
            if (gameEngine->changeType(tileType, mosaicRow[col])) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                if (player->getMosaicStorage()->getMosaic()->addTile(tile, row, col)) {
                    tile = nullptr;
                }
            }
        }
        std::cout << "Loaded mosaic "<<row << ": " << player->getMosaicStorage()->getMosaic()->rowToSaveEnhancements(row) << std::endl;
        gameInformation.erase(gameInformation.begin());
    }
    player->getMosaicStorage()->getMosaic()->resetPoints();
}

void GameEngineIO::loadStorageArea(Player* player){
    for (unsigned int row = 0; row < dimensions; ++row) {
        std::string storageRow = removeSpaces(gameInformation.front());
        std::cout<<"Raw storage row:" << row << ": " << storageRow << std::endl;
        for (unsigned int col = 0; col < storageRow.length(); ++col) {
            Type tileType = Type::NONE;
            if (gameEngine->changeType(tileType, storageRow[col])) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                player->getMosaicStorage()->addTile(tile, row);
                    tile = nullptr;
            }
        }
        std::cout << "Storage row "<<row << ": " << player->getMosaicStorage()->getMosaic()->rowToSaveEnhancements(row) << std::endl;
        gameInformation.erase(gameInformation.begin());
    }
}

void GameEngineIO::loadBrokenTiles(Player* player){
    Type tileType = Type::NONE;
    std::string brokenTiles = removeSpaces(gameInformation.front());
    std::cout<<"Raw broken tiles: " << brokenTiles << std::endl;
    for(unsigned int i = 0; i < brokenTiles.length(); i++){
         if(brokenTiles[i] == '.'){
        // Do nothing
        }
        else if (gameEngine->changeType(tileType, brokenTiles[i])) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            if(tile->getType() == Type::FIRST_PLAYER){
                gameEngine->setPlayerStartingNextRound(player->getName());
            }
            player->getMosaicStorage()->getBrokenTiles()->addTile(tile);
        } else {
            readError = true;
        }
    }
    std::cout<<"Loaded broken tiles: " << player->getMosaicStorage()->getBrokenTiles()->toSave() << std::endl;
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
        if(toConvert[i] == ' ') 
            toConvert.erase(i,1);
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