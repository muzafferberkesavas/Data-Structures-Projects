#include "GameController.h"
#include "fstream"
#include  <vector>
#include "iostream"
#include "ctime"
#define occupiedCellChar "██"
#define unoccupiedCellChar "▒▒"

bool GameController::play(BlockFall& game, const string& commands_file){

    // TODO: Implement the gameplay here while reading the commands from the input file given as the 3rd command-line
    //       argument. The return value represents if the gameplay was successful or not: false if game over,
    //       true otherwise.
    ifstream file(commands_file);
    if (file.is_open()){
        string line;
        vector<string> lines;
        while (getline(file,line)){
            lines.push_back(line);
        }
        file.close();

        if (game.gravity_mode_on){
            for (int i = 0; i < game.rows; ++i) {
                for (int j = 0; j < game.cols; ++j) {
                    if (game.grid[i][j]){
                        recursiveDropGrid(i,j,game);
                    }
                }
            }
        }
        if (powerUpDetector(game)){
            for (int i = 0; i < game.grid.size(); ++i) {
                for (int j = 0; j < game.grid[0].size(); ++j) {
                    if (game.grid[i][j]){
                        game.current_score++;
                        game.grid[i][j] = false;
                    }
                }
            }
            game.current_score += 1000;
            if (game.initial_block == nullptr && game.initial_block->next_block != nullptr && !(game.initial_block->next_block->shape.empty()) && game.initial_block->next_block->shape.size() <= game.grid.size() && game.initial_block->next_block->shape[0].size() <= game.grid[0].size()){
                for (int i = 0; i < game.initial_block->next_block->shape.size(); ++i) {
                    for (int j = 0; j < game.initial_block->next_block->shape[0].size(); ++j) {
                        if (game.grid[i][j] && game.initial_block->next_block->shape[i][j]){
                            gameOver("GAME OVER!",game);
                            return false;
                        }
                    }
                }
            }else {
                gameOver("YOU WIN!",game);
                return true;
            }
            Block* temp = game.initial_block->next_block;
            delete game.initial_block->left_rotation->left_rotation;
            delete game.initial_block->left_rotation;
            delete game.initial_block->right_rotation;
            delete game.initial_block;
            game.initial_block = temp;


        } else{
            rowCheck(game, false);
        }
        if (game.initial_block != nullptr && !(game.initial_block->shape.empty()) && game.initial_block->shape.size() <= game.grid.size() && game.initial_block->shape[0].size() <= game.grid[0].size()){
            for (int i = 0; i < game.initial_block->shape.size(); ++i) {
                for (int j = 0; j < game.initial_block->shape[0].size(); ++j) {
                    if (game.grid[i][j] && game.initial_block->shape[i][j]){
                        gameOver("GAME OVER!",game);
                        return false;
                    }
                }
            }
        }else {
            gameOver("YOU WIN!",game);
            return true;
        }

        int rowBeginning_block = 0;
        int columnBeginning_block = 0;
        int rowEnding_block = game.initial_block->shape.size()-1;
        int columnEnding_block = game.initial_block->shape[0].size()-1;
        for (string command : lines){
            if (command == "PRINT_GRID"){
                cout<<"Score: "<<game.current_score<<endl;
                cout<<"High Score: "<<(game.leaderboard.head_leaderboard_entry == nullptr ? game.current_score : (game.leaderboard.head_leaderboard_entry->score > game.current_score ? game.leaderboard.head_leaderboard_entry->score : game.current_score))<<endl;
                for (int i = 0; i < game.grid.size(); ++i) {
                    for (int j = 0; j < game.grid[0].size(); ++j) {
                        if (rowBeginning_block <= i && i <= rowEnding_block && columnBeginning_block<=j && j<=columnEnding_block){
                            if (game.initial_block->shape[i-rowBeginning_block][j-columnBeginning_block]){
                                cout<<occupiedCellChar;
                            } else{
                                cout<<unoccupiedCellChar;
                            }
                        } else{
                            if (game.grid[i][j]){
                                cout<<occupiedCellChar;
                            } else{
                                cout<<unoccupiedCellChar;
                            }
                        }
                    }
                    cout<<endl;
                }
                cout<<endl<<endl;
            }else if (command == "ROTATE_RIGHT"){
                if (game.initial_block->right_rotation->shape[0].size() +columnBeginning_block  <= game.cols && game.initial_block->right_rotation != nullptr){
                    int tempRow = rowEnding_block;
                    int tempColumn = columnEnding_block;
                    rowEnding_block = rowBeginning_block + tempColumn - columnBeginning_block ;
                    columnEnding_block = columnBeginning_block + tempRow - rowBeginning_block ;
                    bool isRotateValid = true;
                    for (int i = 0; i < rowEnding_block-rowBeginning_block+1; ++i) {
                        for (int j = 0; j < columnEnding_block-columnBeginning_block+1; ++j) {
                            if (game.initial_block->right_rotation->shape[i][j] && game.grid[rowBeginning_block + i][columnBeginning_block + j]){
                                isRotateValid = false;
                            }
                        }
                    }
                    if (isRotateValid){
                        game.initial_block = game.initial_block->right_rotation;
                        game.active_rotation = game.initial_block;
                    } else{
                        rowEnding_block = tempRow;
                        columnEnding_block = tempColumn;
                    }
                }
            }else if (command == "ROTATE_LEFT"){
                if (game.initial_block->left_rotation->shape[0].size() + columnBeginning_block<= game.cols && game.initial_block->left_rotation != nullptr){
                    int tempRow = rowEnding_block;
                    int tempColumn = columnEnding_block;
                    rowEnding_block = rowBeginning_block + tempColumn - columnBeginning_block ;
                    columnEnding_block = columnBeginning_block + tempRow - rowBeginning_block ;
                    bool isRotateValid = true;
                    for (int i = 0; i < rowEnding_block-rowBeginning_block+1; ++i) {
                        for (int j = 0; j < columnEnding_block-columnBeginning_block+1; ++j) {
                            if (game.initial_block->left_rotation->shape[i][j] && game.grid[rowBeginning_block + i][columnBeginning_block + j]){
                                isRotateValid = false;
                                break;
                            }
                        }
                    }
                    if (isRotateValid){
                        game.initial_block = game.initial_block->left_rotation;
                        game.active_rotation = game.initial_block;
                    } else{
                        rowEnding_block = tempRow;
                        columnEnding_block = tempColumn;
                    }
                }
            }else if (command == "MOVE_RIGHT"){
                if (columnEnding_block != game.cols-1){
                    bool isMoveValid = true;
                    for (int row = rowBeginning_block; row <= rowEnding_block; ++row) {
                        if (game.grid[row][columnEnding_block+1] && game.initial_block->shape[row][columnEnding_block]){
                            isMoveValid = false;
                            break;
                        }
                    }
                    if (isMoveValid){
                     columnBeginning_block++;
                     columnEnding_block++;
                    }
                }
            }else if (command == "MOVE_LEFT"){
                if (columnBeginning_block != 0){
                    bool isMoveValid = true;
                    for (int row = rowBeginning_block; row <= rowEnding_block; ++row) {
                        if (game.grid[row][columnBeginning_block-1] && game.initial_block->shape[row][columnBeginning_block]){
                            isMoveValid = false;
                            break;
                        }
                    }
                    if (isMoveValid){
                        columnBeginning_block--;
                        columnEnding_block--;
                    }
                }

            }else if (command == "DROP"){
                recursiveDropBlock(columnEnding_block,rowEnding_block,game);
                if (game.gravity_mode_on){
                    for (int i = 0; i < game.rows; ++i) {
                        for (int j = 0; j < game.cols; ++j) {
                            if (game.grid[i][j]){
                                recursiveDropGrid(i,j,game);
                            }
                        }
                    }
                    if (powerUpDetector(game)){
                        for (int i = 0; i < game.grid.size(); ++i) {
                            for (int j = 0; j < game.grid[0].size(); ++j) {
                                if (game.grid[i][j]){
                                    game.current_score++;
                                    game.grid[i][j] = false;
                                }
                            }
                        }
                        game.current_score += 1000;
                        if (game.initial_block->next_block != nullptr && !(game.initial_block->next_block->shape.empty()) && game.initial_block->next_block->shape.size() <= game.grid.size() && game.initial_block->next_block->shape[0].size() <= game.grid[0].size()){
                            for (int i = 0; i < game.initial_block->next_block->shape.size(); ++i) {
                                for (int j = 0; j < game.initial_block->next_block->shape[0].size(); ++j) {
                                    if (game.grid[i][j] && game.initial_block->next_block->shape[i][j]){
                                        gameOver("GAME OVER!",game);
                                        return false;
                                    }
                                }
                            }
                        }else {
                            gameOver("YOU WIN!",game);
                            return true;
                        }
                        Block* temp = game.initial_block->next_block;
                        delete game.initial_block->left_rotation->left_rotation;
                        delete game.initial_block->left_rotation;
                        delete game.initial_block->right_rotation;
                        delete game.initial_block;
                        game.initial_block = temp;


                        rowBeginning_block = 0;
                        columnBeginning_block = 0;
                        rowEnding_block = game.initial_block->shape.size()-1;
                        columnEnding_block = game.initial_block->shape[0].size()-1;
                    } else{
                        //row temizleniyor mu kontrol et
                        rowCheck(game, false);
                        if (game.initial_block->next_block != nullptr && !(game.initial_block->next_block->shape.empty()) && game.initial_block->next_block->shape.size() <= game.grid.size() && game.initial_block->next_block->shape[0].size() <= game.grid[0].size()){
                            for (int i = 0; i < game.initial_block->next_block->shape.size(); ++i) {
                                for (int j = 0; j < game.initial_block->next_block->shape[0].size(); ++j) {
                                    if (game.grid[i][j] && game.initial_block->next_block->shape[i][j]){
                                        gameOver("GAME OVER!",game);
                                        return false;
                                    }
                                }
                            }
                        }else {
                            gameOver("YOU WIN!",game);
                            return true;
                        }
                        Block* temp = game.initial_block->next_block;
                        delete game.initial_block->left_rotation->left_rotation;
                        delete game.initial_block->left_rotation;
                        delete game.initial_block->right_rotation;
                        delete game.initial_block;
                        game.initial_block = temp;


                        rowBeginning_block = 0;
                        columnBeginning_block = 0;
                        rowEnding_block = game.initial_block->shape.size()-1;
                        columnEnding_block = game.initial_block->shape[0].size()-1;
                    }
                } else{
                    if (powerUpDetector(game)){
                        for (int i = 0; i < game.grid.size(); ++i) {
                            for (int j = 0; j < game.grid[0].size(); ++j) {
                                if (game.grid[i][j]){
                                    game.current_score++;
                                    game.grid[i][j] = false;
                                }
                            }
                        }
                        game.current_score += 1000;
                        if (game.initial_block->next_block != nullptr && !(game.initial_block->next_block->shape.empty()) && game.initial_block->next_block->shape.size() <= game.grid.size() && game.initial_block->next_block->shape[0].size() <= game.grid[0].size()){
                            for (int i = 0; i < game.initial_block->next_block->shape.size(); ++i) {
                                for (int j = 0; j < game.initial_block->next_block->shape[0].size(); ++j) {
                                    if (game.grid[i][j] && game.initial_block->next_block->shape[i][j]){
                                        gameOver("GAME OVER!",game);
                                        return false;
                                    }
                                }
                            }
                        }else {
                            gameOver("YOU WIN!",game);
                            return true;
                        }
                        Block* temp = game.initial_block->next_block;
                        delete game.initial_block->left_rotation->left_rotation;
                        delete game.initial_block->left_rotation;
                        delete game.initial_block->right_rotation;
                        delete game.initial_block;
                        game.initial_block = temp;



                        rowBeginning_block = 0;
                        columnBeginning_block = 0;
                        rowEnding_block = game.initial_block->shape.size()-1;
                        columnEnding_block = game.initial_block->shape[0].size()-1;
                    } else{
                        rowCheck(game, false);
                        if (game.initial_block->next_block != nullptr && !(game.initial_block->next_block->shape.empty()) && game.initial_block->next_block->shape.size() <= game.grid.size() && game.initial_block->next_block->shape[0].size() <= game.grid[0].size()){
                            for (int i = 0; i < game.initial_block->next_block->shape.size(); ++i) {
                                for (int j = 0; j < game.initial_block->next_block->shape[0].size(); ++j) {
                                    if (game.grid[i][j] && game.initial_block->next_block->shape[i][j]){
                                        gameOver("GAME OVER!",game);
                                        return false;
                                    }
                                }
                            }
                        }else {
                            gameOver("YOU WIN!",game);
                            return true;
                        }
                        Block* temp = game.initial_block->next_block;
                        delete game.initial_block->left_rotation->left_rotation;
                        delete game.initial_block->left_rotation;
                        delete game.initial_block->right_rotation;
                        delete game.initial_block;
                        game.initial_block = temp;



                        rowBeginning_block = 0;
                        columnBeginning_block = 0;
                        rowEnding_block = game.initial_block->shape.size()-1;
                        columnEnding_block = game.initial_block->shape[0].size()-1;
                    }
                }

            }else if (command == "GRAVITY_SWITCH"){

                if (game.gravity_mode_on){
                    game.gravity_mode_on = false;
                } else{
                    game.gravity_mode_on = true;
                    for (int i = 0; i < game.rows; ++i) {
                        for (int j = 0; j < game.cols; ++j) {
                            if (game.grid[i][j]){
                                recursiveDropGrid(i,j,game);
                            }
                        }
                    }
                    if (powerUpDetector(game)){
                        for (int i = 0; i < game.grid.size(); ++i) {
                            for (int j = 0; j < game.grid[0].size(); ++j) {
                                if (game.grid[i][j]){
                                    game.current_score++;
                                    game.grid[i][j] = false;
                                }
                            }
                        }
                        game.current_score += 1000;
                        if (game.initial_block->next_block != nullptr && !(game.initial_block->next_block->shape.empty()) && game.initial_block->next_block->shape.size() <= game.grid.size() && game.initial_block->next_block->shape[0].size() <= game.grid[0].size()){
                            for (int i = 0; i < game.initial_block->next_block->shape.size(); ++i) {
                                for (int j = 0; j < game.initial_block->next_block->shape[0].size(); ++j) {
                                    if (game.grid[i][j] && game.initial_block->next_block->shape[i][j]){
                                        gameOver("GAME OVER!",game);
                                        return false;
                                    }
                                }
                            }
                        }else {
                            gameOver("YOU WIN!",game);
                            return true;
                        }
                        Block* temp = game.initial_block->next_block;
                        delete game.initial_block->left_rotation->left_rotation;
                        delete game.initial_block->left_rotation;
                        delete game.initial_block->right_rotation;
                        delete game.initial_block;
                        game.initial_block = temp;


                    } else{
                        rowCheck(game, true);
                    }
                }
            }else {
                cout<<"Unknown command: "<<command<<endl;
            }
        }
        gameOver("GAME FINISHED!",game);
        return true;
    } else{
        exit(0);
    }

}
void GameController::gameOver(string message, BlockFall &game) {
    time_t currentTime;
    time(&currentTime);
    LeaderboardEntry* newEntry = new LeaderboardEntry(game.current_score,currentTime,game.player_name);
    game.leaderboard.insert_new_entry(newEntry);
    if (message =="GAME OVER!"){
        cout<<message<<endl<<"Next block that couldn't fit:"<<endl;
        for (int i = 0; i < game.initial_block->next_block->shape.size(); ++i) {
            for (int j = 0; j < game.initial_block->next_block->shape[0].size(); ++j) {
                if (game.initial_block->next_block->shape[i][j]){
                    cout<<occupiedCellChar;
                } else{
                    cout<<unoccupiedCellChar;
                }
            }
            cout<<endl;
        }
        cout<<endl<<"Final grid and score:\n"<<endl;
        cout<<"Score: "<<game.current_score<<endl;
        cout<<"High Score: "<<game.leaderboard.head_leaderboard_entry->score<<endl;
        for (int i = 0; i < game.rows; ++i) {
            for (int j = 0; j < game.cols; ++j) {
                if (game.grid[i][j]){
                    cout<<occupiedCellChar;
                } else{
                    cout<<unoccupiedCellChar;
                }
            }
            cout<<endl;
        }
        cout<<endl;
        game.leaderboard.print_leaderboard();
        game.leaderboard.write_to_file(game.leaderboard_file_name);

    } else if (message == "GAME FINISHED!"){
        cout<<message<<endl<<"No more commands."<<endl;
        cout<<"Final grid and score:\n"<<endl;
        cout<<"Score: "<<game.current_score<<endl;
        cout<<"High Score: "<<game.leaderboard.head_leaderboard_entry->score<<endl;
        for (int i = 0; i < game.rows; ++i) {
            for (int j = 0; j < game.cols; ++j) {
                if (game.grid[i][j]){
                    cout<<occupiedCellChar;
                } else{
                    cout<<unoccupiedCellChar;
                }
            }
            cout<<endl;
        }
        cout<<endl;
        game.leaderboard.print_leaderboard();
        game.leaderboard.write_to_file(game.leaderboard_file_name);

    } else if (message == "YOU WIN!"){
        cout<<message<<endl<<"No more blocks."<<endl;
        cout<<"Final grid and score:\n"<<endl;
        cout<<"Score: "<<game.current_score<<endl;
        cout<<"High Score: "<<game.leaderboard.head_leaderboard_entry->score<<endl;
        for (int i = 0; i < game.rows; ++i) {
            for (int j = 0; j < game.cols; ++j) {
                if (game.grid[i][j]){
                    cout<<occupiedCellChar;
                } else{
                    cout<<unoccupiedCellChar;
                }
            }
            cout<<endl;
        }
        cout<<endl;
        game.leaderboard.print_leaderboard();
        game.leaderboard.write_to_file(game.leaderboard_file_name);

    }
}

