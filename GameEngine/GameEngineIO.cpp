#include "GameEngine.h"
#include "GameEngineIO.h"


GameEngineIO::GameEngineIO(GameEngine* engine){
    this->gameEngine = engine;
    this->readError = false;
}
GameEngineIO::~GameEngineIO(){}

void GameEngineIO::loadGame(std::string fileName) {

    // Load in each line of the file as a unique game info string.
    std::ifstream ifs (fileName);
    std::string currentInfo;
    std::getline(ifs, currentInfo);

    if(!ifs.is_open()){
        throw "File not found";
    }

    unsigned int i = 0;

    while (ifs.good() || i < 36) {
        gameInfo[i] = currentInfo;
        ++i;
        std::getline(ifs, currentInfo);
    }

    ifs.close();


    //load the individual components from the file
    loadPlayers();
    loadFactories();

    Player* players[] = {gameEngine->getPlayerOne(), gameEngine->getPlayerTwo()};
    loadMosaics(players);
    loadStorageArea(players);
    loadBrokenTiles(players);

    loadBag();
    loadLid();
    loadSeed();

    if(readError){
        throw "There was an error reading the file.";
    }

}

void GameEngineIO::loadPlayers(){

    if (gameInfo[0] == gameInfo[1]) {
        throw "Both players cannot have the same name.";
    }

    // Load Player 1 Name into game
    gameEngine->setPlayerOne(gameInfo[0]);

    // Load Player 2 Name into game
    gameEngine->setPlayerTwo(gameInfo[1]);

    // Load Player 1 Points into game
    std::stringstream player1Points(gameInfo[2]);
    int points1;
    player1Points >> points1;
    gameEngine->getPlayerOne()->setPoints(points1);

    // Load Player 2 Points into game
    std::stringstream player2Points(gameInfo[3]);
    int points2;
    player2Points >> points2;
    gameEngine->getPlayerTwo()->setPoints(points2);

    // Load Next Turn info into game
    gameEngine->setCurrentTurn(gameInfo[4]);

}

void GameEngineIO::loadFactories(){
    for (int i = 0; i < NUM_FACTORIES; ++i) {
        unsigned int factoryIndex = i + MAX_ROWS;

        std::stringstream factoryStream(gameInfo[factoryIndex]);
        unsigned int counter = 0;
        char toAdd;
        factoryStream >> toAdd;
        while (factoryStream.good()) {
            Type tileType = Type::NONE;
            if(toAdd == '.'){
                // Do nothing
            }
            // If the Type is changed from NONE to something else, then add it to the Factory.
            else if (gameEngine->changeType(tileType, toAdd)) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);

                gameEngine->getFactory(i)->addTile(tile);
            } else {
                readError = true;
            }
            factoryStream >> toAdd;
            ++counter;
        }
    }
}

void GameEngineIO::loadMosaics(Player* players[]){
    // Load Player 1 and 2 Mosaics into game 
    for (unsigned int playerNum = 0; playerNum < NUM_PLAYERS; ++playerNum) {

        unsigned int indexIncrement = 11 + playerNum*5;

        for (unsigned int row = 0; row < MAX_ROWS; ++row) {
            unsigned int mosaicRowIndex = row+indexIncrement;

            std::stringstream playerMosaicStream(gameInfo[mosaicRowIndex]); // 11 to 15 and then 16 to 20
            char toAdd;
            playerMosaicStream >> toAdd;

            for (unsigned int col = 0; col < MAX_COLS; ++col) {
                if (playerMosaicStream.good()) {
                    Type tileType = Type::NONE;
                    if (gameEngine->changeType(tileType, toAdd)) {
                        std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                        if (!players[playerNum]->getMosaicStorage()->getMosaic()->addTile(tile, row, col)) {
                            readError = true;
                            tile = nullptr;
                        }
                    }
                    playerMosaicStream >> toAdd;
                }
            }
        }
    }
    gameEngine->getPlayerOne()->getMosaicStorage()->getMosaic()->resetPoints();
    gameEngine->getPlayerTwo()->getMosaicStorage()->getMosaic()->resetPoints();
}

void GameEngineIO::loadStorageArea(Player* players[]){
     // Load Player 1 and 2 MosaicStorages into game. 

    for (unsigned int playerNum = 0; playerNum < NUM_PLAYERS; ++playerNum) {

        unsigned int indexIncrement = 21 + playerNum*5; 

        for (unsigned int row = 0; row < MAX_ROWS; ++row) {

            unsigned int mosaicRowIndex = row+indexIncrement;
            std::stringstream mosaicStorageStream(gameInfo[mosaicRowIndex]); // 21 to 25 and then 26 to 30

            char toAdd;
            mosaicStorageStream >> toAdd;

            while (mosaicStorageStream.good()) {
                Type tileType = Type::NONE;
                if (gameEngine->changeType(tileType, toAdd)) {
                    std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);

                    if (players[playerNum]->getMosaicStorage()->isValidAdd(tileType, row)) {
                        players[playerNum]->getMosaicStorage()->addTile(tile, row);
                    } else {
                        readError = true;
                        tile = nullptr;
                    }
                }
                mosaicStorageStream >> toAdd;
            }
        }
    }
}

