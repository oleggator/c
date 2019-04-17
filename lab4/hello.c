/* hello.c – Простой загружаемый модуль ядра */
#include <linux/init.h>
#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/sched/signal.h> // github.com/torvalds/linux/commit/c3edc40
#include <stddef.h>

void print_process_info(void) /* Печать процессов */
{
    struct task_struct* task_list;

    for_each_process(task_list) {
        pr_info("%d: %s\n", task_list->pid, task_list->comm);
    }
}

static int __init hello_init(void) /* Инициализация */
{
    printk(KERN_INFO "Hello: registered\n");

    print_process_info();

    return 0;
}

static void __exit hello_exit(void) /* Деинициализаия */
{
    printk(KERN_INFO "Hello: unregistered\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Ivan Sidyakin");
MODULE_DESCRIPTION("Simple loadable kernel module");
