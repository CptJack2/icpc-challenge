#redis cluster学习笔记
最近刚好在学习redis和分布式系统的课程，redis作为一个优秀的缓存系统和KV数据库，官方提供的redis cluster集群方案更是将它的性能推高了一个台阶。这篇笔记主要介绍对我对redis cluster的学习和理解，同时结合一些分布式系统的理论来分析一下cluster方案的设计。
redis cluster是redis官方提供的集群方案，自3.0版本起可用，目的是解决单机redis性能不能满足大规模应用的问题。redis cluster自带了高可用保证方案，cluster与redis sentinel有较大区别。
sentinel从2.8版本起开始提供。sentinel只是监控了redis master，并在master失效后提升slave，只是做到了高可用，并没有集群的水平扩展功能。cluster中的节点自带了监控功能，所以sentinel已经没有必要再使用了。

#redis cluster架构简介
redis首先将整个数据空间进行分块，每个数据块由一部分的节点负责读写，就像西周时代的分封制，将整个国家（全部数据）分封给一个个诸侯（一个master节点和它的slave），由诸侯负责那块地盘的事宜。为了分块的方便，redis创建了hash slot的概念，将整个数据库的hash值对应到16384（2^14）个slot，每个master管理一部分的slot。当需要获取某个key时，先对key进行CRC16求hash，然后对hash mod 16384，即可求到对应的slot，再将客户重定向到负责整个slot的master上，从那里读取或修改数据。
由于采用了这种分封制，为了方便和易于实现，在集群模式下，redis只支持一个数据库，即不再支持选择数据库的select命令。

将单机的系统提升到分布式，主要需要解决高可用和一致性的两个问题，来看看redis cluster是如何解决这两个问题的。
对于高可用，传统的方案都是进行replication（数据复制），redis当然也不可能逃离这个套路，cluster里面的节点有两种身份：master和slave，它们共享同样的数据，master作为主数据，负责数据的写操作，在客户写入后再同步给slave；slave在客户指定只读模式后，也可以从中读出数据，进而提升集群的读性能。前面提到redis对数据是实行分封制的，所以一个cluster里面就会有若干个master，每个master又有0-N个slave。

##redis cluster的一致性保证
对于一致性，redis的设计取舍是选择AP的，即放弃强一致性，优先支持可用性。（根据分布式系统的CAP理论，一致性、可用性、分区容错性不能同时具备；由于分布式系统必须有分区容错，所以都是在一致性和可用性间做取舍）redis的定位是高性能缓存，并不是像etcd这样强一致的KV存储，所以作出这种设计也是可以理解的。
当一个master宕机后，由于cluster中的每个节点都是两两相连的，并且持续通过心跳监视，一段时间后master宕机就会被发现，这时候master下属的slave会发起选举，当赢得选举后，这个slave会成为它的前master负责分区的新master，继续承担master的责任（这个选举过程跟raft的leader election几乎是一模一样的，只是多了一些工程化的调整）。当然如果宕机的是slave，那就无所谓了，挂了就挂了，重新加一台机器复制master的数据即可；如果master下没有slave，那就比较麻烦，这个分区就不可用了，而这会导致整个集群都不可用（是的，连正常的分区的数据都读不了，具体参考https://redis.io/docs/manual/scaling/#redis-cluster-master-replica-model），所以还是推荐每个分区至少一个slave。
可以看到，采用了这种设计后，数据不能保证强一致性，发生数据丢失主要是以下情况：数据写入到master，但还没同步到slave，master就挂了；或者master跟集群的majority失联了，在它还没意识到自己成了minority之前接受了客户的写入，这部分写入也会丢（在它意识到自己成了minority后它会停止接受写请求），因为如果majority提升了它的slave，新的master明显是没有这些数据的。

由于选举机制和去中心化的设计，redis cluster中的所有节点都是通过cluster bus（TCP 16379）两两相连的，这种设计注定了cluster没有办法无限地水平扩张下去（因为连接数量是O(N!)的），整个集群支持的节点数大约是数千个（跟kubernetes是类似的,目前的分布式系统应该都是支持到这个数目范围的nodes），对于目前的分布式应用也是足够的，因为数千个节点上的内存可以达到百TB级了。

