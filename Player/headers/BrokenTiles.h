#ifndef BROKENTILES_H
#define BROKENTILES_H

#include "../../Components/headers/Tile.h"
#include <vector>

class BrokenTiles{
public:
    BrokenTiles();
    ~BrokenTiles();

    //Getters
    std::shared_ptr<Tile>* getTiles();
    int getSize();

    //Move tiles
    void addTile(const std::shared_ptr<Tile> tile);
    void clear();

    //Utility
    int calculatePointsLost();
    
    //Printers
    std::string toString();
    std::string toSave();
    
private:
    std::shared_ptr<Tile> tiles[8];
    int size;
};

#endif // BROKENTILES_H