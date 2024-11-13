/**
 * @file main.c
 * @brief A főprogram indítása és vezérlése.
 *
 * A program inicializálja az eszközt és elindítja az alkalmazást.
 * Ha a kernel jelen van, akkor az indítja el a feladatokat.
 */

#include "sl_component_catalog.h"
#include "sl_system_init.h"
#include "app.h"
#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
#include "sl_power_manager.h"
#endif
#if defined(SL_CATALOG_KERNEL_PRESENT)
#include "sl_system_kernel.h"
#else // SL_CATALOG_KERNEL_PRESENT
#include "sl_system_process_action.h"
#endif // SL_CATALOG_KERNEL_PRESENT

/**
 * @brief A fő belépési pont.
 *
 * Ez a függvény inicializálja a rendszer komponenseit, majd elindítja az
 * alkalmazást.
 *
 * @return int Hibakód visszaadásához (jelenleg nem használ).
 */

int main(void)
{
  sl_system_init();
  app_init();

#if defined(SL_CATALOG_KERNEL_PRESENT)
  sl_system_kernel_start();
#else
  while (1) {
      sl_system_process_action();
      app_process_action();

#if defined(SL_CATALOG_POWER_MANAGER_PRESENT)
      sl_power_manager_sleep();
#endif
  }
#endif
}
