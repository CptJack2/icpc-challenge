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

#wait group
http://t.zoukankan.com/failymao-p-15496782.html
结合sync/waitgroup.go看

#cond为什么要带一个for loop
You need to check the condition again once you get out of conditional wait. If you don’t then you run the risk of some other thread changing condition in the time signal came and you got out of condition wait. This is true for all condition waits in all languages including pthreads in c. Checking and waiting for condition with a “if” is a bug.

# 空 struct{} 的用途
用map模拟一个set，那么就要把值置为struct{}，struct{}本身不占任何空间，可以避免任何多余的内存分配。
type Set map[string]struct{}
set := make(Set)

#2 个 interface 可以比较吗 ？
Go 语言中，interface 的内部实现包含了 2 个字段，类型 T 和 值 V，interface 可以使用 == 或 != 比较。2 个 interface 相等有以下 2 种情况

两个 interface 均等于 nil（此时 V 和 T 都处于 unset 状态）
类型 T 相同，且对应的值 V 相等。





