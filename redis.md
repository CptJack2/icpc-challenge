#redis笔记

docker下载redis镜像
docker pull redis:7.0

准备好各自的redis.conf,从docker启动redis-server
docker run -d -v $(pwd)/redis.conf:/etc/redis/redis.conf redis:7.0 redis-server /etc/redis/redis.conf

查看新建的redis容器ip
docker ps -a|grep redis
docker inspect $id |grep "IPAddress"