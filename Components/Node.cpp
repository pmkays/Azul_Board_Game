#include "headers/Node.h"

Node::Node(const std::shared_ptr<Tile> tile){
    this->tile = tile;
    nextNode = nullptr;
}

void Node::setNextNode(const std::shared_ptr<Node> node){
    nextNode = node;
}

std::shared_ptr<Node> Node::getNextNode() const{
    return nextNode;
}

std::shared_ptr<Tile> Node::getTile() const{
    return tile;
}