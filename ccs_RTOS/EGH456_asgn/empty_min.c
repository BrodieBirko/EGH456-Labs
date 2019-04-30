/*
 * Copyright (c) 2015, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*
 *  ======== empty_min.c ========
 */
/* XDCtools Header files */
#include <xdc/std.h>

/* BIOS Header files */
#include <ti/sysbios/BIOS.h>
#include <ti/sysbios/knl/Task.h>
#include <ti/sysbios/hal/Hwi.h>
#include <ti/sysbios/knl/Clock.h>

/* TI-RTOS Header files */
// #include <ti/drivers/EMAC.h>
#include <ti/drivers/GPIO.h>
// #include <ti/drivers/I2C.h>
// #include <ti/drivers/SDSPI.h>
// #include <ti/drivers/SPI.h>
// #include <ti/drivers/UART.h>
// #include <ti/drivers/USBMSCHFatFs.h>
// #include <ti/drivers/Watchdog.h>
// #include <ti/drivers/WiFi.h>

//ADDED
#include <stdint.h>
#include <stdbool.h>
#include "grlib/grlib.h"
#include "grlib/widget.h"
#include "grlib/canvas.h"
#include "grlib/pushbutton.h"
#include "drivers/Kentec320x240x16_ssd2119_spi.h"
#include "drivers/touch.h"
#include <xdc/runtime/System.h>

/* Board Header file */
#include "Board.h"

//Other stuff
#include "user_interface.h"
#include <time.h>
#include <ti/sysbios/hal/Seconds.h>
tContext sContext;
UI_Params uiParams;
#define freq 500000

#define TASKSTACKSIZE           2048

extern tCanvasWidget g_sBackground;
extern tCanvasWidget g_sMenuPage;
extern tCanvasWidget g_sSettingPage1;
extern tCanvasWidget g_sSettingPage2;
extern tCanvasWidget g_sSettingPage3;
extern tCanvasWidget g_sGraphPage1;
extern tCanvasWidget g_sGraphPage2;
extern tCanvasWidget g_sGraphPage3;
extern tPushButtonWidget g_sMotorStartStop;
extern tPushButtonWidget g_sMotorState;
extern tPushButtonWidget g_sMenu;
extern tPushButtonWidget g_sSave1;
extern tPushButtonWidget g_sSave2;
extern tPushButtonWidget g_sSave3;
extern tPushButtonWidget g_sAddition1;
extern tPushButtonWidget g_sAddition2;
extern tPushButtonWidget g_sAddition3;
extern tPushButtonWidget g_sSubtraction1;
extern tPushButtonWidget g_sSubtraction2;
extern tPushButtonWidget g_sSubtraction3;
extern tPushButtonWidget g_sBack1;
extern tPushButtonWidget g_sBack2;
extern tPushButtonWidget g_sBack3;
extern tPushButtonWidget g_sBack4;
extern tPushButtonWidget g_sSetTemp;
extern tPushButtonWidget g_sSetSpeed;
extern tPushButtonWidget g_sSetCurrent;
extern tPushButtonWidget g_sGraphTemp;
extern tPushButtonWidget g_sGraphSpeed;
extern tPushButtonWidget g_sGraphCurrent;
extern tPushButtonWidget g_sNext;

Char taskStack[TASKSTACKSIZE];
Hwi_Handle hallInt0, hallInt1;
Task_Handle uiTask, tempTask;
//Error_Block eb;
//Clock_Struct clk0Struct;
//Clock_Handle clk0Handle;
Task_Struct task0Struct;
void GoHome(tWidget *psWidget);

Canvas(g_sBackground, WIDGET_ROOT, 0, 0,
       &g_sKentec320x240x16_SSD2119, 10, 25, 300, (240 - 25 -10),
       CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);

RectangularButton(g_sMotorStartStop, &g_sBackground, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 175, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrWhite, ClrBlack, ClrWhite, ClrBlack,
                   g_psFontCmss18b, "Start Motor", 0, 0, 0, 0, GoHome);

