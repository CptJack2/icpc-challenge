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
enum Direction{
	leftUp,
	leftDown,
	rightUp,
	rightDown,
};
map<pair<Direction,bool>,int> directionDelta{//bool true代表是偶数行, false奇数
	{{leftUp, true},1},
};
int getPosInDirection(int pos,Direction dir){

}
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
	//put chess
	auto oppositeColor=[&](char m)->char{ if(m == 'W')return 'B'; else return 'W';	};
	char lastMoved=moveNum%2==0? firstMove: oppositeColor(firstMove);
	//求解用到的结构
	enum chessType{
		man=1,
		king
	};
	//描述一个棋子的结构
	struct chess{
		int id;
		int pos;
		char color;
		chessType type;
	};
	chess* emptySquare=new chess{-1, -1, 'o',chessType(0)};
	vector<chess> chesses;
	vector<chess*> chessBoard(33,emptySquare);
	//开始放棋
	auto addChess=[&](int pos, char color)->int{
		int newId=static_cast<int>(chesses.size()+1);
		chesses.push_back(chess{newId,pos,color,man});
		return newId;
	};
	int blackMoveIndex=blackMoves.size()-1,
		whiteMoveIndex=whiteMoves.size()-1;
	struct TryPlace{

	};
	vector<TryPlace> tries;
	while(1){
		auto &moveList=lastMoved=='W'?whiteMoves:blackMoves;
		int& moveIndex=lastMoved=='W'?whiteMoveIndex:blackMoveIndex;
		auto theMove=moveList[moveIndex];
		//先看看dest有没有这个棋子,没有给他加上
		if(chessBoard[theMove.dest]==emptySquare){
			addChess(theMove.dest,lastMoved);
		}
		//如果移动方向不是man能走出来的,那这个棋子是个king
		if(lastMoved=='W' && theMove.dest>theMove.src ||
			lastMoved=='B' && theMove.dest<theMove.src)
			chessBoard[theMove.dest]->type=king;
		//将棋子从这一步的dest移动到src
		swap(chessBoard[theMove.dest],chessBoard[theMove.src]);
		//如果这一步是jump,给他补上被吃掉的棋子
		if(theMove.type==jump)
			for (int i = 0; i <= theMove.midway.size(); ++i) {
				int startPos,endPos;
				if(i==0)
					startPos=theMove.src;
				else
					startPos=theMove.midway[i-1];
				if(i==theMove.midway.size())
					endPos=theMove.dest;
				else
					endPos=theMove.midway[i];
				int eatenPos=(startPos+endPos+abs(startPos-endPos)==7?1:-1)/2;
				if(chessBoard[eatenPos]==emptySquare)
					addChess(eatenPos, oppositeColor(lastMoved));
			}
		//检查当前局面,是不是有棋子可以jump,但它却没有jump
		for(auto ch:chesses){
			if(ch.color!=lastMoved || ch.pos==theMove.src)
				continue;
			int lu,ld,ru,rd;
			if((ch.pos-1)/4%2==0){
				lu=ch.pos-4;
				ld=ch.pos+4;
				ru=ch.pos-3;
				rd=ch.pos+5;
			} else{
				lu=ch.pos-5;
				ld=ch.pos+3;
				ru=ch.pos-4;
				rd=ch.pos+4;
			}
			set<int> LBorder{5,13,21,29};
			set<int> RBorder{4,12,20,28};
			set<int> UBorder{1,2,3,4};
			set<int> DBorder{29,30,31,32};
			if(LBorder.count(ch.pos)){lu=-1;ld=-1;}
			if(RBorder.count(ch.pos)){ru=-1;rd=-1;}
			if(UBorder.count(ch.pos)){lu=-1;ru=-1;}
			if(DBorder.count(ch.pos)){rd=-1;ld=-1;}
			bool canJumpButDont= false;
			if(ch.type==man){

			}else{
				if(lu!=-1 && )
			}
		}
	}
}