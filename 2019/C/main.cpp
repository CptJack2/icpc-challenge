#include "bits/stdc++.h"
using namespace std;

char firstMove;
int moveNum;
enum moveType{
	move=1,
	jump=2
};
typedef pair<int,int> pii;
struct Move{
	int src;
	int dest;
	moveType type;
	vector<int> midway;
};
vector<Move> blackMoves, whiteMoves;
int main(){
	//read input
	cin>>firstMove>>moveNum;
	vector<Move>& move1=firstMove=='W'?whiteMoves:blackMoves,
		&move2=firstMove=='W'?blackMoves:whiteMoves;
	for (int i = 0; i < moveNum; ++i) {
		Move theMove;
		cin>>theMove.src;
		char c;
		cin>>c;
		if(c=='-'){
			theMove.type=moveType::move;
			cin>>theMove.dest;
		} else{
			theMove.type=moveType::jump;
			while(1){
				int t;
				cin >> t;
				theMove.midway.push_back(t);
				if(cin.peek()!='x')
					break;
				else
					cin>>c;
			}
			theMove.dest=theMove.midway.back();
			theMove.midway.pop_back();
		}
		i%2? move2.push_back(theMove): move1.push_back(theMove);
	}

}