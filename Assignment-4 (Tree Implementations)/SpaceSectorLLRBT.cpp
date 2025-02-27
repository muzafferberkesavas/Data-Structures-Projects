#include "SpaceSectorLLRBT.h"

using namespace std;

SpaceSectorLLRBT::SpaceSectorLLRBT() : root(nullptr) {}

void SpaceSectorLLRBT::readSectorsFromFile(const std::string& filename) {
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

SpaceSectorLLRBT::~SpaceSectorLLRBT() {
    clear(root);
}

void SpaceSectorLLRBT::clear(Sector *Node) {
    if (Node != nullptr){
        clear(Node->left);
        clear(Node->right);
        delete Node;
    }
}

void SpaceSectorLLRBT::insertSectorByCoordinates(int x, int y, int z) {
    if (root == nullptr){
        root = new Sector(x,y,z);
        root->color = BLACK;
    }else{
        Sector* Node = new Sector(x,y,z);
        insertSectorByRecursive(root,Node);
        balance(root, true);
        balance(root, true);
        balance(root, true);
    }
    balance(root, true);
    balance(root, true);
}

Sector* SpaceSectorLLRBT::insertSectorByRecursive(Sector* Node_1 , Sector* Node_2){
    if (Node_1 == nullptr){
        return Node_2;
    }


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

void SpaceSectorLLRBT::balance(Sector *Node, bool isRoot) {
    if (Node == nullptr){
        return;
    }
    if (Node->right != nullptr && Node->left != nullptr && Node->right->color == RED && Node->left->color == RED){
        flipColor(Node);
    }

    if (isRoot){

        if (Node->right != nullptr && Node->right->color == RED){
            root = leftRotate(Node);
            root->color = BLACK;
        }
        if (Node->right != nullptr && Node->right->right != nullptr && Node->right->right->color == RED){
            root->right = leftRotate(Node->right);
        }
        if (Node->left != nullptr && Node->left->right != nullptr && Node->left->right->color == RED){
            root->left = leftRotate(Node->left);
        }
        if (Node->left != nullptr && Node->left->left != nullptr && Node->left->color == RED && Node->left->left->color == RED){
            root = rightRotate(Node);
            root->color = BLACK;
        }
        if (Node->left != nullptr && Node->left->left != nullptr && Node->left->left->left != nullptr && Node->left->left->color == RED && Node->left->left->left->color == RED){
            root->left = rightRotate(Node->left);
        }
        if (Node->right != nullptr && Node->right->left != nullptr && Node->right->left->left != nullptr && Node->right->left->color == RED && Node->right->left->left->color == RED){
            root->right = rightRotate(Node->right);
        }
    } else{

        if (Node->right != nullptr && Node->right->right != nullptr && Node->right->right->color == RED){
            Node->right = leftRotate(Node->right);
        }
        if (Node->left != nullptr && Node->left->right != nullptr && Node->left->right->color == RED){
            Node->left = leftRotate(Node->left);
        }
        if (Node->left != nullptr && Node->left->left != nullptr && Node->left->left->left != nullptr && Node->left->left->color == RED && Node->left->left->left->color == RED){
            Node->left = rightRotate(Node->left);
        }
        if (Node->right != nullptr && Node->right->left != nullptr && Node->right->left->left != nullptr && Node->right->left->color == RED && Node->right->left->left->color == RED){
            Node->right = rightRotate(Node->right);
        }
    }
    if (root->color == RED){
        root->color = BLACK;
    }
    balance(Node->left, false);
    balance(Node->right, false);
}

Sector* SpaceSectorLLRBT::leftRotate(Sector *Node) {
    Sector* temp = Node->right;
    Node->right = temp->left;
    temp->left = Node;
    temp->color = Node->color;
    Node->color = RED;
    return temp;
}

Sector* SpaceSectorLLRBT::rightRotate(Sector *Node) {
    Sector* temp = Node->left;
    Node->left = temp->right;
    temp->right = Node;
    temp->color = Node->color;
    Node->color = RED;
    return temp;
}

void SpaceSectorLLRBT::flipColor(Sector *Node) {
    if (Node != nullptr && Node->left != nullptr && Node->right != nullptr){
        Node->color = RED;
        Node->left->color = BLACK;
        Node->right->color = BLACK;
    }
}

void SpaceSectorLLRBT::displaySectorsInOrder() {
    balance(root, true);
    cout<<"Space sectors inorder traversal:"<<endl;
    InOrderTraversal(root);
    cout<<endl;
}

void SpaceSectorLLRBT::InOrderTraversal(Sector *Node) {
    if (Node != nullptr){
        InOrderTraversal(Node->left);
        string out = !Node->color ? "BLACK" : "RED";
        out = out + " sector: ";
        cout<<out<<Node->sector_code<<endl;
        InOrderTraversal(Node->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPreOrder() {
    balance(root, true);
    cout<<"Space sectors preorder traversal:"<<endl;
    PreOrderTraversal(root);
    cout<<endl;
}

void SpaceSectorLLRBT::PreOrderTraversal(Sector *Node) {
    if (Node != nullptr){
        string out = !Node->color ? "BLACK" : "RED";
        out = out + " sector: ";
        cout<<out<<Node->sector_code<<endl;
        PreOrderTraversal(Node->left);
        PreOrderTraversal(Node->right);
    }
}

void SpaceSectorLLRBT::displaySectorsPostOrder() {
    balance(root, true);
    cout<<"Space sectors postorder traversal:"<<endl;
    PostOrderTraversal(root);
    cout<<endl;
}

void SpaceSectorLLRBT::PostOrderTraversal(Sector *Node) {
    if (Node != nullptr){
        PreOrderTraversal(Node->left);
        PreOrderTraversal(Node->right);
        string out = !Node->color ? "BLACK" : "RED";
        out = out + " sector: ";
        cout<<out<<Node->sector_code<<endl;
    }
}

bool SpaceSectorLLRBT::isRight(Sector* Node, const std::string & sector_code){
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

std::vector<Sector*> SpaceSectorLLRBT::getStellarPath(const std::string& sector_code) {
    std::vector<Sector*> path;
    Sector* temp = root;
    bool isPathValid = false;
    if (root != nullptr && root->sector_code == sector_code){
        isPathValid = true;
        path.push_back(temp);
        goto end;
    }
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
    end:
    if (isPathValid){
        return path;
    } else{
        path.clear();
        return path;
    }
}

void SpaceSectorLLRBT::printStellarPath(const std::vector<Sector*>& path) {
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