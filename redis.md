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
    docker inspect --format='{{.NetworkSettings.IPAddress}}' $id
done

#创建redis cluster命令行
redis-cli --cluster create 172.17.0.7:6379 172.17.0.6:6379 172.17.0.5:6379 172.17.0.4:6379 172.17.0.3:6379 172.17.0.2:6379 --cluster-replicas 1

#redis-cli通过集群模式连接,-h跟其中一个集群ip即可
redis-cli -c -h 172.17.0.7

在任意一台机器上get/set，会先把cli redirect到处理这个key的hash slot的机器，然后再进行操作。这时如果通过redis-cli普通模式连上处理那个slot的ip，也是能获取到数据的。
如果在cli中select（更改数据库）会报(error) ERR SELECT is not allowed in cluster mode（无论redis-cli启动是否加-c)

#查看cluster node
直接在redis-cli里输入命令cluster node

This second port(16379) is used for the cluster bus, which is a node-to-node communication channel using a binary protocol. The cluster bus is used by nodes for failure detection, configuration update, failover authorization, and so forth.

#https://redis.io/docs/manual/scaling/#redis-cluster-master-replica-model
如果master没有slave,master挂了,它负责的slot range将不能提供服务

#对node的slot进行reshard
redis-cli --cluster reshard 172.17.0.7:6379 ,然后根据提示操作
或者使用
redis-cli --cluster reshard <host>:<port> --cluster-from <node-id> --cluster-to <node-id> --cluster-slots <number of slots> --cluster-yes

















