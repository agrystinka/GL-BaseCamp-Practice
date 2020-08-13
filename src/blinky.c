/*
 * This file containse few alternative samples of work with
 * LEDs, buttons, timers(TIM7, TIM2), systick and interrupts.
 * It is just studing samples, which are not united in one task/project.
 *
 * All samples use mygpiolib - an abstraction over libopencm3/stm32/gpio
*/
#include "mygpiolib.h"
#include <libopencm3/cm3/cortex.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/cm3/scb.h>
#include <libopencm3/stm32/exti.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/cm3/sync.h>
#include <libopencm3/cm3/systick.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

volatile uint32_t __nticks = 0;

const uint32_t TICK_RATE_HZ = 10000ul;

// SysTick ISR
void __attribute__ ((weak)) sys_tick_handler(void)
{
	__nticks++;
}

// basic init
void systick_setup(void)
{
	// disable just in case we're trying to reconfigure
	// while the SysTick is running
	systick_counter_disable();

	// frequency 16 MHz
	systick_set_clocksource(STK_CSR_CLKSOURCE_AHB);
	systick_interrupt_enable();

	// tick frequency will be 10 kHz (100 us tick rate)
	uint32_t period = 16000000lu / 10000lu;
	systick_set_reload(period);
	// load value
	STK_CVR = period;

	nvic_set_priority(NVIC_SYSTICK_IRQ, 2);
	nvic_enable_irq(NVIC_SYSTICK_IRQ);
	__nticks = 0;

	systick_counter_enable();
}


static void delay_ms_systick(uint32_t ms)
{
	uint32_t cur = __nticks;
	uint32_t delta = (TICK_RATE_HZ / 1000) * ms;
	uint32_t next = cur + delta;

	// overflow not taken into account here
	while (__nticks <= next) {
		__asm__ volatile ("wfi");
	}
}

/*
 * Realization of timer that make interrupt every N ms.
 */
 /**
  * timer_tim7_setup() - setup timer based on TIM7.
  * @uint16_t time_ms: time in ms.
  *
  * Function make whole setup of TIM7 timer:
  * 1) enable TIM7
  * 2) set prescaler which garantees timing time approximately equal to time_ms.
  * 3) set TIM7 period equal to max possible period (2 ^ 16)
  * 4) enable interrupts by timer
  *
  * To work with this timer you could use void tim7_isr(void)
  *
  * Return: void.
  */
void timer_tim7_setup(uint16_t ms)
{
	//calculate div and period

	rcc_periph_clock_enable(RCC_TIM7);
	// 1 timer tick == 1 microsecond
	timer_set_prescaler(TIM7, 16 - 1);
	timer_set_period(TIM7, ms * 1000);
	timer_enable_preload(TIM7);
	// Set URS bit to update only at overflow (when auto-reload value is reached)
	timer_enable_irq(TIM7, TIM_DIER_UIE);
	timer_clear_flag(TIM7, TIM_SR_UIF);		// just in case
	nvic_set_priority(NVIC_TIM7_IRQ, 2);
	nvic_enable_irq(NVIC_TIM7_IRQ);
	timer_enable_counter(TIM7);
}

/*
 * Realization of delay for N ms based on one shot timer.
 */
void one_shot_timer_tim7_setup()
{
	rcc_periph_clock_enable(RCC_TIM7);
	// 1 timer tick == 1 microsecond
	timer_set_prescaler(TIM7, 16 - 1);
	timer_disable_preload(TIM7);
	timer_one_shot_mode(TIM7);
	// Set URS bit to update only at overflow (when auto-reload value is reached)
	timer_update_on_overflow(TIM7);
	timer_enable_irq(TIM7, TIM_DIER_UIE);
	timer_clear_flag(TIM7, TIM_SR_UIF);		// just in case
	nvic_set_priority(NVIC_TIM7_IRQ, 2);
	nvic_enable_irq(NVIC_TIM7_IRQ);
}

void one_shot_timer_tim7_1ms()
{
	timer_set_period(TIM7, 1000);
	timer_generate_event(TIM7, TIM_EGR_UG);
	__dmb();
	timer_enable_counter(TIM7);
	__asm__ volatile ("wfi");

}

void one_shot_timer_tim7(uint32_t ms)
{
	while(ms--)
		one_shot_timer_tim7_1ms();
}

void tim7_isr(void)
{
	//sample 1
	//for realization of delay for N ms based on one shot timer.
	timer_clear_flag(TIM7, TIM_SR_UIF);

	//for sample with timer(tim7) which make interruption every N ms and switch LED
	//mgl_toggle(mgl_led_orange);
}

int main(void)
{
    rcc_periph_clock_enable(RCC_GPIOD);  //LEDs enable
	//rcc_periph_clock_enable(RCC_TIM7);
	//Switch on red diod untill all settings end
	mgl_mode_setup_default(mgl_led_red);
	mgl_set(mgl_led_red);

	mgl_mode_setup_default(mgl_led_blue);
	mgl_mode_setup_default(mgl_led_green);
	mgl_mode_setup_default(mgl_led_orange);
	gpio_set_output_options(GPIOE, GPIO_OTYPE_PP, GPIO_OSPEED_100MHZ, GPIO12 | GPIO13 | GPIO15);
	//switch on diods: blue, green, orange
	mgl_set(mgl_led_blue);
	mgl_set(mgl_led_green);
	mgl_set(mgl_led_orange);

	//sample 3
	//timer_tim7_setup(1000);

	//sample 2
	//systick_setup();

	//sample 1
	one_shot_timer_tim7_setup();
	timer_tim7_setup(1000);

	cm_enable_interrupts();

	//Switch aff red diod
	mgl_clear(mgl_led_red);

    while (1) {
		//sample 2
		//mgl_toggle(mgl_led_orange);
		//delay_ms_systick(500);
		//__asm__ volatile ("wfi");

		//sample 1
		mgl_toggle(mgl_led_orange);
		one_shot_timer_tim7(500);
    }
}
