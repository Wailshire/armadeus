/*
* linux kernel driver for generation signal with sleep  
*
* Copyright (C) 2009 <gwenhael.goavec-merou@armadeus.com>
*                         Armadeus Project / Armadeus Systems
*
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2, or (at your option)
* any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/timer.h>
#include <asm/gpio.h>

#include "../../../../common_kernel.h"

MODULE_AUTHOR("Gwenhael GOAVEC MEROU");
MODULE_DESCRIPTION("sleep kernel test");
MODULE_SUPPORTED_DEVICE("none");
MODULE_LICENSE("GPL");

struct task_struct *th;

/* thread fonction */
int fct_thread_sleep(void *data)
{
	int iomask=1;
	int timesleep = TIMESLEEP/1000;

  	do {
    		gpio_set_value(PULSE_OUTPUT_GPIO, iomask); 
		iomask^=1;            
    		msleep(timesleep);
  	} while (!kthread_should_stop());

  	return 0;
}

/* loading (insmod) */
static int __init blink_init(void)
{
  	th = kthread_create(fct_thread_sleep, NULL, "pulse_linux_kernel_thread_sleep");
	wake_up_process(th);

	return 0;
}


/* unloading (rmmod) */
static void __exit blink_exit(void)
{
	printk(KERN_INFO "blink_exit\n");
	kthread_stop(th);
}

/* API Linux devices */
module_init(blink_init);
module_exit(blink_exit);

