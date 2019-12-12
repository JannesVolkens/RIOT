#ifndef BOARD_H
#define BOARD_H

#include "periph/gpio.h"

#ifdef __cplusplus
extern "C" {
#endif

#define NCV7356_MODE0_PIN     GPIO_PIN(PORT_D, 14)
#define NCV7356_MODE1_PIN     GPIO_PIN(PORT_D, 15)

/**
 * @brief   Initialize board specific hardware, including clock, LEDs, and std-IO
 */
void board_init(void);

#ifdef __cplusplus
}
#endif

#endif /* BOARD_H */
/** @} */
