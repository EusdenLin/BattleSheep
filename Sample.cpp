
#include "STcpClient.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <utility>
#define ITERS 10000
int total_n =0;
const double exploration_constant = 5.0;
using namespace std;

struct node{
	public:	
		int player;
		int board[12][12];
		int sheep[12][12];
		vector<int> child;
		int parent;
		vector<vector<int>> step; // <x,y,m,dir> 
		int n;
		double q;
		double w;
};

/*
    選擇起始位置
    選擇範圍僅限場地邊緣(至少一個方向為牆)
    
    return: init_pos
    init_pos=<x,y>,代表你要選擇的起始位置
    
*/

int selection(vector<struct node> &tree){
	bool leaf_node_found = false;
    int leaf_node_id = 0;
	while(!leaf_node_found){
		int node_id = leaf_node_id;
		int child_n = tree[node_id].child.size();
		if(child_n == 0){
			leaf_node_id = node_id;
			leaf_node_found = true;
		}
		else{
			float max_uct = -100.0;
			for(int i=0; i<tree[node_id].child.size(); i++){
				int child_id = tree[node_id].child[i];
				double w = tree[child_id].w;
				double n = tree[child_id].n;
				if(n == 0){
					n = 1e-4;
				} 
				double exploitation_value = w / n;
				double exploration_value = sqrt(log(total_n)/n);
				double uct_value = exploitation_value + exploration_constant * exploration_value;
				if(uct_value > max_uct){
					max_uct = uct_value;
					leaf_node_id = child_id;
				}
			}
		}
	}
	return leaf_node_id;
}


/*
y % 2 == 0
if(y >0 && x>0 && mapStat[x-1][y-1] == 0) //dir 1
if(y >0 && mapStat[x][y-1] == 0) // dir 2
if(x >0 && mapStat[x-1][y] == 0) // dir 3
if(x<11 && mapStat[x+1][y] == 0) // dir 4
if(x >0 && y<11 && mapStat[x-1][y+1] == 0) // dir 5
if(y<11 && mapStat[x][y+1] == 0) // dir 6

y % 2 == 1
if(y>0 && mapStat[x][y-1] == 0) // dir 1
if(x<11 && y>0 && mapStat[x+1][y-1] == 0) // dir 2
if(x >0 && mapStat[x-1][y] == 0) // dir 3
if(x<11 && mapStat[x+1][y] == 0) // dir 4
if(y<11 && mapStat[x][y+1] == 0) // dir 5
if(x<11 && y<11 && mapStat[x+1][y+1] == 0) //dir 6

ava_dir++;
vector<int> temp_step = {x, y, 0, 1};
for (int split = 1; split <  tree[sel_node].sheep[i][j]; split++){ // 1~k-1
	temp_step[2] = split;
	tree[sel_node].step.push_back(temp_step);
}

*/


