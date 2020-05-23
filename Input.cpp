#include "Input.h"

Input::Input() {

}

Input::~Input() {

}

std::string Input::getString() const {
    std::string stringToRead = "";
    std::cout << INPUT_CHAR;
    std::getline(std::cin,stringToRead);

    return stringToRead;

}

int Input::getInt() const {
    std::string inputGiven;
    unsigned int intToRead = -1;
    std::cout << INPUT_CHAR;
    std::cin >> inputGiven;
    std::cin.ignore(1, '\n');

    if(inputIsInt(inputGiven)){
        std::stringstream inputAsString(inputGiven);
        inputAsString >> intToRead;
    }else{
        intToRead = 0;
    }

    if(std::cin.eof()){
        intToRead = 4;
    }

    return intToRead;

}

bool Input::inputIsInt(std::string input) const{
    bool success = true;
    for(unsigned int i = 0; i < input.length(); i++){
        if(std::isalpha(input[i]))
            success = false;
    }
    return success;
}