RectangularButton(g_sMenu, &g_sBackground, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 205, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "Main menu", 0, 0, 0, 0, GoHome);

RectangularButton(g_sMotorState, &g_sBackground, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 50, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlue, ClrBlack, ClrBlue, ClrWhite,
                   g_psFontCmss18b, "Motor is in idle state", 0, 0, 0, 0, GoHome);

Canvas(g_sMenuPage, &g_sMenu, 0, 0,
       &g_sKentec320x240x16_SSD2119, 10, 25, 300, (240 - 25 -10),
       CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);

RectangularButton(g_sSetTemp, &g_sMenu, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 25, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrWhite, ClrBlack, ClrWhite, ClrBlack,
                   g_psFontCmss18b, "Set Temperature Limit", 0, 0, 0, 0, GoHome);

RectangularButton(g_sSetSpeed, &g_sMenu, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 55, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "Set Speed", 0, 0, 0, 0, GoHome);

RectangularButton(g_sSetCurrent, &g_sMenu, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 85, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "Set Current Limit", 0, 0, 0, 0, GoHome);

RectangularButton(g_sGraphTemp, &g_sMenu, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 115, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "Temperature Graph", 0, 0, 0, 0, GoHome);

RectangularButton(g_sGraphSpeed, &g_sMenu, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 145, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "Speed Graph", 0, 0, 0, 0, GoHome);

RectangularButton(g_sGraphCurrent, &g_sMenu, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 175, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "Current Graph", 0, 0, 0, 0, GoHome);

RectangularButton(g_sBack1, &g_sMenu, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 205, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "Back to home page", 0, 0, 0, 0, GoHome);

RectangularButton(g_sNext,  &g_sMenu, 0, 0, &g_sKentec320x240x16_SSD2119, 270, 190,
                  50, 50, PB_STYLE_IMG | PB_STYLE_TEXT, ClrBlack, ClrBlack, 0,
                  ClrSilver, &g_sFontCm20, "+", 0,
                  0, 0, 0, GoHome);

Canvas(g_sSettingPage1, &g_sSetTemp, 0, 0,
       &g_sKentec320x240x16_SSD2119, 10, 25, 300, (240 - 25 -10),
       CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);

Canvas(g_sSettingPage2, &g_sSetSpeed, 0, 0,
       &g_sKentec320x240x16_SSD2119, 10, 25, 300, (240 - 25 -10),
       CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);

Canvas(g_sSettingPage3, &g_sSetCurrent, 0, 0,
       &g_sKentec320x240x16_SSD2119, 10, 25, 300, (240 - 25 -10),
       CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);

Canvas(g_sGraphPage1, &g_sGraphTemp, 0, 0,
       &g_sKentec320x240x16_SSD2119, 10, 25, 300, (240 - 25 -10),
       CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);

Canvas(g_sGraphPage2, &g_sGraphSpeed, 0, 0,
       &g_sKentec320x240x16_SSD2119, 10, 25, 300, (240 - 25 -10),
       CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);

Canvas(g_sGraphPage3, &g_sGraphCurrent, 0, 0,
       &g_sKentec320x240x16_SSD2119, 10, 25, 300, (240 - 25 -10),
       CANVAS_STYLE_FILL, ClrBlack, 0, 0, 0, 0, 0, 0);

RectangularButton(g_sAddition1, &g_sSettingPage1, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 147, 50, 25, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrWhite, ClrBlack, ClrWhite, ClrBlack,
                   g_psFontCmss18b, "+", 0, 0, 0, 0, GoHome);

RectangularButton(g_sSubtraction1, &g_sSettingPage1, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 147, 120, 25, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "-", 0, 0, 0, 0, GoHome);

RectangularButton(g_sSave1, &g_sSettingPage1, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 110, 175, 100, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "Save", 0, 0, 0, 0, GoHome);


