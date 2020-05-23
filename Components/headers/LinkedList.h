#ifndef LINKEDLIST_H
#define LINKEDLIST_H

#include "Node.h"
#include <string>

class LinkedList {
public:
    LinkedList();
    ~LinkedList();
    void addTileToFront(const std::shared_ptr<Tile> tile);
    std::shared_ptr<Tile> getAndRemoveFirstTile();
    int getSize() const;
    std::string toSave() const;
    void clear();
    void resetSize();


private:
    std::shared_ptr<Node> head; 
    unsigned int size;
};

#endif // LINKEDLIST_H