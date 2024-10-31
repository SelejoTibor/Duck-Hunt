/***************************************************************************//**
 * @file
 * @brief Top level application functions
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * The licensor of this software is Silicon Laboratories Inc. Your use of this
 * software is governed by the terms of Silicon Labs Master Software License
 * Agreement (MSLA) available at
 * www.silabs.com/about-us/legal/master-software-license-agreement. This
 * software is distributed to you in Source Code format and is governed by the
 * sections of the MSLA applicable to Source Code.
 *
 ******************************************************************************/

/***************************************************************************//**
 * @file     app.c
 * @brief    Duck Hunt mini game
 *
 * @details  A képernyőn véletlenszerűen megjelennek kacsák, amiket le kell
 * tudni vadászni. A kacsák az alsó (alfanumerikus) kijelző valamelyik négy
 * mezejének legfelső vízszintes szegmensében jelenhetnek meg véletlenszerűen
 * (lásd zöld jelölést), majd a nehézségi szinttől függő ideig maradnak.
 * A vadásznak (alsó vízszintes szegmens) alájuk kell állnia és lelőni őket.
 *
 * Created on:
 *
 * Author:
 ******************************************************************************/

/***************************************************************************//**
 * Initialize application.
 ******************************************************************************/

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "caplesense.h"
#include "segmentlcd.h"
#include "segmentlcd_individual.h"
#include "sl_simple_button_instances.h"
#include "start.h"

#define PB1_PORT    gpioPortB
#define PB1_PIN     10

void app_init(void)
{
  GPIO_PinModeSet(PB1_PORT, PB1_PIN, gpioModeInput, 1);
  SegmentLCD_Init(false);
  CAPLESENSE_Init(false);
  game_init();
  if (SysTick_Config(CMU_ClockFreqGet(cmuClock_CORE)/1000)) {
      while (1) ;
    }
}

/***************************************************************************//**
 * App ticking function.
 ******************************************************************************/
void app_process_action(void)
{
  game();
  while(1);
}