void GameController::recursiveDropGrid(int row_index, int column_index,BlockFall &game) {
    if (row_index == game.rows-1 || row_index == -1){
        return;
    } else{
            if (game.grid[row_index][column_index] && !game.grid[row_index+1][column_index]){
                game.grid[row_index][column_index] = false;
                game.grid[row_index+1][column_index] = true;
                recursiveDropGrid(row_index+1,column_index,game);
                recursiveDropGrid(row_index-1,column_index,game);
            }
    }
}

void GameController::recursiveDropBlock(int columnEnding_index,int rowEnding_index, BlockFall &game) {
    if (rowEnding_index + 1 == game.rows){
        int columnBegining_index = columnEnding_index - game.initial_block->shape[0].size() + 1;
        int rowBegining_index = rowEnding_index - game.initial_block->shape.size() + 1;
        for (int i = rowBegining_index; i <= rowEnding_index ; ++i) {
            for (int j = columnBegining_index; j <= columnEnding_index ; ++j) {
                if (!game.grid[i][j]){
                    game.grid[i][j] = game.initial_block->shape[i-rowBegining_index][j-columnBegining_index];
                }
            }
        }
        return;
    } else{
            int columnBegining_index =columnEnding_index - game.initial_block->shape[0].size() + 1;
            int rowBegining_index = rowEnding_index - game.initial_block->shape.size() + 1;
            bool isNextRowHaveOne = false;
        for (int i = rowBegining_index; i <= rowEnding_index ; ++i) {
            for (int k = columnBegining_index; k <= columnEnding_index ; ++k) {
                if (game.grid[i+1][k] && game.initial_block->shape[i-rowBegining_index][k-columnBegining_index]){
                    isNextRowHaveOne = true;
                }
            }
        }

        if (isNextRowHaveOne){
            int rowBegining_index = rowEnding_index - game.initial_block->shape.size()+1;
            for (int i = rowBegining_index; i <= rowEnding_index ; ++i) {
                for (int j = columnBegining_index; j <= columnEnding_index ; ++j) {
                    if (!game.grid[i][j]){
                        game.grid[i][j] = game.initial_block->shape[i-rowBegining_index][j-columnBegining_index];
                    }
                }
            }
            return;
        } else{
            int pointCount = 0;
            for (int i = 0; i < game.initial_block->shape.size(); ++i) {
                for (int j = 0; j < game.initial_block->shape[0].size(); ++j) {
                    if (game.initial_block->shape[i][j]){
                        pointCount++;
                    }
                }
            }
            game.current_score += pointCount;
            recursiveDropBlock(columnEnding_index,rowEnding_index+1,game);
        }
    }
}

