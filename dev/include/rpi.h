#pragma once
#define PERIPH_BASE		0x20000000

#define SYSTMR_BASE		(PERIPH_BASE+0x003000)
#define IRQ_BASE		(PERIPH_BASE+0x00B000)
#define MBOX_BASE		(PERIPH_BASE+0x00B800)
#define CLOCK_BASE		(PERIPH_BASE+0x101000)
#define RAND_BASE		(PERIPH_BASE+0x104000)
#define GPIO_BASE		(PERIPH_BASE+0x200000)
#define UART0_BASE		(PERIPH_BASE+0x201000)
#define SPI_BASE 		(PERIPH_BASE+0x204000)
#define I2C_BASE 		(PERIPH_BASE+0x205000)
#define PWM_BASE		(PERIPH_BASE+0x20C000)
#define MMIO_BASE		(PERIPH_BASE+0x300000)

#define SYSTMR1_IRQ	1
#define SYSTMR3_IRQ 3
#define GPIO_IRQ_0  49
#define GPIO_IRQ_1  50
#define GPIO_IRQ_2  51
#define GPIO_IRQ_3  52
#define IRQ_COUNT	72
