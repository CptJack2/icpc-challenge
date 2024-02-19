#include "bits/stdc++.h"

using namespace std;

struct Node{
    char type;
    int input1;
    int input2;
    int output1;
    int output2;
    int inode1;
    int inode2;
    int onode1;
    int onode2;
};

int n,m,q;

int main(){
    cin>>m>>n>>q;
    //n个node最多有2*n个流
    vector<int> inputToNode(2*n+2);
    vector<int> outputToNode(2*n+2);
    vector<Node> nodes(n+1);
    for(int i=1;i<=n;++i){
        char type;
        int x,y,z;
        cin>>type>>x>>y>>z;
        //读入node信息
        Node& node= nodes[i];
        node.type=type;
        if(type=='S'){
            inputToNode[x] =i;
            outputToNode[y]=i;
            outputToNode[z]=i;
            node.input1=x;
            node.output1=y;
            node.output2=z;
        }else{
            inputToNode[x] =i;
            inputToNode[y] =i;
            outputToNode[z]=i;
            node.input1=x;
            node.input2=y;
            node.output1=z;
        }
    }
    //将各个node连接成树
    for(int i=1;i<=n;++i) {
        Node& node= nodes[i];
        node.inode1=outputToNode[node.input1];
        node.inode2=outputToNode[node.input2];
        node.onode1=inputToNode[node.output1];
        node.onode2=inputToNode[node.output2];
    }
    //通过一次dfs计算每个stream的size
    vector<int> streamSize(2*n+2);
    streamSize[1]=m;
    function<void(int)> dfsCalcStreamSize=[&](int nodeIndex){
        if(!nodeIndex)return;
        auto p=&nodes[nodeIndex];
        if(p->type=='S'){
            streamSize[p->output1]=streamSize[p->input1]/2+streamSize[p->input1]%2;
            streamSize[p->output2]=streamSize[p->input1]/2;
        }else{
            streamSize[p->output1]=streamSize[p->input1]+streamSize[p->input2];
        }
        dfsCalcStreamSize(p->onode1);
        dfsCalcStreamSize(p->onode2);
    };
//    Node* root=inputToNode[1];
    dfsCalcStreamSize(inputToNode[1]);
    //读入查询, 通过在树上回溯,计算出原来的编号
    for(int i=0;i<q;++i){
        int x,k;
        cin>>x>>k;
        if(k>streamSize[x]){
            cout<<"none"<<endl;
            continue;
        }
        //沿着树回溯
        auto nodeIndex=outputToNode[x];
        while(nodeIndex){
            if(nodes[nodeIndex].type=='S'){
                //如果ostream x是node的o2, 则k是对应的istream 1的第2*k个元素；如果是o1，则是第2*k-1个
                k=2*k-(x==nodes[nodeIndex].output1);
                x=nodes[nodeIndex].input1;
                nodeIndex=nodes[nodeIndex].inode1;
            }else{
                int sz=min(streamSize[nodes[nodeIndex].input1],streamSize[nodes[nodeIndex].input2]);
                if(k<=2*sz){
                    //对于M node，ostream x的第k个，对应于i1的第k/2+1（k为奇数），或i2的第k/2个（k为偶数）
                    x=k%2==0?nodes[nodeIndex].input2:nodes[nodeIndex].input1;
                    nodeIndex=k%2==0?nodes[nodeIndex].inode2:nodes[nodeIndex].inode1;
                    k=(k+1)/2;
                }else{
                    //
                    x=streamSize[nodes[nodeIndex].input1]>streamSize[nodes[nodeIndex].input2]?nodes[nodeIndex].input1:nodes[nodeIndex].input2;
                    nodeIndex=streamSize[nodes[nodeIndex].input1]>streamSize[nodes[nodeIndex].input2]?nodes[nodeIndex].inode1:nodes[nodeIndex].inode2;
                    k=k-sz;
                }

            }
        }
        cout<<k<<endl;
    }
}