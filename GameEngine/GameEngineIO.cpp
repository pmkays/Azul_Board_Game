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

// void GameEngineIO::loadGame(std::string fileName) {

//     // Load in each line of the file as a unique game info string.
//     std::ifstream ifs (fileName);
//     std::string currentInfo;
//     std::getline(ifs, currentInfo);

//     if(!ifs.is_open()){
//         throw "File not found";
//     }

//     unsigned int i = 0;

//     while (ifs.good() || i < 36) {
//         gameInfo[i] = currentInfo;
//         ++i;
//         std::getline(ifs, currentInfo);
//     }

//     ifs.close();

    
//     //load the individual components from the file
//     loadPlayers();
//     std::cout<<"Loading factories"<<std::endl;
//     loadFactories();

//     Player* players[] = {gameEngine->getPlayerOne(), gameEngine->getPlayerTwo()};
//     gameEngine->setPlayers();
//     std::cout<<"Loading mosaics"<<std::endl;
//     loadMosaics(players);
//     std::cout<<"Loading storage rows"<<std::endl;
//     loadStorageArea(players);
//     std::cout<<"Loading broken tiles"<<std::endl;
//     loadBrokenTiles(players);
//     std::cout<<"Loading bag"<<std::endl;
//     loadBag();
//     std::cout<<"Loading lid"<<std::endl;
//     loadLid();
//     std::cout<<"Loading seed"<<std::endl;
//     loadSeed();

//     if(readError){
//         throw "There was an error reading the file.";
//     }

// }

// void GameEngineIO::loadPlayers(){

//     if (gameInfo[0] == gameInfo[1]) {
//         throw "Both players cannot have the same name.";
//     }
//     // Load Player 1 Name into game
//     gameEngine->setPlayerOne(gameInfo[0], dimensions);

//     // Load Player 2 Name into game
//     gameEngine->setPlayerTwo(gameInfo[1], dimensions);

//     // Load Player 1 Points into game
//     std::stringstream player1Points(gameInfo[2]);
//     int points1;
//     player1Points >> points1;
//     gameEngine->getPlayerOne()->setPoints(points1);

//     // Load Player 2 Points into game
//     std::stringstream player2Points(gameInfo[3]);
//     int points2;
//     player2Points >> points2;
//     gameEngine->getPlayerTwo()->setPoints(points2);

//     // Load Next Turn info into game
//     gameEngine->setCurrentTurn(gameInfo[4]);
// }

// void GameEngineIO::loadFactories(){
//     for (int i = 0; i < NUM_FACTORIES; ++i) {
//         unsigned int factoryIndex = i + MAX_ROWS;

//         std::stringstream factoryStream(gameInfo[factoryIndex]);
//         unsigned int counter = 0;
//         char toAdd;
//         factoryStream >> toAdd;
//         while (factoryStream.good()) {
//             Type tileType = Type::NONE;
//             if(toAdd == '.'){
//                 // Do nothing
//             }
//             // If the Type is changed from NONE to something else, then add it to the Factory.
//             else if (gameEngine->changeType(tileType, toAdd)) {
//                 std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);

//                 gameEngine->getFactory(i)->addTile(tile);
//             } else {
//                 readError = true;
//             }
//             factoryStream >> toAdd;
//             ++counter;
//         }
//     }
// }

// void GameEngineIO::loadMosaics(Player* players[]){
//     // Load Player 1 and 2 Mosaics into game 
//     for (unsigned int playerNum = 0; playerNum < NUM_PLAYERS; ++playerNum) {
//         unsigned int indexIncrement;
//         if(modeSelection != 2){
//             indexIncrement = 11 + playerNum*5;
//         } else{
//             indexIncrement = 11 + playerNum*6;
//         }

//         for (unsigned int row = 0; row < dimensions; ++row) {
//             unsigned int mosaicRowIndex = row+indexIncrement;

//             std::stringstream playerMosaicStream(gameInfo[mosaicRowIndex]); // 11 to 15 and then 16 to 20
//             char toAdd;
//             playerMosaicStream >> toAdd;

//             for (unsigned int col = 0; col < dimensions; ++col) {
//                 if (playerMosaicStream.good()) {
//                     Type tileType = Type::NONE;
//                     if (gameEngine->changeType(tileType, toAdd)) {
//                         std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
//                         if (!players[playerNum]->getMosaicStorage()->getMosaic()->addTile(tile, row, col)) {
//                             readError = true;
//                             tile = nullptr;
//                         }
//                     }
//                     playerMosaicStream >> toAdd;
//                 }
//             }
//         }
//     }
//     gameEngine->getPlayerOne()->getMosaicStorage()->getMosaic()->resetPoints();
//     gameEngine->getPlayerTwo()->getMosaicStorage()->getMosaic()->resetPoints();
// }

