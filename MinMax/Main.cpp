#include <iostream>
#include <string>
#include "Tree.h"
#include "CutOff.h"
#include "AlphaBeta.h"

using namespace std;

int main()
{
	string playerName;

	//float time1;
	//float time2;
	//float time3;

	cout << "BEAT ME IF YOU CAN." << endl << endl;

	cout << "Enter player's name : ";
    cin >> playerName;
    cout  << endl << "Welcome " << playerName << endl;

//---------------------- GENERATE A FULL TREE ----------------------//   

	//cout << endl << endl << "********** PERFECT DECISION TREE **********" << endl << endl;
	Tree *tree = new Tree();
	//tree->startGame();
	//time1 = tree->getRunTime();
	
/*
//----------------------------- CUT OFF -----------------------------//

	cout << endl << endl << "****************** CUTOFF ******************"<< endl << endl;
	CutOff *cut = new CutOff();
	cut->startGame();
	time2 = cut->getRunTime();

//----------------------- ALPHA-BETA PRUNNING -----------------------//

	cout << endl << endl << "************ ALPHA-BETA PRUNNING ************" << endl << endl;
	AlphaBeta *ab = new AlphaBeta();
	ab->startGame();
	time3 = ab->getRunTime();

//----------------------------- RUN TIME -----------------------------//

	cout << endl << endl << "[ RUN TIME ]"<< endl << endl;
	cout << "Perfect Tree : " << time1 << " milliseconds" << endl;
	cout << "CutOff       : " << time2 << " milliseconds" << endl;
	cout << "Alpha-Beta   : " << time3 << " milliseconds" << endl;
	cout << endl << endl;

	*/

//------------------------------- TEST -------------------------------//
	bool newGame = true;
	char ans;
	int num;
	while(newGame){
		Tree *tree = new Tree();
		tree->startGame();
		cout << endl << "Play again? [y/n] : ";
		cin >> ans;
		if(ans == 'y' || ans == 'Y')
			newGame = true;
		else
			newGame = false;
	}
	
	return 0;
}
