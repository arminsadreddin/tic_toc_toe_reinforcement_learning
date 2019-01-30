#include <QCoreApplication>
#include <iostream>
#include <vector>
using namespace std;
char board[3][3] = {{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}};

struct node{
    int x;
    int y;
};

struct state_action{
    char state_board[3][3];
    double state_value[3][3];
};
vector<state_action> my_table;

void show_map();
bool static_player_act(char my_shape);

bool win_check_act(char my_shape);
bool win_row_check_act(char my_shape);
bool win_col_check_act(char my_shape);
bool win_hor_check_act(char my_shape);

bool lost_check_act(char my_shape);
bool lost_row_check_act(char my_shape);
bool lost_col_check_act(char my_shape);
bool lost_hor_check_act(char my_shape);

bool normal_move_act(char my_shape);

bool game_ends();
bool side_win(char shape);
bool draw_check();



bool qlearning_player_act(char my_shape);
int get_state_index();
void random_move(char my_shape);

int main()
{
    my_table.clear();
    show_map();
    int turn = 0;
    while(!game_ends()){
        if(turn % 2 == 0){
            static_player_act('X');
        }
        else{
            int col , row;
            cin >> row;
            cin >> col;
            board[row][col] = 'O';
        }
        show_map();
        turn++;


    }
    return 0;
}
void show_map(){
    cout << "-------"<<endl;
    for(int row = 0 ; row < 3 ; row++){
        cout << "|";
        for(int col = 0 ; col < 3 ; col++){
            cout << board[row][col]<< "|";
        }
        cout << endl;
        cout << "-------"<<endl;
    }
}
bool static_player_act(char my_shape){


    if(win_check_act(my_shape)){
        return true;
    }
    if(lost_check_act(my_shape)){
        return true;
    }
    if(normal_move_act(my_shape)){
        return true;
    }
    return false;

}
bool win_check_act(char my_shape){
    if(win_row_check_act(my_shape)){
        return true;
    }
    if(win_col_check_act(my_shape)){
        return true;
    }
    if(win_hor_check_act(my_shape)){
        return true;
    }
    return false;
}
bool win_row_check_act(char my_shape){
    for(int row = 0 ; row < 3 ; row++){
        int my_shapes_in_row = 0;
        bool can_win = true;
        int empty_col_index = -1;
        for(int col = 0 ; col < 3 ; col++){
            if(board[row][col] == my_shape){
                my_shapes_in_row++;
            }
            else if(board[row][col] != ' '){
                can_win = false;
            }
            else{
                empty_col_index = col;
            }
        }
        if(my_shapes_in_row != 2){
            can_win = false;
        }
        if(can_win){
            board[row][empty_col_index] = my_shape;
            return true;
        }
    }
    return false;
}
bool win_col_check_act(char my_shape){
    for(int col = 0 ; col < 3 ; col++){
        int my_shapes_in_col = 0;
        bool can_win = true;
        int empty_row_index = -1;
        for(int row = 0 ; row < 3 ; row++){
            if(board[row][col] == my_shape){
                my_shapes_in_col++;
            }
            else if(board[row][col] != ' '){
                can_win = false;
            }
            else{
                empty_row_index = row;
            }
        }
        if(my_shapes_in_col != 2){
            can_win = false;
        }
        if(can_win){
            board[empty_row_index][col] = my_shape;
            return true;
        }
    }
    return false;
}
bool win_hor_check_act(char my_shape){

    if(board[0][0] == ' ' && board[1][1] == my_shape && board[2][2] == my_shape){
        board[0][0] = my_shape;
        return true;
    }
    if(board[0][0] == my_shape && board[1][1] == ' ' && board[2][2] == my_shape){
        board[1][1] = my_shape;
        return true;
    }
    if(board[0][0] == my_shape && board[1][1] == my_shape && board[2][2] == ' '){
        board[2][2] = my_shape;
        return true;
    }

    if(board[0][2] == ' ' && board[1][1] == my_shape && board[2][0] == my_shape){
        board[0][2] = my_shape;
        return true;
    }
    if(board[0][2] == my_shape && board[1][1] == ' ' && board[2][0] == my_shape){
        board[1][1] = my_shape;
        return true;
    }
    if(board[0][2] == my_shape && board[1][1] == my_shape && board[2][0] == ' '){
        board[2][0] = my_shape;
        return true;
    }
    return false;
}
bool normal_move_act(char my_shape){
    if(board[1][1] == ' '){
        board[1][1] = my_shape;
        return true;
    }
    if(board[0][0] == ' '){
        board[0][0] = my_shape;
        return true;
    }
    if(board[0][2] == ' '){
        board[0][2] = my_shape;
        return true;
    }
    if(board[2][0] == ' '){
        board[2][0] = my_shape;
        return true;
    }
    if(board[2][2] == ' '){
        board[2][2] = my_shape;
        return true;
    }
    if(board[0][1] == ' '){
        board[0][1] = my_shape;
        return true;
    }
    if(board[1][0] == ' '){
        board[1][0] = my_shape;
        return true;
    }
    if(board[1][2] == ' '){
        board[1][2] = my_shape;
        return true;
    }
    if(board[2][1] == ' '){
        board[2][1] = my_shape;
        return true;
    }
    return false;
}
bool lost_check_act(char my_shape){
    if(lost_row_check_act(my_shape)){
        return true;
    }
    if(lost_col_check_act(my_shape)){
        return true;
    }
    if(lost_hor_check_act(my_shape)){
        return true;
    }
    return false;
}
bool lost_row_check_act(char my_shape){
    char opp_shape;
    if(my_shape == 'X'){
        opp_shape = 'O';
    }
    else{
        opp_shape = 'X';
    }
    for(int row = 0 ; row < 3 ; row++){
        int opp_in_row = 0;
        bool can_lose = true;
        int empty_col_index = -1;
        for(int col = 0 ; col < 3 ; col++){
            if(board[row][col] == opp_shape){
                opp_in_row++;
            }
            else if(board[row][col] != ' '){
                can_lose = false;
            }
            else{
                empty_col_index = col;
            }
        }
        if(opp_in_row != 2){
            can_lose = false;
        }
        if(can_lose){
            board[row][empty_col_index] = my_shape;
            return true;
        }
    }
    return false;
}
bool lost_col_check_act(char my_shape){
    char opp_shape;
    if(my_shape == 'X'){
        opp_shape = 'O';
    }
    else{
        opp_shape = 'X';
    }
    for(int col = 0 ; col < 3 ; col++){
        int opp_in_col = 0;
        bool can_lose = true;
        int empty_row_index = -1;
        for(int row = 0 ; row < 3 ; row++){
            if(board[row][col] == opp_shape){
                opp_in_col++;
            }
            else if(board[row][col] != ' '){
                can_lose = false;
            }
            else{
                empty_row_index = row;
            }
        }
        if(opp_in_col != 2){
            can_lose = false;
        }
        if(can_lose){
            board[empty_row_index][col] = my_shape;
            return true;
        }
    }
    return false;
}
bool lost_hor_check_act(char my_shape){
    char opp_shape;
    if(my_shape == 'X'){
        opp_shape = 'O';
    }
    else{
        opp_shape = 'X';
    }
    if(board[0][0] == ' ' && board[1][1] == opp_shape && board[2][2] == opp_shape){
        board[0][0] = my_shape;
        return true;
    }
    if(board[0][0] == opp_shape && board[1][1] == ' ' && board[2][2] == opp_shape){
        board[1][1] = my_shape;
        return true;
    }
    if(board[0][0] == opp_shape && board[1][1] == opp_shape && board[2][2] == ' '){
        board[2][2] = my_shape;
        return true;
    }

    if(board[0][2] == ' ' && board[1][1] == opp_shape && board[2][0] == opp_shape){
        board[0][2] = my_shape;
        return true;
    }
    if(board[0][2] == opp_shape && board[1][1] == ' ' && board[2][0] == opp_shape){
        board[1][1] = my_shape;
        return true;
    }
    if(board[0][2] == opp_shape && board[1][1] == opp_shape && board[2][0] == ' '){
        board[2][0] = my_shape;
        return true;
    }
    return false;
}

