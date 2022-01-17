#随便写写icpc world final -- 2019 F Directing Rainfall 题解

想随便找点事情做，就找了点icpc world final的题来写一写，由于之前完全没有acm的经验，又刚好挑了19年最难的一题之一来写，前前后后一共写了三个星期才AC了，这篇题解以一个非竞赛者，而是一个软件工程师的视角来探讨下。

##问题算法解析

问题算法可以参考官方题解视频：https://www.youtube.com/watch?v=GISocxWB8Mk ，
或者http://www.csc.kth.se/~austrin/icpc/finals2019solutions.pdf ，
测试的数据可以在https://icpc.global/worldfinals/problems找到。

问题的解答可以分成三个大步骤：
1.如果tarp a的水在不打洞的情况下，能滴到另一个tarp b上，那么称tarp a依赖于tarp b。从x轴左到右扫描，将各个tarp的依赖关系找出来，形成一张有向无环图。
2.将上一步中的有向无环图进行拓扑排序，得到一个tarp序列。
3.使用动态规划的方法求解问题。做一个无限长的dp数组，初始化将[L,R]的区间置为0，其他位置置为∞，然后按照上面的拓扑排序序列，从后往前按照一定规则更新dp数组，更新完最后一个tarp后，从[L,R]区间内找最小值，就是答案。

###依赖获取

先注意到一个很重要的题设，题目中所有的tarp的x坐标和L、R都是不同的，一开始我没注意到这个前提，整个问题的复杂度一下打了很多。
有了这个前提后，
