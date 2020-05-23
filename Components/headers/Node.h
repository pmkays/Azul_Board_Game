#ifndef NODE_H
#define NODE_H

#include <string>
#include "Tile.h"

class Node {
public:
    Node(const std::shared_ptr<Tile> tile);
    void setNextNode(const std::shared_ptr<Node> node);
    std::shared_ptr<Node> getNextNode() const;
    std::shared_ptr<Tile> getTile() const;
    
private:
    std::shared_ptr<Node> nextNode;
    std::shared_ptr<Tile> tile;   
};

#endif // NODE_H