int expansion(int sel_node, vector<struct node> &tree, int playerID){
	// find if there's any ava move and list all the steps
	int canMove = false; //<x,y,m,dir> 
	for(int y=0; y<12; y++){
		for(int x=0; x<12; x++){
			if(tree[sel_node].board[x][y] == playerID && tree[sel_node].sheep[x][y] > 1){ // canMove
				int ava_dir=0;
				if(y%2 == 0){
					if(y >0 && x>0 && tree[sel_node].board[x-1][y-1] == 0){
						vector<int> temp_step = {x, y, 0, 1};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;
					} // dir 1
					if(y >0 && tree[sel_node].board[x][y-1] == 0){
						vector<int> temp_step = {x, y, 0, 2};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;
					} // dir 2
					if(x >0 && tree[sel_node].board[x-1][y] == 0){
						vector<int> temp_step = {x, y, 0, 3};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;						
					} // dir 3
					if(x<11 && tree[sel_node].board[x+1][y] == 0){
						vector<int> temp_step = {x, y, 0, 4};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;
					} // dir 4
					if(x >0 && y<11 && tree[sel_node].board[x-1][y+1] == 0){
						vector<int> temp_step = {x, y, 0, 5};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;
					} // dir 5
					if(y<11 && tree[sel_node].board[x][y+1] == 0){
						vector<int> temp_step = {x, y, 0, 6};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;
					} // dir 6
				}
				else{
					if(y>0 && tree[sel_node].board[x][y-1] == 0){
						vector<int> temp_step = {x, y, 0, 1};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;						
					} // dir 1
					if(x<11 && y>0 && tree[sel_node].board[x+1][y-1] == 0){
						vector<int> temp_step = {x, y, 0, 2};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;
					} // dir 2
					if(x >0 && tree[sel_node].board[x-1][y] == 0){
						vector<int> temp_step = {x, y, 0, 3};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;
					} // dir 3
					if(x<11 && tree[sel_node].board[x+1][y] == 0){
						vector<int> temp_step = {x, y, 0, 4};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;
					} // dir 4
					if(y<11 && tree[sel_node].board[x][y+1] == 0){
						vector<int> temp_step = {x, y, 0, 5};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;
					} // dir 5
					if(x<11 && y<11 && tree[sel_node].board[x+1][y+1] == 0){
						vector<int> temp_step = {x, y, 0, 6};
						for (int split = 1; split <  tree[sel_node].sheep[x][y]; split++){ // 1~k-1
							temp_step[2] = split;
							tree[sel_node].step.push_back(temp_step);
						}
						ava_dir++;						
					} //dir 6
				}
				if(ava_dir > 0) canMove = true;
			}
		}
	} // tree[sel_node].step has the all the ava movements to next state
	// cout<<"canMove?: "<<canMove<<endl;
	
	if(canMove){
		for (int k = 0; k<tree[sel_node].step.size(); k++){
			struct node newNode;
			int x = tree[sel_node].step[k][0];
			int y = tree[sel_node].step[k][1];
			int m = tree[sel_node].step[k][2];
			int dir = tree[sel_node].step[k][3];
			int x_step = x;
			int y_step = y;

			// x_step, y_step: the space after move toward direction dir 
			while(1){ // terminate condition: border, occupied
				if(dir == 1){
					if(y_step %2 == 0){
						if(x_step > 0 && y_step > 0 && (tree[sel_node].board[x_step-1][y_step-1] == 0)){
							x_step--;
							y_step--;
						}
						else
							break;
					}
					else{
						if(y_step>0 && (tree[sel_node].board[x_step][y_step-1] == 0)){
							y_step--;
						}
						else
							break;
					}
				}
				else if(dir == 2){
					if(y_step %2 == 0){
						if(y_step >0 && tree[sel_node].board[x_step][y_step-1] == 0){
							y_step--;
						}
						else
							break;
					}
					else{
						if(x_step<11 && y_step>0 && tree[sel_node].board[x_step+1][y_step-1] == 0){
							x_step++;
							y_step--;
						}
						else
							break;
					}
				}
				else if(dir == 3){
					if(x_step >0 && tree[sel_node].board[x_step-1][y_step] == 0){
						x_step--;
					}
					else
						break;
				}
				else if(dir == 4){
					if(x_step <11 && tree[sel_node].board[x_step+1][y_step] == 0){
						x_step++;
					}
					else
						break;
				}
				else if(dir == 5){
					if(y_step %2 == 0){
						if(x_step >0 && y_step<11 && tree[sel_node].board[x_step-1][y_step+1] == 0){
							x_step--;
							y_step++;
						}
						else
							break;
					}
					else{
						if(y_step<11 && tree[sel_node].board[x_step][y_step+1] == 0){
							y_step++;
						}
						else
							break;
					}
				}
				else if(dir == 6){
					if(y_step %2 == 0){
						if(y_step<11 && tree[sel_node].board[x_step][y_step+1] == 0){
							y_step++;
						}
						else
							break;
					}
					else{
						if(x_step<11 && y_step<11 && tree[sel_node].board[x_step+1][y_step+1] == 0){
							x_step++;
							y_step++;
						}
						else
							break;
					}
				}
			}
			
			
			for (int i = 0; i<12; i++){
				for (int j = 0; j < 12; j++){
					if(i == x && j == y){
						newNode.board[i][j] = tree[sel_node].board[i][j]; // alreade have sheep flock before movement
						newNode.sheep[i][j] = tree[sel_node].sheep[i][j]-m;
					}
					else if(i == x_step && j == y_step){
						newNode.board[i][j] = playerID; // before movement it's a space 
						newNode.sheep[i][j] = m;
					}
					else{
						newNode.board[i][j] = tree[sel_node].board[i][j];
						newNode.sheep[i][j] = tree[sel_node].sheep[i][j];
					}
				}
			}
			newNode.player = (tree[sel_node].player)%4+1; //next player
			newNode.n = 0;
			newNode.w = 0;
			newNode.q = 0;
			newNode.parent = sel_node;
			tree[sel_node].child.push_back(tree.size()); // the last index as tree[sel_node].child
			tree.push_back(newNode);
		}
		//cout << "root child size: " << tree[sel_node].child.size() << endl;
		srand( time(NULL) );
		int rand_child = rand() % tree[sel_node].child.size(); // random child
		// cout<<"sel child num: "<<rand_child << endl; // nth child of this node
		// cout<<"next node num: "<<tree[sel_node].child[rand_child] << endl; // the actual index on the tree vector
		return tree[sel_node].child[rand_child];
	}
	else{
		return sel_node;
	}
}
/*
	int player;
		int board[12][12];
		int sheep[12][12];
		vector<int> child;
		vector<int> parent;
		vector<vector<int>> step; // <x,y,m,dir> 
		int n;
		int q;
		int w;
*/

