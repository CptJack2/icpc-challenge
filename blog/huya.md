#并发安全的LRU Cache
参考并发的hashmap的实现

orcaman/concurrent-map
https://github.com/orcaman/concurrent-map/blob/master/concurrent_map.go

Java ConcurrentHashMap How It Works Internally.
https://youtu.be/Ejw0GQLI9z8

主要思想是对hashmap管理的key先进行一次分区(sharding),然后用读写锁(结合下面的读写锁优化, 如果V类型是简单类型, 可以简化为只需要写时加锁)管理每个区的读写操作,这样每个写操作只会锁住自己区的写锁,提升了并发度

回到并发安全的lru cache,由于非并发安全的实现用了双向链表存储数据, 用hash map加速访问, 首先将hash map替换成并发安全的concurrent hash map, 然后在将链表节点删除或移动到列头的时侯使用读写锁保护即可.

网络上的实现代码, 都是上述的思路:
Detailed Explanation with Threadsafe LRU Cache in Java
https://leetcode.com/problems/lru-cache/discuss/724784/detailed-explanation-with-threadsafe-lru-cache-in-java

tstarling/thread-safe-lru
https://github.com/tstarling/thread-safe-lru/blob/master/thread-safe-lru/lru-cache.h

#读写锁优化
如果读数据的操作是原子的(即只读一个数据,而非结构体之类, 或多个有内在逻辑关系的数据),读线程可以不加读锁, 只有写线程要更新数据的时候才加写锁.
读写锁应用场景是修改在有并发读的情况下, 修改多个互相有逻辑关联的数据时需要加上读锁, 以免修改到一半的不一致的数据被读走.


#linux svr: socket bind listen accept recv
cli: socket connect send	

https://blog.csdn.net/tennysonsky/article/details/45621341

#线程池中的线程
用linux sig stop 挂起， sig cont继续
kill -STOP $pid 
kill -CONT $pid