#ifndef PLAYER_H
#define PLAYER_H

#include <string>
#include <memory>
#include "MosaicStorage.h"

class Player{
public:
    Player(std::string name, unsigned int dimensions);
    ~Player();
    int getPoints();
    void setPoints(unsigned const int points);
    std::string getName();
    std::shared_ptr<MosaicStorage> getMosaicStorage();
    
private:
    int points;
    std::string name;
    std::shared_ptr<MosaicStorage> mosaicStorage;
};

#endif // PLAYER_H