int simulation(vector<struct node> &tree, int node_id, int player_ID){ // player's order 
	total_n++;
	int currentPlayer = tree[node_id].player;
	int gameBoard[12][12];
	int sheep[12][12];
	for (int y=0; y<12; y++){
		for (int x = 0; x<12; x++){
			gameBoard[x][y] = tree[node_id].board[x][y];
			sheep[x][y] = tree[node_id].sheep[x][y];
		}
	}
	bool canMove[4] = {true, true, true, true}; //<x,y,m,dir>
	bool terminate = false;						//<x,y,m,dir>
	int winner = 0;
	while(!terminate){
		bool move = false;
		if(canMove[currentPlayer-1] == false){
			currentPlayer = (currentPlayer%4) +1;
			//cout << "skip" << endl;
			continue;
		}
		vector<vector<int>> step;
		for(int y=0; y<12; y++){ // get valid actions
			for(int x=0; x<12; x++){
				if(gameBoard[x][y] == currentPlayer && sheep[x][y] > 1){ // canMove
					int ava_dir=0;
					if(y%2 == 0){
						if(y >0 && x>0 && gameBoard[x-1][y-1] == 0){
							vector<int> temp_step = {x, y, 0, 1};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;
						} // dir 1
						if(y >0 && gameBoard[x][y-1] == 0){
							vector<int> temp_step = {x, y, 0, 2};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);	
							}
							ava_dir++;
						} // dir 2
						if(x >0 && gameBoard[x-1][y] == 0){
							vector<int> temp_step = {x, y, 0, 3};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;						
						} // dir 3
						if(x<11 && gameBoard[x+1][y] == 0){
							vector<int> temp_step = {x, y, 0, 4};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;
						} // dir 4
						if(x >0 && y<11 && gameBoard[x-1][y+1] == 0){
							vector<int> temp_step = {x, y, 0, 5};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;
						} // dir 5
						if(y<11 && gameBoard[x][y+1] == 0){
							vector<int> temp_step = {x, y, 0, 6};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;
						} // dir 6
					}
					else{
						if(y>0 && gameBoard[x][y-1] == 0){
							vector<int> temp_step = {x, y, 0, 1};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;						
						} // dir 1
						if(x<11 && y>0 && gameBoard[x+1][y-1] == 0){
							vector<int> temp_step = {x, y, 0, 2};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;
						} // dir 2
						if(x >0 && gameBoard[x-1][y] == 0){
							vector<int> temp_step = {x, y, 0, 3};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;
						} // dir 3
						if(x<11 && gameBoard[x+1][y] == 0){
							vector<int> temp_step = {x, y, 0, 4};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;
						} // dir 4
						if(y<11 && gameBoard[x][y+1] == 0){
							vector<int> temp_step = {x, y, 0, 5};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;
						} // dir 5
						if(x<11 && y<11 && gameBoard[x+1][y+1] == 0){
							vector<int> temp_step = {x, y, 0, 6};
							for (int split = 1; split <  sheep[x][y]; split++){ // 1~k-1
								temp_step[2] = split;
								step.push_back(temp_step);
							}
							ava_dir++;						
						} //dir 6
					}
					if(ava_dir > 0) move = true;
				}
			}
		}
		if(move == false){
			//cout<<"currentPlayer: "<<currentPlayer<<", number of valid actions: 0"<<endl;
			//cout << canMove[0] << " " << canMove[1] << " " << canMove[2] << " " << canMove[3] << endl;
			canMove[currentPlayer - 1] = false;
			currentPlayer = (currentPlayer % 4) + 1;
		}
		else{
			//cout<<"currentPlayer: "<<currentPlayer<<", number of valid actions: " << step.size()<<endl;
			srand( time(NULL) );
			int rand_action = rand() % step.size();
			// do step[rand_action]
			int x = step[rand_action][0];
			int y = step[rand_action][1];
			int m = step[rand_action][2];
			int dir = step[rand_action][3];
			int x_step = x;
			int y_step = y;
			while(1){ // terminate condition: border, occupied
				if(dir == 1){
					if(y_step %2 == 0){
						if(x_step > 0 && y_step > 0 && (gameBoard[x_step-1][y_step-1] == 0)){
							x_step--;
							y_step--;
						}
						else
							break;
					}
					else{
						if(y_step>0 && (gameBoard[x_step][y_step-1] == 0)){
							y_step--;
						}
						else
							break;
					}
				}
				else if(dir == 2){
					if(y_step %2 == 0){
						if(y_step >0 && gameBoard[x_step][y_step-1] == 0){
							y_step--;
						}
						else
							break;
					}
					else{
						if(x_step<11 && y_step>0 && gameBoard[x_step+1][y_step-1] == 0){
							x_step++;
							y_step--;
						}
						else
							break;
					}
				}
				else if(dir == 3){
					if(x_step >0 && gameBoard[x_step-1][y_step] == 0){
						x_step--;
					}
					else
						break;
				}
				else if(dir == 4){
					if(x_step <11 && gameBoard[x_step+1][y_step] == 0){
						x_step++;
					}
					else
						break;
				}
				else if(dir == 5){
					if(y_step %2 == 0){
						if(x_step >0 && y_step<11 && gameBoard[x_step-1][y_step+1] == 0){
							x_step--;
							y_step++;
						}
						else
							break;
					}
					else{
						if(y_step<11 && gameBoard[x_step][y_step+1] == 0){
							y_step++;
						}
						else
							break;
					}
				}
				else if(dir == 6){
					if(y_step %2 == 0){
						if(y_step<11 && gameBoard[x_step][y_step+1] == 0){
							y_step++;
						}
						else
							break;
					}
					else{
						if(x_step<11 && y_step<11 && gameBoard[x_step+1][y_step+1] == 0){
							x_step++;
							y_step++;
						}
						else
							break;
					}
				}
			}
			// x, y to x_step, y_step with m split
			gameBoard[x_step][y_step] = currentPlayer;
			sheep[x][y] = sheep[x][y] - m;
			sheep[x_step][y_step] = m;
			currentPlayer = (currentPlayer % 4) + 1;
		}
		if (canMove[0] == false && canMove[1] == false && canMove[2] == false && canMove[3] == false){
			terminate = true;
			//cout<<"terminate"<<endl;
		}
	}
	// end game, calculate points
	vector<int> order;
	order.resize(4);
	int territory[4] = {0, 0, 0, 0};
	int max_connect[4] = {0, 0, 0, 0};
	for(int y = 0; y <12; y++){ // find territory area size
		for (int x = 0; x<12; x++){
			if(gameBoard[x][y] == 0 || gameBoard[x][y] == -1){
				continue;
			}
			else{
				territory[gameBoard[x][y] - 1]++;
			}
		}
	}

	// for (int y = 0; y < 12; y++){
	// 	if(y%2 == 1)
	// 		cout << "  ";
	// 	for(int x = 0; x < 12; x++){
	// 		if(gameBoard[x][y] == -1)
	// 			cout << "-1  ";
	// 		else
	// 			cout << " " << gameBoard[x][y] << "  ";
	// 	}
	// 	cout << endl;
	// }

	for (int y = 0; y < 12; y++)
		{ // find territory area size
			for (int x = 0; x < 12; x++)
			{
				if (gameBoard[x][y] == 0 || gameBoard[x][y] == -1)
				{
					continue;
				}
				else{
				int playerID = gameBoard[x][y];
				vector<pair<int, int>> connected;
				connected.push_back(pair<int, int>(x, y));
				gameBoard[x][y] = 0;
				for(int i=0; i<connected.size(); i++){
					if(connected[i].second % 2 == 0){
						if(connected[i].second >0 && connected[i].first>0 && gameBoard[connected[i].first-1][connected[i].second-1] == playerID) {
							gameBoard[connected[i].first - 1][connected[i].second - 1] = 0;
							connected.push_back(pair<int, int>(connected[i].first - 1, connected[i].second - 1));
						}
						if(connected[i].second >0 && gameBoard[connected[i].first][connected[i].second-1] == playerID) {
							gameBoard[connected[i].first][connected[i].second-1] = 0;
							connected.push_back(pair<int, int>(connected[i].first, connected[i].second - 1));
						}
						if(connected[i].first >0 && gameBoard[connected[i].first-1][connected[i].second] == playerID) {
							gameBoard[connected[i].first-1][connected[i].second] = 0;
							connected.push_back(pair<int, int>(connected[i].first-1, connected[i].second));
						}
						if(connected[i].first<11 && gameBoard[connected[i].first+1][connected[i].second] == playerID) {
							gameBoard[connected[i].first+1][connected[i].second] = 0;
							connected.push_back(pair<int, int>(connected[i].first+1, connected[i].second));
						}
						if(connected[i].first >0 && connected[i].second<11 && gameBoard[connected[i].first-1][connected[i].second+1] == playerID) {
							gameBoard[connected[i].first-1][connected[i].second+1] = 0;
							connected.push_back(pair<int, int>(connected[i].first-1, connected[i].second + 1));
						}
						if(connected[i].second<11 && gameBoard[connected[i].first][connected[i].second+1] == playerID) {
							gameBoard[connected[i].first][connected[i].second+1] = 0;
							connected.push_back(pair<int, int>(connected[i].first, connected[i].second + 1));
						}
					}
					else{
						if(connected[i].second>0 && gameBoard[connected[i].first][connected[i].second-1] == playerID) {
							gameBoard[connected[i].first][connected[i].second-1] = 0;
							connected.push_back(pair<int, int>(connected[i].first, connected[i].second - 1));
						}
						if(connected[i].first<11 && connected[i].second>0 && gameBoard[connected[i].first+1][connected[i].second-1] == playerID) {
							gameBoard[connected[i].first+1][connected[i].second-1] = 0;
							connected.push_back(pair<int, int>(connected[i].first+1, connected[i].second - 1));
						}
						if(connected[i].first >0 && gameBoard[connected[i].first-1][connected[i].second] == playerID) {
							gameBoard[connected[i].first-1][connected[i].second] = 0;
							connected.push_back(pair<int, int>(connected[i].first-1, connected[i].second));
						}
						if(connected[i].first<11 && gameBoard[connected[i].first+1][connected[i].second] == playerID) {
							gameBoard[connected[i].first+1][connected[i].second] = 0;
							connected.push_back(pair<int, int>(connected[i].first+1, connected[i].second));
						}
						if(connected[i].second<11 && gameBoard[connected[i].first][connected[i].second+1] == playerID) {
							gameBoard[connected[i].first][connected[i].second+1] = 0;
							connected.push_back(pair<int, int>(connected[i].first, connected[i].second + 1));
						}
						if(connected[i].first<11 && connected[i].second<11 && gameBoard[connected[i].first+1][connected[i].second+1] == playerID) {
							gameBoard[connected[i].first+1][connected[i].second+1] = 0;
							connected.push_back(pair<int, int>(connected[i].first+1, connected[i].second + 1));
						}
					}
				}
				if(max_connect[playerID-1] < connected.size())
					max_connect[playerID - 1] = connected.size();
			}
		}
	}
	int result = territory[player_ID-1] * 3 + max_connect[player_ID-1];
	int cnt = 0;
	for (int i = 0; i<4; i++){
		if(result > territory[i] * 3 + max_connect[i])
			cnt++;
	}// 3> 1, 2> 2, 1> 3, 0> 4
	return 4 - cnt;
}

