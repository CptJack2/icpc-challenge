#include "bits/stdc++.h"
using namespace std;

int moveNum;
enum moveType{
	move=1,
	jump=2
};
enum color{
	white=1,
	black=2
};
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
//	int id;
	int pos;
	color side;
	chessType type;
	bool eaten;
};
chess emptySquare{-1, color(0),chessType(0),true};
inline bool operator==(const chess& a,const chess& b){return a.pos==b.pos && a.side==b.side && a.type==b.type && a.eaten==b.eaten;}
inline bool operator!=(const chess& a,const chess& b){return !(a==b);}
vector<chess> chessBoard(33,emptySquare);
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
	if(chessBoard[dirPos]!=emptySquare && chessBoard[dirPos].side != ch.side && chessBoard[jumpPos] == emptySquare)
		return true;
	return false;
}
inline pair<int,int> squareIndexToCordinate(int ind){//(x,y) == ([1,8],[1,8])
	int row=(ind-1)/4+1;
	int col=(ind-1)%4*2+row%2+1;
	return {row,col};
}
inline int squareCordinateToIndex(pair<int,int> cord){//1-32
	return (cord.first-1)*4 + (cord.second-1)/2 + 1;
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
		auto sCord= squareIndexToCordinate(startPos),
			eCord= squareIndexToCordinate(endPos);
		int eatenPos = squareCordinateToIndex({(sCord.first+eCord.first)/2,(sCord.second+eCord.second)/2});
		ret.push_back(eatenPos);
	}
	return ret;
}
int main(){
	//read input
	char tc;
	color firstMove;
	cin>>tc>>moveNum;
	firstMove=tc=='W'?white:black;
	vector<Move>& move1=firstMove==white?whiteMoves:blackMoves,
		&move2=firstMove==white?blackMoves:whiteMoves;
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
	auto oppositeColor=[&](color m)->color{ if(m == white)return black; else return white;};
	color lastMoved=moveNum%2==1? firstMove: oppositeColor(firstMove);
	//开始放棋
	auto addChess=[&](int pos, color color){
//		int newId=static_cast<int>(chesses.size()+1);
//		chesses.push_back(chess{newId,pos,color,man, false});
		chessBoard[pos]=chess{pos,color,man, false};
		//return newId;
	};
	int blackMoveIndex=blackMoves.size()-1,
		whiteMoveIndex=whiteMoves.size()-1;
	while(blackMoveIndex>=0 || whiteMoveIndex>=0){
		auto &moveList=lastMoved==white?whiteMoves:blackMoves;
		int& moveIndex=lastMoved==white?whiteMoveIndex:blackMoveIndex;
		auto& theMove=moveList[moveIndex];
		//先看看dest有没有这个棋子,没有给他加上
		if(chessBoard[theMove.dest]==emptySquare){
			addChess(theMove.dest,lastMoved);
		}else if(chessBoard[theMove.dest].side != lastMoved){
		//不是己方的棋子，可能是前面猜测性添加的时候，颜色错了
			chessBoard[theMove.dest].side=lastMoved;
		}
		//如果移动方向不是man能走出来的,那这个棋子改成king
		if(lastMoved==white && theMove.dest>theMove.src ||
			lastMoved==black && theMove.dest<theMove.src)
			chessBoard[theMove.dest].type=king;
		//将棋子从这一步的dest移动到src
		swap(chessBoard[theMove.dest],chessBoard[theMove.src]);
		chessBoard[theMove.src].pos=theMove.src;
		//如果这一步是jump,给他补上被吃掉的棋子
		if(theMove.type==jump) {
			auto eatenPos = getEatenPos(theMove);
			for (auto ep:eatenPos)
				if (chessBoard[ep] == emptySquare)
					addChess(ep, oppositeColor(lastMoved));
			//如果man向反方向jump了,将他改成king
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
				if(lastMoved==white && startPos>endPos ||
				   lastMoved==black && startPos<endPos)
					chessBoard[theMove.src].type=king;
			}
		}
		//检查当前局面,是不是有棋子可以jump,但它却没有jump
		for(auto ch:chessBoard){
			if(ch==emptySquare)continue;
			if(ch.side != lastMoved)
				continue;
			vector<Direction> checkDirections;
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
				if(lastMoved==white){
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
	vector<chess>afterChessboard=chessBoard;
	while(whiteMoveIndex<whiteMoves.size() || blackMoveIndex<blackMoves.size()){
		auto &moveList=firstMove==white?whiteMoves:blackMoves;
		int& moveIndex=firstMove==white?whiteMoveIndex:blackMoveIndex;
		auto& theMove=moveList[moveIndex];
		//操作的棋子移动位置
		afterChessboard[theMove.src].pos=theMove.dest;
		afterChessboard[theMove.dest]=afterChessboard[theMove.src];
		afterChessboard[theMove.src]=emptySquare;
		//如果是jump,处理被吃掉的棋子
		if(theMove.type==jump){
			auto eatenPos = getEatenPos(theMove);
			for (auto ep:eatenPos){
				afterChessboard[ep]=emptySquare;
			}
		}
		++moveIndex;
		firstMove=oppositeColor(firstMove);
	}
	//输出答案
	map<pair<color,chessType>,char> outputMap={
			{{white,man},'w'},
			{{white,king},'W'},
			{{black,man},'b'},
			{{black,king},'B'},
	};
	for (int row = 1; row <= 8 ; ++row) {
		auto outputOneLine=[&](vector<chess>& board){
			for(int col=row%2+1;col<=8;col+=2){
				int sind=squareCordinateToIndex({row,col});
				if (row % 2 == 1)
					cout << '-';
				if (board[sind] != emptySquare)
					cout << outputMap[make_pair(board[sind].side, board[sind].type)];
				else
					cout << '.';
				if (row % 2 == 0)
					cout << '-';
			}
		};
		outputOneLine(chessBoard);
		cout<<' ';
		outputOneLine(afterChessboard);
		cout<<endl;
	}
}