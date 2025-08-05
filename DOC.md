
# todo
* co_eventloop
* 超时如何实现
    * Q：example_copystack里的poll如何实现的超时
        * A：最终的epoll_wait会设上1ms超时，然后走TimeoutLink时间轮机制。
    * Q：协程超时后，timer是怎么删除的？
        * A：通过RemoveFromLink<stTimeoutItem_t,stTimeoutItemLink_t>(...)

# 疑问


# QA
* Q: stCoRoutineEnv_t::pCallStack 调用环的问题如何解决？
  A: 原版没有解决这个问题。co_yield_env里加上了自己的解决方案。