#网络分区和脑裂对redis cluster的影响
脑裂一直是分布式系统中比较棘手的一个问题，意思是当集群中发生了网络分区的情况下（一个集群中的两部分网络不能连通，被分割成了分区），一个分区中有旧的master，新分区中另外晋升了master，两个分区都接受了数据写入，当网络分区修复后，集群中就有了两个master，并且它们的数据不一致，真麻烦。
redis为了应对这个问题，设计了node-timeout这个配置参数。前面提到redis中每两个节点都是互连的，并且会互发心跳，当超过node-timeout没有收到对方的心跳是，节点就会认为对方挂了；当slave认为它的master挂了之后，会向其他节点发起自己选举为master的投票，当收到了majority的赞成票（超过半数）后，就成为了新的master（这部分其实和raft也是一样的，可以看到，经典之所以成为经典，是因为它简洁且能应对一切情况）；老的master如果没有宕机，当它发现自己在node-timeout的时间后，都没有收到majority的心跳，它就意识到自己已经成了minority，从而停止对外服务。
可以看到，在这样的机制下，只可能在node-timeout的短暂时间内存在两个master，也就应对了脑裂问题，前面提到可能丢数据的情况也是发生在这一短暂的时间窗口。node-timeout是redis cluster中一个至关重要的参数。



#redis cluster搭建过程
关于redis cluster的文字介绍大概就是这么多，接下来亲自动手搭建一个redis集群进行实验吧，只有动手才能巩固并更好地理解。

##参考资料
学习redis cluster的主要资料，主要看redis的这两篇官方文档
Scaling with Redis Cluster https://redis.io/docs/manual/scaling/
Redis cluster specification Detailed specification for Redis cluster: https://redis.io/docs/reference/cluster-spec/

##开始搭建
这里为了方便和更加真实，和官方文档的搭建不太一样，使用单机通过docker部署多个容器的方式，进行集群的搭建。

首先通过docker下载redis镜像，采用了7.0的版本
docker pull redis:7.0

##创建redis cluster使用的redis.conf
```
cluster-enabled yes
cluster-config-file nodes.conf
cluster-node-timeout 5000
appendonly yes
```

##用redis.conf从docker启动redis-server
docker run -d -v $(pwd)/redis.conf:/etc/redis/redis.conf redis:7.0 redis-server /etc/redis/redis.conf
这里通过文件映射把redis.conf映射到了容器内
直接批量创建6个实例
for i in {1..6};do docker run -d -v $(pwd)/redis.conf:/etc/redis/redis.conf redis:7.0 redis-server /etc/redis/redis.conf;done

##查看新建的redis容器ip
docker ps -a|grep redis
通过命令行直接获取容器IP
docker inspect --format='{{.NetworkSettings.IPAddress}}' $id
方便自己查看
docker inspect $id |grep "IPAddress"
也可以直接批量获取容器IP
for id in $(docker ps -a|grep redis|awk '{print $1}');do
    echo $id
    docker inspect --format='{{.NetworkSettings.IPAddress}}' $id
done

##创建redis cluster
redis-cli --cluster create 172.17.0.7:6379 172.17.0.6:6379 172.17.0.5:6379 172.17.0.4:6379 172.17.0.3:6379 172.17.0.2:6379 --cluster-replicas 1
6个实例，3个master，3个slave



##redis-cli通过集群模式连接到cluster，-h跟其中一个容器ip即可
redis-cli -c -h <container_ip>

在任意一台机器上get/set，会先把redis-cli重定向到处理这个key的机器，然后再进行操作。这时如果通过redis-cli普通模式连上处理那个slot的ip，也是能获取到数据的。
如果在cli中select（更改数据库）会报(error) ERR SELECT is not allowed in cluster mode（无论redis-cli启动是否加-c)

##查看集群中的节点信息
直接在redis-cli的交互模式里输入命令cluster node
或者在bash中
redis-cli -c -h 172.17.0.3 cluster nodes
IP后跟redis命令可以不进入交互模式

