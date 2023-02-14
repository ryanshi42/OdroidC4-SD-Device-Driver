#pragma once

//#define MMIO_BASE       0x3F000000
//
//#define GPFSEL0         ((volatile unsigned int*)(MMIO_BASE+0x00200000))
//#define GPFSEL1         ((volatile unsigned int*)(MMIO_BASE+0x00200004))
//#define GPFSEL2         ((volatile unsigned int*)(MMIO_BASE+0x00200008))
//#define GPFSEL3         ((volatile unsigned int*)(MMIO_BASE+0x0020000C))
//#define GPFSEL4         ((volatile unsigned int*)(MMIO_BASE+0x00200010))
//#define GPFSEL5         ((volatile unsigned int*)(MMIO_BASE+0x00200014))
//#define GPSET0          ((volatile unsigned int*)(MMIO_BASE+0x0020001C))
//#define GPSET1          ((volatile unsigned int*)(MMIO_BASE+0x00200020))
//#define GPCLR0          ((volatile unsigned int*)(MMIO_BASE+0x00200028))
//#define GPLEV0          ((volatile unsigned int*)(MMIO_BASE+0x00200034))
//#define GPLEV1          ((volatile unsigned int*)(MMIO_BASE+0x00200038))
//#define GPEDS0          ((volatile unsigned int*)(MMIO_BASE+0x00200040))
//#define GPEDS1          ((volatile unsigned int*)(MMIO_BASE+0x00200044))
//#define GPHEN0          ((volatile unsigned int*)(MMIO_BASE+0x00200064))
//#define GPHEN1          ((volatile unsigned int*)(MMIO_BASE+0x00200068))
//#define GPPUD           ((volatile unsigned int*)(MMIO_BASE+0x00200094))
//#define GPPUDCLK0       ((volatile unsigned int*)(MMIO_BASE+0x00200098))
//#define GPPUDCLK1       ((volatile unsigned int*)(MMIO_BASE+0x0020009C))

#define GPIO_BASE       0x3f200000

#define GPFSEL0         ((volatile unsigned int*)(GPIO_BASE+0x00))
#define GPFSEL1         ((volatile unsigned int*)(GPIO_BASE+0x04))
#define GPFSEL2         ((volatile unsigned int*)(GPIO_BASE+0x08))
#define GPFSEL3         ((volatile unsigned int*)(GPIO_BASE+0x0C))
#define GPFSEL4         ((volatile unsigned int*)(GPIO_BASE+0x10))
#define GPFSEL5         ((volatile unsigned int*)(GPIO_BASE+0x14))
#define GPSET0          ((volatile unsigned int*)(GPIO_BASE+0x1C))
#define GPSET1          ((volatile unsigned int*)(GPIO_BASE+0x20))
#define GPCLR0          ((volatile unsigned int*)(GPIO_BASE+0x28))
#define GPLEV0          ((volatile unsigned int*)(GPIO_BASE+0x34))
#define GPLEV1          ((volatile unsigned int*)(GPIO_BASE+0x38))
#define GPEDS0          ((volatile unsigned int*)(GPIO_BASE+0x40))
#define GPEDS1          ((volatile unsigned int*)(GPIO_BASE+0x44))
#define GPHEN0          ((volatile unsigned int*)(GPIO_BASE+0x64))
#define GPHEN1          ((volatile unsigned int*)(GPIO_BASE+0x68))
#define GPPUD           ((volatile unsigned int*)(GPIO_BASE+0x94))
#define GPPUDCLK0       ((volatile unsigned int*)(GPIO_BASE+0x98))
#define GPPUDCLK1       ((volatile unsigned int*)(GPIO_BASE+0x9C))


