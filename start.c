/**
 * @file start.c
 * @brief Nehézségi szint kiválasztása a játék kezdetén.
 *
 * A játékos a csúszka segítségével választhat nehézségi szintet, amit
 * az LCD kijelzőn is megjelenítünk.
 */


#include <caplesense.h>
#include <em_gpio.h>
#include <segmentlcd.h>

#define PB1_PORT    gpioPortB
#define PB1_PIN     10

/**
 * @brief Nehézségi szint kiválasztása.
 *
 * A függvény megvárja, míg a játékos a csúszka segítségével kiválasztja a
 * nehézségi szintet, majd visszatér az értékkel.
 *
 * @return int A kiválasztott nehézségi szint (1-5).
 */

int start() {
    int difficulty = 1;

    // Bekapcsoljuk a COL10 (:) szegmenst
    SegmentLCD_Symbol(LCD_SYMBOL_COL10, 1);

    // Nehézségi szint választása, a PB1 gomb lenyomására vár
    while (GPIO->P[PB1_PORT].DIN & (1 << PB1_PIN)) {
        int sliderPos = CAPLESENSE_getSliderPosition();

        // Ha a slidert megérintjük, frissítjük a nehézségi szintet
        if (-1 != sliderPos) {
            difficulty = (sliderPos / 12) + 1;  // Skála 1-5 között

            // Megjelenítjük az új nehézségi szintet az LCD-n
            SegmentLCD_Number(difficulty);
        } else {
            // Ha nincs érintés, az alapértelmezett nehézségi szint jelenik meg
            SegmentLCD_Number(difficulty);
        }
    }
    return difficulty;
}
