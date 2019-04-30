/* --------------------------------------
 *
 * @File:       interface_button.h
 * @Author:     Saujan Thapa
 * @Date:       2018 May
 * @Version:    1.0.0
 *
 * --------------------------------------
 */

#ifndef INTERFACE_BUTTON_H_
#define INTERFACE_BUTTON_H_
#ifndef MOTOR_IDLE
    #define MOTOR_IDLE      1
#endif
#ifndef MOTOR_STARTING
    #define MOTOR_STARTING  2
#endif
#ifndef MOTOR_RUNNING
    #define MOTOR_RUNNING   3
#endif
#ifndef MOTOR_ESTOPPING
    #define MOTOR_ESTOPPING 4
#endif

#ifndef EVENT_SETSPEED
    #define EVENT_SETSPEED 1
#endif
#ifndef EVENT_SETSPEED_TO_ZERO
    #define EVENT_SETSPEED_TO_ZERO 0
#endif
#ifndef EVENT_BRAKE
    #define EVENT_BRAKE 2
#endif
#include <stdint.h>
#include <stdbool.h>
#include "driverlib/gpio.h"
//#include "driverlib/rom.h"
//#include "driverlib/rom_map.h"
#include "driverlib/sysctl.h"
//#include "driverlib/udma.h"
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"
#include "drivers/frame.h"
#include "drivers/kentec320x240x16_ssd2119_spi.h"
#include "drivers/pinout.h"
#include "drivers/touch.h"
#include "drivers/buttons.h"

#ifndef BUTTON_NONE
    #define BUTTON_NONE 0
#endif
#ifndef BUTTON_UP
    #define BUTTON_UP 3
#endif
#ifndef BUTTON_DOWN
    #define BUTTON_DOWN 1
#endif
#ifndef BUTTON_SELECT
    #define BUTTON_SELECT 2
#endif
//#define UP_BUTTON               GPIO_PIN_3
//#define DOWN_BUTTON             GPIO_PIN_5
//#define SELECT_BUTTON           GPIO_PIN_1
//
//#define ALL_BUTTONS             (UP_BUTTON | DOWN_BUTTON | SELECT_BUTTON)


void initInterfaceButton();

uint8_t validInterfaceButton(uint8_t screen, uint8_t *button_pressed);

uint8_t OnButtonPress();

#endif /* INTERFACE_BUTTON_H_ */
