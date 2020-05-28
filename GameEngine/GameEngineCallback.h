#ifndef GAMEENGINECALLBACK_H
#define GAMEENGINECALLBACK_H

#include <iostream>
#include <string>
#include "../Player/headers/Player.h"
#include "../Components/headers/Factory.h"

class GameEngineCallback{
public:
    GameEngineCallback();
    ~GameEngineCallback();

    // Prints board of a player
    void playerBoardUpdate(Player* player) const;
    void playerBoardUpdateAfterTurn(Player** players, int numberOfPlayers) const;

    // Prints whose turn is it now
    void playerTurnUpdate(const std::string playerName) const;

    // Prints the results of the turn (eg. turn success , error)
    void playerTurnResult(const std::string turnResult) const;

    // Prints results of the end of round (board + points)
    void playerEndOfRoundResult(Player* playerOne, Player* playerTwo) const;
    void playerEndOfRoundResult(Player* player) const;
    void endOfRoundStatement() const;

    // Prints results of the end of game (board + points + winner)
    void playerEndOfGameResult(Player* playerOne, Player* playerTwo) const;
    void playerEndOfGameResult(Player** players, int numberOfPlayers) const;


    // Prints board components (factories)
    void boardComponentUpdate(Factory** factory, int numberOfFactories, int numberOfCentralFactories) const;

    void setDimensions(unsigned int dimensions);
    void setModeSelection(int modeSelection);

    void promptColumnPlacement(unsigned int row, Player* player);
    
    void gameplayHelp(int numberOfCentralFactories);
    void greyBoardHelp();
    void orangeHelp();
    void twoCFHelp();
    void generalTurnHelp(); 
    void helpFooter();

private:
    int dimensions;
    int modeSelection;
    std::string factoryHelper(Factory* factory, int index, std::string formatter) const;
};

#endif // GAMEENGINECALLBACK_H