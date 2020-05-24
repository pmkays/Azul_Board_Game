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
    std::cout<<"Loading factories"<<std::endl;
    loadFactories();

    Player* players[] = {gameEngine->getPlayerOne(), gameEngine->getPlayerTwo()};
    std::cout<<"Loading mosaics"<<std::endl;
    loadMosaics(players);
    std::cout<<"Loading storage rows"<<std::endl;
    loadStorageArea(players);
    std::cout<<"Loading broken tiles"<<std::endl;
    loadBrokenTiles(players);
    std::cout<<"Loading bag"<<std::endl;
    loadBag();
    std::cout<<"Loading lid"<<std::endl;
    loadLid();
    std::cout<<"Loading seed"<<std::endl;
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
    gameEngine->setPlayerOne(gameInfo[0], dimensions);

    // Load Player 2 Name into game
    gameEngine->setPlayerTwo(gameInfo[1], dimensions);

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
        unsigned int indexIncrement;
        if(modeSelection != 2){
            indexIncrement = 11 + playerNum*5;
        } else{
            indexIncrement = 11 + playerNum*6;
        }

        for (unsigned int row = 0; row < dimensions; ++row) {
            unsigned int mosaicRowIndex = row+indexIncrement;

            std::stringstream playerMosaicStream(gameInfo[mosaicRowIndex]); // 11 to 15 and then 16 to 20
            char toAdd;
            playerMosaicStream >> toAdd;

            for (unsigned int col = 0; col < dimensions; ++col) {
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
        unsigned int indexIncrement = 0; 
        if(modeSelection != 2){
            indexIncrement = 21 + playerNum*5; 
        } else{
            indexIncrement = 23 + playerNum*6; 
        }

        for (unsigned int row = 0; row < dimensions; ++row) {

            unsigned int mosaicRowIndex = row+indexIncrement;
            std::stringstream mosaicStorageStream(gameInfo[mosaicRowIndex]); // 21 to 25 and then 26 to 30
            std::cout<<"Line for storage area" << gameInfo[mosaicRowIndex]<< std::endl;
            char toAdd;
            mosaicStorageStream >> toAdd;

            while (mosaicStorageStream.good()) {
                Type tileType = Type::NONE;
                if (gameEngine->changeType(tileType, toAdd)) {
                    std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
                    std::cout<<"made a new tile: "<< tile->getColourType()<<std::endl;
                    if (players[playerNum]->getMosaicStorage()->isValidAdd(tileType, row)) {
                        std::cout<<"trying to add tile to mosaic storage"<<std::endl;
                        players[playerNum]->getMosaicStorage()->addTile(tile, row);
                        std::cout<<"successfully added tile"<<std::endl;
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

        unsigned int brokenTilesIndex = 0;
        if(modeSelection != 2){
            brokenTilesIndex = playerNum+31;
        }else{
            brokenTilesIndex = playerNum+35;
        }
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
    
    std::string toConvert = "";
    if(modeSelection!=2){
        toConvert = gameInfo[33];
    } else{
        toConvert = gameInfo[37];
    }
    // Load Box Lid Tiles into  game
    std::stringstream boxLidStream(toConvert);
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
    std::string toConvert = "";
    if(modeSelection!=2){
        toConvert = gameInfo[34];
    } else{
        toConvert = gameInfo[38];
    }
    // Load Bag Tiles into game
    std::stringstream tileBagStream(toConvert);
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
    std::string toConvert = "";
    if(modeSelection!=2){
        toConvert = gameInfo[35];
    } else{
        toConvert = gameInfo[39];
    }
    std::stringstream seedStream(toConvert);
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

        if(modeSelection != 1){
             //save mosaics 
            for(unsigned int i = 0; i < dimensions; i++){
                outFile << playerOne->getMosaicStorage()->getMosaic()->rowToSaveEnhancements(i) << std::endl;
            }

            for(unsigned int i = 0; i < dimensions; i++){
                outFile << playerTwo->getMosaicStorage()->getMosaic()->rowToSaveEnhancements(i) << std::endl;
            }
        }else{
            //save mosaics 
            for(unsigned int i = 0; i < dimensions; i++){
                outFile << playerOne->getMosaicStorage()->getMosaic()->rowToSave(i) << std::endl;
            }

            for(unsigned int i = 0; i < dimensions; i++){
                outFile << playerTwo->getMosaicStorage()->getMosaic()->rowToSave(i) << std::endl;
            }
        }

        //save storage areas
        for(unsigned int i = 0; i < dimensions; i++){
            outFile << playerOne->getMosaicStorage()->rowToSave(i) << std::endl;
        }

        for(unsigned int i = 0; i < dimensions; i++){
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