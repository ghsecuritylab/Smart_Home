/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     misonyo   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>

/* defined the LED0 pin: PD14 */
#define LED0_PIN    GET_PIN(D, 14)
#define LED1_PIN		GET_PIN(E, 14)
#define LED2_PIN		GET_PIN(E, 15)

int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    rt_pin_mode(LED0_PIN, PIN_MODE_OUTPUT);
		rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
		rt_pin_mode(LED2_PIN, PIN_MODE_OUTPUT);

    while (count++)
    {
				rt_kprintf("123456\r\n");
        rt_pin_write(LED0_PIN, PIN_HIGH);
				rt_pin_write(LED1_PIN, PIN_HIGH);
				rt_pin_write(LED2_PIN, PIN_HIGH);
        rt_thread_mdelay(500);
        rt_kprintf("123456\r\n");
				rt_pin_write(LED0_PIN, PIN_LOW);
				rt_pin_write(LED1_PIN, PIN_LOW);
				rt_pin_write(LED2_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }
    return RT_EOK;
}
