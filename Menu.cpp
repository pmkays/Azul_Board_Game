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
    std::cout << "----" << std::endl;
    std::cout << "Please select game mode" << std::endl;
    std::cout << "----" << std::endl;
    std::cout << "1. Original" << std::endl;
    std::cout << "2. 6 x 6 Orange" << std::endl;
    std::cout << "3. Grey Mosaic" << std::endl;
    std::cout << "4. Original - 3 players" << std::endl;
    std::cout << "5. Original - 4 players" << std::endl;
}

bool Menu::runSelection(unsigned const int selection, int modeSelection) {

    // Parameters used for io redirection
    bool eof = false;
    bool continueMenuLoop = true;

    Input input;

    if (selection == OPTIONS::NEW_GAME) {
        int numberOfCentralFactories = -1;
        if(modeSelection == 1 || modeSelection == 4 || modeSelection == 5){
            bool continueLoop = true;
            while(continueLoop){
                std::cout<< "Amount of central factories (1 or 2):"<< std::endl;
                numberOfCentralFactories = input.getInt();
                if(numberOfCentralFactories != 0){
                    continueLoop = false;
                }
            }
        }

        std::cout << "Please enter player one name:" << std::endl;
        std::string playerOneName = input.getString();
        std::cout << "Please enter player two name:" << std::endl;
        std::string playerTwoName = input.getString();

        if (playerOneName == playerTwoName) {
            throw "Both players cannot have the same name.";
        }

        std::string playerThreeName = "";
        std::string playerFourName = "";
        if(modeSelection == 4 || modeSelection == 5){
            std::cout << "Please enter player three name:" << std::endl;
            playerThreeName = input.getString();
        }

        if(modeSelection == 5){
            std::cout << "Please enter player four name:" << std::endl;
            playerFourName = input.getString();
        }

        std::cout << "Welcome " << playerOneName << " and " << playerTwoName << "!" << std::endl;

        this->gameEngine->newGame(playerOneName, playerTwoName, playerThreeName, playerFourName, numberOfCentralFactories, modeSelection);
        this->gameEngine->gameplayLoop(eof, continueMenuLoop, modeSelection);
        

    } else if (selection == OPTIONS::LOAD_GAME) { 
        std::string file = input.getString();

        GameEngineIO* gameEngineIO = new GameEngineIO(this->gameEngine, modeSelection);
        try{
            if(modeSelection == 1){
                gameEngineIO->loadGame(file);
                this->gameEngine->gameplayLoop(eof, continueMenuLoop, modeSelection); 
            }else{
                gameEngineIO->readEnhancements(file);
            }
            // this->gameEngine->loadGame(file);

            std::cout << "Azul game successfully loaded" << std::endl;

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
    } else {
        std::cout << "Please enter a valid number." << std::endl;
    }

    return continueMenuLoop;
}

void Menu::printCredits(const std::string name, const std::string studentID) {
    std::cout << "Name: " << name << std::endl;
    std::cout << "Student ID: " << studentID << std::endl;
    std::cout << "Email: " << studentID << "@student.rmit.edu.au" << std::endl << std::endl;
}


