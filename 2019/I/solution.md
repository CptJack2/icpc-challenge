#ICPC 2019 World Final I题 Karel the Robot题解

这个题如果理解了算法，还是比较简单的，只是有几个点需要注意的。
官方的题解和解析视频都做的比较烂，而且网上都没有这题的blog，包括外网，所以就打算写一写。

#问题简述
karel是一种简单的机器人编程语言，用来控制机器人，只有转向和移动两种命令，还有循环、分支和函数调用语句。现在给一个网格（小于40x40，上面有些点是障碍物，不能走），和一些程序（每段程序小于100个字符），让你模拟karel语言的执行过程，输出机器人最后的位置，如果机器人执行的程序停不下来，输出Inf。

#算法解析
没有奇怪的算法，就是解析这段程序，然后按照它的程序一步步跑就完了，就是写一个简单的编译器前端。
需要注意的是问题规模，由于最多有26个子程序，每个子程序可以被递归调用，所以老实解析程序模拟函数调用，程序执行的指令数可以去到100^(100^(100^100……))，一共27个括号，中间包着的100在指数的位置，真是个天文数字。。。这样的话，可以将机器人的状态当做函数输入，当调用一个函数的时候，记录下对应的机器人变化状态作为输出，由于网格大小为40x40，机器人有4个方向，所以机器人最多只有4x40x40=6400种状态，再乘上26个子程序，最多只有166400种不同组合，大小是合理的。同时，这个函数输出缓存还有一个大作用，就是可以识别出无限递归的函数。由于只有26个子程序，每个子程序100条命令，由于有缓存的存在，所以最多也只会执行6400x26x100=1.664x10^7条指令，时间复杂度也是合理的。
为什么只记录到函数级别，而不记录到分支指令和循环指令的级别。因为造成程序复杂度最高的还是函数调用，而且分支指令和循环指令都是在函数内部的，所以记录了函数等于记录了这两种指令，而且由于程序最多只能有100个字符，所以这两种指令的复杂度也是有限的。
另外，死循环的识别靠执行循环的时候记录执行路径，碰到重复的路径就知道是死循环了。

#实现
由于语言简单，没有空白字符，所以分词都不用实现，可以直接生成语法解析树，然后执行。
实现上定义了Command基类，各种移动、转向、分支、循环命令实现为派生类。
```c++
class Command{
public:
    virtual bool Execute(RobotState& st)=0;//返回值指示这个命令是否陷入了无限循环
    string origianl_program;//原始的程序文本，debug用
};
class MoveCommand:public Command{
public:
    bool Execute(RobotState& st) final{
        if(LookAhead(st)=='#')
            return false;
        auto pairRC=TryMove(st);
        int newR= pairRC.first;
        int newC= pairRC.second;
        if(newR <= r && newR >= 1 && newC <= c && newC >= 1){
            st.row=newR;
            st.col=newC;
        }
        return false;
    }
};
```
解析输入程序则在ParseProgram函数内。
```c++
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
            iCmd->origianl_program=prog.substr(i,secParEnd-i+1);
            ret.push_back(iCmd);
            i=secParEnd;
        }else if(prog[i]=='u'){
            UntilCommand* uCmd=new UntilCommand();
            uCmd->condition=prog[i+1];
            int parEnd= FindMatchingParentheses(prog,i+2);
            uCmd->Exec= ParseProgram(prog.substr(i+3,parEnd-1-(i+3)+1));
            uCmd->origianl_program=prog.substr(i,parEnd-i+1);
            ret.push_back(uCmd);
            i=parEnd;
        }else if(prog[i]>='A' && prog[i]<='Z'){
            CallCommand* cCmd=new CallCommand();
            cCmd->procedure=prog[i];
            cCmd->origianl_program=prog[i];
            ret.push_back(cCmd);
        }else if(prog[i]=='l'){
            auto cmd=new TurnCommand();
            cmd->origianl_program='l';
            ret.push_back(cmd);
        }else if(prog[i]=='m'){
            auto cmd=new MoveCommand();
            cmd->origianl_program='m';
            ret.push_back(cmd);
        }
    }
    return ret;
}
```
最后，为了识别死循环，用loopRoute来记录循环执行路径
```c++
class UntilCommand:public Command{
public:
    char condition;
    vector<Command*> Exec;
    bool Execute(RobotState& st) final{
        vector<set<RobotState>> loopRoute(Exec.size()+1);//the last route for the while expression
        while(!JudgeCond(st,condition)) {
            if(loopRoute[Exec.size()].insert(st).second==false)
                return true;
            for (int i = 0; i < Exec.size(); ++i) {
                auto cmd = Exec[i];
                if (loopRoute[i].insert(st).second == false ||
                    cmd->Execute(st))
                    return true;
            }
        }
        return false;
    }
};
```

#代码实现
https://github.com/CptJack2/icpc-challenge/blob/master/2019/I/main.cpp