bool GameController::powerUpDetector(BlockFall &game) {
    int gridRowEnd = game.power_up.size()-1;
    int gridColumnEnd = game.power_up[0].size()-1;
    for (int i = gridRowEnd; i < game.grid.size() ; ++i) {
        for (int j = gridColumnEnd; j <game.grid[0].size() ; ++j) {
            bool isPowerupValid = true;
            for (int k = 0; k < game.power_up.size(); ++k) {
                for (int l = 0; l < game.power_up[0].size(); ++l) {
                    if (!((!(game.power_up[k][l]) && game.grid[i+k-gridRowEnd][j+l-gridColumnEnd]) || (game.power_up[k][l] && !(game.grid[i+k-gridRowEnd][j+l-gridColumnEnd])))){
                        continue;
                    } else{
                        isPowerupValid = false;
                    }
                }
            }
            if (isPowerupValid){
                beforeCleaning(game, false);
                return true;
            }
        }
    }
    return false;
}

void GameController::rowCheck(BlockFall &game, bool gravity) {
    bool clearanceNeed = false;
    for (int i = 0; i < game.rows ; ++i) {
        bool areRowDirty = true;
        for (int j = 0; j < game.cols; ++j) {
            if (!game.grid[i][j]){
                areRowDirty = false;
            }
        }
        if (areRowDirty){
            clearanceNeed = true;
        }
    }
    if (clearanceNeed){
        beforeCleaning(game,gravity);
        for (int i = 0; i < game.rows ; ++i) {
            bool areRowDirty = true;
            for (int j = 0; j < game.cols; ++j) {
                if (!game.grid[i][j]){
                    areRowDirty = false;
                }
            }
            if (areRowDirty){
                game.current_score += game.grid[i].size();
                for (int j = 0; j < game.cols; ++j) {
                    game.grid[i][j] = false;
                }
                for (int j = i; j >=0 ; --j) {
                    for (int k = 0; k < game.cols; ++k) {
                        if (game.grid[j][k]){
                            game.grid[j+1][k] = true;
                            game.grid[j][k] = false;
                        }
                    }
                }
            }
        }
    }
}

void GameController::beforeCleaning(BlockFall &game ,bool gravity) {
    if (!gravity){
        cout<<"Before clearing:"<<endl;
        for (int i = 0; i < game.rows; ++i) {
            for (int j = 0; j < game.cols; ++j) {
                if (game.grid[i][j]){
                    cout<<occupiedCellChar;
                } else{
                    cout<<unoccupiedCellChar;
                }
            }
            cout<<endl;
        }
        cout<<endl<<endl;
    }
}





