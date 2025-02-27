#ifndef SPACESECTORLLRBT_H
#define SPACESECTORLLRBT_H

#include "Sector.h"
#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

class SpaceSectorLLRBT {
public:
    Sector* root;
    SpaceSectorLLRBT();
    ~SpaceSectorLLRBT();
    void clear(Sector* Node);
    void readSectorsFromFile(const std::string& filename);
    void insertSectorByCoordinates(int x, int y, int z);
    Sector* insertSectorByRecursive(Sector* Node_1 , Sector* Node_2);
    void balance(Sector* Node, bool isRoot);
    Sector* rightRotate(Sector* Node);
    Sector* leftRotate(Sector* Node);
    void flipColor(Sector* Node);
    void displaySectorsInOrder();
    void InOrderTraversal(Sector* Node);
    void displaySectorsPreOrder();
    void PreOrderTraversal(Sector* Node);
    void displaySectorsPostOrder();
    void PostOrderTraversal(Sector* Node);
    bool isRight(Sector* Node, const std::string & sector_code);
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
};

#endif // SPACESECTORLLRBT_H