// void GameEngineIO::loadStorageArea(Player* players[]){
//      // Load Player 1 and 2 MosaicStorages into game. 

//     for (unsigned int playerNum = 0; playerNum < NUM_PLAYERS; ++playerNum) {
//         unsigned int indexIncrement = 0; 
//         if(modeSelection != 2){
//             indexIncrement = 21 + playerNum*5; 
//         } else{
//             indexIncrement = 23 + playerNum*6; 
//         }

//         for (unsigned int row = 0; row < dimensions; ++row) {

//             unsigned int mosaicRowIndex = row+indexIncrement;
//             std::stringstream mosaicStorageStream(gameInfo[mosaicRowIndex]); // 21 to 25 and then 26 to 30
//             std::cout<<"Line for storage area" << gameInfo[mosaicRowIndex]<< std::endl;
//             char toAdd;
//             mosaicStorageStream >> toAdd;

//             while (mosaicStorageStream.good()) {
//                 Type tileType = Type::NONE;
//                 if (gameEngine->changeType(tileType, toAdd)) {
//                     std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
//                     std::cout<<"made a new tile: "<< tile->getColourType()<<std::endl;
//                     if (players[playerNum]->getMosaicStorage()->isValidAdd(tileType, row)) {
//                         std::cout<<"trying to add tile to mosaic storage"<<std::endl;
//                         players[playerNum]->getMosaicStorage()->addTile(tile, row);
//                         std::cout<<"successfully added tile"<<std::endl;
//                     } else {
//                         readError = true;
//                         tile = nullptr;
//                     }
//                 }
//                 mosaicStorageStream >> toAdd;
//             }
//         }
//     }
// }

// void GameEngineIO::loadBrokenTiles(Player* players[]){
//     // Load Player 1 and 2 Broken Tiles into game

//     for (unsigned int playerNum = 0; playerNum < NUM_PLAYERS; ++playerNum) {

//         unsigned int brokenTilesIndex = 0;
//         if(modeSelection != 2){
//             brokenTilesIndex = playerNum+31;
//         }else{
//             brokenTilesIndex = playerNum+35;
//         }
//         std::stringstream brokenTileStream(gameInfo[brokenTilesIndex]); // 31 and 32
//         char toAdd;
//         brokenTileStream >> toAdd;

//         while (brokenTileStream.good()) {
//             Type tileType = Type::NONE;
//             if(toAdd == '.'){
//                 // Do nothing
//             }
//             else if (gameEngine->changeType(tileType, toAdd)) {
//                 std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
//                 players[playerNum]->getMosaicStorage()->getBrokenTiles()->addTile(tile);
//             } else {
//                 readError = true;
//             }
//             brokenTileStream >> toAdd;
//         }
//     }
// }

// void GameEngineIO::loadLid(){
    
//     std::string toConvert = "";
//     if(modeSelection!=2){
//         toConvert = gameInfo[33];
//     } else{
//         toConvert = gameInfo[37];
//     }
//     // Load Box Lid Tiles into  game
//     std::stringstream boxLidStream(toConvert);
//     // Going to need to traverse the vector backwards since our box lid only has an add to front method
//     std::vector<char> tilesToAdd;
//     char toAdd;
//     boxLidStream >> toAdd;

//     while (boxLidStream.good()) {
//         tilesToAdd.push_back(toAdd);
//         boxLidStream >> toAdd;
//     }

//     unsigned int lastIndex = tilesToAdd.size() - 1;
//     for (int i = lastIndex; i >= 0; --i) {

//         Type tileType = Type::NONE;
//         if (gameEngine->changeType(tileType, tilesToAdd.at(i))) {
//             std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
//             gameEngine->getBoxLid()->addTileToFront(tile);
//         } else {
//             readError = true;
//         }
//     }
//     tilesToAdd.clear();
// }

// void GameEngineIO::loadBag(){
//     std::string toConvert = "";
//     if(modeSelection!=2){
//         toConvert = gameInfo[34];
//     } else{
//         toConvert = gameInfo[38];
//     }
//     // Load Bag Tiles into game
//     std::stringstream tileBagStream(toConvert);
//     char toAdd;
//     std::vector<char> tilesToAdd;
//     // Going to need to traverse the vector backwards since our tile bag only has an add to front method
//     tileBagStream >> toAdd;

