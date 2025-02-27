#include "Leaderboard.h"
#include "fstream"
#include "vector"
#include "sstream"
#include "iostream"

struct Leaderboard::Data{
    time_t timestamp{};
    unsigned long score{};
    string name;
};

void Leaderboard::insert_new_entry(LeaderboardEntry * new_entry) {
    // TODO: Insert a new LeaderboardEntry instance into the leaderboard, such that the order of the high-scores
    //       is maintained, and the leaderboard size does not exceed 10 entries at any given time (only the
    //       top 10 all-time high-scores should be kept in descending order by the score).
    if (head_leaderboard_entry != nullptr){
        if (head_leaderboard_entry->next_leaderboard_entry == nullptr){
            if (head_leaderboard_entry->score < new_entry->score){
                new_entry->next_leaderboard_entry = head_leaderboard_entry;
                head_leaderboard_entry = new_entry;
            } else{
                head_leaderboard_entry->next_leaderboard_entry = new_entry;
            }
        } else{
            if (head_leaderboard_entry->score < new_entry->score){
                new_entry->next_leaderboard_entry = head_leaderboard_entry;
                head_leaderboard_entry = new_entry;
            } else{
                LeaderboardEntry* current = head_leaderboard_entry;
                while (current->next_leaderboard_entry != nullptr && new_entry->score <= current->next_leaderboard_entry->score){
                    current = current->next_leaderboard_entry;
                }
                new_entry->next_leaderboard_entry = current->next_leaderboard_entry;
                current->next_leaderboard_entry = new_entry;
            }
        }
        size_t leaderboard_size = leaderBoardSize(head_leaderboard_entry);

        while(leaderboard_size > 10){
            LeaderboardEntry* temp = head_leaderboard_entry;
            LeaderboardEntry* prev = nullptr;
            while (temp->next_leaderboard_entry != nullptr){
                prev = temp;
                temp = temp->next_leaderboard_entry;
            }
            prev->next_leaderboard_entry = nullptr;
            delete temp;
            leaderboard_size--;
        }
    } else{
        head_leaderboard_entry = new_entry;
    }
}

void Leaderboard::write_to_file(const string& filename) {
    // TODO: Write the latest leaderboard status to the given file in the format specified in the PA instructions
    ofstream file(filename);
    if (file.is_open()){
        LeaderboardEntry* temp = head_leaderboard_entry;
        while (temp != nullptr){
            file<< temp->score<<" "<<temp->last_played<<" "<<temp->player_name<<endl;
            temp = temp->next_leaderboard_entry;
        }
        delete temp;
        file.close();
    }
}

void Leaderboard::read_from_file(const string& filename) {
    // TODO: Read the stored leaderboard status from the given file such that the "head_leaderboard_entry" member
    //       variable will point to the highest all-times score, and all other scores will be reachable from it
    //       via the "next_leaderboard_entry" member variable pointer.
    ifstream file(filename);
    if (file.is_open()){
        vector<Data> dataList;
        string line;
        while (getline(file,line)){
            istringstream ss(line);
            Data data;
            ss>> data.score >> data.timestamp >> data.name;
            dataList.push_back(data);
        }
        file.close();
        bubleSort(dataList);
        int n = 1;
        while (n<=10 && dataList.size() >= n){
            LeaderboardEntry* newEntry = new LeaderboardEntry(dataList[n-1].score,dataList[n-1].timestamp,dataList[n-1].name);
            insert_new_entry(newEntry);
            n++;
        }
    }
}


void Leaderboard::print_leaderboard() {
    // TODO: Print the current leaderboard status to the standard output in the format specified in the PA instructions
    int order = 1;
    cout<<"Leaderboard"<<endl<<"-----------";
    LeaderboardEntry* temp = head_leaderboard_entry;
    while (temp != nullptr){
        cout<<endl;
        tm* ltm = localtime(&temp->last_played);
        string hour,min,sec,day,mnth;
        if (ltm->tm_hour <= 9){
            hour = '0'+ to_string(ltm->tm_hour);
        } else{
            hour = to_string(ltm->tm_hour);
        }
        if (ltm->tm_min <= 9){
            min = '0'+ to_string(ltm->tm_min);
        } else{
            min = to_string(ltm->tm_min);
        }
        if (ltm->tm_sec <= 9){
            sec = '0'+ to_string(ltm->tm_sec);
        } else{
            sec = to_string(ltm->tm_sec);
        }
        if (ltm->tm_mday <= 9){
            day = '0'+ to_string(ltm->tm_mday);
        } else{
            day = to_string(ltm->tm_mday);
        }
        if (ltm->tm_mon + 1 <= 9){
            mnth = '0'+ to_string(ltm->tm_mon + 1);
        } else{
            mnth = to_string(ltm->tm_mon + 1);
        }
        cout<<order<<". "<<temp->player_name<<" "<<temp->score<<" "<<hour<<":"<<min<<":"<<sec<<"/"<<day<<"."<<mnth<<"."<<1900+ltm->tm_year;
        temp = temp->next_leaderboard_entry;
        order++;
    }
}

void Leaderboard::bubleSort(vector<Leaderboard::Data> &data) {
    size_t n = data.size();
    for (int i = 0; i < n-1; ++i) {
        for (int j = 0; j < n-i-1; ++j) {
            if (data[j].score < data[j + 1].score){
                swap(data[j],data[j+1]);
            }
        }
    }
}

int Leaderboard::leaderBoardSize(const LeaderboardEntry *leaderboardEntry) {
    int count = 0;
    const LeaderboardEntry* current = leaderboardEntry;
    while (current){
        count++;
        current = current->next_leaderboard_entry;
    }
    return count;
}

void Leaderboard::clear(LeaderboardEntry * leaderboardEntry) {
    if (!leaderboardEntry){
        return;
    }
    clear(leaderboardEntry->next_leaderboard_entry);
    delete leaderboardEntry;
}


Leaderboard::~Leaderboard() {
    // TODO: Free dynamically allocated memory used for storing leaderboard entries
    clear(head_leaderboard_entry);
}
