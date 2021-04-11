问题:

前提条件：单核

1.当一个实时进程P，以SCHED_FIFO为调度策略，一直执行while(1)，其他进程是否有机会占用cpu?

答案:可以，会触发sched: RT throttling activated机制

触发机制路径如下：

SMP:
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

非SMP && CONFIG_HZ_PERIODIC
[<c010e0a4>] (unwind_backtrace) from [<c010bfa8>] (show_stack+0x20/0x24)
[<c010bfa8>] (show_stack) from [<c03aee5c>] (dump_stack+0x24/0x28)
[<c03aee5c>] (dump_stack) from [<c0147718>] (update_curr_rt+0x154/0x1ac)
[<c0147718>] (update_curr_rt) from [<c0147790>] (task_tick_rt+0x20/0x118)
[<c0147790>] (task_tick_rt) from [<c0143ec8>] (scheduler_tick+0x4c/0x60)
[<c0143ec8>] (scheduler_tick) from [<c015fd48>] (update_process_times+0x60/0x70)
[<c015fd48>] (update_process_times) from [<c016e9ec>] (tick_periodic.constprop.5+0x48/0xa0)
[<c016e9ec>] (tick_periodic.constprop.5) from [<c016ea68>] (tick_handle_periodic+0x24/0x80)
[<c016ea68>] (tick_handle_periodic) from [<c053ac64>] (sp804_timer_interrupt+0x40/0x48)
[<c053ac64>] (sp804_timer_interrupt) from [<c0153a94>] (__handle_irq_event_percpu+0x90/0x208)
[<c0153a94>] (__handle_irq_event_percpu) from [<c0153c38>] (handle_irq_event_percpu+0x2c/0x68)
[<c0153c38>] (handle_irq_event_percpu) from [<c0153cac>] (handle_irq_event+0x38/0x4c)
[<c0153cac>] (handle_irq_event) from [<c0156e04>] (handle_fasteoi_irq+0xbc/0x184)
[<c0156e04>] (handle_fasteoi_irq) from [<c0152cd8>] (generic_handle_irq+0x34/0x44)
[<c0152cd8>] (generic_handle_irq) from [<c01532c4>] (__handle_domain_irq+0x64/0xb8)
[<c01532c4>] (__handle_domain_irq) from [<c0101520>] (gic_handle_irq+0x58/0x8c)
[<c0101520>] (gic_handle_irq) from [<c067e218>] (__irq_usr+0x58/0x80)

非SMP && CONFIG_NO_HZ_IDLE
[<c010e450>] (unwind_backtrace) from [<c010c278>] (show_stack+0x20/0x24)
[<c010c278>] (show_stack) from [<c03ea790>] (dump_stack+0x24/0x28)
[<c03ea790>] (dump_stack) from [<c014d0ec>] (update_curr_rt+0x15c/0x1e4)
[<c014d0ec>] (update_curr_rt) from [<c014d194>] (task_tick_rt+0x20/0x118)
[<c014d194>] (task_tick_rt) from [<c0148db0>] (scheduler_tick+0x54/0xb4)
[<c0148db0>] (scheduler_tick) from [<c016eff8>] (update_process_times+0x60/0x70)
[<c016eff8>] (update_process_times) from [<c0180a10>] (tick_sched_handle+0x58/0x64)
[<c0180a10>] (tick_sched_handle) from [<c0180a7c>] (tick_sched_timer+0x60/0xa4)
[<c0180a7c>] (tick_sched_timer) from [<c01700d0>] (__hrtimer_run_queues.constprop.4+0x174/0x330)
[<c01700d0>] (__hrtimer_run_queues.constprop.4) from [<c01704f0>] (hrtimer_interrupt+0xac/0x258)
[<c01704f0>] (hrtimer_interrupt) from [<c0588cf8>] (sp804_timer_interrupt+0x40/0x48)
[<c0588cf8>] (sp804_timer_interrupt) from [<c015b2b0>] (__handle_irq_event_percpu+0x84/0x268)
[<c015b2b0>] (__handle_irq_event_percpu) from [<c015b4c0>] (handle_irq_event_percpu+0x2c/0x68)
[<c015b4c0>] (handle_irq_event_percpu) from [<c015b558>] (handle_irq_event+0x5c/0x94)
[<c015b558>] (handle_irq_event) from [<c015ebec>] (handle_fasteoi_irq+0xe0/0x1d8)
[<c015ebec>] (handle_fasteoi_irq) from [<c015a324>] (generic_handle_irq+0x34/0x44)
[<c015a324>] (generic_handle_irq) from [<c015aa64>] (__handle_domain_irq+0x80/0xec)
[<c015aa64>] (__handle_domain_irq) from [<c01015ac>] (gic_handle_irq+0x58/0x8c)
[<c01015ac>] (gic_handle_irq) from [<c06d7eb8>] (__irq_usr+0x58/0x80)



2.如果其他进程P1也是SCHED_FIFO，且优先级比P高，是否有机会占用cpu？
