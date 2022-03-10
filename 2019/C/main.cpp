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
	pii src;
	pii dest;
	moveType type;
	vector<pii> midway;
};
vector<Move> blackMoves, whiteMoves;
int main(){
	cin>>firstMove>>moveNum;
	vector<Move>& move1=firstMove=='W'?whiteMoves:blackMoves,
		move2=firstMove=='W'?blackMoves:whiteMoves;
	for (int i = 0; i < moveNum; ++i) {
		Move theMove;
		cin>>theMove.src;
		char c;
	}
}