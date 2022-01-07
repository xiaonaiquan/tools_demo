
### boost库的使用demo
1. timer_demo 计时器的demo
2. post_demo post任务的demo，执行一次

### C++11消息工厂

​	message_demo消息工厂的demo   :  g++ main.cpp -o main

### C++pimpl模式

### 利用策略和工厂设计模式消除过多的if-else语句

代码是java的伪代码，增加了C++的实现

### 增加了**[hyper_function.hpp](https://gist.github.com/lingol/ed9feab92da9e341487855084411df4e)**

hyper_function.hpp是腾讯大佬的优化版本，作用是类似于std :: function的util，具有接受继承功能的功能。main.cpp是使用案例，也是微信团队优化的案例（地址：https://mp.weixin.qq.com/s/-wgBhE11xEXDS7Hqgq3FjA），避免基类被模板化。

### C++实现snowflake算法

### 字符串分割

支持多种分隔符，利用不常见的关键字实现，使用之前去先测试自己的字符串能否分割正常

### 支持过期时间的Map

此代码在项目里使用无问题

### 增加对象池，提前申请内存放数据

该对象池支持多线程使用

### 增加注册对象

支持注册对象，对象函数等

### Linux守护进程

静默运行，单一实例，https://www.cnblogs.com/lvnux/p/12942512.html

### 对象的安全操作某个函数

使用lock_guard

### 区间数据的查找

list : [ [1,5],[7,10] ]  找到某个数是在哪个区间

###  对多参数和返回值的封装

目前是应用于封装于hiredis的

### 对struct赋值

利用初始化列表

Status和Slice

这两个是在leveldb中拿出来的，Status实现的很好用啊，实现对返回值的code的封装，Slice是实现了string的功能。

### lru_cache

对lru的实现

### 定时器实现

目前是利用堆实现的

### 函数耗时

timelog

### 消息队列

linux下使用系统消息队列-linux-sharedMsg

### Pipe

Windows下Pipe通信（半双工）- pipe （不完善，需要自己添加）

### Curl_Breakpoint

利用libcurl实现断点续传，检查服务器是否支持断电续传："curl -i --range 0-9 http://www.baidu.com/img/bdlogo.gif",如果支持在返回信息的响应头中能找到Content-Range或者Accept-Ranges。

Accept-Ranges：表明服务器是否支持指定范围请求及哪种类型的分段请求

Content-Range：在整个返回体中本部分的字节位置，因为我们请求的是图片的前10个字节，所以Content-Range的值是bytes 0-9/1575，后面的1575是图片总的字节数。

注：libcurl库未放入此工程，需要自己下载编译