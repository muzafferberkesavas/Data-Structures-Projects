#include "BlockFall.h"
#include "fstream"
#include "iostream"

BlockFall::BlockFall(string grid_file_name, string blocks_file_name, bool gravity_mode_on, const string &leaderboard_file_name, const string &player_name) : gravity_mode_on(
        gravity_mode_on), leaderboard_file_name(leaderboard_file_name), player_name(player_name) {
    initialize_grid(grid_file_name);
    read_blocks(blocks_file_name);
    leaderboard.read_from_file(leaderboard_file_name);
}

void BlockFall::read_blocks(const string &input_file) {
    // TODO: Read the blocks from the input file and initialize "initial_block" and "active_rotation" member variables
    // TODO: For every block, generate its rotations and properly implement the multilevel linked list structure
    //       that represents the game blocks, as explained in the PA instructions.
    // TODO: Initialize the "power_up" member variable as the last block from the input file (do not add it to the linked list!)
    ifstream file(input_file);
    if (file.is_open()){
        string line;
        vector<string>lines;

        while (getline(file,line)){
            lines.push_back(line);
        }
        file.close();
        vector<vector<vector<bool>>>Blocks;
        for (int index = 0; index < lines.size();index++){
            vector<vector<bool>>matrix_2D;
            vector<bool>matrix_1D;
            restart:
            string line1 =  lines.at(index);
            if (!line1.empty()){
                if (line1.find('[') != string::npos && line1.find(']') != string::npos){
                    string substring = line1.substr(1,line1.length()-2);
                    for (char c : substring){
                        if (c == '1'){
                            matrix_1D.push_back(true);
                        } else{
                            matrix_1D.push_back(false);
                        }
                    }
                    matrix_2D.push_back(matrix_1D);
                    Blocks.push_back(matrix_2D);
                    matrix_1D.clear();
                    matrix_2D.clear();
                } else if (line1.find('[') != string::npos && line1.find(']') == string::npos){
                    string substring = line1.substr(1,line1.length()-1);
                    for(char c : substring){
                        if (c == '1'){
                            matrix_1D.push_back(true);
                        }else{
                            matrix_1D.push_back(false);
                        }
                    }
                    matrix_2D.push_back(matrix_1D);
                    matrix_1D.clear();
                    index++;
                    goto restart;
                }else if (line1.find('[') == string::npos && line1.find(']') != string::npos){
                    string substring = line1.substr(0,line1.length()-1);
                    for(char c : substring){
                        if (c == '1'){
                            matrix_1D.push_back(true);
                        } else{
                            matrix_1D.push_back(false);
                        }
                    }
                    matrix_2D.push_back(matrix_1D);
                    Blocks.push_back(matrix_2D);
                }else{
                    string substring = line1.substr(0,line1.length());
                    for(char c : substring){
                        if (c == '1'){
                            matrix_1D.push_back(true);
                        }else {
                            matrix_1D.push_back(false);
                        }
                    }
                    matrix_2D.push_back(matrix_1D);
                    matrix_1D.clear();
                    index++;
                    goto restart;
                }
            }
        }

        for (int i = 0; i < Blocks.back().size();++i){
            power_up.push_back(Blocks.back().at(i));
        }

        initial_block = new Block;
        active_rotation = initial_block;
        nextBlock(Blocks,initial_block,0);


    }
}

void BlockFall::initialize_grid(const string &input_file) {
    // TODO: Initialize "rows" and "cols" member variables
    // TODO: Initialize "grid" member variable using the command-line argument 1 in main
    ifstream file(input_file);
    if (file.is_open()){
        string line;
        vector<string>lines;

        while (getline(file,line)){
            lines.push_back(line);
        }
        file.close();
        rows = lines.size();
        int colm = 0;
        for (int index = 0; index < lines.size(); ++index) {
            vector<int>row;
            string line1 = lines.at(index);
            for (char c : line1){
                if (c =='0'){
                    row.push_back(0);
                    colm++;
                } else if (c == '1'){
                    row.push_back(1);
                    colm++;
                }
            }
            grid.push_back(row);
        }
        cols = colm / lines.size();
    }
}


void BlockFall::rotate(vector<vector<bool>> &shape, Block* block) {
    vector<vector<bool>> leftRotated1(leftRotation(shape));
    vector<vector<bool>> leftRotated2(leftRotation(leftRotated1));
    vector<vector<bool>> leftRotated3(leftRotation(leftRotated2));


    block->left_rotation = new Block;
    block->left_rotation->left_rotation = new Block;
    block->right_rotation = new Block;

    for (const auto& row : leftRotated1){
        block->left_rotation->shape.push_back(row);
    }
    for (const auto& row : leftRotated2){
        block->left_rotation->left_rotation->shape.push_back(row);
    }
    for(const auto& row : leftRotated3){
        block->right_rotation->shape.push_back(row);
    }

    block->left_rotation->right_rotation = block;
    block->left_rotation->left_rotation->left_rotation = block->right_rotation;
    block->left_rotation->left_rotation->right_rotation = block->left_rotation;
    block->right_rotation->left_rotation = block;
    block->right_rotation->right_rotation = block->left_rotation->left_rotation;



}

void BlockFall::nextBlock(vector<vector<vector<bool>>> &Blocks,Block* block, int index) {
    if (index != Blocks.size()-2){

        Block* newBlock = new Block;
        for (const auto& row : Blocks.at(index)){
            block->shape.push_back(row);
        }
        rotate(Blocks.at(index),block);
        block->next_block = newBlock;
        block->left_rotation->next_block = newBlock;
        block->right_rotation->next_block = newBlock;
        block->left_rotation->left_rotation->next_block = newBlock;
        nextBlock(Blocks,newBlock,index+1);
    } else{


        for (const auto& row : Blocks.at(index)){
            block->shape.push_back(row);
        }
        rotate(Blocks.at(index),block);
        block->next_block = nullptr;
        block->left_rotation->next_block = nullptr;
        block->right_rotation->next_block = nullptr;
        block->left_rotation->left_rotation->next_block = nullptr;
    }
}


vector<vector<bool>>BlockFall::leftRotation(const vector<vector<bool>> &currentShape) {
    vector<vector<bool>>leftRotated(currentShape[0].size(), vector<bool>(currentShape.size(), false));
    for (int k = currentShape[0].size()-1; k >=0 ; --k) {
        for (int j = 0; j < currentShape.size(); ++j) {
            leftRotated[currentShape[0].size()-k-1][j] = currentShape[j][k];
        }
    }
    return leftRotated;
}

void BlockFall::clear(Block *block) {
    if (!block){

        return;
    }
    clear(block->next_block);

    delete block->left_rotation->left_rotation;
    delete block->left_rotation;
    delete block->right_rotation;
    delete block;
}


BlockFall::~BlockFall() {
    // TODO: Free dynamically allocated memory used for storing game blocks
    clear(initial_block);
    active_rotation = nullptr;

}
