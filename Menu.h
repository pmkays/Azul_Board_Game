#ifndef MENU_H
#define MENU_H

#include <string>
#include "Input.h"
#include "GameEngine/GameEngine.h"
#include "GameEngine/GameEngineIO.h"

enum OPTIONS {
    NO_OPTION,
    NEW_GAME,
    LOAD_GAME,
    CREDITS,
    QUIT,
    HELP = 100
};

enum MODE {
    NO_OPTION,
    ORIGINAL,
    ORANGE,
    GREY,
    THREE_PLAYER,
    FOUR_PLAYER
};

class Menu {
public:
    // Constructor
    Menu();

    // Constructor for when they define seed
    Menu(int seed);
    ~Menu();
    // Print the menu.
    void runMenu();

    //choose mode done during enhancements
    void runGamePlayType();

    void runGamePlayTypeHelp();

    void runMenuSelectionHelp();

    int promptCentralFactoryAmount();

    void promptNames(std::string number, std::string& playerName);
    void checkForDuplicateNames();
    void printWelcomeMessage();
    void runNewGame();

    // Print Welcome Message.
    void printWelcome();

    bool runSelection(unsigned const int selection, int modeSelection);

    void printCredits(const std::string name, const std::string studentID);

private:
    // Input for Menu
    Input input;
    GameEngine* gameEngine;
    std::vector<std::string> playerNames;
};

#endif // MENU_H