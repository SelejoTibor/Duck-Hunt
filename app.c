/**
 * @file app.c
 * @brief Fő alkalmazásvezérlő funkciók.
 *
 * Ez a fájl tartalmazza a játék inicializálását és a folyamatosan futó
 * akcióvezérlő ciklust.
 */

#include "em_device.h"
#include "em_cmu.h"
#include "em_gpio.h"
#include "caplesense.h"
#include "segmentlcd.h"
#include "segmentlcd_individual.h"
#include "sl_simple_button_instances.h"
#include "start.h"
#include "game_init.h"
#include "game.h"

#define PB1_PORT    gpioPortB
#define PB1_PIN     10

/**
 * @brief Alkalmazás inicializálása.
 *
 * Beállítja a gombot bemenetre, inicializálja az LCD-t és a csúszkát,
 * valamint elindítja a játékot.
 */

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

/**
 * @brief Az alkalmazás folyamatosan futó akcióciklusa.
 *
 * Ezt a függvényt a fő ciklus hívja, és itt futtatja a játékot.
 */

void app_process_action(void)
{
  game();
  while(1);
}
