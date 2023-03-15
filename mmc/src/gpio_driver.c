#include "gpio_driver.h"

result_t gpio_driver_setup_pin(
        bcm_gpio_regs_t *bcm_gpio_regs,
        uint8_t pin_no,
        gpio_mode_t gpio_mode
) {
    if (pin_no >= MAX_GPIO_NUM) {
        return result_err("Invalid pin number in gpio_driver_setup_pin().");
    }
    if (gpio_mode > 7) {
        return result_err("Invalid GPIO mode in gpio_driver_setup_pin().");
    }
    uint32_t bit = ((pin_no % 10) * 3); /* Create bit mask */
    uint32_t mem = bcm_gpio_regs->GPFSEL[pin_no / 10]; /* Read register */
    mem &= ~(7 << bit); /* Clear GPIO mode bits for that port */
    mem |= (gpio_mode << bit); /* Logical OR GPIO mode bits */
    bcm_gpio_regs->GPFSEL[pin_no / 10] = mem; /* Write value to register */
    return result_ok();
}
