#include <iostream>
#include <windows.h>
using namespace std;

class pData
{
public:
	int player;		// current player ; min = -1 (User) , max = 1 (AI)
    int board;		// number of remaining spaces
    int utility;
	int depth;
	int branch;

	pData(int p,int b,int d,int br){
        player = p;
        board = b;
		depth = d;
		utility = 0;
		branch = br;
    }
};

class pNode
{
public:
	pNode *left;
	pNode *mid;
	pNode *right;
	pNode *parent;
	pData *data;

	pNode(pData *d)
	{
		left = NULL;
		mid  = NULL;
		right = NULL;
		parent = NULL;

		data = new pData(d->player,d->board,d->depth,d->branch);
	}
};

class AlphaBeta
{
public:
	pNode *root;
	pNode **pointer;

	int size;
	int lvl;
	bool stop;

	LARGE_INTEGER frequency;        // ticks per second
    LARGE_INTEGER t1, t2;           // ticks
    double elapsedTime;

		/* 
			3 Cases :
				1) isLeaf & board != 0
				2) isLeaf & board == 0
				2) isNotLeaf

			1 ->	consider from their data

			2 ->	last node is MAX then +100
					last node is MIN then +10
					also +depth

			3 ->	use evaluation()
		*/
    
    AlphaBeta(){
        root = NULL;
		pointer = NULL;
		size = 10;
		lvl = 3;
		stop = false;
		QueryPerformanceFrequency(&frequency);
    }

	float getRunTime(){
		elapsedTime = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;	
		return elapsedTime;
	}
    
