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

result_t gpio_driver_fix_resistor(
        bcm_gpio_regs_t *bcm_gpio_regs,
        uint8_t pin_no,
        gpio_fix_resistor_t gpio_fix_resistor
) {
    if (pin_no >= MAX_GPIO_NUM) {
        return result_err("Invalid pin number in gpio_driver_fix_resistor().");
    }
    if (gpio_fix_resistor > 3) {
        return result_err("Invalid GPIO fix resistor in gpio_driver_fix_resistor().");
    }
    uint32_t regnum = pin_no / 32;								// Create register number
    uint32_t bit = 1 << (pin_no % 32);							// Create mask bit
    bcm_gpio_regs->GPPUD = gpio_fix_resistor;										// Set fixed resistor request to PUD register
    sleep_cyc(150); /* Sleep for 150 cycles. */
    bcm_gpio_regs->GPPUDCLK[regnum] = bit;								// Set the PUD clock bit register
    sleep_cyc(150); /* Sleep for 150 cycles. */
    bcm_gpio_regs->GPPUD = 0;											// Clear GPIO resistor setting
    bcm_gpio_regs->GPPUDCLK[regnum] = 0;									// Clear PUDCLK from GPIO
    return result_ok();
}