
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