RectangularButton(g_sAddition2, &g_sSettingPage2, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 147, 50, 25, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrWhite, ClrBlack, ClrWhite, ClrBlack,
                   g_psFontCmss18b, "+", 0, 0, 0, 0, GoHome);

RectangularButton(g_sSubtraction2, &g_sSettingPage2, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 147, 120, 25, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "-", 0, 0, 0, 0, GoHome);

RectangularButton(g_sSave2, &g_sSettingPage2, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 110, 175, 100, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "Save", 0, 0, 0, 0, GoHome);


RectangularButton(g_sAddition3, &g_sSettingPage3, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 147, 50, 25, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrWhite, ClrBlack, ClrWhite, ClrBlack,
                   g_psFontCmss18b, "+", 0, 0, 0, 0, GoHome);

RectangularButton(g_sSubtraction3, &g_sSettingPage3, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 147, 120, 25, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "-", 0, 0, 0, 0, GoHome);

RectangularButton(g_sSave3, &g_sSettingPage3, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 110, 175, 100, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrBlack, ClrWhite, ClrWhite, ClrWhite,
                   g_psFontCmss18b, "Save", 0, 0, 0, 0, GoHome);


RectangularButton(g_sBack2, &g_sGraphPage1, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 205, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrWhite, ClrBlack, ClrWhite, ClrBlack,
                   g_psFontCmss18b, "Back to main menu", 0, 0, 0, 0, GoHome);


RectangularButton(g_sBack3, &g_sGraphPage2, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 205, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrWhite, ClrBlack, ClrWhite, ClrBlack,
                   g_psFontCmss18b, "Back to main menu", 0, 0, 0, 0, GoHome);


RectangularButton(g_sBack4, &g_sGraphPage3, 0, 0,
                  &g_sKentec320x240x16_SSD2119, 10, 205, 300, 25,
                  (PB_STYLE_OUTLINE | PB_STYLE_TEXT_OPAQUE | PB_STYLE_TEXT |
                    PB_STYLE_FILL | PB_STYLE_RELEASE_NOTIFY),
                   ClrWhite, ClrBlack, ClrWhite, ClrBlack,
                   g_psFontCmss18b, "Back to main menu", 0, 0, 0, 0, GoHome);
uint32_t check;
void GoHome(tWidget *psWidget)
{
    uiParams.screen = 1;
    check+= 10;
    PushButtonImageOff(&g_sNext);
    PushButtonTextOff(&g_sNext);
    PushButtonFillOn(&g_sNext);
    WidgetPaint((tWidget *)&g_sNext);
}

void userInterfaceFxn(UArg arg0, UArg arg1);

void userInterfaceFxn(UArg arg0, UArg arg1)
{
    //TemperatureInit(&tempParams);
    UserInterfaceInit(arg0, &sContext, &uiParams);

    while(1)
    {
        UserInterfaceDraw(&uiParams, &sContext);
        check+=1;
    }

}

#define TASKSTACKSIZE   512

Task_Struct task0Struct;
Char task0Stack[TASKSTACKSIZE];

/*
 *  ======== heartBeatFxn ========
 *  Toggle the Board_LED0. The Task_sleep is determined by arg0 which
 *  is configured for the heartBeat Task instance.
 */
Void heartBeatFxn(UArg arg0, UArg arg1)
{
    while (1) {
        Task_sleep((unsigned int)arg0);
        GPIO_toggle(Board_LED0);
    }
}
void hallInterruptFxn(UArg arg);
/*
 *  ======== main ========
 */
