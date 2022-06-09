#分布式系统综述（MIT 6.824 分布式系统课程）
![a](dont_know.gif)
在互联网公司担任了两年的后端开发工作，虽然日常工作中做的都是分布式系统相关的开发工作，但发现自己对分布式系统的理解还是不够深入、系统，所以在研究redis代码的情况下，结合MIT 6.824分布式系统的课程，对分布式系统做一次深入的学习。

#为什么需要分布式系统
单机性能有限，使用更快更好的计算机的成本，远大于使用多台性能一般的计算机提供服务的成本

#分布式系统难点
节点上的进程崩溃、节点崩溃、节点网络断开、集群产生分区（partition）
网络不可靠，总会出错，数据包可能丢失、延迟到达
系统可能出错 -> 需要容错(fault tolerance) -> 需要复制数据,存储副本(replication) -> 副本数据不一致(inconsistent)

#redis cluster
redis cluster简介 https://redis.io/docs/manual/scaling/
redis cluster详细设计 https://redis.io/docs/reference/cluster-spec/#write-safety

redis cluster如何处理脑裂问题？
redis对每个master node都有timeout，如果sentinel在timeout过后仍然联系不上master，就会对它的slave进行promote，同时，master如果发现在timeout的时间后仍联系不上majority，就会停止对客户端的写操作进行服务。

#脑裂 split brain
多个master/primary，同时为client服务的情况

#Google File System要点
MIT DS Lec3 GFS https://www.bilibili.com/video/BV1CU4y1P7PE?p=3
GFS Paper http://nil.csail.mit.edu/6.824/2017/papers/gfs.pdf
单master，多chunk server
master保存文件目录结构、文件由哪些块组成、chunk server持有块信息
chunk server持有块信息由master启动时向各chunk server查询 

×为什么不保存文件版本号而是块版本号？
gfs论文中，consistent指内容一致（和其他分布式指的一致不一样），defined指内容和内容的顺序都一致（因为有append追加操作，并发append顺序不一定一致）

#raftdis
论文 https://www.usenix.org/system/files/conference/atc14/atc14-paper-ongaro.pdf