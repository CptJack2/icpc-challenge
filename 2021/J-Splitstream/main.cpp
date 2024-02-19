#include "bits/stdc++.h"

using namespace std;

struct Node{
    char type;
    int input1;
    int input2;
    int output1;
    int output2;
    Node* inode1;
    Node* inode2;
    Node* onode1;
    Node* onode2;
};

int n,m,q;

int main(){
    cin>>m>>n>>q;
    //n个node最多有2*n个流
    vector<Node*> inputToNode(2*n+2);
    vector<Node*> outputToNode(2*n+2);
    vector<Node> nodes(n);
    for(int i=0;i<n;++i){
        char type;
        int x,y,z;
        cin>>type>>x>>y>>z;
        //读入node信息
        Node& node= nodes[i];
        node.type=type;
        if(type=='S'){
            inputToNode[x] =&node;
            outputToNode[y]=&node;
            outputToNode[z]=&node;
            node.input1=x;
            node.output1=y;
            node.output2=z;
        }else{
            inputToNode[x] =&node;
            inputToNode[y] =&node;
            outputToNode[z]=&node;
            node.input1=x;
            node.input2=y;
            node.output1=z;
        }
    }
    //将各个node连接成树
    for(int i=0;i<n;++i) {
        Node& node= nodes[i];
        node.inode1=outputToNode[node.input1];
        node.inode2=outputToNode[node.input2];
        node.onode1=inputToNode[node.output1];
        node.onode2=inputToNode[node.output2];
    }
    //通过一次dfs计算每个stream的size
    vector<int> streamSize(2*n+2);
    streamSize[1]=m;
    function<void(Node*)> dfsCalcStreamSize=[&](Node* p){
        if(!p)return;
        if(p->type=='S'){
            streamSize[p->output1]=streamSize[p->input1]/2+streamSize[p->input1]%2;
            streamSize[p->output2]=streamSize[p->input1]/2;
        }else{
            streamSize[p->output1]=streamSize[p->input1]+streamSize[p->input2];
        }
        dfsCalcStreamSize(p->onode1);
        dfsCalcStreamSize(p->onode2);
    };
    Node* root=inputToNode[1];
    dfsCalcStreamSize(root);
    //读入查询, 通过在树上回溯,计算出原来的编号
    for(int i=0;i<q;++i){
        int x,k;
        cin>>x>>k;
        if(k>streamSize[x]){
            cout<<"none"<<endl;
            continue;
        }
        //沿着树回溯
        auto node=outputToNode[x];
        while(node){
            if(node->type=='S'){
                //如果ostream x是node的o2, 则k是对应的istream 1的第2*k个元素；如果是o1，则是第2*k-1个
                k=2*k-(x==node->output1);
                x=node->input1;
                node=node->inode1;
            }else{
                int sz=min(streamSize[node->input1],streamSize[node->input2]);
                if(k<=2*sz){
                    //对于M node，ostream x的第k个，对应于i1的第k/2+1（k为奇数），或i2的第k/2个（k为偶数）
                    x=k%2==0?node->input2:node->input1;
                    node=k%2==0?node->inode2:node->inode1;
                    k=(k+1)/2;
                }else{
                    //
                    x=streamSize[node->input1]>streamSize[node->input2]?node->input1:node->input2;
                    node=streamSize[node->input1]>streamSize[node->input2]?node->inode1:node->inode2;
                    k=k-sz;
                }

            }
        }
        cout<<k<<endl;
    }
}