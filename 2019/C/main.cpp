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
chess emptySquare{-1, color(0),chessType(0),-1};
chess unknownSquare{-2, color(0),chessType(0),-1};
inline bool operator==(const chess& a,const chess& b){return a.pos==b.pos && a.side==b.side && a.type==b.type && a.beginningPos==b.beginningPos;}
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
void printMove(const Move& theMove){
	if(theMove.type==moveType::move){
		cout<<theMove.route.front()<<"-"<<theMove.route.back()<<endl;
	}else{
		cout<<theMove.route.front();
		for (int i = 1; i < theMove.route.size() ; ++i)
			cout<<"x"<<theMove.route[i];
	}
}
void printChessboard(const vector<chess>& board){
	cout<<"----------------------------------"<<endl;
	for (int row = 1; row <= 8 ; ++row) {
		for(int col=row%2+1;col<=8;col+=2){
			int sind=squareCordinateToIndex({row,col});
			if (row % 2 == 1)
				cout << "---";
			if (board[sind] == emptySquare || board[sind] == unknownSquare) {
				char fill=board[sind] == emptySquare?'.':'?';
				cout << setw(3) << setfill(fill) << right << sind;
			} else {
				char c = outputMap[make_pair(board[sind].side, board[sind].type)];
				cout << c<<c<<c;
			}
			if (row % 2 == 0)
				cout << "---";
		}
		cout<<endl;
	}
	cout<<"----------------------------------"<<endl;
	cout.flush();
}
inline color oppositeColor(color m){
	if(m == white)
		return black;
	else
		return white;
}
pair<vector<chess>,vector<chess>> placeBlocker(vector<chess> start,color firstMove);
//向4个方向看看能不能跳. bool表示是否继续,两个vector分别是开始和结束的局面
pair<bool,pair<vector<chess>,vector<chess>>> checkJump(int chPos,vector<chess>& start, vector<chess>& board,color moving,color firstMove){
	for (int dr = -1; dr <= 1; dr += 2)
		for (int dc = -1; dc <= 1; dc += 2) {
			//man不能向反方向跳
			if (board[chPos].type == man && moving == white ^ dr == 1)
				continue;
			auto rc = squareIndexToCordinate(chPos);
			rc.first += dr;
			rc.second += dc;
			int nextPos = squareCordinateToIndex(rc);
			rc.first += dr;
			rc.second += dc;
			int jumpPos = squareCordinateToIndex(rc);
			//这个棋子可以jump，需要用各种棋子挡住去测试能不能通,递归地检查
			if (nextPos != -1 && jumpPos != -1 &&
				board[nextPos].side == oppositeColor(moving) &&
				board[jumpPos]==unknownSquare) {
				//这个格已经从未知被确认成了空格，说明这个格之前有棋子经过，用来挡住的棋子是不能动的，所以方案不可能
				if (board[jumpPos] == emptySquare)
					return {false,{{},{}}};
				//先用白棋试一下
				start[jumpPos] = chess{jumpPos, white, UBorder.count(jumpPos) ? king : man, jumpPos};
				//调试信息
				if(printDebugInfo)
					printChessboard(board);
				auto ret = placeBlocker(start,firstMove);
				//如果挡法可行，返回
				if (ret.first.size())
					return {false,ret};
				//否则用黑棋再试一下，不行的话就无解了
				start[jumpPos] = chess{jumpPos, black, DBorder.count(jumpPos) ? king : man, jumpPos};
				ret = placeBlocker(start,firstMove);
				return {false,ret};
			}
		}
	return {true,{{},{}}};
}
//递归放置没被动过的棋子，以适应jump必须优先的规则. 返回值是开始和结束的局面
pair<vector<chess>,vector<chess>> placeBlocker(vector<chess> start,color firstMove){
	int i;
	color moving;
	vector<chess> board=start;
	for(i=0, moving=firstMove;i<moves.size();++i,moving= oppositeColor(moving)){
		//对于move，检查自己的棋子有没有可以jump的，有的话需要挡住
		if(moves[i].type==moveType::move){
			for (int j = 1; j <= 32; ++j) {
				//空格或者对方的棋子，跳过
				if (board[j] == emptySquare || board[j] == unknownSquare ||board[j].side== oppositeColor(moving))
					continue;
				auto ret=checkJump(j,start,board,moving,firstMove);
				if(!ret.first)
					return ret.second;
			}
		}
		//将棋子从这一步的dest移动到src,注意src跳一圈又回到dest的情况
		int src=moves[i].route.front(),
				dest=moves[i].route.back();
		auto tCh=board[src];
		tCh.pos=dest;
		board[src]=emptySquare;
		board[dest]=tCh;
		//晋升
		bool promoted=false;
		if(board[dest].type==man && (moving==white && UBorder.count(dest) || moving==black && DBorder.count(dest))){
			board[dest].type=king;
			promoted=true;
		}
		//如果是jump(除去最后一跳晋升了的情况)，需要确保到达dest后不能再jump
		if(moves[i].type==jump && !promoted){
			auto ret=checkJump(dest,start,board,moving,firstMove);
			if(!ret.first)
				return ret.second;
		}
	}
	return {start,board};
}
int main(){
	printDebugInfo=true;
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
		moves.push_back(theMove);
	}
	//开始放棋
	int i;
	color moving;
	vector<chess> beginning(33,unknownSquare), board=beginning;
	//先将move涉及到的移动的和吃掉的棋子放上
	for(i=0, moving=firstMove;i<moves.size();++i,moving= oppositeColor(moving)){
		auto& theMove=moves[i];
		int src=theMove.route.front(),
			dest=theMove.route.back();
		//先看看src位置上有没有这个棋子,没有给他加上
		if(board[src]==unknownSquare){
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
		//将棋子从这一步的dest移动到src,注意src跳一圈又回到dest的情况
		auto tCh=board[src];
		tCh.pos=dest;
		board[src]=emptySquare;
		board[dest]=tCh;
		//看看是否晋升
		for (int j = 1; j < theMove.route.size(); ++j)
			if((moving==white && UBorder.count(dest) ||
				moving==black && DBorder.count(dest)) &&
	  			board[dest].type==man)
				board[dest].type=king;
		//如果这一步是jump,给他补上被吃掉的棋子
		if(theMove.type==jump) {
			auto eatenPos = getEatenPos(theMove);
			for (auto ep:eatenPos)
				if (board[ep] == unknownSquare){
					beginning[ep]=chess{ep,oppositeColor(moving),man,ep};
					board[ep]=beginning[ep];
				}
		}
//		if(printDebugInfo)
//			printChessboard(board,theMove);
	}
	if(printDebugInfo){
		vector<chess*> ps,pe;
		for(auto& ch:beginning)
			if(!(ch==emptySquare || ch==unknownSquare))
				ps.push_back(&ch);
		for(auto& ch:board)
			if(!(ch==emptySquare || ch==unknownSquare))
				pe.push_back(&ch);
		int a=1;
	}
	auto ret=placeBlocker(beginning,firstMove);
	//输出答案
	for (int row = 1; row <= 8 ; ++row) {
		auto outputOneLine=[&](vector<chess>& board){
			for(int col=row%2+1;col<=8;col+=2){
				int sind=squareCordinateToIndex({row,col});
				if (row % 2 == 1)
					cout << '-';
				if (board[sind] == emptySquare || board[sind] == unknownSquare)
					cout << outputMap[make_pair(board[sind].side, board[sind].type)];
				else
					cout << '.';
				if (row % 2 == 0)
					cout << '-';
			}
		};
		outputOneLine(ret.first);
		cout<<' ';
		outputOneLine(ret.second);
		cout<<endl;
	}
}