void backprop(vector<struct node> &tree, int order, int playerID, int leaf_node_id){ // update the tree
	double reward;
	if(order == 1)
		reward = 0.7;
	else if(order == 2)
		reward = 0.2;
	else if(order == 3)
		reward = 0.1;
	else
		reward = 0;

	bool finish_bp = false;
	int node_id = leaf_node_id;
	while(!finish_bp){
		tree[node_id].n += 1;
		tree[node_id].w += reward;
		tree[node_id].q = tree[node_id].w / tree[node_id].n;
		int parent_id = tree[node_id].parent;
		if(parent_id == 0)
			finish_bp = true;
		else
			node_id = parent_id;
	}
	return;
}

std::vector<int> InitPos(int mapStat[12][12])
{
	std::vector<int> init_pos;
	init_pos.resize(2);
	int all_ava_dir[7] = {0, 0, 0, 0, 0, 0, 0};
	int zero_cnt=0;
	for(int y=0; y<12; y++){
		if(y %2 == 1)
			//cout << "  ";
		for(int x=0; x<12; x++){
			int ava_dir=0;
			if(mapStat[x][y] != 0){
				//cout<<mapStat[x][y]<<"  ";
				continue; // next field
			}
			else{
				zero_cnt++;
				if(y%2 == 0){
					if(y >0 && x>0 && mapStat[x-1][y-1] == 0){
						ava_dir++;
					} // dir 1
					if(y >0 && mapStat[x][y-1] == 0){
						ava_dir++;
					} // dir 2
					if(x >0 && mapStat[x-1][y] == 0){
						ava_dir++;
					} // dir 3
					if(x<11 && mapStat[x+1][y] == 0){
						ava_dir++;
					} // dir 4
					if(x>0 && y<11 && mapStat[x-1][y+1] == 0){
						ava_dir++;
					}
					if(y<11 && mapStat[x][y+1] == 0){
						ava_dir++;
					}
				}
				else{
					if(y>0 && mapStat[x][y-1] == 0){
						ava_dir++;
					} // dir 1
					if(x<11 && y>0 && mapStat[x+1][y-1] == 0){
						ava_dir++;
					} // dir 2
					if(x >0 && mapStat[x-1][y] == 0){
						ava_dir++;
					} // dir 3
					if(x<11 && mapStat[x+1][y] == 0){
						ava_dir++;
					} // dir 4
					if(y<11 && mapStat[x][y+1] == 0){
						ava_dir++;
					}
					if(x<11 && y<11 && mapStat[x+1][y+1] == 0){
						ava_dir++;
					}
				}
			}
			if(ava_dir == 5){
				init_pos[0] = x;
				init_pos[1] = y;
				goto leave;
			}
		}
		//cout << endl;
	}
	leave:
		cout << init_pos[0] << " " << init_pos[1] << endl;
	return init_pos;
}

