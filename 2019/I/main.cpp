#include "bits/stdc++.h"
using namespace std;

struct RobotState{
    //row and col in [0,40]
    char row;
    char col;
    char direction;
    bool operator<(const RobotState& st2)const{return row<st2.row || row==st2.row && col<st2.col || row==st2.row && col==st2.col &&direction<st2.direction;}
    bool operator==(const RobotState& st2)const{return row==st2.row && col==st2.col &&direction==st2.direction;}
};

RobotState Inf{-1,-1,-1};

class Command{
public:
    virtual bool Execute(RobotState& st)=0;//is infinite loop
};

int r,c,d,e;
vector<vector<char>> grid;
vector<vector<Command*>> programs(27); //procedure A-Z, and last is main
map<pair<char,RobotState>,RobotState> executionCache;

pair<char,char> TryMove(const RobotState& st){
    char dr,dc;
    switch(st.direction){
        case 'w':
            dr=0;dc=-1;
            break;
        case 'e':
            dr=0;dc=1;
            break;
        case 's':
            dr=1;dc=0;
            break;
        case 'n':
            dr=-1;dc=0;
            break;
    }
    char newR= st.row + dr;
    char newC= st.col + dc;
    return make_pair(newR,newC);
}
char LookAhead(const RobotState& st){
    auto pairRC=TryMove(st);
    char newR= pairRC.first;
    char newC= pairRC.second;
    if(newR < r && newR > 0 && newC < c)
        return grid[newR][newC];
    else
        return '#';
}

class MoveCommand:public Command{
public:
    bool Execute(RobotState& st) final{
        auto pairRC=TryMove(st);
        char newR= pairRC.first;
        char newC= pairRC.second;
        if(newR < r && newR > 0 && newC < c && newC>0){
            st.row=newR;
            st.col=newC;
        }
        return false;
    }
};
class TurnCommand:public Command{
public:
    bool Execute(RobotState& st) final{
        string directions="nwse";
        int i=(directions.find(st.direction)+1)%directions.size();
        st.direction=directions[i];
        return false;
    }
};
class CallCommand:public Command {
public:
    char procedure;//A-Z for defined procedures, m for main function
    bool Execute(RobotState& st) final {
        auto cacheKey=make_pair(procedure,st);
        if(executionCache.find(cacheKey)!=executionCache.end()){
            st= executionCache[cacheKey];
            return st==Inf;
        }
        executionCache[cacheKey]=Inf;
        int ind=(procedure>='A' && procedure<='Z')?procedure-'A':26;
        for (int i = 0; i < programs[ind].size(); ++i) {
            auto cmd=programs[ind][i];
            if(cmd->Execute(st))
                return true;
        }
        executionCache[cacheKey]=st;
        return false;
    }
};
bool JudgeCond(const RobotState& st,const char cond){
    if(cond=='b')
        return LookAhead(st)=='#';
    else
        return st.direction==cond;
}
class IfCommand:public Command{
public:
    char condition;
    vector<Command*> TrueExec;
    vector<Command*> FalseExec;
    bool Execute(RobotState& st) final{
        for(auto cmd :(JudgeCond(st,condition)?TrueExec:FalseExec))
            cmd->Execute(st);
        return false;
    }
};
class UntilCommand:public Command{
public:
    char condition;
    vector<Command*> Exec;
    bool Execute(RobotState& st) final{
        vector<set<RobotState>> loopRoute(Exec.size());
        while(!JudgeCond(st,condition))
            for (int i = 0; i < Exec.size(); ++i) {
                auto cmd=Exec[i];
                if(loopRoute[i].insert(st).second==false)
                    return true;
                if(cmd->Execute(st))
                    return true;
            }
        return false;
    }
};

int FindMatchingParentheses(string prog,int leftParIndex){ //return Right Parentheses Index
    stack<int> stackPar;
    stackPar.push(leftParIndex);
    for(int i=leftParIndex+1;i<prog.size();++i){
        if(prog[i]=='(')
            stackPar.push(i);
        else if(prog[i]==')'){
            stackPar.pop();
            if(stackPar.empty())
                return i;
        }
    }
    if(!stackPar.empty())
        return -1;//no answer
}

vector<Command*> ParseProgram(string prog){
    vector<Command*> ret;
    for(int i=0;i<prog.size();++i){
        if(prog[i]=='i'){
            IfCommand* iCmd=new IfCommand();
            iCmd->condition=prog[i+1];
            int firstParEnd= FindMatchingParentheses(prog,i+2);
            int secParEnd=FindMatchingParentheses(prog,firstParEnd+1);
            iCmd->TrueExec= ParseProgram(prog.substr(i+3,firstParEnd-1-(i+3)+1));
            iCmd->FalseExec= ParseProgram(prog.substr(firstParEnd+2,secParEnd-1-(firstParEnd+2)+1));
            ret.push_back(iCmd);
            i=secParEnd;
        }else if(prog[i]=='u'){
            UntilCommand* uCmd=new UntilCommand();
            uCmd->condition=prog[i+1];
            int parEnd= FindMatchingParentheses(prog,i+2);
            uCmd->Exec= ParseProgram(prog.substr(i+3,parEnd-1-(i+3)+1));
            ret.push_back(uCmd);
            i=parEnd;
        }else if(prog[i]>='A' && prog[i]<='Z'){
            CallCommand* cCmd=new CallCommand();
            cCmd->procedure=prog[i];
            ret.push_back(cCmd);
        }else if(prog[i]=='l'){
            ret.push_back(new TurnCommand());
        }else if(prog[i]=='m'){
            ret.push_back(new MoveCommand());
        }
    }
    return ret;
}

int main(){
    cin>>r>>c>>d>>e;
    grid.resize(r, vector<char>(c));
    for(int i=0;i<r;++i)
        for(int j=0; j < c; ++j)
            cin >> grid[i][j];
    for(int i=0;i<d;++i){
        string prog;
        cin>>prog;
        char prog_n=prog.front();
        programs[prog_n-'A']=ParseProgram(prog.substr(2));
    }
    map<pair<char,RobotState>,RobotState> exectuionCache;
    for (int i = 0; i < e; ++i) {
        RobotState st;
        cin>>st.row>>st.col>>st.direction;
        string prog;
        cin>>prog;
        programs[26]=ParseProgram(prog);
        CallCommand cCmd;
        cCmd.procedure='m';
        if(cCmd.Execute(st))
            cout<<"inf"<<endl;
        else
            cout<<st.row<<" "<<st.col<<" "<<st.direction<<" "<<endl;
    }
}