//     while (tileBagStream.good()) {
//         tilesToAdd.push_back(toAdd);
//         tileBagStream >> toAdd;
//     }

//     unsigned int lastIndex = tilesToAdd.size() - 1;
//     for (int i = lastIndex; i >= 0; --i) {
//         Type tileType = Type::NONE;
//         if (gameEngine->changeType(tileType, tilesToAdd.at(i))) {
//             std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
//             gameEngine->getTileBag()->addTileToFront(tile);
//         } else {
//             readError = true;
//         }
//     }
//     tilesToAdd.clear();
// }

// void GameEngineIO::loadSeed(){
//     int seed;
//     std::string toConvert = "";
//     if(modeSelection!=2){
//         toConvert = gameInfo[35];
//     } else{
//         toConvert = gameInfo[39];
//     }
//     std::stringstream seedStream(toConvert);
//     seedStream >> seed;
//     gameEngine->setSeed(seed);
// }

// void GameEngineIO::saveGame(std::string fileName) {
//     std::ofstream outFile;
//     outFile.open(fileName);

//     Player* playerOne = gameEngine->getPlayerOne();
//     Player* playerTwo = gameEngine->getPlayerTwo();
//     if(outFile.good()){
//         //save player info
//         outFile << playerOne->getName() << std::endl;
//         outFile << playerTwo->getName() << std::endl;
//         outFile << playerOne->getPoints() << std::endl;
//         outFile << playerTwo->getPoints() << std::endl; 
//         outFile << gameEngine->getCurrentTurn() << std::endl; 

//         //save factories
//         for(int i = 0; i < NUM_FACTORIES; i++){
//             outFile << gameEngine->getFactory(i)->toSave() << std::endl;
//         }

//         if(modeSelection != 1){
//              //save mosaics 
//             for(unsigned int i = 0; i < dimensions; i++){
//                 outFile << playerOne->getMosaicStorage()->getMosaic()->rowToSaveEnhancements(i) << std::endl;
//             }

//             for(unsigned int i = 0; i < dimensions; i++){
//                 outFile << playerTwo->getMosaicStorage()->getMosaic()->rowToSaveEnhancements(i) << std::endl;
//             }
//         }else{
//             //save mosaics 
//             for(unsigned int i = 0; i < dimensions; i++){
//                 outFile << playerOne->getMosaicStorage()->getMosaic()->rowToSave(i) << std::endl;
//             }

//             for(unsigned int i = 0; i < dimensions; i++){
//                 outFile << playerTwo->getMosaicStorage()->getMosaic()->rowToSave(i) << std::endl;
//             }
//         }

//         //save storage areas
//         for(unsigned int i = 0; i < dimensions; i++){
//             outFile << playerOne->getMosaicStorage()->rowToSave(i) << std::endl;
//         }

//         for(unsigned int i = 0; i < dimensions; i++){
//             outFile << playerTwo->getMosaicStorage()->rowToSave(i) << std::endl;
//         }

//         //save brokenTiles
//         outFile << playerOne->getMosaicStorage()->getBrokenTiles()->toSave() << std::endl;
//         outFile << playerTwo->getMosaicStorage()->getBrokenTiles()->toSave() << std::endl;

//         //save box lid tiles
//         outFile << gameEngine->getBoxLid()->toSave() <<std::endl;

//         //save bag tiles
//         outFile << gameEngine->getTileBag()->toSave() << std::endl;

//         //save random seed
//         outFile << gameEngine->getSeed() << std::endl;
//     }
//     outFile.close();
// }

