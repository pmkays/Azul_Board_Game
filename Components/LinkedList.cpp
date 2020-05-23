#include "headers/LinkedList.h"

LinkedList::LinkedList(){
    head = nullptr;
    size = 0;
}

LinkedList::~LinkedList(){
    clear();
}

void LinkedList::clear(){
    head = nullptr;
    this->resetSize();
}

void LinkedList::addTileToFront(const std::shared_ptr<Tile> tile){
    std::shared_ptr<Node> nodeToInsert = std::make_shared<Node>(tile);  
    if(head == nullptr){
        head = nodeToInsert;
    } else{
        nodeToInsert->setNextNode(head);
        head = nodeToInsert;
    }
    ++size;
}

std::shared_ptr<Tile> LinkedList::getAndRemoveFirstTile(){
    std::shared_ptr<Tile> firstTile;
    if(head != nullptr){
        firstTile = std::make_shared<Tile>(*head->getTile());
        head = head->getNextNode();
        if(head == nullptr){
        }
        --size;    
    }
    return firstTile;
}

int LinkedList::getSize() const{
    return size;
}

void LinkedList::resetSize(){
    this->size = 0;
}

std::string LinkedList::toSave() const{
    std::string output;
    std::shared_ptr<Node> currentNode = head;

    while(currentNode != nullptr){
        output += currentNode->getTile()->getColourType();
        output += " ";
        currentNode = currentNode->getNextNode();
    }
    return output;
}