节点信息输出列含义
Node ID
ip:port
flags: master, replica, myself, fail, ...
if it is a replica, the Node ID of the master
Time of the last pending PING still waiting for a reply.
Time of the last PONG received.
Configuration epoch for this node (see the Cluster specification).
Status of the link to this node.
Slots served...
```
This second port(16379) is used for the cluster bus, which is a node-to-node communication channel using a binary protocol. The cluster bus is used by nodes for failure detection, configuration update, failover authorization, and so forth.
```

##对节点负责的hash slot进行reshard（hash slot移动到另一个master）
redis-cli --cluster reshard 172.17.0.7:6379 ,然后根据提示操作
或者使用
redis-cli --cluster reshard <host>:<port> --cluster-from <node-id> --cluster-to <node-id> --cluster-slots <number of slots> --cluster-yes

##手动failover,强制让一个slave推翻它的master
redis-cli -c -h <slave-ip>
cluster failover
查看cluster
cluster nodes
发现slave成了master,原来的master重启了,node id也变了,成了slave
这个命令的详细文档https://redis.io/commands/cluster-failover/
有force和takeover选项,真像种植园奴隶起义

##往集群添加节点
作为master
redis-cli --cluster add-node 172.17.0.8:6379 172.17.0.3:6379
作为slave
redis-cli --cluster add-node 127.0.0.1:7006 127.0.0.1:7000 --cluster-slave --cluster-master-id 3c3a0c74aae0b56170ccb03a76b60cfe7dc1912e

##从集群中删除一个节点
redis-cli --cluster del-node 172.17.0.3:6379 2f8dc4905078898d194323b9faa824061a90bdf9
只适用于要删除的node没有挂
如果待删除node挂了，需要每个node执行forget
for id in $(docker ps -a|grep redis|awk '{print $1}');do
    ip=$(docker inspect --format='{{.NetworkSettings.IPAddress}}' $id)
    redis-cli -c -h $ip cluster forget <node id>
done

note:这里redis cluster的开发就不太好了,各节点持有的集群元数据不一致,在一个节点上恢复了集群状态,在gossip的时候又被打乱了，甚至到了7.0的版本还是没有解决这个问题

##node加入cluster原理
https://redis.io/commands/cluster-meet/
```
So in order for a given node to accept another one into the list of nodes composing a Redis Cluster, there are only two ways:

1.The system administrator sends a CLUSTER MEET command to force a node to meet another one.
2.An already known node sends a list of nodes in the gossip section that we are not aware of. If the receiving node trusts the sending node as a known node, it will process the gossip section and send a handshake to the nodes that are still not known.
```

##从slave中读数据
redis-cli -c -h <slave-ip>
readonly 表明操作只读
get <key> 读取数据
readwrite 取消只读

##干掉任意一个节点，观察情况
docker rm -f <container_id>
可以到别的节点上查看cluster nodes，会发现那个节点状态变为fail了

##cluster因为master挂了,没有slave顶上,进入fail状态
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

note:上面删除节点的部分提过了，需要在每个master里面执行,不然后面这些forget的信息又会回来;这时候如果登录上slave,会发现删掉的node信息还在
https://redis.io/docs/reference/cluster-spec/##cluster-node-attributes

这时候，原来的master负责的slot还没有人负责，需要向集群中添加这些slot
cluster addslotsrange <lower bound> <upper bound>添加回slots
同样也需要每个节点都操作，因为这些集群元信息是每个节点各自持有的。

##cluster topology
https://redis.io/docs/reference/cluster-spec/##cluster-topology
redis cluster中每个节点都会通过cluster bus的端口于其他任何一个节点相连
集群信息交换 https://redis.io/docs/reference/cluster-spec/##heartbeat-and-gossip-messages

##failover机制, slave如何晋升master
https://redis.io/docs/reference/cluster-spec/##failure-detection
与raft的leader election基本一致,多了一些工程化的参数设置

##redis-cli --cluster <cmd>
https://redis.io/commands/cluster-nodes/

##redis中的epoch
https://redis.io/docs/reference/cluster-spec/##cluster-current-epoch
其实就是分布式系统里的lamport logical timestamp






