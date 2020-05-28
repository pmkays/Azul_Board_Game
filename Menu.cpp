#include <iostream>
#include "Menu.h"
#include<string>
#include<stdlib.h>
#include<exception>

Menu::Menu() {
    this->gameEngine = new GameEngine();
}

Menu::Menu(int seed) {
    this->gameEngine = new GameEngine(seed);
}

Menu::~Menu(){
    delete this->gameEngine;
    this->gameEngine = nullptr;
}

void Menu::printWelcome() {
    std::cout << "********************" << std::endl;
    std::cout << "Welcome to Azul!" << std::endl;
    std::cout << "********************" << std::endl;
}

void Menu::runMenu() {
    std::cout << "----" << std::endl;
    std::cout << "Menu" << std::endl;
    std::cout << "----" << std::endl;
    std::cout << "1. New Game" << std::endl;
    std::cout << "2. Load Game" << std::endl;
    std::cout << "3. Credits (Show Student Information)" << std::endl;
    std::cout << "4. Quit" << std::endl << std::endl;

}

void Menu::runGamePlayType() {
    std::cout << "-----------------------" << std::endl;
    std::cout << "Please select game mode" << std::endl;
    std::cout << "-----------------------" << std::endl;
    std::cout << "1. Original" << std::endl;
    std::cout << "2. 6 x 6 Orange" << std::endl;
    std::cout << "3. Grey Mosaic" << std::endl;
    std::cout << "4. Original - 3 players" << std::endl;
    std::cout << "5. Original - 4 players" << std::endl;
}

int Menu::promptCentralFactoryAmount(){
    int numberOfCentralFactories = -1;
    bool continueLoop = true;
    while(continueLoop){
        std::cout<< "Amount of central factories (1 or 2):"<< std::endl;
        numberOfCentralFactories = input.getInt();
        if(numberOfCentralFactories > 0 && numberOfCentralFactories <= 2){
            continueLoop = false;
        }
    }
    return numberOfCentralFactories;
}

void Menu::promptNames(std::string number, std::string& playerName){
    std::cout << "Please enter player " << number << " name:" << std::endl;
    playerName = input.getString();
    playerNames.push_back(playerName);
}

void Menu::checkForDuplicateNames(){
    for(unsigned int i = 0; i < playerNames.size(); i++){
        for(unsigned int j = i + 1; j < playerNames.size(); j++){
            if(playerNames[i] == playerNames[j]){
                playerNames.clear();
                throw "Please ensure there are no duplicate names!";
            }
        }  
    } 
}

void Menu::printWelcomeMessage(){
    std::string welcomeStr = "";
    welcomeStr += "Welcome ";
    for(unsigned int i = 0; i < playerNames.size(); i++){
        welcomeStr += playerNames[i] + " "; 
    }
    welcomeStr.erase(welcomeStr.length() - 1, 1);
    welcomeStr += "!";
    std::cout << welcomeStr << std::endl;
}

void Menu::runNewGame(){
    
}