int main(void)
{
    check = 0;
    Task_Params taskParams;
    Hwi_Params hwiParams;

    /* Call board init functions */
    Board_initGeneral();
    // Board_initEMAC();
    Board_initGPIO();
    Board_initI2C();
    // Board_initSDSPI();
    // Board_initSPI();
    // Board_initUART();
    // Board_initUSB(Board_USBDEVICE);
    // Board_initUSBMSCHFatFs();
    // Board_initWatchdog();
    // Board_initWiFi();
    uint32_t ui32SysClock = SysCtlClockFreqSet((SYSCTL_XTAL_25MHZ |
                                                    SYSCTL_OSC_MAIN | SYSCTL_USE_PLL |
                                                    SYSCTL_CFG_VCO_480), 120000000);

    /* Construct heartBeat Task  thread */
    //setSystemTime();
    Task_Params_init(&taskParams);
    taskParams.stackSize = TASKSTACKSIZE;
    taskParams.priority = 5;
    taskParams.stack = &taskStack;
    taskParams.arg0 = ui32SysClock;
    uiTask = Task_create((Task_FuncPtr)userInterfaceFxn, &taskParams, NULL);

   Hwi_Params_init(&hwiParams);
   hwiParams.arg = 1;
   hwiParams.priority = 40;
   hallInt0 = Hwi_create(31, (Hwi_FuncPtr)hallInterruptFxn, &hwiParams, NULL);
   //hallInt1 = Hwi_create(92, (Hwi_FuncPtr)hallInterruptFxn, &hwiParams, NULL);
   if (hallInt0 == NULL)
           System_abort("Hwi create failed\n");

    /* Turn on user LED  */
    GPIO_write(Board_LED0, Board_LED_ON);

    /* Start BIOS */
    //System_flush();
    BIOS_start();

    return (0);
}

void hallInterruptFxn(UArg arg)
{
    check +=1;


//    uint32_t timer_5 = TimerValueGet(TIMER5_BASE, TIMER_A);
//
//    if (HWREG(GPIO_PORTL_BASE|GPIO_O_RIS) & GPIO_PIN_3){
//        HWREG(GPIO_PORTL_BASE|GPIO_O_ICR) = GPIO_PIN_3;
//      /*  if (HWREG(GPIO_PORTL_BASE+(GPIO_O_DATA + (GPIO_PIN_3 << 2))))
//            motorParams.motor.hall_sensor_state[0]=1;
//        else
//            motorParams.motor.hall_sensor_state[0]=0;*/
//    }
//    if (HWREG(GPIO_PORTP_BASE|GPIO_O_RIS) & GPIO_PIN_4){
//        HWREG(GPIO_PORTP_BASE|GPIO_O_ICR) = GPIO_PIN_4;
//       /*     if (HWREG(GPIO_PORTP_BASE+(GPIO_O_DATA + (GPIO_PIN_4 << 2))))
//                motorParams.motor.hall_sensor_state[1]=1;
//            else
//                motorParams.motor.hall_sensor_state[1]=0;*/
//    }
//    if (HWREG(GPIO_PORTP_BASE|GPIO_O_RIS) & GPIO_PIN_5){
//        HWREG(GPIO_PORTP_BASE|GPIO_O_ICR) = GPIO_PIN_5;
//         /*   if (HWREG(GPIO_PORTP_BASE+(GPIO_O_DATA + (GPIO_PIN_5 << 2))))
//                motorParams.motor.hall_sensor_state[2]=1;
//            else
//                motorParams.motor.hall_sensor_state[2]=0;*/
//    }
//    MotorRotate(&motorParams);
//    motorParams.actual_speed = (uint8_t) round((120000000.0)/(timer_5 * 16 * NUM_MOTOR_PAIR_POLES * 6));
//    MotorAdjustSpeed(&motorParams);
//    //System_printf("Current speed %d\n",motorParams.actual_speed);
//    HWREG(TIMER5_BASE + 0x00000050) = 0;
}
//void setSystemTime()
//{
//    time_t time_seconds;
//    struct tm time_info = {0};
//
//    time_info.tm_year = 2018-1970;
//    time_info.tm_mon = 5-1;
//    time_info.tm_mday = 21;
//    time_info.tm_hour = 15;
//    time_info.tm_min = 01;
//    time_info.tm_sec = 30;
//
//    time_seconds = mktime(&time_info);
//
//    Seconds_set(time_seconds);
//}