bool game_ends(){
    if(side_win('X')){
        cout << "X won !"<<endl;
        return true;
    }
    if(side_win('O')){
        cout << "O won !"<<endl;
        return true;
    }
    if(draw_check()){
        cout << "DRAW"<<endl;
        return true;
    }
    return false;
}
bool side_win(char shape){
    for(int row = 0 ; row < 3 ; row++){
        bool win = true;
        for(int col = 0 ; col < 3 ; col++){
            if(board[row][col] != shape){
                win = false;
                break;
            }
        }
        if(win){
            return true;
        }
    }

    for(int col = 0 ; col < 3 ; col++){
        bool win = true;
        for(int row = 0 ; row < 3 ; row++){
            if(board[row][col] != shape){
                win = false;
                break;
            }
        }
        if(win){
            return true;
        }
    }

    if(board[0][0] == shape && board[1][1] == shape && board[2][2] == shape){
        return true;
    }
    if(board[0][0] == shape && board[1][1] == shape && board[2][2] == shape){
        return true;
    }
    if(board[0][0] == shape && board[1][1] == shape && board[2][2] == shape){
        return true;
    }

    if(board[0][2] == shape && board[1][1] == shape && board[2][0] == shape){
        return true;
    }
    if(board[0][2] == shape && board[1][1] == shape && board[2][0] == shape){
        return true;
    }
    if(board[0][2] == shape && board[1][1] == shape && board[2][0] == shape){
        return true;
    }


    return false;





}
bool draw_check(){
    bool no_vacant_left = true;
    for(int row = 0 ; row < 3 ; row++){
        for(int col = 0 ; col < 3 ; col++){
            if(board[row][col] == ' '){
                no_vacant_left = false;
                break;
            }
        }
    }
    return no_vacant_left;
}
bool qlearning_player_act(char my_shape){

}
int get_state_index(){
    for(int n = 0 ; n < my_table.size() ; n++){
        bool diff = false;
        for(int i = 0 ; i < 3 ; i++){
            for(int j = 0 ; j < 3 ; j++){
                if(board[i][j] != my_table.at(n).state_board[i][j]){
                    diff = true;
                    break;
                }
            }
            if(diff){
                break;
            }
        }
        if(!diff){
            return n;
        }
    }
    return -1;
}
void random_move(char my_shape){
    vector <node> vacants;
    vacants.clear();
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            if(board[i][j] == ' '){
                node tmp;
                tmp.x = i;
                tmp.y = j;
                vacants.push_back(tmp);
            }
        }
    }
    int random_index = rand()%vacants.size();
    node chosen_node = vacants.at(random_index);
    board[chosen_node.x][chosen_node.y] = my_shape;

}












