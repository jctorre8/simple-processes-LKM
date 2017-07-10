#include <linux/sched.h>
#include <linux/list.h>
#include <linux/init.h>             
#include <linux/module.h>        
#include <linux/kernel.h>  
#include <linux/tty.h>        
 
MODULE_LICENSE("GPL");              
MODULE_AUTHOR("Jean C Torres");      
MODULE_DESCRIPTION("A simple Linux Kernel Module to see processes with a greater PID than the given number");  

static int inp_pid = 0;        ///default value is 
module_param(inp_pid, int, 0644); 
MODULE_PARM_DESC(name, "The PID to lookup processes.\n");  ///Parameter description
 
/** @brief The LKM initialization function
 *  @return returns 0 if successful
 */
static int __init init(void){

	struct task_struct *task;
	struct task_struct *child;

	printk(KERN_INFO "Processes lookup started printing Processes with PID > %d \n", inp_pid);

	//Go through each process that falls in the corect PID category
	for_each_process(task){
		if(task->pid >= inp_pid){
			printk(KERN_INFO "%20s%8s%8s%8s%8s%12s\n", "PROCESS","PID","STATE","PRIO","ST_PRIO",
				"NORM_PRIO");
			printk(KERN_INFO "%20s%8d%8ld%8d%8d%12d\n",task->comm,task->pid,
				task->state,task->prio,task->static_prio, task->normal_prio);

			//Iterates through each process again looking for children
			//Tried to do it with the children head list but I got bad data
			for_each_process(child){
				//If task is a childs parent
				if(child->real_parent->pid == task->pid){
					printk(KERN_INFO "%20s\n", "CHILD");
					printk(KERN_INFO "%20s%8d%8ld%8d%8d%12d\n", child->comm,child->pid,child->state,
						child->prio,child->static_prio, child->normal_prio);					
				}
			}

			//Print parameters for the real parent process
			printk(KERN_INFO "%20s\n","PARENT");
			printk(KERN_INFO "%20s%8d%8ld%8d%8d%12d\n", task->real_parent->comm,
				task->real_parent->pid, task->real_parent->state,task->real_parent->prio,
				task->real_parent->static_prio,	task->real_parent->normal_prio);
			
			//Print a separator between tasks
			printk(KERN_INFO "\n\n");

		}
	}

   return 0;
}
 
/** @brief The LKM cleanup function
 */
static void __exit exitLMK(void){
   printk(KERN_INFO "Processes lookup finished printing Processes with PID > %d \n", inp_pid);
}
 
module_init(init);
module_exit(exitLMK);