/*
	產出指令
    
    input: 
	playerID: 你在此局遊戲中的角色(1~4)
    mapStat : 棋盤狀態, 為 12*12矩陣, 
					0=可移動區域, -1=障礙, 1~4為玩家1~4佔領區域
    sheepStat : 羊群分布狀態, 範圍在0~16, 為 12*12矩陣

    return Step
    Step : <x,y,m,dir> 
            x, y 表示要進行動作的座標 
            m = 要切割成第二群的羊群數量
            dir = 移動方向(1~6),對應方向如下圖所示
              1  2
            3  x  4
              5  6
*/
std::vector<int> GetStep(int playerID,int mapStat[12][12], int sheepStat[12][12])
{
	vector<struct node> tree;
	std::vector<int> step;
	step.resize(4);
	// init_tree
	total_n = 0;
	struct node rootNode;
	for(int i=0; i<12; i++){
		for(int j =0; j<12; j++){
			rootNode.board[i][j] = mapStat[i][j];
			rootNode.sheep[i][j] = sheepStat[i][j];
		}
	}
	rootNode.player = (playerID+3)%5+1; // previous player
	tree.push_back(rootNode);

	// no child, ava step, n, w, q, training MCTS 
	clock_t start, finish;
	start = clock();
	for(int i=0; i<ITERS; i++){
		int sel_node = selection(tree);
		//cout <<"sel_node :"<< sel_node << endl;
		int expand_child = expansion(sel_node, tree, playerID);
		//cout <<"expand :"<< expand_child << endl;
		int order =  simulation(tree, expand_child, playerID);
		//cout <<"order :"<< order << endl;
		backprop(tree, order, playerID, expand_child);
	}
	finish = clock();
	cout<<"time elapsed :"<< finish - start << endl;

	double maxq = -100.0;
	int best_action_id = 0;
	for(int i=0; i<tree[0].child.size(); i++){
		if(tree[tree[0].child[i]].q > maxq){
			maxq = tree[tree[0].child[i]].q;
			best_action_id = i;
		}
	}
	/*
		Write your code here
	*/
    
    return tree[0].step[best_action_id];
}

