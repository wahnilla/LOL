#include <iostream>
#include <windows.h>
using namespace std;

class Data
{
public:
	int player;		// current player ; min = -1 (User) , max = 1 (AI)
    int board;		// number of remaining spaces
    int utility;
	int depth;

	Data(int p,int b,int d){
        player = p;
        board = b;
		depth = d;
		utility = 0;
    }
};

class Node
{
public:
	Node *left;
	Node *mid;
	Node *right;
	Node *parent;
	Data *data;

	Node(Data *d)
	{
		left = NULL;
		mid  = NULL;
		right = NULL;
		parent = NULL;

		data = new Data(d->player,d->board,d->depth);
	}
};

class Tree
{
public:
	Node *root;
	Node **pointer;

	int size;

	//LARGE_INTEGER frequency;        // ticks per second
    //LARGE_INTEGER t1, t2;           // ticks
    //double elapsedTime;

	bool stop;
    
    Tree(){
        root = NULL;
		pointer = &root;
		size = 15;
		//QueryPerformanceFrequency(&frequency);
		//stop = false;
    }
    
	/*float getRunTime(){
		elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;	
		return elapsedTime;
	}
	*/

	void startGame(){
		int *board = new int[size];
		// initial board
		cout << endl;
		for(int i=0;i<size;i++){
			board[i] = 0;
			cout << " _";
		}
		cout << endl;

		bool endGame = false;
		int count;
		
		int winner;

		// start timer
		//QueryPerformanceCounter(&t1);
		
		genTree();
		addUtility(*pointer);


		int posB = -1;

		// playing
		while(!endGame){
			// MAX's turn
			if(((*pointer)->data->player == 1)){
				cout << endl << "AI's turn" << endl;
				count = maxMove(pointer);

				// STOP THE TIMER
				/*if(!stop){
					QueryPerformanceCounter(&t2);
					stop = true;
				}*/

				if(count == 1){
						board[++posB] = -1;
				}
				else if(count == 2){
						board[++posB] = -1;
						board[++posB] = -1;
				}
				else if(count == 3){
						board[++posB] = -1;
						board[++posB] = -1;
						board[++posB] = -1;  
				}
				
			}

			else{
				// MIN's turn
				bool err = true;
				while(err){
					cout << endl << "Your turn" << endl;
					cout << "Enter X (1-3) : ";
					cin >> count;
					if(count == 1){
						board[++posB] = 1;
						pointer = &(*pointer)->left;
						err = false;
					}
					else if(count == 2){
						board[++posB] = 1;
						board[++posB] = 1;
						pointer = &(*pointer)->mid;
						err = false;
					}
					else if(count == 3){
						board[++posB] = 1;
						board[++posB] = 1;
						board[++posB] = 1;
						pointer = &(*pointer)->right;
						err = false;
					}
					else{
						cout << endl << "Please enter a valid number (1-3)" << endl;
						err = true;
					}
				}
			}

			// display board
			cout << endl;
			for(int i=0;i<size;i++){
				if(board[i] != 0){
					if(board[i] == 1)
						cout << " X";
					else
						cout << " O";
				}
				else
					cout << " _";
			}
			cout << endl;
			
			if(((*pointer)->data->board == 0)){
				winner = (*pointer)->data->player;
				endGame = true;
			}
			if(((*pointer)->data->board == 1)){
				winner = ((*pointer)->data->player)*(-1);
				endGame = true;
			}
		}
		
		if(winner == 1)
			cout << endl << "!!!!!!!!!! YOU LOSE !!!!!!!!!!" << endl << endl;
		else
			cout << endl << "!!!!!!!!!! YOU WIN !!!!!!!!!!" << endl << endl;

		delete [] board;
	}

    void insert(Node **n, Data *d){  
		if(*n == NULL){
			*n = new Node(d);
		}

		Data *d1 = new Data(d->player*(-1),d->board-1,d->depth+1);
		Data *d2 = new Data(d->player*(-1),d->board-2,d->depth+1);
		Data *d3 = new Data(d->player*(-1),d->board-3,d->depth+1);
			
		if((d1->board >= 0) && ((*n)->left == NULL))
			insert(&((*n)->left),d1);
			
		if((d2->board >= 0) && ((*n)->mid == NULL))
			insert(&((*n)->mid),d2);

		if((d3->board >= 0) && ((*n)->right == NULL))
			insert(&((*n)->right),d3);
	}
    
    void genTree(){
		// player, board, depth
        Data *data = new Data(1,size,0); //1st player is AI (MAX)
        insert(&root,data);
    }
    
	void addUtility(Node *n){
		// MAX wins then +100
		// MIN wins then +10
		// also +depth

		if(n != NULL){
			addUtility(n->left);
			addUtility(n->mid);
			addUtility(n->right);

			// the last turn
			if(n->data->board == 0){
				if(n->data->player == 1) 
					n->data->utility += 100;
				else
					n->data->utility += 10;
				n->data->utility -= n->data->depth;
			}
		}
	}


	void search(Node *n){
		if(n != NULL){
			search(n->left);
			search(n->mid);
			search(n->right);

			if(n->data->board != 0){
				int util[3]; 
				// [0] = left
				// [1] = mid
				// [2] = right
				
				if(n->left != NULL)
					util[0] = n->left->data->utility;
				else
					util[0] = 0;

				if(n->mid != NULL)
					util[1] = n->mid->data->utility;
				else
					util[1] = 0;

				if(n->right != NULL)
					util[2] = n->right->data->utility;
				else
					util[2] = 0;

				//choose min utility
				if(n->data->player == -1){
					int min = util[0];
					for(int i=1;i<3;i++){
						if(util[i]==0)
							continue;
						else{
							if(util[i]<min)
								min = util[i];
						}
					}
					n->data->utility = min;
				}
				//choose max utility
				else{
					int max = util[0];
					for(int i=1;i<3;i++){
						if(util[i]==0)
							continue;
						else{
							if(util[i]>max)
								max = util[i];
						}
					}
					n->data->utility = max;
				}
			}
		}
	}

	// find best move for MAX
	int maxMove(Node **p){
		int count;
		int util[3]; 
		// [0] = left
		// [1] = mid
		// [2] = right
		
		for(int i=0;i<5;i++){
			search(*pointer);
		}
				
		if((*p)->left != NULL)
			util[0] = (*p)->left->data->utility;
		else
			util[0] = 0;

		if((*p)->mid != NULL)
			util[1] = (*p)->mid->data->utility;
		else
			util[1] = 0;

		if((*p)->right != NULL)
			util[2] = (*p)->right->data->utility;
		else
			util[2] = 0;

		// choose min utility
		int max = util[0];
		for(int i=1;i<3;i++){
			if(util[i]==0)
				continue;
			else{
				if(util[i]>=max)
					max = util[i];
			}
		}

		// get the number of O and *p will point to the node which has max utility
		if((*p)->left->data->utility == max){
			count = 1;
			pointer = &(*pointer)->left;
		}
		else if((*p)->mid->data->utility == max){
			count = 2;
			pointer = &(*pointer)->mid;
		}
		else if((*p)->right->data->utility == max){
			count = 3;
			pointer = &(*pointer)->right;
		}

		return count;
	}

	void printTreePreorder(Node *n){
		if(n != NULL){
			cout << n->data->player << "*" << n->data->board << "*" << n->data->utility << "  ";
			printTreePreorder(n->left);
			printTreePreorder(n->mid);
			printTreePreorder(n->right);
		}
	}
};