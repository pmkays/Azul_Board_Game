#ifndef INPUT_H
#define INPUT_H

#include <iostream>
#include <string>
#include <sstream>

#define INPUT_CHAR  '>'

class Input{
public:

    Input();
    ~Input();

    // Method that reads in user input as a string
    std::string getString() const;

    // Method that reads in user input as an Integer
    int getInt() const;

    //checks if input is int
    bool inputIsInt(std::string input) const;

};

#endif // INPUT_H