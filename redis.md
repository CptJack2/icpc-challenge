#redis笔记

docker下载redis镜像
docker pull redis:7.0

准备好各自的redis.conf,从docker启动redis-server
docker run -d -v $(pwd)/redis.conf:/etc/redis/redis.conf redis:7.0 redis-server /etc/redis/redis.conf

查看新建的redis容器ip
docker ps -a|grep redis
docker inspect $id |grep "IPAddress"
docker inspect --format='{{.NetworkSettings.IPAddress}}' $id

创建redis cluster的redis.conf
```
cluster-enabled yes
cluster-config-file nodes.conf
cluster-node-timeout 5000
appendonly yes
```

for id in $(docker ps -a|grep redis|awk '{print $1}');do
    docker inspect --format='{{.NetworkSettings.IPAddress}}' $id
done

创建redis cluster命令行
redis-cli --cluster create 172.17.0.7:6379 172.17.0.6:6379 172.17.0.5:6379 172.17.0.4:6379 172.17.0.3:6379 172.17.0.2:6379 --cluster-replicas 1



















