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
	bool eaten;
};
chess* emptySquare=new chess{-1, -1, 'o',chessType(0),true};
vector<chess> chesses;
vector<chess*> chessBoard(33,emptySquare);
enum Direction{
	leftUp,
	leftDown,
	rightUp,
	rightDown,
};
map<pair<Direction,bool>,int> directionDelta{//bool true代表是偶数行, false奇数
	{{leftUp, true},-4},
	{{leftDown, true},4},
	{{rightUp, true},-3},
	{{rightDown, true},5},
	{{leftUp, false},-5},
	{{leftDown, false},3},
	{{rightUp, false},-4},
	{{rightDown, false},4}
};
int getPosInDirection(int pos,Direction dir){
	set<int> LBorder{5,13,21,29};
	set<int> RBorder{4,12,20,28};
	set<int> UBorder{1,2,3,4};
	set<int> DBorder{29,30,31,32};
	if(LBorder.count(pos) && (dir==leftUp || dir==leftDown))
		return -1;
	if(RBorder.count(pos) && (dir==rightUp || dir==rightDown))
		return -1;
	if(UBorder.count(pos) && (dir==leftUp || dir==rightUp))
		return -1;
	if(DBorder.count(pos) && (dir==rightDown || dir==leftDown))
		return -1;
	return pos+directionDelta[make_pair(dir,(pos-1)/4%2==0)];
}
bool canJumpInDirection(chess& ch,Direction dir){
	int dirPos= getPosInDirection(ch.pos,dir);
	int jumpPos=getPosInDirection(dirPos,dir);
	if(dirPos==-1 || jumpPos==-1)
		return false;
	if(chessBoard[dirPos]!=emptySquare && chessBoard[dirPos]->color!=ch.color && chessBoard[jumpPos]==emptySquare)
		return true;
	return false;
}
vector<int> getEatenPos(const Move& theMove){
	vector<int> ret;
	for (int i = 0; i <= theMove.midway.size(); ++i) {
		int startPos, endPos;
		if (i == 0)
			startPos = theMove.src;
		else
			startPos = theMove.midway[i - 1];
		if (i == theMove.midway.size())
			endPos = theMove.dest;
		else
			endPos = theMove.midway[i];
		int eatenPos = (startPos + endPos + abs(startPos - endPos) == 7 ? 1 : -1) / 2;
		ret.push_back(eatenPos);
	}
	return ret;
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
	//开始放棋
	auto addChess=[&](int pos, char color)->int{
		int newId=static_cast<int>(chesses.size()+1);
		chesses.push_back(chess{newId,pos,color,man, false});
		return newId;
	};
	int blackMoveIndex=blackMoves.size()-1,
		whiteMoveIndex=whiteMoves.size()-1;
	while(blackMoveIndex || whiteMoveIndex){
		auto &moveList=lastMoved=='W'?whiteMoves:blackMoves;
		int& moveIndex=lastMoved=='W'?whiteMoveIndex:blackMoveIndex;
		auto& theMove=moveList[moveIndex];
		//先看看dest有没有这个棋子,没有给他加上
		if(chessBoard[theMove.dest]==emptySquare){
			addChess(theMove.dest,lastMoved);
		}else if(chessBoard[theMove.dest]->color!=lastMoved){
		//不是己方的棋子，可能是前面猜测性添加的时候，颜色错了
			chessBoard[theMove.dest]->color=lastMoved;
		}else if(lastMoved=='W' && theMove.dest>theMove.src ||
			lastMoved=='B' && theMove.dest<theMove.src)
		//如果移动方向不是man能走出来的,那这个棋子改成king
			chessBoard[theMove.dest]->type=king;
		//将棋子从这一步的dest移动到src
		swap(chessBoard[theMove.dest],chessBoard[theMove.src]);
		//如果这一步是jump,给他补上被吃掉的棋子
		if(theMove.type==jump) {
			auto eatenPos = getEatenPos(theMove);
			for (auto ep:eatenPos)
				if (chessBoard[ep] == emptySquare)
					addChess(ep, oppositeColor(lastMoved));
		}
		//检查当前局面,是不是有棋子可以jump,但它却没有jump
		for(auto ch:chesses){
			if(ch.color!=lastMoved)
				continue;
			vector<Direction> checkDirections;
			//man chess
			//check two dirs, if the direction
			// if can jump but don't, must have a piece in its way
			auto canAddDirection=[&](Direction dir)->bool{
				int dirPos=getPosInDirection(theMove.src,dir);
				if(dirPos==theMove.dest)
					return false;
				for(auto mid:theMove.midway)
					if(dirPos == mid)
						return false;
				return true;
			};
			if(ch.type==man){
				if(lastMoved=='W'){
					if(canAddDirection(leftUp))
						checkDirections.push_back(leftUp);
					if(canAddDirection(rightUp))
						checkDirections.push_back(rightUp);
				}else {
					if(canAddDirection(rightDown))
						checkDirections.push_back(rightDown);
					if(canAddDirection(leftDown))
						checkDirections.push_back(leftDown);
				}
			}else {
				if(canAddDirection(leftUp))
					checkDirections.push_back(leftUp);
				if(canAddDirection(rightUp))
					checkDirections.push_back(rightUp);
				if(canAddDirection(rightDown))
					checkDirections.push_back(rightDown);
				if(canAddDirection(leftDown))
					checkDirections.push_back(leftDown);
			}
			for(auto dir:checkDirections){
				if(canJumpInDirection(ch,dir))
					addChess(getPosInDirection(ch.pos,dir),oppositeColor(lastMoved));
			}
		}
		lastMoved=oppositeColor(lastMoved);
		--moveIndex;
	}
	whiteMoveIndex=0;
	blackMoveIndex=0;
	//因为要同时输出开始和结束的棋盘,复制一份
	vector<chess>beforeChesses=chesses;
	vector<chess*>afterChessboard=chessBoard;
	while(whiteMoveIndex<whiteMoves.size() || blackMoveIndex<blackMoves.size()){
		auto &moveList=firstMove=='W'?whiteMoves:blackMoves;
		int& moveIndex=firstMove=='W'?whiteMoveIndex:blackMoveIndex;
		auto& theMove=moveList[moveIndex];
		auto pch=afterChessboard[theMove.src];
		//操作的棋子移动位置
		pch->pos=theMove.dest;
		//棋盘数据同步
		afterChessboard[theMove.dest]=pch;
		afterChessboard[theMove.src]=emptySquare;
		//如果是jump,处理被吃掉的棋子
		if(theMove.type==jump){
			auto eatenPos = getEatenPos(theMove);
			for (auto ep:eatenPos){
				chessBoard[ep]->eaten=true;
				chessBoard[ep]=emptySquare;
			}
		}
		++moveIndex;
	}
	//输出答案
	int row=0;
	for (int i = 0; i < chessBoard.size(); ++i) {
		map<pair<char,chessType>,char> outputMap={
				{{'W',man},'w'},
				{{'W',king},'W'},
				{{'B',man},'b'},
				{{'B',man},'B'},
		};
		auto outputOneLine=[&](vector<chess*>& board){
			if(row%2==0)
				cout<<'-';
			if(board[i] != emptySquare)
				cout<<outputMap[make_pair(board[i]->color, board[i]->type)];
			else
				cout<<'.';
			if(row%2==1)
				cout<<'-';
		};
		outputOneLine(chessBoard);
		cout<<' ';
		outputOneLine(afterChessboard);
		if(i%4==3){
			++row;
			cout<<endl;
		}
	}
}