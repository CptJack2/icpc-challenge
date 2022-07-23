#Go语言的一些深入学习
一些基础的：https://zhuanlan.zhihu.com/p/471490292

#字符串拼接性能及原理
https://geektutu.com/post/hpg-string-concat.html

#GO内存管理
https://cloud.tencent.com/developer/article/1422392

#Channel原理
底层实现是一个环形队列，用mutex控制并发，并且有当前阻塞的写者和读者的协程队列。
https://blog.csdn.net/wys74230859/article/details/121844191

#channel阻塞
当缓冲区满或者无缓冲区时，写/读会一直阻塞到有读/写者把消息读出。

#Mutex原理
https://blog.csdn.net/baolingye/article/details/111357407#:~:text=%E6%AF%8F%E4%B8%AAMutex%E9%83%BD,tarving%E3%80%82
结合go的源码sync/mutex.go看,里面有大量注释。

#select行为
如果没有default，会阻塞直到其中一个子句能运行；如果有default且所有子句都不能执行，就会执行default里面的内容。
如果有多个子句都被触发，则随机选择一个执行，其他的不执行。
https://www.runoob.com/go/go-select-statement.html