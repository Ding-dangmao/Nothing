# 线程池

## 大体架构

​	为避免重复创建销毁线程造成的资源时间消耗

- 线程组
- 总任务队列
- 线程任务队列
- 任务分配线程
- 任务处理线程
- 任务接收线程

## 筛选机制

​	采用random库的类

~~~C++
    std::random_device rd;
    std::mt19937 mt{rd()};
    std::uniform_int_distribution<>::param_type p{lo,hi};
    std::uniform_int_distribution uid{p};
    return uid(mt); 
~~~

### 特殊

对于多次随机都是状态运行中的情形,考虑直接对持有任务最少者进行添加任务操作,若任务量相等,按自定的thread_idx操作
