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
}

bool Menu::runSelection(unsigned const int selection, int modeSelection) {

    // Parameters used for io redirection
    bool eof = false;
    bool continueMenuLoop = true;

    Input input;

    if (selection == OPTIONS::NEW_GAME) {

        std::cout << "Please enter player one name:" << std::endl;
        std::string playerOneName = input.getString();
        std::cout << "Please enter player two name:" << std::endl;
        std::string playerTwoName = input.getString();

        if (playerOneName == playerTwoName) {
            throw "Both players cannot have the same name.";
        }

        std::cout << "Welcome " << playerOneName << " and " << playerTwoName << "!" << std::endl;

        this->gameEngine->newGame(playerOneName, playerTwoName, modeSelection);
        this->gameEngine->gameplayLoop(eof, continueMenuLoop);
        

    } else if (selection == OPTIONS::LOAD_GAME) { 
        std::string file = input.getString();

        GameEngineIO* gameEngineIO = new GameEngineIO(this->gameEngine);
        try{
            gameEngineIO->loadGame(file);
            // this->gameEngine->loadGame(file);

            std::cout << "Azul game successfully loaded" << std::endl;

            this->gameEngine->gameplayLoop(eof, continueMenuLoop); 
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


