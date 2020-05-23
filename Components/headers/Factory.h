#ifndef FACTORY_H
#define FACTORY_H

#include <vector>
#include "Tile.h"
#include "../../Player/headers/MosaicStorage.h"

class Factory{
public:
    Factory();
    ~Factory();
    void addTile(const std::shared_ptr<Tile> tile);
    void clear();
    std::vector<std::shared_ptr<Tile>> getAllTiles() const;
    std::vector<std::shared_ptr<Tile>> getCopiedTilesAndRemove();
    std::string toSave() const;

private:
    std::vector<std::shared_ptr<Tile>> tiles;
    bool hasFirstPlayerToken;
};

#endif // FACTORY_H