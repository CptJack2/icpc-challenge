#include "bits/stdc++.h"
using namespace std;

bool printDebugInfo=false;
int moveNum;
enum moveType{
	move=1,
	jump=2
};
enum color{
	white=1,
	black=2
};
enum chessType{
	man=1,
	king
};
struct Move{
	moveType type;
	vector<int> route;
	string str;//for debug
};
vector<Move> moves;

//描述一个棋子的结构
struct chess{
//	int id;
	int pos;
	color side;
	chessType type;
	int beginningPos;
};
map<pair<color,chessType>,char> outputMap={
		{{white,man},'w'},
		{{white,king},'W'},
		{{black,man},'b'},
		{{black,king},'B'},
};
chess emptySquare{-1, color(0),chessType(0),true};
inline bool operator==(const chess& a,const chess& b){return a.pos==b.pos && a.side==b.side && a.type==b.type && a.eaten==b.eaten;}
inline bool operator!=(const chess& a,const chess& b){return !(a==b);}
enum Direction{
	leftUp,
	leftDown,
	rightUp,
	rightDown,
};
set<int> LBorder{5,13,21,29};
set<int> RBorder{4,12,20,28};
set<int> UBorder{1,2,3,4};
set<int> DBorder{29,30,31,32};
int getPosInDirection(int pos,Direction dir){
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
bool canJumpInDirection(chess& ch,Direction dir,vector<chess>& chessBoard){
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
inline int squareCordinateToIndex(pair<int,int> rc){//1-32
	if(rc.first>8 || rc.first<1 || rc.second>8 || rc.second<1)
		return -1;
	return (rc.first - 1) * 4 + (rc.second - 1) / 2 + 1;
}
vector<int> getEatenPos(const Move& theMove){
	vector<int> ret;
	for (int i = 0; i <= theMove.route.size()-2; ++i) {
		int startPos=theMove.route[i],
			endPos=theMove.route[i+1];
		auto sCord= squareIndexToCordinate(startPos),
			eCord= squareIndexToCordinate(endPos);
		int eatenPos = squareCordinateToIndex({(sCord.first+eCord.first)/2,(sCord.second+eCord.second)/2});
		ret.push_back(eatenPos);
	}
	return ret;
}
void printChessboard(const vector<chess>& board,const Move& theMove){
	if(theMove.type==moveType::move){
		cout<<theMove.src<<"-"<<theMove.dest<<endl;
	}else{
		cout<<theMove.src;
		for(auto m:theMove.midway)
			cout<<"x"<<m;
		cout<<"x"<<theMove.dest<<endl;
	}
	cout<<"----------------------------------"<<endl;
	for (int row = 1; row <= 8 ; ++row) {
		for(int col=row%2+1;col<=8;col+=2){
			int sind=squareCordinateToIndex({row,col});
			if (row % 2 == 1)
				cout << "--";
			if (board[sind] != emptySquare) {
				char c = outputMap[make_pair(board[sind].side, board[sind].type)];
				cout << c<<c;
			} else
				cout << setw(2)<<setfill('0')<<right<<sind;
			if (row % 2 == 0)
				cout << "--";
		}
		cout<<endl;
	}
	cout<<"----------------------------------"<<endl;
	cout.flush();
}
inline color oppositeColor(color m){ if(m == white)return black; else return white;}
void blockAllJump(vector<chess>& board, color movingColor, const Move& nextMove,vector<chess>* oldBoard){
	for(auto ch:board){
		if(ch==emptySquare || ch.side != movingColor)
			continue;
		vector<Direction> checkDirections;
		if(ch.type==man)
			if(movingColor == white)
				checkDirections={leftUp,rightUp};
			else
				checkDirections={rightDown,leftDown};
		else
			checkDirections={leftUp,rightUp,rightDown,leftDown};
		for(auto dir:checkDirections){
			//棋子ch在这个方向能跳过去,就要堵住
			if(canJumpInDirection(ch,dir,board)) {
				int dirPos=getPosInDirection(ch.pos, dir);
				int jumpPos=getPosInDirection(dirPos, dir);
				addChess(board, jumpPos, oppositeColor(movingColor));
				if (oldBoard != nullptr)
					addChess(*oldBoard, getPosInDirection(ch.pos, dir), oppositeColor(movingColor));
			}
		}
	}
}
int main(){
//	printDebugInfo=true;
	//read input
	char tc;
	color firstMove;
	cin>>tc>>moveNum;
	firstMove=tc=='W'?white:black;
	for (int i = 0; i < moveNum; ++i) {
		string mStr;
		cin>>mStr;
		Move theMove;
		theMove.str=mStr;
		stringstream ssin(mStr);
		int pos;
		ssin>>pos;
		theMove.route.push_back(pos);
		char c;
		ssin>>c;
		if(c=='-')
			theMove.type=moveType::move;
		else
			theMove.type=moveType::jump;
		while(1){
			ssin >> pos;
			theMove.route.push_back(pos);
			if(ssin.peek()!='x' && ssin.peek()!='-')
				break;
			else
				ssin>>c;
		}
	}
	//开始放棋
	int i;
	color moving;
	vector<chess> beginning(33,emptySquare), board=beginning;
	//先将move涉及到的移动的和吃掉的棋子放上
	for(i=0, moving=firstMove;i<moves.size();++i,moving= oppositeColor(moving)){
		auto& theMove=moves[i];
		//先看看dest有没有这个棋子,没有给他加上
		int src=theMove.route.front(),
			dest=theMove.route.back();
		if(board[src]==emptySquare){
			beginning[src]=chess{src,moving,man,src};
			board[src]=beginning[src];
		}
		//如果移动方向不是man能走出来的,那这个棋子改成king
		for (int j = 0; j <= theMove.route.size() - 2; ++j) {
			int startPos=theMove.route[j],
				endPos=theMove.route[j + 1];
			if(moving==white ^ startPos>endPos && board[src].type==man){
				board[src].type=king;
				beginning[board[src].beginningPos].type=king;
			}
		}
		//将棋子从这一步的dest移动到src
		swap(board[dest],board[src]);
		//看看是否晋升
		if((moving==white && UBorder.count(dest) ||
			moving==black && DBorder.count(dest)) &&
		   board[dest].type==man)
		   board[dest].type=king;
		//如果这一步是jump,给他补上被吃掉的棋子
		if(theMove.type==jump) {
			auto eatenPos = getEatenPos(theMove);
			for (auto ep:eatenPos)
				if (board[ep] == emptySquare){
					beginning[ep]=chess{ep,oppositeColor(moving),man,ep};
					board[ep]=beginning[ep];
				}
		}
	}
	//递归放置没被动过的棋子，以适应jump必须优先的规则
	auto placeBlocker=[&](vector<chess>& start)->vector<chess>{//返回值是结束的局面
		int i;
		color moving;
		vector<chess> board=start;
		for(i=0, moving=firstMove;i<moves.size();++i,moving= oppositeColor(moving)){
			//对于move，检查自己的棋子有没有可以jump的，有的话需要挡住
			if(moves[i].type==moveType::move){
				for (int j = 1; j <= 32; ++j) {
					//空格或者对方的棋子，跳过
					if (board[j] == emptySquare || board[j].side== oppositeColor(moving))
						continue;
					//向4个方向看看能不能跳
					for (int dr = -1; dr <= 1; dr += 2)
						for (int dc = -1; dc <= 1; dc += 2) {
							//man不能向反方向跳
							if (board[j].type == man && moving == white ^ dr == 1)
								continue;
							auto rc = squareIndexToCordinate(j);
							rc.first += dr;
							rc.second += dc;
							int nextPos = squareCordinateToIndex(rc);
							rc.first += dr;
							rc.second += dc;
							int jumpPos = squareCordinateToIndex(rc);
							//这个棋子可以jump，需要用各种棋子挡住去测试能不能通
							if (nextPos!=-1 && jumpPos!=-1 && board[nextPos].side==oppositeColor(moving) && board[jumpPos]==emptySquare){

							}
						}

				}
			}
		}
	};
	vector<chess*> debugChVec;
	if(printDebugInfo){
		for (auto &ch: chessBoard)
			if (ch != emptySquare)
				debugChVec.push_back(&ch);
	}
	whiteMoveIndex=0;
	blackMoveIndex=0;
	//因为要同时输出开始和结束的棋盘,复制一份
	vector<chess>afterChessboard=chessBoard;
	if(printDebugInfo)
		cout<<"start game"<<endl;
	while(whiteMoveIndex<whiteMoves.size() || blackMoveIndex<blackMoves.size()){
		auto &moveList=firstMove==white?whiteMoves:blackMoves;
		int& moveIndex=firstMove==white?whiteMoveIndex:blackMoveIndex;
		auto& theMove=moveList[moveIndex];
		//如果这一步是移动,需要检查当前局面,将所有可以jump的棋子挡住
		if(theMove.type==moveType::move){
			blockAllJump(afterChessboard,firstMove,theMove, &chessBoard);
		}
		//操作的棋子移动位置
		afterChessboard[theMove.src].pos=theMove.dest;
		swap(afterChessboard[theMove.src],afterChessboard[theMove.dest]);
		//晋升
		if((firstMove==white && UBorder.count(theMove.dest) ||
			firstMove==black && DBorder.count(theMove.dest)) &&
			afterChessboard[theMove.dest].type==man)
			afterChessboard[theMove.dest].type=king;
		//如果是jump,处理被吃掉的棋子
		if(theMove.type==jump){
			auto eatenPos = getEatenPos(theMove);
			for (auto ep:eatenPos){
				afterChessboard[ep]=emptySquare;
			}
		}
		++moveIndex;
		firstMove=oppositeColor(firstMove);
		if(printDebugInfo)
			printChessboard(afterChessboard,theMove);
	}
	if(printDebugInfo){
		debugChVec.clear();
		for (auto &ch: afterChessboard)
			if (ch != emptySquare)
				debugChVec.push_back(&ch);
	}
	//输出答案
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