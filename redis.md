#redis笔记

docker下载redis镜像
docker pull redis:7.0

#redis cluster支持的nodes
数千个,跟kubernetes是类似的,目前的分布式系统应该都是支持到这个数目范围的nodes

#准备好各自的redis.conf,从docker启动redis-server
docker run -d -v $(pwd)/redis.conf:/etc/redis/redis.conf redis:7.0 redis-server /etc/redis/redis.conf

#查看新建的redis容器ip
docker ps -a|grep redis
docker inspect $id |grep "IPAddress"
docker inspect --format='{{.NetworkSettings.IPAddress}}' $id

#创建redis cluster的redis.conf
```
cluster-enabled yes
cluster-config-file nodes.conf
cluster-node-timeout 5000
appendonly yes
```

for id in $(docker ps -a|grep redis|awk '{print $1}');do
    echo $id
    docker inspect --format='{{.NetworkSettings.IPAddress}}' $id
done

for i in {1..6};do docker run -d -v $(pwd)/redis.conf:/etc/redis/redis.conf redis:7.0 redis-server /etc/redis/redis.conf;done

#创建redis cluster命令行
redis-cli --cluster create 172.17.0.7:6379 172.17.0.6:6379 172.17.0.5:6379 172.17.0.4:6379 172.17.0.3:6379 172.17.0.2:6379 --cluster-replicas 1



#redis-cli通过集群模式连接,-h跟其中一个集群ip即可
redis-cli -c -h 172.17.0.7

在任意一台机器上get/set，会先把cli redirect到处理这个key的hash slot的机器，然后再进行操作。这时如果通过redis-cli普通模式连上处理那个slot的ip，也是能获取到数据的。
如果在cli中select（更改数据库）会报(error) ERR SELECT is not allowed in cluster mode（无论redis-cli启动是否加-c)

#查看cluster node
直接在redis-cli里输入命令cluster node
输出列含义
Node ID
ip:port
flags: master, replica, myself, fail, ...
if it is a replica, the Node ID of the master
Time of the last pending PING still waiting for a reply.
Time of the last PONG received.
Configuration epoch for this node (see the Cluster specification).
Status of the link to this node.
Slots served...

或者
redis-cli -c -h 172.17.0.3 cluster nodes

This second port(16379) is used for the cluster bus, which is a node-to-node communication channel using a binary protocol. The cluster bus is used by nodes for failure detection, configuration update, failover authorization, and so forth.

#https://redis.io/docs/manual/scaling/#redis-cluster-master-replica-model
如果master没有slave,master挂了,它负责的slot range将不能提供服务

#对node的slot进行reshard
redis-cli --cluster reshard 172.17.0.7:6379 ,然后根据提示操作
或者使用
redis-cli --cluster reshard <host>:<port> --cluster-from <node-id> --cluster-to <node-id> --cluster-slots <number of slots> --cluster-yes

#手动failover,强制让一个slave推翻它的master
redis-cli -c -h <slave-ip>
cluster failover
查看cluster
cluster nodes
发现slave成了master,原来的master重启了,node id也变了,成了slave
这个命令的详细文档https://redis.io/commands/cluster-failover/
有force和takeover选项,真像种植园奴隶起义

#往集群添加节点
作为master
redis-cli --cluster add-node 172.17.0.8:6379 172.17.0.3:6379
作为slave
redis-cli --cluster add-node 127.0.0.1:7006 127.0.0.1:7000 --cluster-slave --cluster-master-id 3c3a0c74aae0b56170ccb03a76b60cfe7dc1912e

#删除
redis-cli --cluster del-node 172.17.0.3:6379 2f8dc4905078898d194323b9faa824061a90bdf9
只适用于要删除的node没有挂
如果待删除node挂了，需要每个node执行forget
for id in $(docker ps -a|grep redis|awk '{print $1}');do
    ip=$(docker inspect --format='{{.NetworkSettings.IPAddress}}' $id)
    redis-cli -c -h $ip cluster forget <node id>
done

这里redis cluster的开发就不太好了,各节点持有的集群元数据不一致,在一个节点上恢复了集群状态,在gossip的时候又被打乱了.

#node加入cluster原理
https://redis.io/commands/cluster-meet/
```
So in order for a given node to accept another one into the list of nodes composing a Redis Cluster, there are only two ways:

1.The system administrator sends a CLUSTER MEET command to force a node to meet another one.
2.An already known node sends a list of nodes in the gossip section that we are not aware of. If the receiving node trusts the sending node as a known node, it will process the gossip section and send a handshake to the nodes that are still not known.
```

#从slave中读数据
redis-cli -c -h <slave-ip>
readonly 表明操作只读
get <key> 读取数据
readwrite 取消只读

#直接干掉一个slave
cluster nodes命令，很久没有发现slave掉线

#直接干掉一个master

#cluster因为master挂了,没有slave顶上,进入fail状态
cluster info查看集群状态
处理方式https://blog.51cto.com/u_15239532/2835959
cluster nodes查看挂掉的node

cluster forget <node-id>删除node

需要对每个节点都进行forget node，否则node配置会通过gossip再加回来
for id in $(docker ps -a|grep redis|awk '{print $1}');do
    ip=$(docker inspect --format='{{.NetworkSettings.IPAddress}}' $id)
    echo $ip
    ./redis-cli -c -h $ip cluster forget 1302b48cc038cfa954d7ee3acb77d53880099bd6
    echo
done

注意:需要在每个master里面执行,不然后面这些forget的信息又会回来;这时候如果登录上slave,会发现删掉的node信息还在
https://redis.io/docs/reference/cluster-spec/#cluster-node-attributes

cluster addslotsrange <lower bound> <upper bound>添加回slots  

#cluster topology
https://redis.io/docs/reference/cluster-spec/#cluster-topology
redis cluster中每个节点都会通过cluster bus的端口于其他任何一个节点相连
集群信息交换 https://redis.io/docs/reference/cluster-spec/#heartbeat-and-gossip-messages

#failover机制, slave如何晋升master
https://redis.io/docs/reference/cluster-spec/#failure-detection
与raft的leader election基本一致,多了一些工程化的参数设置

#redis-cli --cluster <cmd>
https://redis.io/commands/cluster-nodes/

#redis中的epoch
https://redis.io/docs/reference/cluster-spec/#cluster-current-epoch
其实就是分布式系统里的lamport logical timestamp






