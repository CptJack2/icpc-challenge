#include<bits/stdc++.h>
using namespace std;
int n,m,cnt=1,res[1001000];
struct AC_Automaton{
	int ch[26],fail,sz;
	vector<int>v,ed;
}t[2001000];
char s[1001000];

void Ins(int id,int len){
	int x=1;
	for(int i=0;i<len;i++){
		if(!t[x].ch[s[i]-'A'])
			t[x].ch[s[i]-'A']=++cnt;
		x=t[x].ch[s[i]-'A'];
	}
	t[x].ed.push_back(id);
}

queue<int>q;
void build(){
	for(int i=0;i<26;i++){
		if(t[1].ch[i])
			t[t[1].ch[i]].fail=1,
			q.push(t[1].ch[i]),
			t[1].v.push_back(t[1].ch[i]);
		else
			t[1].ch[i]=1;
	}
	while(!q.empty()){
		int x=q.front();q.pop();
		for(int i=0;i<26;i++){
			if(t[x].ch[i])
				t[t[x].ch[i]].fail=t[t[x].fail].ch[i],
				q.push(t[x].ch[i]),
				//根据failure指针的反向建树
				t[t[t[x].fail].ch[i]].v.push_back(t[x].ch[i]);
			else
				t[x].ch[i]=t[t[x].fail].ch[i];
		}
	}
}

void dfs(int x){
	for(auto y:t[x].v)
		dfs(y),
		t[x].sz+=t[y].sz;
	for(auto i:t[x].ed)
		res[i]=t[x].sz;
}

int main(){
	scanf("%d%d",&n,&m);
	for(int i=2,x;i<=n+1;i++) {
		scanf("%s%d", s, &x);
		t[x + 1].ch[s[0] - 'A'] = ++cnt;
		t[cnt].sz = 1;
	}
	for(int i=1,len;i<=m;i++) {
		scanf("%s", s);
		len = strlen(s);
		reverse(s, s + len);
		Ins(i, len);
	}
	build();
	dfs(1);
//	for(int i=1;i<=cnt;i++)printf("%d<-%d %d\n",i,t[i].fail,t[i].sz);
	for(int i=1;i<=m;i++)printf("%d\n",res[i]);
	return 0;
}
