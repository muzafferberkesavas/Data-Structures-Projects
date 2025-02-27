#ifndef SPACESECTORBST_H
#define SPACESECTORBST_H

#include <iostream>
#include <fstream>  
#include <sstream>
#include <vector>

#include "Sector.h"

class SpaceSectorBST {
  
public:
    Sector *root;
    SpaceSectorBST();
    ~SpaceSectorBST();
    void readSectorsFromFile(const std::string& filename); 
    void insertSectorByCoordinates(int x, int y, int z);
    Sector* insertSectorByRecursive(Sector* Node_1 , Sector* Node_2);
    void deleteSector(const std::string& sector_code);
    Sector* deleteRecursive(Sector* Node_1, const std::string& sector_code);
    void displaySectorsInOrder();
    void InOrderTraversal(Sector* Node);
    void displaySectorsPreOrder();
    void PreOrderTraversal(Sector* Node);
    void displaySectorsPostOrder();
    void PostOrderTraversal(Sector* Node);
    bool isRight(Sector* Node, const std::string & sector_code);
    void clear(Sector* Node);
    std::vector<Sector*> getStellarPath(const std::string& sector_code);
    void printStellarPath(const std::vector<Sector*>& path);
};

#endif // SPACESECTORBST_H
