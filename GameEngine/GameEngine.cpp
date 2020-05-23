#include "GameEngine.h"
#include "GameEngineIO.h"
#include <random>
#include "../Input.h"


GameEngine::GameEngine() {
    commonGameEngine();
    this->testing = false;
}

GameEngine::GameEngine(const int seed) {
    commonGameEngine();
    this->seed = seed;
    this->testing = true;
}

void GameEngine::commonGameEngine(){
    gec = new GameEngineCallback();

    for(int i = 0; i<NUM_FACTORIES; ++i){
        factory[i] = new Factory();
    }

    this->bag = new LinkedList();
    this->boxLid = new LinkedList();

    this->currentTurn = "";
    this->playerStartingNextRound = "";
    this->playerOne = nullptr;
    this->playerTwo = nullptr;
    this->dimensions = 0;
}

GameEngine::~GameEngine() {

    delete gec;
    delete bag;   
    delete boxLid;

    //must check players first to avoid double free or corruption error 
    if(playerOne != nullptr){
        delete playerOne;
        playerOne = nullptr; 
    }
    if(playerTwo != nullptr){
        delete playerTwo;
        playerTwo = nullptr; 
    } 

    for(int i = 0;  i < NUM_FACTORIES; i++){
        if(factory[i] != nullptr){
            delete factory[i];
            factory[i] = nullptr;
        }
    }

}

//setters
void GameEngine::setPlayerOne(std::string player){
    unsigned int dimensions = 5;
    this->playerOne = new Player(player , dimensions);
}
void GameEngine::setPlayerTwo(std::string player){
    unsigned int dimensions = 5;
    this->playerTwo = new Player(player, dimensions);
}

void GameEngine::setSeed(int seed){
    this->seed = seed;
}

void GameEngine::setPlayerStartingNextRound(std::string playerName){
    this->playerStartingNextRound = playerName;
}

void GameEngine::setCurrentTurn(std::string playerName){
    this->currentTurn = playerName;
}

void GameEngine::swapCurrentTurn(){
    if(this->currentTurn == this->playerOne->getName())
        this->currentTurn = this->playerTwo->getName();
    else
        this->currentTurn = this->playerOne->getName();
}


//getters
Player* GameEngine::getPlayerOne() const {
    return playerOne;
}

Player* GameEngine::getPlayerTwo() const {
    return playerTwo;
}

Factory* GameEngine::getFactory(unsigned const int number) const {
    return factory[number];
}

LinkedList* GameEngine::getTileBag()const {
    return bag;
}

LinkedList* GameEngine::getBoxLid() const {
    return boxLid;
}

std::string GameEngine::getPlayerStartingNextRound() const {
    return this->playerStartingNextRound;
}

Player* GameEngine::getCurrentPlayer() const {
    Player* toReturn;
    if(this->playerOne->getName() == this->currentTurn){
        toReturn = playerOne;
    }else{
        toReturn = playerTwo;
    }

    return toReturn;
}

std::string GameEngine::getCurrentTurn() const {
    return this->currentTurn;
}

int GameEngine::getSeed() const {
    return this->seed;
}


//gameplay

void GameEngine::newGame(const std::string player1Name, const std::string player2Name, int modeSelection) {
    std::cout<<"In new game" << std::endl;
    unsigned int dimensions = 0;
    if(modeSelection == 1){
        dimensions = 5;
    }  
    else if(modeSelection == 2){
        dimensions = 6;
    }
    
    this->dimensions = dimensions;

    playerOne = new Player(player1Name, dimensions);
    playerTwo = new Player(player2Name, dimensions);

    playerOne->setPoints(0);
    playerTwo->setPoints(0);
    playerOne->getMosaicStorage()->getMosaic()->resetPoints();
    playerTwo->getMosaicStorage()->getMosaic()->resetPoints();
    currentTurn = playerOne->getName();
    playerStartingNextRound = "";

    populateBagAndShuffle(modeSelection);
    populateFactories();
    gec->setDimensions(dimensions);
}