void GameEngineIO::loadBrokenTiles(Player* players[]){
    // Load Player 1 and 2 Broken Tiles into game

    for (unsigned int playerNum = 0; playerNum < NUM_PLAYERS; ++playerNum) {

        unsigned int brokenTilesIndex = playerNum+31;
        std::stringstream brokenTileStream(gameInfo[brokenTilesIndex]); // 31 and 32
        char toAdd;
        brokenTileStream >> toAdd;

        while (brokenTileStream.good()) {
            Type tileType = Type::NONE;
            if(toAdd == '.'){
                // Do nothing
            }
            else if (gameEngine->changeType(tileType, toAdd)) {
                std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                players[playerNum]->getMosaicStorage()->getBrokenTiles()->addTile(tile);
            } else {
                readError = true;
            }
            brokenTileStream >> toAdd;
        }
    }
}

void GameEngineIO::loadLid(){
    
    // Load Box Lid Tiles into  game
    std::stringstream boxLidStream(gameInfo[33]);
    // Going to need to traverse the vector backwards since our box lid only has an add to front method
    std::vector<char> tilesToAdd;
    char toAdd;
    boxLidStream >> toAdd;

    while (boxLidStream.good()) {
        tilesToAdd.push_back(toAdd);
        boxLidStream >> toAdd;
    }

    unsigned int lastIndex = tilesToAdd.size() - 1;
    for (int i = lastIndex; i >= 0; --i) {

        Type tileType = Type::NONE;
        if (gameEngine->changeType(tileType, tilesToAdd.at(i))) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            gameEngine->getBoxLid()->addTileToFront(tile);
        } else {
            readError = true;
        }
    }
    tilesToAdd.clear();
}

void GameEngineIO::loadBag(){
    // Load Bag Tiles into game
    std::stringstream tileBagStream(gameInfo[34]);
    char toAdd;
    std::vector<char> tilesToAdd;
    // Going to need to traverse the vector backwards since our tile bag only has an add to front method
    tileBagStream >> toAdd;

    while (tileBagStream.good()) {
        tilesToAdd.push_back(toAdd);
        tileBagStream >> toAdd;
    }

    unsigned int lastIndex = tilesToAdd.size() - 1;
    for (int i = lastIndex; i >= 0; --i) {
        Type tileType = Type::NONE;
        if (gameEngine->changeType(tileType, tilesToAdd.at(i))) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            gameEngine->getTileBag()->addTileToFront(tile);
        } else {
            readError = true;
        }
    }
    tilesToAdd.clear();
}

void GameEngineIO::loadSeed(){
    int seed;
    std::stringstream seedStream(gameInfo[35]);
    seedStream >> seed;
    gameEngine->setSeed(seed);
}

void GameEngineIO::saveGame(std::string fileName) {
    std::ofstream outFile;
    outFile.open(fileName);

    Player* playerOne = gameEngine->getPlayerOne();
    Player* playerTwo = gameEngine->getPlayerTwo();
    if(outFile.good()){
        //save player info
        outFile << playerOne->getName() << std::endl;
        outFile << playerTwo->getName() << std::endl;
        outFile << playerOne->getPoints() << std::endl;
        outFile << playerTwo->getPoints() << std::endl; 
        outFile << gameEngine->getCurrentTurn() << std::endl; 

        //save factories
        for(int i = 0; i < NUM_FACTORIES; i++){
            outFile << gameEngine->getFactory(i)->toSave() << std::endl;
        }

        //save mosaics 
        for(int i = 0; i < MAX_ROWS; i++){
            outFile << playerOne->getMosaicStorage()->getMosaic()->rowToSave(i) << std::endl;
        }

        for(int i = 0; i < MAX_ROWS; i++){
            outFile << playerTwo->getMosaicStorage()->getMosaic()->rowToSave(i) << std::endl;
        }

        //save storage areas
        for(int i = 0; i < MAX_ROWS; i++){
            outFile << playerOne->getMosaicStorage()->rowToSave(i) << std::endl;
        }

        for(int i = 0; i < MAX_ROWS; i++){
            outFile << playerTwo->getMosaicStorage()->rowToSave(i) << std::endl;
        }

        //save brokenTiles
        outFile << playerOne->getMosaicStorage()->getBrokenTiles()->toSave() << std::endl;
        outFile << playerTwo->getMosaicStorage()->getBrokenTiles()->toSave() << std::endl;

        //save box lid tiles
        outFile << gameEngine->getBoxLid()->toSave() <<std::endl;

        //save bag tiles
        outFile << gameEngine->getTileBag()->toSave() << std::endl;

        //save random seed
        outFile << gameEngine->getSeed() << std::endl;
    }
    outFile.close();
}