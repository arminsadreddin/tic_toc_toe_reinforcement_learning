#include <QCoreApplication>
#include <iostream>
#include <vector>
using namespace std;
char board[3][3] = {{' ',' ',' '},{' ',' ',' '},{' ',' ',' '}};

char learner_shape = 'O';
char static_player_shape = 'X';

struct node{
    int x;
    int y;
};

int q_agent_wins = 0;

double alpha = 0.5;
double gamma = 0.9;

struct state_action{
    char state_board[3][3];
    double state_value[3][3];
};
vector<state_action> my_table;

void show_map();
void init_board();
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
node random_move(char my_shape);
node best_move(char my_shape);
double get_state_reward(char my_shape);
double get_state_value(char cur_board[3][3]);
bool opp_can_win(char my_shape);
int get_board_index(char given_board[3][3]);

void print_my_table();


int main()
{
    srand(time(NULL));
    my_table.clear();
    show_map();

    for(int i = 0 ; i < 10000 ; i++){
        int turn = 0;
        if(i >= 9990){
            alpha = 1.0;
        }
        init_board();

        while(!game_ends()){
            if(turn % 2 == 0){
                qlearning_player_act(learner_shape);

            }
            else{
                static_player_act(static_player_shape);
                //            int col , row;
                //            cin >> row;
                //            cin >> col;
                //            board[row][col] = 'O';
            }
            //test
            if(i >= 9990){
                //print_my_table();
                int w;
                cin >> w;
            }
            //test


            show_map();
            turn++;
        }
        //        cout << "loop end"<<endl;
    }
    cout << "wins : "<<q_agent_wins<<endl;
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
        q_agent_wins++;
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
    int index = get_state_index();
    if(index == -1){
        state_action cur_state;
        for(int i = 0 ; i < 3 ; i++){
            for(int j = 0 ; j < 3 ; j++){
                cur_state.state_board[i][j] = board[i][j];
                cur_state.state_value[i][j] = -100.0;
            }
        }
        my_table.push_back(cur_state);
    }

    double decision = rand()%100;
    decision = decision/100.0;
    node agent_action;
    if(decision <= alpha){
        agent_action = best_move(my_shape);
    }
    else{
        agent_action = random_move(my_shape);
    }

    double reward = get_state_reward(my_shape);
    if(index == -1){
        index = my_table.size() - 1;
    }

    double state_value = -1000;

    char cur_board[3][3];
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            cur_board[i][j] = board[i][j];
        }
    }


    state_value = get_state_value(cur_board);
    //    }
    cout << "reward : "<<reward<<endl;
    cout << "state value :"<<state_value<<endl;

    my_table.at(index).state_value[agent_action.x][agent_action.y] = reward +  (gamma * (state_value));

    for(int i = 0 ; i < 3 ; i ++){
        for(int j = 0 ; j < 3 ; j++){
            cout << my_table.at(index).state_value[i][j] << "  ";
        }
        cout << endl;
    }


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
node random_move(char my_shape){
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
    return chosen_node;
}
node best_move(char my_shape){
    int index = get_state_index();
    //cout << "best move index : " <<index <<endl;
    //cout << "best move table size : " << my_table.size() << endl;
    double max_value = -110;
    vector<node> candidates;
    candidates.clear();
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            if(my_table.at(index).state_value[i][j] > max_value && my_table.at(index).state_board[i][j] == ' '){
                max_value = my_table.at(index).state_value[i][j];
            }
        }
    }

    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            if(my_table.at(index).state_value[i][j] == max_value && my_table.at(index).state_board[i][j] == ' '){
                node tmp;
                tmp.x = i;
                tmp.y = j;
                candidates.push_back(tmp);
            }
        }
    }

    int best_index = rand()%candidates.size();
    node chosen = candidates.at(best_index);
    board[chosen.x][chosen.y] = my_shape;
    return chosen;
}
double get_state_reward(char my_shape){
    char opp_shape = 'O';
    if(my_shape == 'O'){
        opp_shape = 'X';
    }
    if(side_win(my_shape)){
        return 5.0;
    }
    if(opp_can_win(my_shape)){
        return -5.0;
    }
    if(draw_check()){
        return 0.0;
    }
    return 0.0;
}
void init_board(){
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            board[i][j] = ' ';
        }
    }
}
double get_state_value(char cur_board[3][3]){
    double state_value = 0;
    //    char main_board[3][3];
    vector<node> vacants;
    vacants.clear();
    for(int i = 0 ; i < 3 ; i++){
        for(int j = 0 ; j < 3 ; j++){
            if(cur_board[i][j] == ' '){
                node tmp;
                tmp.x = i;
                tmp.y = j;
                vacants.push_back(tmp);
            }
        }
    }

    cout << "v size : "<<vacants.size()<<endl;
    for(int v = 0 ; v < vacants.size() ; v++){

        //double values[3][3];
        node tmp_node = vacants.at(v);

        cur_board[tmp_node.x][tmp_node.y] = static_player_shape;
        int board_index = get_board_index(cur_board);
        cur_board[tmp_node.x][tmp_node.y] = ' ';



        cout << "board index : " << board_index << endl;
        if(board_index == -1){
            //state_value = max(0.0,state_value);
            continue;
        }


        for(int i = 0 ; i < 3 ; i++){
            for(int j = 0 ; j < 3 ; j++){
                if(cur_board[i][j] == ' '){
                    state_value = max(my_table.at(board_index).state_value[i][j],state_value);
                }
            }
        }


    }


    return state_value;
}
bool opp_can_win(char my_shape){
    char opp_shape = 'X';
    if(my_shape == 'X'){
        opp_shape = 'O';
    }


    for(int row = 0 ; row < 3 ; row++){
        int my_shapes_in_row = 0;
        bool can_win = true;
        int empty_col_index = -1;
        for(int col = 0 ; col < 3 ; col++){
            if(board[row][col] == opp_shape){
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
            //board[row][empty_col_index] = my_shape;
            return true;
        }
    }



    for(int col = 0 ; col < 3 ; col++){
        int my_shapes_in_col = 0;
        bool can_win = true;
        int empty_row_index = -1;
        for(int row = 0 ; row < 3 ; row++){
            if(board[row][col] == opp_shape){
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
            //board[empty_row_index][col] = my_shape;
            return true;
        }
    }


    if(board[0][0] == ' ' && board[1][1] == opp_shape && board[2][2] == opp_shape){
        //board[0][0] = my_shape;
        return true;
    }
    if(board[0][0] == opp_shape && board[1][1] == ' ' && board[2][2] == opp_shape){
        //board[1][1] = my_shape;
        return true;
    }
    if(board[0][0] == opp_shape && board[1][1] == opp_shape && board[2][2] == ' '){
        //board[2][2] = my_shape;
        return true;
    }

    if(board[0][2] == ' ' && board[1][1] == opp_shape && board[2][0] == opp_shape){
        //board[0][2] = my_shape;
        return true;
    }
    if(board[0][2] == opp_shape && board[1][1] == ' ' && board[2][0] == opp_shape){
        //board[1][1] = my_shape;
        return true;
    }
    if(board[0][2] == opp_shape && board[1][1] == opp_shape && board[2][0] == ' '){
        //board[2][0] = my_shape;
        return true;
    }
    return false;





}
int get_board_index(char given_board[3][3]){

//    cout << "given board"<<endl;
//    for(int i = 0 ; i < 3 ; i++){
//        for(int j = 0 ; j < 3 ; j++){
//            cout << given_board[i][j];
//        }
//        cout << endl;
//    }

    //cout << "table size :"<<my_table.size() << endl;

    for(int n = 0 ; n < my_table.size() ; n++){
        bool diff = false;
        for(int i = 0 ; i < 3 ; i++){
            for(int j = 0 ; j < 3 ; j++){
                if(given_board[i][j] != my_table.at(n).state_board[i][j]){
                    //cout << given_board[i][j] << " - " << my_table.at(n).state_board[i][j];
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



void print_my_table(){
    for(int n = 0 ; n < my_table.size() ; n++){
        for(int i = 0 ; i < 3 ; i++){
            for(int j = 0 ; j < 3 ; j++){
                cout << my_table.at(n).state_board[i][j];
            }
            cout << endl;
        }
        cout << "------------------------------"<<endl;
    }

}