int GameEngine::playerTurn(std::string playerTurnCommand){
    int toReturn = 1;
    std::stringstream commandLine(playerTurnCommand);
    std::string commandPart;

    //0 = "turn", 1 = factory num, 2 = Tile, 3 = storage row
    std::string commands[4];
    commandLine >> commandPart;

    int counter = 0;
    while (commandLine.good() || counter < 4) {
        commands[counter] = commandPart;
        commandLine >> commandPart;
        counter++;
    }

    if(commands[0] == "turn"){

        //pass by ref to get converted values (if possible)
        int factoryNo;
        Type tileType;
        int storageRow;

        //validate the three command arguments first before proceeding
        if(checkCommand1(commands[1], factoryNo) && checkCommand2(commands[2], tileType) && checkCommand3(commands[3],storageRow)){

            if(factoryNo == 0 && centralFactoryOnlyHasFirstTile()){
               toReturn = Error_Message::NO_TILES_IN_CENTRAL;
            }

            if(!tileExistsInFactory(tileType, factoryNo)){
                toReturn = Error_Message::COLOUR_NOT_IN_FACTORY;
            }

            //continue if the aforementioned checks pass
            if(toReturn == Error_Message::SUCCESS){
                if (commands[3] != "B") {
                    if (moveTilesFromFactory(this->getCurrentPlayer(),factoryNo,(storageRow-1),tileType, false)) {
                        toReturn = Error_Message::SUCCESS;
                    } else {
                        toReturn = Error_Message::INVALID_MOVE;
                    }
                } else {
                    if (moveTilesFromFactory(this->getCurrentPlayer(),factoryNo,(storageRow-1),tileType, true)) {
                        toReturn = Error_Message::SUCCESS;
                    } else {
                        toReturn = Error_Message::INVALID_MOVE;
                    }
                }
            }  
        } else{
            toReturn = Error_Message::INVALID_COMMAND;
        }
    } else if (commands[0] == "save") {
        GameEngineIO* geIO = new GameEngineIO(this);
        geIO->saveGame(commands[1]);
        toReturn = Error_Message::SAVED;
        delete geIO;
    } else {
        toReturn = Error_Message::INVALID_COMMAND;
    }

    return toReturn;
}

//check if input is int and valid
bool GameEngine::checkCommand1(const std::string input, int& factoryNo){
    bool success = inputIsInt(input);
    int convertedDimensions = dimensions;

    if(success){
        std::stringstream factoryNoAsString(input);
        factoryNoAsString >> factoryNo;       
        if(factoryNo < 0 || factoryNo > convertedDimensions){
            success = false;
        }
    }
    return success;
}

//check if input isn't first player tile and is a valid colour
bool GameEngine::checkCommand2(const std::string input, Type& tileType){
    bool success = true;
    if (input.length() != 1) {
        success = false;
    } else if (input[0] == 'F') {
        success = false;
    } else {
        success = changeType(tileType,input[0]);
    }
    return success;
}

//check if storage row is convertible to int or if trying to move to broken tiles
bool GameEngine::checkCommand3(const std::string input, int& storageRow){
    bool success = false;
    int convertedDimensions = dimensions;

    if(input.length() == 1){
        success = true;
        if (inputIsInt(input)) {
            std::stringstream storageRowNoAsString(input);
            storageRowNoAsString >> storageRow;

            if(storageRow < 1 || storageRow > convertedDimensions)
                success = false;

        } else if (input[0] != 'B') {
            success = false;
        }
    }
    return success;
}

bool GameEngine::inputIsInt(const std::string input){
    bool success = true;
    for(unsigned int i = 0; i < input.length(); i++){
        if(std::isalpha(input[i]))
            success = false;
    }
    return success;
}

bool GameEngine::tileExistsInFactory(Type tileType, int factoryNo){
    bool success = false;
    std::vector<std::shared_ptr<Tile>> chosenFactory = factory[factoryNo]->getAllTiles();
    int size = chosenFactory.size();

    for(int i = 0; i < size; ++i){
        if(chosenFactory[i]->getType() == tileType)
            success = true;
    }
    return success;
}

bool GameEngine::centralFactoryOnlyHasFirstTile(){
    bool success = false;
    std::vector<std::shared_ptr<Tile>> centralFactory = factory[0]->getAllTiles();
    int size = centralFactory.size();

    if(size == 1 && centralFactory[0]->getType() == Type::FIRST_PLAYER)
        success = true;

    return success;
}

//must check each round if there is a full row so the game can end
bool GameEngine::winConditionMet(){
    bool winConditionMet = false;

    Mosaic* playerOneMosaic = this->playerOne->getMosaicStorage()->getMosaic();
    Mosaic* playerTwoMosaic = this->playerTwo->getMosaicStorage()->getMosaic();

    if(playerOneMosaic->findFullRow() || playerTwoMosaic->findFullRow())
        winConditionMet = true;

    return winConditionMet;
}