int main(){
	int id_package;
	int playerID;
    int mapStat[12][12];
    int sheepStat[12][12];

	// player initial
	GetMap(id_package,playerID,mapStat);
	
	cout << endl;
	std::vector<int> init_pos = InitPos(mapStat);
	SendInitPos(id_package,init_pos);

	while (true)
	{
		if (GetBoard(id_package, mapStat, sheepStat))
			break;

		std::vector<int> step = GetStep(playerID,mapStat,sheepStat);
		SendStep(id_package, step);
	}
}

/*
-1  -1  -1  -1   1   1   1  -1  -1  -1  -1  -1
  -1  -1  -1   1   1   1   1  -1  -1  -1  -1  -1  
-1  -1  -1  -1  -1   0  -1  -1  -1   3   3  -1
   0  -1  -1  -1   0   0   2   2   2   3  -1  -1
 0   0   0   0   0   0  -1   2   2   3  -1  -1
   0   0   0   0   0   0  -1   2  -1  -1  -1  -1
 0   0   0   0   0  -1   4   4   2  -1  -1  -1  
   0  -1   0   0  -1   0   0   4  -1  -1  -1  -1
-1   0   0   0   0  -1   0  -1   4   4  -1  -1
   1   1   0   0  -1  -1  -1   4   4  -1  -1  -1  
 1   1   1  -1  -1  -1  -1   4  -1  -1  -1  -1
   1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1  -1
order :1
*/