	void startGame(){
		int *board = new int[size];
		// initial board
		cout << endl;
		for(int i=0;i<size;i++){
			board[i] = 0;
			cout << " _";
		}

		bool endGame = false;
		int count;
		
		int winner;
		pointer = &root;

		// start timer
		QueryPerformanceCounter(&t1);

		genTreeFromRoot(&root,lvl);
		addUtility(root);

		
		cout << endl;
		int posB = -1;

		// playing
		while(!endGame){
			
			// MAX's turn
			if(((*pointer)->data->player == 1)){
				cout << endl << "AI's turn" << endl;
				count = maxMove(pointer);

				// STOP THE TIMER
				if(!stop){
					QueryPerformanceCounter(&t2);
					stop = true;
				}

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

			// MIN's turn
			else{
				bool err = true;
				while(err){
					cout << endl << "Your turn" << endl;
					cout << "Enter X (1-3) : ";
					cin >> count;

					// if there's no child then generating tree is needed
					// check only left node is enough
					if((*pointer)->left == NULL){ 
							genTreeFromNode(pointer,lvl,(*pointer)->data->player);
							addUtility(*pointer);
						}

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
						cout << "Error : Please enter a valid number (1-3)";
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
			cout << endl << "!!!!!!!!!! AI WIN !!!!!!!!!!" << endl << endl;
		else
			cout << endl << "!!!!!!!!!! YOU WIN !!!!!!!!!!" << endl << endl;

		delete [] board;	
	}

    void insert(pNode **n, pData *d,int level, pNode **parent){
		if(level > 0){
			if(*n == NULL){
				*n = new pNode(d);
				(*n)->parent = *parent;
			}

			pData *d1 = new pData(d->player*(-1),d->board-1,d->depth+1,1);
			pData *d2 = new pData(d->player*(-1),d->board-2,d->depth+1,2);
			pData *d3 = new pData(d->player*(-1),d->board-3,d->depth+1,3);
			
			if((d1->board >= 0) && ((*n)->left == NULL))
				insert(&((*n)->left),d1,level-1,n);
			
			if((d2->board >= 0) && ((*n)->mid == NULL))
				insert(&((*n)->mid),d2,level-1,n);

			if((d3->board >= 0) && ((*n)->right == NULL))
				insert(&((*n)->right),d3,level-1,n);
		}
	}
    
    void genTreeFromRoot(pNode **node,int level){
		// player, board, depth
        pData *data = new pData(1,size,0,1); //1st player is AI (MAX)
        insert(node,data,level,node);
    }

	void genTreeFromNode(pNode **node,int level,int player){
		// player, board, depth
		pData *data = new pData(player,(*node)->data->board,0,1);
		insert(node,data,level,node);
	}

	bool isLeaf(pNode *n){
		if(n->left == NULL && n->mid == NULL && n->right == NULL)
			return true;
		else
			return false;
	}
    
	int evaluation(pNode *n){
		int value = 0;

		// 1 space left
		if(n->data->board == 1){
			if(n->data->player == 1)
				value += 10;
			else
				value += 100;
		}

		else if(n->data->board == 0){
			if(n->data->player == 1)
				value += 100;
			else
				value += 10;
		}

		//// 2 spaces left
		//else if(n->data->board == 2){
		//	if(n->data->board == 1)
		//		value += 20;
		//	else
		//		value += 120;

		//}

		//// 3 spaces left
		//else if(n->data->board == 3){
		//	if(n->data->player == 1)
		//		value += 30;
		//	else
		//		value += 130;
		//}

		else{
			if(n->data->player == 1)
				value += 0;
			else
				value += 0;
		}

		return value;
	}

	void addUtility(pNode *n){
		int value;

		if(n != NULL){
			addUtility(n->left);
			addUtility(n->mid);
			addUtility(n->right);

			// Case 1
			if(isLeaf(n) && n->data->board != 0){
				value = evaluation(n);
				n->data->utility = value;
			}

			// Case 2
			else if(isLeaf(n) && n->data->board == 0){
				if(n->data->player == 1) 
					n->data->utility += 100;
				else
					n->data->utility += 10;
				n->data->utility -= n->data->depth;
			}
		}
	}

	void search(pNode *n){
		if(n != NULL){
			search(n->left);
			search(n->mid);
			search(n->right);

			// node left must check every children
			if(n->data->branch == 1){
				if(n->data->board != 0 && !isLeaf(n)){
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

			// node middle or right check some children
			// USE ALPHA-BETA PRUNING
			else{
				if(n->data->board != 0 && !isLeaf(n)){

					//MIN
					if(n->data->player = -1){
						int beta,value;

						beta = (n->parent)->left->data->utility;

						if(n->left != NULL){
							value = n->left->data->utility;
							if(value < beta)
								beta = value;
						}
						else if(n->mid != NULL){
							value = n->mid->data->utility;
							if(value < beta)
								beta = value;
						}
						else if(n->right != NULL){
							value = n->right->data->utility;
							if(value < beta)
								beta = value;
						}
						
						n->data->utility = beta;
					}

					//MAX
					if(n->data->player = -1){
						int alpha,value;

						alpha = (n->parent)->left->data->utility;

						if(n->left != NULL){
							value = n->left->data->utility;
							if(value > alpha)
								alpha = value;
						}
						else if(n->mid != NULL){
							value = n->mid->data->utility;
							if(value > alpha)
								alpha = value;
						}
						else if(n->right != NULL){
							value = n->right->data->utility;
							if(value > alpha)
								alpha = value;
						}

						n->data->utility = alpha;
					}
				}
			}
		}
			
	}

	// find best move for MAX
	int maxMove(pNode **p){
		int count;
		int util[3]; 
		/*	[0] = left
			[1] = mid
			[2] = right
		*/

		// if there's no child then generating tree is needed
		// check only left node is enough
		if((*p)->left == NULL){
			genTreeFromNode(p,lvl,(*p)->data->player);
			addUtility(*p);
		}
				
		search(*pointer);

		// if there has at least one child
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

	void printTreePreorder(pNode *n){
		if(n != NULL){
			cout << (n->parent)->data->branch << endl;
			printTreePreorder(n->left);
			printTreePreorder(n->mid);
			printTreePreorder(n->right);
		}
	}

	void printLastDepth(pNode *n){
		printLastDepth(n->left);
		printLastDepth(n->mid);
		printLastDepth(n->right);
		if(isLeaf(n)){
			cout << n->data->depth << " & ";
		}
	}
};