void GameEngineIO::readEnhancements(std::string fileName){
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

    for(unsigned int i = 0; i < gameInformation.size(); i++){
        std::cout<<gameInformation[i]<<std::endl;
    }

    int seed = convertToInt(gameInformation.front());
    gameEngine->setSeed(seed);
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded seed: " << gameEngine->getSeed() << std::endl;

    int modeSelection = convertToInt(gameInformation.front());
    gameEngine->setModeSelection(modeSelection);
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded mode Selection: " << gameEngine->getModeSelection() << std::endl;

    int centralFactories = convertToInt(gameInformation.front());
    gameEngine->setNumberOfCentralFactories(centralFactories);
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded amount of central factories: " << gameEngine->getNumberOfCentralFactories() << std::endl;

    int totalFactories = convertToInt(gameInformation.front());
    gameEngine->setNumberOfFactories(totalFactories);
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded amount of factories: " << gameEngine->getNumberOfFactories() << std::endl;

    int numberOfPlayers = convertToInt(gameInformation.front());
    gameEngine->setNumberOfPlayers(numberOfPlayers);
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded number of players: " << gameEngine->getNumberOfPlayers() << std::endl;

    std::string currentTurn = gameInformation.front();
    gameEngine->setCurrentTurn(currentTurn);
    gameInformation.erase(gameInformation.begin());
    std::cout<<"Loaded current turn: " << gameEngine->getCurrentTurn() << std::endl;

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

    for (int i = 0; i < totalFactories; ++i) {
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

    for(int i = 0; i < totalFactories; i++){
        std::cout << "Factory " << i << ": " << gameEngine->getFactory(i)->toSave() << std::endl;
    }

    gameEngine->setPlayerOne(gameInformation.front(), dimensions);
    gameInformation.erase(gameInformation.begin());
    readPlayerDetails(gameEngine->getPlayerOne());

    gameEngine->setPlayerTwo(gameInformation.front(), dimensions);
    gameInformation.erase(gameInformation.begin());
    readPlayerDetails(gameEngine->getPlayerTwo());

    if(modeSelection == 4 || modeSelection == 5){
         gameEngine->setPlayerThree(gameInformation.front(), dimensions);
        gameInformation.erase(gameInformation.begin());
        readPlayerDetails(gameEngine->getPlayerThree());
    }

    if(modeSelection == 5){
        gameEngine->setPlayerFour(gameInformation.front(), dimensions);
        gameInformation.erase(gameInformation.begin());
        readPlayerDetails(gameEngine->getPlayerFour()); 
    }

    gameEngine->setPlayers();

    Player** players = gameEngine->getPlayers();
    for(int i = 0; i < numberOfPlayers; i++){
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

int GameEngineIO::convertToInt(std::string toConvert){
    int toReturn;
    std::stringstream number(toConvert);
    number >> toReturn;
    return toReturn;
}

std::string GameEngineIO::removeSpaces(std::string string) 
{ 
    std::string toConvert = string;
    // std::cout << "Passed in string: " << string << std::endl; 
    for(unsigned int i=0; i<toConvert.length(); i++){
        if(toConvert[i] == ' ') 
            toConvert.erase(i,1);
    }
    // std::cout << "Removed space string: " << toConvert << std::endl; 
    return toConvert;
} 

void GameEngineIO::readPlayerDetails(Player * player){
    player->setPoints(convertToInt(gameInformation.front()));
    gameInformation.erase(gameInformation.begin());
    std::cout <<"Player points: " << player->getPoints() <<std::endl;

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

    Type tileType = Type::NONE;
    std::string brokenTiles = removeSpaces(gameInformation.front());
    std::cout<<"Raw broken tiles: " << brokenTiles << std::endl;
    for(unsigned int i = 0; i < brokenTiles.length(); i++){
         if(brokenTiles[i] == '.'){
        // Do nothing
        }
        else if (gameEngine->changeType(tileType, brokenTiles[i])) {
            std::shared_ptr<Tile> tile = std::make_shared<Tile>(tileType);
            player->getMosaicStorage()->getBrokenTiles()->addTile(tile);
        } else {
            readError = true;
        }
    }
    std::cout<<"Loaded broken tiles: " << player->getMosaicStorage()->getBrokenTiles()->toSave() << std::endl;
    gameInformation.erase(gameInformation.begin());
}

void GameEngineIO::saveEnhancements(std::string fileName){

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

        //save random seed
        outFile << gameEngine->getSeed() << std::endl;

        //save mode  selection
        outFile << gameEngine->getModeSelection() << std::endl;

        //save number of central factories
        outFile << gameEngine->getNumberOfCentralFactories() << std::endl;

        outFile << gameEngine->getNumberOfFactories() << std::endl;

        outFile << gameEngine->getNumberOfPlayers() << std::endl;

        //save current turn
        outFile << gameEngine->getCurrentTurn() << std::endl; 

        //save box lid tiles
        outFile << gameEngine->getBoxLid()->toSave() <<std::endl;

        //save bag tiles
        outFile << gameEngine->getTileBag()->toSave() << std::endl;

        //save factories
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