bool Menu::runSelection(unsigned const int selection, int modeSelection) {

    // Parameters used for io redirection
    bool eof = false;
    bool continueMenuLoop = true;

    Input input;

    if (selection == OPTIONS::NEW_GAME) {
        int numberOfCentralFactories = -1;
        if(modeSelection == MODE::ORIGINAL || modeSelection == MODE::THREE_PLAYER || modeSelection == MODE::FOUR_PLAYER){
            numberOfCentralFactories = promptCentralFactoryAmount(); 
        }
        std::string playerOneName, playerTwoName, playerThreeName, playerFourName = "";

        promptNames("one", playerOneName);
        promptNames("two", playerTwoName);

        if(modeSelection == MODE::THREE_PLAYER || modeSelection == MODE::FOUR_PLAYER){
            promptNames("three", playerThreeName);
        }

        if(modeSelection == MODE::FOUR_PLAYER){
            promptNames("four", playerFourName);
        }

        checkForDuplicateNames();
        printWelcomeMessage();

        this->gameEngine->newGame(playerOneName, playerTwoName, playerThreeName, playerFourName, numberOfCentralFactories, modeSelection);
        this->gameEngine->gameplayLoop(eof, continueMenuLoop, modeSelection);
    } else if (selection == OPTIONS::LOAD_GAME) { 
        std::string file = input.getString();

        GameEngineIO* gameEngineIO = new GameEngineIO(this->gameEngine, modeSelection);
        try{
            gameEngineIO->readEnhancements(file);
            this->gameEngine->gameplayLoop(eof, continueMenuLoop, modeSelection); 

        }catch(const char* e){
            std::cerr<< e << std::endl;
        }
        delete gameEngineIO;
        
    } else if (selection == OPTIONS::CREDITS) {
        std::cout << "------------------------" << std::endl << std::endl;
        printCredits("Ian Nguyen", "s3788210");
        printCredits("Paula Kurniawan", "s3782041");
        printCredits("Josiah Miranda", "s3782051");
        std::cout << "------------------------" << std::endl << std::endl;
    } else if (selection == OPTIONS::QUIT) {
        std::cout << "Goodbye" << std::endl;
    } else if(selection == OPTIONS::HELP){
        runMenuSelectionHelp();
    }else {
        std::cout << "Please enter a valid number." << std::endl;
    }

    return continueMenuLoop;
}

void Menu::printCredits(const std::string name, const std::string studentID) {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Student ID: " << studentID << std::endl;
    std::cout << "Email: " << studentID << "@student.rmit.edu.au" << std::endl << std::endl;
}

void Menu::runGamePlayTypeHelp(){
    std::cout << "----" << std::endl;
    std::cout << "Help" << std::endl;
    std::cout << "----" << std::endl;

    bool continueLoop = true;
    while(continueLoop){
        std::cout << "\nWhich game mode would you like to find out more about? Press 6 to exit Help.\n" << std::endl;
        int mode = input.getInt(); 
        if(mode == 1){
            std::cout << "\nThis is the classic and well-loved version of Azul with two players. Start here if you're new to the game! " 
                      << " P.S. For an extra challenge, play with 2 central factories.\n" << std::endl;
        } else if (mode == 2){
            std::cout << "This is a two-player advanced mode with an extra orange tile and 6 storage and mosaic rows. "
                      << " Play this for an extra challenge!\n" << std::endl;
        } else if (mode == 3){
            std::cout << "\nThis is a two-player advanced mode with a grey mosaic that doesn't have any designated coloured tile placements. "
                      << " Play this for an extra challenge!\n" << std::endl;
        } else if (mode == 4){
            std::cout << "\nThis is the classic and well-loved version of Azul with three players. Play this if you want to have an Azul party! "
                      << " P.S. For an extra challenge, play with 2 central factories.\n" << std::endl;
        }else if (mode == 5){
            std::cout << "\nThis is the classic and well-loved version of Azul with four players. Play this if you want to have an Azul party! "
                      <<" P.S. For an extra challenge, play with 2 central factories.\n" << std::endl;
        }else if (mode == 6){
            continueLoop = false;
        }
    }   
}

void Menu::runMenuSelectionHelp(){
    std::cout << "----" << std::endl;
    std::cout << "Help" << std::endl;
    std::cout << "----" << std::endl;
    bool continueLoop = true;
    while(continueLoop){
        std::cout << "\nWhich menu option would you like to find out more about? Press 5 to exit Help. \n" << std::endl;
        int mode = input.getInt(); 
        if(mode == 1){
            std::cout << "\nStart a brand new game. Enter . Have fun!\n" << std::endl;
        } else if (mode == 2){
            std::cout << "Use a saved file to continue a past game. Games can be saved by typing 'save' at any time during the game. "
                      <<" Make sure you specify the file name when both loading and saving the file.\n" << std::endl;
        } else if (mode == 3){
            std::cout << "\nCheck out who made this awesome program!\n" << std::endl;
        } else if (mode == 4){
            std::cout << "\nExit the game. We'll be sad to see you go :( \n" << std::endl;
        }else if (mode == 5){
            continueLoop = false;
        }
    }  
}


