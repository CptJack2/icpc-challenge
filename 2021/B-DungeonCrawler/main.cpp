#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
#include<cassert>
using namespace std;

int main() {
    int N, Q, U, V, W, S, K, T;
    while (cin >> N >> Q) {
        vector<vector<pair<int,int>>> c(N);//dest length
        int64_t tot = 0;
        for (int i = 0; i < N-1; i++) {
            cin >> U >> V >> W;
            U--; V--;
            c[U].push_back({V, W});
            c[V].push_back({U, W});
            tot += W;
        }

        vector<int> depth(N);//每个节点在树中的高度，0号根节点高度为0
        vector<vector<pair<int64_t,int>>> longest(N);//length dest longest[x]={ {y1,d1}, {y2,d2}, ...}, 从x经yi节点能达到的最长路径（终点一定是一个叶子节点）
        function<int64_t(int,int,int)> doLongest = [&](int x, int prev, int dp) {
            depth[x] = dp;
            int64_t ret = 0;
            for (auto [y, d] : c[x]) {
                longest[x].push_back({y == prev ? -1 : d+doLongest(y, x, dp+1), y});
                ret = max(ret, longest[x].back().first);
            }
            return ret;
        };
        doLongest(0, -1, 0);
        function<int64_t(int,int,int)> getLongest = [&](int x, int ex1, int ex2) -> int64_t { //选出在x节点，经过除ex1和ex2以外的节点，能达到的最长路径
            for (auto [l, y] : longest[x]) {
                if (y != ex1 && y != ex2) return l;
            }
            return 0;
        };
        function<void(int,int,int64_t)> doParLongest = [&](int x, int prev, int64_t parLongest) {//前面getLongest对于经过父节点离开的最长路径留空为-1,现在把这个最长路径补齐
            int a=1;
            for (auto& [l, _] : longest[x])
                if (l == -1)
                    l = parLongest;
            sort(longest[x].begin(), longest[x].end(), greater<pair<int64_t,int>>());//距离从大到小排序
            for (auto [y, d] : c[x]) if (y != prev)
                doParLongest(y, x, d + getLongest(x, y, -1));
        };
        doParLongest(0, -1, 0);

        vector<vector<int>> skipNd(N);  // skip-paths going up the tree of length 2^n 从这个点一直往根走，距离为2^n次方的点，也就是skipNd[x][b]为x的第2^b号父节点
        vector<vector<int>> skipPrev(N);  // first node on the skip-path going down //从这个点一直往根走，距离为2^n-1次方的点。skipNd[x][b]是skipPrev[x][b]的父节点
        vector<vector<int64_t>> skipSUp(N);  // max path-to-endpoint, starting at bottom, maybe entering interior subtree
        vector<vector<int64_t>> skipSDn(N);  // same, but starting at top instead
        vector<vector<int64_t>> skipKUp(N);  // skipSUp, but costs along the skip path are subtracted, not added
        vector<vector<int64_t>> skipKDn(N);  // same, but starting at top instead
        vector<vector<int64_t>> skipDist(N);  // total length of skip //到2^n号父节点的距离
        function<void(int,int,int64_t)> doSkip = [&](int x, int prev, int64_t d) {
            int al=1, dep=depth[x];
            while((dep & 1) == 0){
                ++al;
                dep>>=1;
            }
            {
                skipNd[x].resize(al);
                skipPrev[x].resize(al);
                skipDist[x].resize(al);
                skipSUp[x].resize(al);
                skipSDn[x].resize(al);
                skipKUp[x].resize(al);
                skipKDn[x].resize(al);

                skipNd[x][0] = prev;
                skipPrev[x][0] = x;
                skipDist[x][0] = d;
                skipSUp[x][0] = d;
                skipSDn[x][0] = d;
                skipKUp[x][0] = 0;
                skipKDn[x][0] = 0;
            }
            for (int b = 1; b<al; b++) {//(1<<b)-1 = b个1
                int y = skipNd[x][b-1];//x的第2^(b-1)号父节点
                skipNd[x][b]=skipNd[y][b-1];//x的2^(b-1)号父节点的2^(b-1)号父节点，是x的2^b号父节点
                skipPrev[x][b]=skipPrev[y][b-1];//skipNd[x][b]的父节点也是同上计算方法
                skipDist[x][b]=skipDist[x][b-1] + skipDist[y][b-1];//到x的2^b号父节点的距离，先从x移动到2^(b-1)父节点（y），再从y移动到它的2^(b-1)父节点
//                if(skipPrev[x][b-1]!=skipNd[y][0])
//                    fprintf(stderr,"y:%d skipPrev[x][b-1]:%d skipNd[y][0]:%d\n",y,skipPrev[x][b-1], skipNd[y][0]);
                int64_t ymx = getLongest(y, skipPrev[x][b-1], skipNd[y][0]);//不算和x距离为2^(b-1)-1的点（y的一个子节点）和y的父节点
                /*设x的2^b号父节点skipNd[x][b]为z，skipPrev[x][b-1]为a，skipNd[y][0]为b
                 * 可知b为y的父节点，y为a的父节点，且从x出发到y必会经过a
                 * 可以证明：skipSUp[x][b]是从x出发，在以z为根节点的子树中，最长的一条路径
                 *skipSUp[x][b-1]是从x出发，在以y为根节点的子树中，最长的一条路径
                 * skipDist[x][b-1]是x到y的距离，
                 * ymx是从y出发，不经过a和b的，能到达的最长的路径*/
                skipSUp[x][b]=max(skipSUp[x][b-1],  skipDist[x][b-1] + max(ymx, skipSUp[y][b-1]));
                skipSDn[x][b]=max(skipSDn[y][b-1],  skipDist[y][b-1] + max(ymx, skipSDn[x][b-1]));
                skipKUp[x][b]=max(skipKUp[x][b-1], -skipDist[x][b-1] + max(ymx, skipKUp[y][b-1]));
                skipKDn[x][b]=max(skipKDn[y][b-1], -skipDist[y][b-1] + max(ymx, skipKDn[x][b-1]));
            }
            for (int i = 0; i < c[x].size(); i++) if (c[x][i].first != prev) doSkip(c[x][i].first, x, c[x][i].second);
        };
        for (int i = 0; i < c[0].size(); i++) doSkip(c[0][i].first, 0, c[0][i].second);

        auto anc = [&](int x, int y) { //ancestor, 找从x出发到y，经过的x的祖先
            vector<pair<int,int>> ret;
            while (depth[y] > depth[x]) {
                for (int b = skipNd[y].size()-1; b >= 0; b--) if (depth[y]-(1<<b) >= depth[x]) {
                        y = skipNd[y][b];
                        break;
                    }
            }
            while (depth[x] > depth[y]) {
                for (int b = skipNd[x].size()-1; b >= 0; b--) if (depth[x]-(1<<b) >= depth[y]) {
                        ret.push_back({x, b});
                        x = skipNd[x][b];
                        break;
                    }
            }
            while (x != y) { // 找公共祖先
                for (int b = skipNd[x].size()-1; b >= 0; b--) if (b == 0 || skipNd[x][b] != skipNd[y][b]) {
                        ret.push_back({x, b});
                        x = skipNd[x][b]; y = skipNd[y][b];
                        break;
                    }
            }
            ret.push_back({x, -1});
            return ret;
        };

        for (int q = 0; q < Q; q++) {
            cin >> S >> K >> T;
            S--; K--; T--;

            auto sk = anc(S, K), st = anc(S, T), ks = anc(K, S), kt = anc(K, T);
            auto path1 = (depth[sk.back().first] > depth[st.back().first] ? sk : st);
            auto path2 = anc(path1.back().first, K);//anc(S,K) - anc(S,T)
            auto path4 = (depth[ks.back().first] > depth[kt.back().first] ? ks : kt);//anc(K,S) - anc(K,T)
            auto path3 = anc(path4.back().first, S);
            // 如果path1/path4是sk/ks,back==T意味着T是S和K的公共祖先,则不可能先经过K然后才到T; 如果path1/path4是st/kt,back==T意味着T是S/K的祖先,且层高比SK的公共祖先高,则从S出发必然先要经过T才能到K
            if (path1.back().first == T || path4.back().first == T) { cout << "impossible" << endl; continue; }

            int x = S, prev = -1;
            int64_t base = 0, ret = 0;
            //先到达anc(S,K)和anc(S,T)的层高较大者
            for (int i = 0; i+1 < path1.size(); i++) {  // path1 rises from S, not overlapping path T->K
                auto [y, b] = path1[i];
                ret = max(ret, base + getLongest(x, prev, skipNd[y][0]));
                ret = max(ret, base + skipSUp[y][b]);
                base += skipDist[y][b];
                prev = skipPrev[y][b];
                x = skipNd[y][b];
            }
            for (int i = 0; i+1 < path2.size(); i++) {  // path2 rises from path1, overlapping path T->K
                auto [y, b] = path2[i];
                ret = max(ret, base + getLongest(x, prev, skipNd[y][0]));
                ret = max(ret, base + skipKUp[y][b]);
                base -= skipDist[y][b];
                prev = skipPrev[y][b];
                x = skipNd[y][b];
            }
            for (int i = path3.size()-2; i >= 0; i--) {  // path3 descends from path2, not overlapping path T->K
                auto [y, b] = path3[i];
                ret = max(ret, base + getLongest(x, prev, skipPrev[y][b]));
                ret = max(ret, base + skipSDn[y][b]);
                base += skipDist[y][b];
                prev = skipNd[y][0];
                x = y;
            }
            for (int i = path4.size()-2; i >= 0; i--) {  // path4 descends from path3, overlapping path T->K
                auto [y, b] = path4[i];
                ret = max(ret, base + getLongest(x, prev, skipPrev[y][b]));
                ret = max(ret, base + skipKDn[y][b]);
                base -= skipDist[y][b];
                prev = skipNd[y][0];
                x = y;
            }
            ret = max(ret, base + getLongest(x, prev, -1));
            cout << 2*tot - ret << endl;
        }
    }
}