//convert character version of tiles to enums for more efficient use in the program
bool GameEngine::changeType(Type& tileType, char tileChar){
    bool changed = true;
    if (tileChar == RED_CHAR) {
        tileType = Type::RED;
    } else if (tileChar == YELLOW_CHAR) {
        tileType = Type::YELLOW;
    } else if (tileChar == DARKBLUE_CHAR) {
        tileType = Type::DARK_BLUE;
    } else if (tileChar == LIGHTBLUE_CHAR) {
        tileType = Type::LIGHT_BLUE;
    } else if (tileChar == BLACK_CHAR) {
        tileType = Type::BLACK;
    } else if (tileChar == FIRST_CHAR) {
        tileType = Type::FIRST_PLAYER;
    } else if (tileChar == ORANGE_CHAR) {
        tileType = Type::ORANGE;
    } else {
        changed = false;
    }
    return changed;
}

//calculates the points gained and lost through broken tiles and mosaic 
void GameEngine::calculatePointsPerRound() {
    std::shared_ptr<MosaicStorage> playerOneMosaicStorage = playerOne->getMosaicStorage();
    std::shared_ptr<MosaicStorage> playerTwoMosaicStorage = playerTwo->getMosaicStorage();

    int playerOneLostPoints = playerOneMosaicStorage->getBrokenTiles()->calculatePointsLost();
    int playerTwoLostPoints = playerTwoMosaicStorage->getBrokenTiles()->calculatePointsLost();

    int playerOnePointsThisRound = playerOneMosaicStorage->getMosaic()->getPointsThisRound();
    int playerTwoPointsThisRound = playerTwoMosaicStorage->getMosaic()->getPointsThisRound();

    int playerOneEndRoundPoints = (playerOne->getPoints() + playerOnePointsThisRound)-playerOneLostPoints;
    int playerTwoEndRoundPoints = (playerTwo->getPoints() + playerTwoPointsThisRound)-playerTwoLostPoints;

    if (playerTwoEndRoundPoints < 0)
        playerTwoEndRoundPoints = 0;
    if (playerOneEndRoundPoints < 0)
        playerOneEndRoundPoints = 0;

    playerOne->setPoints(playerOneEndRoundPoints);
    playerTwo->setPoints(playerTwoEndRoundPoints);

    playerOneMosaicStorage->getMosaic()->resetPoints();
    playerTwoMosaicStorage->getMosaic()->resetPoints();
    playerOneMosaicStorage->endOfRoundDiscardBrokenTiles();
    playerTwoMosaicStorage->endOfRoundDiscardBrokenTiles();

    gec->playerEndOfRoundResult(playerOne, playerTwo);

}

//terminates the round when there are no more tiles in the factories
bool GameEngine::endOfRoundConditionMet(){
    bool endOfRound = false;
    int counter = 0;
    for(int i = 0; i < NUM_FACTORIES; ++i){
        if(factory[i]->getAllTiles().size() == 0)
            ++counter;
    }
    if(counter == 6)
        endOfRound = true;

    return endOfRound;
}

void GameEngine::movePlayerTilesToMosaic(){
    std::shared_ptr<MosaicStorage> playerOneMosaicStorage = playerOne->getMosaicStorage();
    std::shared_ptr<MosaicStorage> playerTwoMosaicStorage = playerTwo->getMosaicStorage();

    playerOneMosaicStorage->endOfRoundMove();
    playerTwoMosaicStorage->endOfRoundMove();
}

void GameEngine::endOfRoundPreparations(){
    movePlayerTilesToMosaic();
    calculatePointsPerRound();
    moveTilesToLid(playerOne);
    moveTilesToLid(playerTwo);
}

bool GameEngine::moveTilesFromFactory(Player* player, unsigned int factoryNumber, unsigned int row, const Type type, const bool toBroken) {
    bool turnSuccess = true;
    if (toBroken)

        //need to take into consideration wanting to move tiles to broken tiles manually
        moveTilesToBrokenTiles(player, factoryNumber, type);
        
    else if (player->getMosaicStorage()->isValidAdd(type, row))
        //player has chosen to put the tiles from the factory somewhere in their mosaic storage
        moveTilesToMosaicStorage(player, factoryNumber, row, type);
        
    else
        //no turns have been taken 
        turnSuccess = false;
    return turnSuccess;
}

void GameEngine::moveTilesToMosaicStorage(Player* player, unsigned const int factoryNumber, unsigned const int row, const Type type){
std::vector<std::shared_ptr<Tile>> allTiles =  factory[factoryNumber]->getCopiedTilesAndRemove();
        int size = allTiles.size();
        for(int i = 0; i < size; i++){
            std::shared_ptr<Tile> tileToAdd = allTiles[i];
            if(tileToAdd->getType() == Type::FIRST_PLAYER){
                //automatically move the first player tile to the broken tiles
                player->getMosaicStorage()->getBrokenTiles()->addTile(tileToAdd);
                this->setPlayerStartingNextRound(player->getName());
            } else if(allTiles[i]->getType() == type){
                player->getMosaicStorage()->addTile(tileToAdd, row);
            } else{
                //add the remaining unchosen tiles to central factory
                factory[0]->addTile(tileToAdd);
            }  
        }
}

