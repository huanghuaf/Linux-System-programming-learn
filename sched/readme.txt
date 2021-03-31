问题:

前提条件：单核

1.当一个实时进程P，以SCHED_FIFO为调度策略，一直执行while(1)，其他进程是否有机会占用cpu?

答案:可以，会触发sched: RT throttling activated机制

触发机制路径如下：

1fe0: b6e8d7d1 bee42cf0 000106b0 000106d0 600b0010 ffffffff
CPU: 0 PID: 854 Comm: child Not tainted 4.9.191-rc1+ #37
Hardware name: ARM-Versatile Express
[<c0110204>] (unwind_backtrace) from [<c010c390>] (show_stack+0x20/0x24)
[<c010c390>] (show_stack) from [<c03f4a74>] (dump_stack+0xac/0xd8)
[<c03f4a74>] (dump_stack) from [<c016061c>] (update_curr_rt+0x204/0x254)
[<c016061c>] (update_curr_rt) from [<c0160b24>] (task_tick_rt+0x20/0x134)
[<c0160b24>] (task_tick_rt) from [<c014ed60>] (scheduler_tick+0x70/0xf0)
[<c014ed60>] (scheduler_tick) from [<c01837d0>] (update_process_times+0x60/0x6c)
[<c01837d0>] (update_process_times) from [<c019310c>] (tick_periodic+0x4c/0xcc)
[<c019310c>] (tick_periodic) from [<c0193330>] (tick_handle_periodic+0x38/0x98)
[<c0193330>] (tick_handle_periodic) from [<c010f4f4>] (twd_handler+0x40/0x50)
[<c010f4f4>] (twd_handler) from [<c01748e4>] (handle_percpu_devid_irq+0x98/0x24c)
[<c01748e4>] (handle_percpu_devid_irq) from [<c016f254>] (generic_handle_irq+0x34/0x44)
[<c016f254>] (generic_handle_irq) from [<c016f8a8>] (__handle_domain_irq+0x6c/0xc4)
[<c016f8a8>] (__handle_domain_irq) from [<c0101568>] (gic_handle_irq+0x58/0x9c)
[<c0101568>] (gic_handle_irq) from [<c06d2418>] (__irq_usr+0x58/0x80)

2.如果其他进程P1也是SCHED_FIFO，且优先级比P高，是否有计划占用cpu？
