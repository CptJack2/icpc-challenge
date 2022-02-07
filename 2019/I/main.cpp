#include "bits/stdc++.h"
using namespace std;

int r,c,d,e;
vector<vector<char>> field;
vector<string> programs(27); //procedure A-Z, and last is main
int main(){
    cin>>r>>c>>d>>e;
    field.resize(r,vector<char>(c));
    for(int i=0;i<r;++i)
        for(int j=0; j < c; ++j)
            cin >> field[i][j];
    for(int i=0;i<d;++i){
        string prog;
        cin>>prog;
        char prog_n=prog.front();
        programs[prog_n-'A']=prog.substr(2);
    }

}