void GameEngine::moveTilesToBrokenTiles(Player* player, unsigned const int factoryNumber, const Type type){
    int maxBrokenTiles = 0;
    if(dimensions == 5){
        maxBrokenTiles = 7;
    } else{
        maxBrokenTiles = 8;
    }
    std::vector<std::shared_ptr<Tile>> allTiles =  factory[factoryNumber]->getCopiedTilesAndRemove();
    std::shared_ptr<MosaicStorage> mosaicStorage = player->getMosaicStorage();
    BrokenTiles* brokenTiles = mosaicStorage->getBrokenTiles();
    
        int size = allTiles.size();
        for (int i = 0; i < size; ++i) {
            std::shared_ptr<Tile> tileToAdd = allTiles[i];
            if (allTiles[i]->getType() == type) {
                //make sure that the player can only have a max of 7 tiles; the rest go to the box lid if the max is reached
                if(brokenTiles->getSize() < maxBrokenTiles)
                    brokenTiles->addTile(tileToAdd);
                else 
                    mosaicStorage->addTileToDiscardedTiles(tileToAdd);
            } else {
                factory[0]->addTile(tileToAdd);
            }
        }
}

//called at the end of each round to get rid of no longer usable tiles
void GameEngine::moveTilesToLid(Player* player){
    std::vector<std::shared_ptr<Tile>> discarded = *(player->getMosaicStorage()->getDiscardedTiles());

    int discardedSize = discarded.size();

    for(int i = 0; i < discardedSize; i++){
        boxLid->addTileToFront(discarded[i]);
        discarded[i] = nullptr;
    }

    player->getMosaicStorage()->resetDiscardTilesVector();
} 

void GameEngine::refillBag() {
    int totalTiles = boxLid->getSize();

    for(int i = 0; i < totalTiles; ++i)
        bag->addTileToFront(boxLid->getAndRemoveFirstTile());
}

void GameEngine::populateFactories(){
    factory[0]->addTile(std::make_shared<Tile>(Type::FIRST_PLAYER));

    int convertedDimensions = dimensions;
    //start at 1 so we don't populate the central factory
    for(int i = 1; i < NUM_FACTORIES; i++){
        //fill each factory with 4 tiles
        for(int j = 0; j < convertedDimensions-1; ++j){
            if (bag->getSize() > 0) {
                factory[i]->addTile(bag->getAndRemoveFirstTile());
            } else {
                refillBag();
                factory[i]->addTile(bag->getAndRemoveFirstTile());
            }
        }
    }
}

void GameEngine::populateBagAndShuffle(int modeSelection){

    std::cout << "Mode selection: "<< modeSelection <<std::endl;
    //populate array for later shuffling
    std::vector<std::shared_ptr<Tile>> bagToShuffle;
    GameEngine::addTilesByColourToBag(Type::BLACK, bagToShuffle);
    GameEngine::addTilesByColourToBag(Type::DARK_BLUE, bagToShuffle);
    GameEngine::addTilesByColourToBag(Type::LIGHT_BLUE, bagToShuffle);
    GameEngine::addTilesByColourToBag(Type::RED, bagToShuffle);
    GameEngine::addTilesByColourToBag(Type::YELLOW, bagToShuffle);
    if(modeSelection == 2){
        GameEngine::addTilesByColourToBag(Type::ORANGE, bagToShuffle);
    }

    int bagToShuffleSize = bagToShuffle.size();
    GameEngine::shuffle(bagToShuffle, bagToShuffleSize);
    
    //add to linked list format (which is the one used for the rest of the game)
    for(int i = 0; i < bagToShuffleSize; i++){
        bag->addTileToFront(bagToShuffle[i]);
        bagToShuffle[i] = nullptr;
    }
}

//shuffle the bag through a Knuths/Fisher-Yates shuffle algorithm. Can't use seed 0.
void GameEngine::shuffle(std::vector<std::shared_ptr<Tile>> &bagToShuffle , unsigned const int size){
    std::default_random_engine engine;

    //instantiate the engine with either the given seed or random seed
    if (seed != 0) {
        engine.seed(seed);
    } else {
        seed = std::random_device{}();
        engine.seed(seed);
    }

    for(int i = size - 1; i > 0 ; --i){
        //pick an index so that 0 <= j <= i
        std::uniform_int_distribution<int> uniform_dist(0, i);
        int j = uniform_dist(engine);

        //swap the tiles
        std::shared_ptr<Tile> temp = bagToShuffle[i];
        bagToShuffle[i] = bagToShuffle[j];
        bagToShuffle[j] = temp;
    }
}

