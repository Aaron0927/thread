线程函数介绍：
1.线程创建：thread_create()
2.加入到就绪队列：thread_add_runqueue()
3.线程启动（只要在启动主线程就可以）：thread_start_threading()
4.进入待执行状态：thread_yield()

运行例子程序：
1. make clean
2. make
3. ./thread 

注：线程调度方式是用轮询调度，用双向循环链表实现。



