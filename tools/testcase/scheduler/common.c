/*
 * Test Common task scheduler
 *
 * (C) 2017.12 BiscuitOS <buddy.zhang@aliyun.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License version 2 as
 * published by the Free Software Foundation.
 */
#include <linux/kernel.h>
#include <linux/sched.h>

#include <test/task.h>

/* common task interface */
int test_task_scheduler(void)
{
#ifdef CONFIG_TESTCASE_GDT
    debug_gdt_common();
#endif

#ifdef CONFIG_TESTCASE_SEGMENT
    debug_segment_common();
#endif

#ifdef CONFIG_TESTCASE_GATE
    debug_system_descriptor_common();
#endif

#ifdef CONFIG_TESTCASE_IDT
    debug_idt_segment_desc_common();
#endif

#ifdef CONFIG_TESTCASE_STACK
    debug_stack_common();
#endif
    return 0;
}

#if defined (CONFIG_DEBUG_USERLAND_EARLY) || \
    defined (CONFIG_DEBUG_USERLAND_SHELL)
/* debug kernel on userland stage */
void debug_scheduler_kernel_on_userland(void)
{
#ifdef CONFIG_TESTCASE_MULT_PRIVILEGE
    debug_stack_kernel_on_userland();
#endif
}
#endif
