#include "SpaceSectorBST.h"
#include "string"

using namespace std;

SpaceSectorBST::SpaceSectorBST() : root(nullptr) {}

SpaceSectorBST::~SpaceSectorBST() {
    clear(root);
}

void SpaceSectorBST::clear(Sector *Node) {
    if (Node != nullptr){
        clear(Node->left);
        clear(Node->right);
        delete Node;
    }
}

void SpaceSectorBST::readSectorsFromFile(const std::string& filename) {
    ifstream file(filename);
    if (file.is_open()){
        string first_line,line;
        getline(file,first_line);
        while (getline(file,line)){
            istringstream ss(line);
            int x,y,z;
            int count = 0;
            string number;
            while (getline(ss,number,',')){
                if (count == 0){
                    x = stoi(number);
                    count++;
                }else if (count == 1){
                    y = stoi(number);
                    count++;
                } else if(count == 2){
                    z = stoi(number);
                    count++;
                }
            }
            insertSectorByCoordinates(x,y,z);

        }
        file.close();
    }
}

void SpaceSectorBST::insertSectorByCoordinates(int x, int y, int z) {
    if (root == nullptr){
        root = new Sector(x,y,z);
    }else{
        Sector* Node = new Sector(x,y,z);
        insertSectorByRecursive(root,Node);
    }
}
Sector* SpaceSectorBST::insertSectorByRecursive(Sector* Node_1, Sector* Node_2) {
    if (Node_1 == nullptr){
        Node_2->parent = nullptr;
        return Node_2;
    }

    Node_2->parent = Node_1;

    if (Node_2->x < Node_1->x){
        Node_1->left = insertSectorByRecursive(Node_1->left,Node_2);
    }else if (Node_2->x > Node_1->x){
        Node_1->right = insertSectorByRecursive(Node_1->right,Node_2);
    } else{
        if (Node_2->y < Node_1->y){
            Node_1->left = insertSectorByRecursive(Node_1->left,Node_2);
        }else if (Node_2->y > Node_1->y){
            Node_1->right = insertSectorByRecursive(Node_1->right,Node_2);
        }else{
            if (Node_2->z < Node_1->z){
                Node_1->left = insertSectorByRecursive(Node_1->left,Node_2);
            }else if (Node_2->z > Node_1->z){
                Node_1->right = insertSectorByRecursive(Node_1->right,Node_2);
            }else{
                Node_1->right = insertSectorByRecursive(Node_1->right,Node_2);
            }
        }
    }
    return Node_1;
}

void SpaceSectorBST::deleteSector(const std::string& sector_code) {
    deleteRecursive(root,sector_code);
}

Sector* SpaceSectorBST::deleteRecursive(Sector *Node_1, const string &sector_code) {
    if (Node_1 == nullptr){
        return Node_1;
    }

    bool is_Right = isRight(Node_1->right,sector_code);


    if (Node_1->sector_code == sector_code){
        if (root->sector_code == sector_code){
            if (Node_1->left == nullptr){
                Sector* temp = Node_1->right;
                delete Node_1;
                root = temp;
                return temp;
            } else if (Node_1->right == nullptr){
                Sector* temp = Node_1->left;
                delete Node_1;
                root = temp;
                return temp;
            } else{
                Sector* temp = Node_1->right;
                while (temp->left != nullptr){
                    temp = temp->left;
                }
                Node_1->sector_code = temp->sector_code;
                Node_1->x = temp->x;
                Node_1->y = temp->y;
                Node_1->z = temp->z;
                Node_1->distance_from_earth = temp->distance_from_earth;
                Node_1->right = deleteRecursive(Node_1->right,Node_1->sector_code);
            }
        } else{
            if (Node_1->left == nullptr){
                Sector* temp = Node_1->right;
                delete Node_1;
                return temp;
            }else if (Node_1->right == nullptr){
                Sector* temp = Node_1->left;
                delete Node_1;
                return temp;
            } else{
                Sector* temp = Node_1->right;
                while (temp->left != nullptr){
                    temp = temp->left;
                }
                Node_1->sector_code = temp->sector_code;
                Node_1->x = temp->x;
                Node_1->y = temp->y;
                Node_1->z = temp->z;
                Node_1->distance_from_earth = temp->distance_from_earth;
                Node_1->right = deleteRecursive(Node_1->right,Node_1->sector_code);
            }
        }
    } else if (is_Right){
        Node_1->right = deleteRecursive(Node_1->right,sector_code);
    } else{
        Node_1->left = deleteRecursive(Node_1->left,sector_code);
    }
    return Node_1;
}

bool SpaceSectorBST::isRight(Sector *Node, const std::string &sector_code) {
    if (Node == nullptr){
        return false;
    }
    if (Node->sector_code == sector_code){
        return true;
    } else{
        bool bool_1 = isRight(Node->left,sector_code);
        bool bool_2 = isRight(Node->right,sector_code);
        if (bool_1 || bool_2){
            return true;
        } else{
            return false;
        }
    }
}

void SpaceSectorBST::displaySectorsInOrder() {
    cout<<"Space sectors inorder traversal:"<<endl;
    InOrderTraversal(root);
    cout<<endl;
}

void  SpaceSectorBST::InOrderTraversal(Sector *Node) {
    if (Node != nullptr){
        InOrderTraversal(Node->left);
        cout<<Node->sector_code<<endl;
        InOrderTraversal(Node->right);
    }
}

void SpaceSectorBST::displaySectorsPreOrder() {
    cout<<"Space sectors preorder traversal:"<<endl;
    PreOrderTraversal(root);
    cout<<endl;
}

void SpaceSectorBST::PreOrderTraversal(Sector *Node) {
    if (Node != nullptr){
        cout<<Node->sector_code<<endl;
        PreOrderTraversal(Node->left);
        PreOrderTraversal(Node->right);
    }
}

void SpaceSectorBST::displaySectorsPostOrder() {
    cout<<"Space sectors postorder traversal:"<<endl;
    PostOrderTraversal(root);
    cout<<endl;
}

void SpaceSectorBST::PostOrderTraversal(Sector *Node) {
    if (Node != nullptr){
        PostOrderTraversal(Node->left);
        PostOrderTraversal(Node->right);
        cout<<Node->sector_code<<endl;
    }
}

std::vector<Sector*> SpaceSectorBST::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    Sector* temp = root;
    bool isPathValid = false;
    while (temp != nullptr){

        bool is_Right = isRight(temp->right,sector_code);

        if (temp->sector_code == sector_code){
            path.push_back(temp);
            isPathValid = true;
            break;
        } else if (is_Right){
            path.push_back(temp);
            temp = temp->right;
        } else{
            path.push_back(temp);
            temp = temp->left;
        }
    }
    if (isPathValid){
        return path;
    } else{
        path.clear();
        return path;
    }
}


void SpaceSectorBST::printStellarPath(const std::vector<Sector*>& path) {
    if (!path.empty()){
        cout<<"The stellar path to Dr. Elara: ";
        for(size_t i = 0; i < path.size(); i++){
            Sector* Node = path[i];
            if (i == path.size() - 1){
                cout<<Node->sector_code<<endl;
            } else{
                cout<<Node->sector_code<<"->";
            }
        }
    } else{
        cout<<"A path to Dr. Elara could not be found."<<endl;
    }
}