//enables 20 tiles of each colour to be added to the tile bag for later shuffling
void GameEngine::addTilesByColourToBag(const Type type, std::vector<std::shared_ptr<Tile>> &bagToShuffle){
    int maxTiles = 20;

    for(int i = 0; i < maxTiles; ++i){
        bagToShuffle.push_back(std::make_shared<Tile>(type));
    }
}

/*
*Interpret the following integers as:
* 0: invalid command
* 1: successful turn
* 2: saved game
* 3: error due to nothing in central factory
* 4: colour not in factory
* 5: invalid moves
*/
std::string GameEngine::interpretPlayerTurn(const int result){
    std::string toReturn;
    if(result == Error_Message::INVALID_COMMAND)
        toReturn = "Error: Invalid Command.\n";
    if(result == Error_Message::SUCCESS)
        toReturn = "Turn Successful.\n";
    if(result == Error_Message::SAVED)
        toReturn = "Saved Game.\n";
    if(result == Error_Message::NO_TILES_IN_CENTRAL)
        toReturn = "Error: There is no tiles in the central factory!\n";
    if(result == Error_Message::COLOUR_NOT_IN_FACTORY)
        toReturn = "Error: The colour specified is not in this factory.\n";
    if(result == Error_Message::INVALID_MOVE)
        toReturn = "Error: The move you are trying to make is invalid.\n";

    return toReturn;
}

//loop enables the game to keep playing until someone wins or someone quits
void GameEngine::gameplayLoop(bool& endOfCommands, bool& continueMenuLoop) {
    Input input;
    while(!endOfCommands && !std::cin.eof() && !winConditionMet()){
        while(!endOfCommands && !endOfRoundConditionMet()){

            //output relevant information to players
            gec->boardComponentUpdate(factory);
            gec->playerBoardUpdate(playerOne);
            gec->playerBoardUpdate(playerTwo);
            gec->playerTurnUpdate(currentTurn);

            std::string playerCommand = "";
            int turnResult = 0;
            while(!endOfCommands && !std::cin.eof() && (turnResult != 1)){
                playerCommand = input.getString();
                turnResult = playerTurn(playerCommand);
                gec->playerTurnResult(interpretPlayerTurn(turnResult));
            }
            // This only runs for io redirection; program automatically exits if the eof is reached
            if(std::cin.eof()){
                gec->playerTurnResult("Program will now exit.");
                endOfCommands = true;
                continueMenuLoop = false;
            }

            swapCurrentTurn();
        }
        if(!endOfCommands){
            endOfRoundPreparations();
            populateFactories();
            setCurrentTurn(getPlayerStartingNextRound());
            setPlayerStartingNextRound("");
        }
    }

    //loop breaks so we can finalise scores and decide on winner
    if (winConditionMet()) {
        gec->playerBoardUpdate(playerOne);
        gec->playerBoardUpdate(playerTwo);
        calculateEndGamePoints();

        // When testing, we save the game before it ends to see the end of game save file
        if (testing) {
            GameEngineIO* geIO = new GameEngineIO(this);
            geIO->saveGame("actualoutcome.save");
            delete geIO;
        }

        resetGame();
    }

}

void GameEngine::calculateEndGamePoints() {
    int playerOneAdditionalPoints = playerOne->getMosaicStorage()->getMosaic()->calculateEndGamePoints();
    int playerTwoAdditionalPoints = playerTwo->getMosaicStorage()->getMosaic()->calculateEndGamePoints();

    playerOne->setPoints(playerOne->getPoints() + playerOneAdditionalPoints);
    playerTwo->setPoints(playerTwo->getPoints() + playerTwoAdditionalPoints);

    gec->playerEndOfGameResult(playerOne, playerTwo);
}

void GameEngine::resetGame(){
    // new players are made with each new game instance
    if(playerOne != nullptr){
        delete playerOne;
        playerOne = nullptr; 
    }
    if(playerTwo != nullptr){
        delete playerTwo;
        playerTwo = nullptr; 
    } 
    
    //don't delete components as they get instantiated with GE
    for(int i = 0; i < NUM_FACTORIES; i++){
        factory[i]->clear();
    }
    bag->clear();
    boxLid->clear();
}

