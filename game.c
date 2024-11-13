/**
 * @file game.c
 * @brief A játék logikájának fő funkciói.
 *
 * Ez a fájl kezeli a kacsa megjelenítését, a lövéseket és a találatokat,
 * valamint a játék állapotának kezelését.
 */

#include <segmentlcd.h>
#include <sys/_stdint.h>
#include "segmentlcd_individual.h"
#include "caplesense.h"
#include "sl_simple_button_instances.h"
#include "start.h"
#include "game_over.h"

#define MAX_SEGMENTS 4          /**< Kijelző szegmensek maximális száma */
#define MAX_DUCKS 25            /**< Maximum kacsák száma a játékban */
#define HIT_BLINK_DURATION 100  /**< Találat jelzésének időtartama */

/** Szegmens definíciók. */
SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];

/** Globális változók a játék állapotának nyomon követésére. */
volatile int32_t msTicks;                   /**< 1 ms-os időzítések számlálója */
volatile uint32_t randomTime;               /**< Véletlenszerű idő változó */
volatile uint32_t bulletTime[MAX_SEGMENTS]; /**< Lövedék időzítők */
volatile uint32_t hitBlinkTime;             /**< Találat villogási ideje */
volatile bool hitInProgress;                /**< Találat folyamatban állapot */
volatile bool bullets[MAX_SEGMENTS];        /**< Lövedék állapotok */
volatile bool hits[MAX_SEGMENTS];           /**< Találati állapotok */

bool first;             /**< Első indítás jelző */
bool firstDuck;         /**< Első kacsa megjelenítése */
int sliderPos;          /**< Csúszka pozíció */
int sliderDownsc;       /**< Csúszka skálázott pozíció */
int pre_sliderDownsc;   /**< Előző csúszka pozíció */
int difficulty;         /**< Nehézségi szint */
int duckCounter;        /**< Kacsa számláló */
int score;              /**< Játékos pontszáma */
int duckPosition;       /**< Kacsa pozíció */

/**
 * @brief SysTick megszakítás kezelő.
 *
 * Növeli a msTicks számlálót és kezeli a lövedékek és találatok időzítését.
 */
void SysTick_Handler(void) {
    msTicks++;  // Időzítés számláló
    randomTime++;
    for (int i = 0; i < MAX_SEGMENTS; i++) {
        if (bullets[i]) {
            bulletTime[i]++;
        }
    }
    if (hitInProgress) {
        hitBlinkTime++;
    }
}

/**
 * @brief A játék inicializálása.
 *
 * Beállítja a játék indulási állapotait, és visszaállítja a változókat.
 */
void game_init(void) {
    sliderDownsc = 0;
    pre_sliderDownsc = 0;
    duckCounter = 0;
    score = 0;
    first = true;
    firstDuck = true;
    hitInProgress = false;

    for (int i = 0; i < MAX_SEGMENTS; i++) {
        bulletTime[i] = 0;
        bullets[i] = false;
        hits[i] = false;
    }
}

/**
 * @brief Lövedékek kirajzolása az idő alapján.
 *
 * A lövedékek állapotát és megjelenítését kezeli a nehézségi szint alapján.
 */
void bulletDraw(void) {
    for (int i = 0; i < MAX_SEGMENTS; i++) {
        if (bullets[i]) {
            if (bulletTime[i] <= (uint32_t)((-250 * difficulty + 2000) / 4)) {
                lowerCharSegments[i].p = 1;
                lowerCharSegments[i].j = 0;
            } else if (bulletTime[i] <= (uint32_t)((-250 * difficulty + 2000) / 2)) {
                lowerCharSegments[i].p = 0;
                lowerCharSegments[i].j = 1;
            } else {
                lowerCharSegments[i].p = 0;
                lowerCharSegments[i].j = 0;
                bullets[i] = false;
                bulletTime[i] = 0;
                if (i == duckPosition) {
                    hitInProgress = true;
                    score += 100;
                    hits[i] = true;
                }
            }
        }
    }
    SegmentLCD_LowerSegments(lowerCharSegments);
}

/**
 * @brief Találati effekt megjelenítése.
 *
 * Villogtatja a szegmenseket a találat jelzésére, és visszaállítja az
 * állapotot a villogás után.
 */
void hit(void) {
    if (hitInProgress) {
        for (int i = 0; i < MAX_SEGMENTS; i++) {
            if (hits[i]) {
                if (hitBlinkTime <= HIT_BLINK_DURATION) {
                    lowerCharSegments[i].a = 0;
                } else if (hitBlinkTime <= 2 * HIT_BLINK_DURATION) {
                    lowerCharSegments[i].a = 1;
                } else if (hitBlinkTime <= 3 * HIT_BLINK_DURATION) {
                    lowerCharSegments[i].a = 0;
                } else if (hitBlinkTime <= 4 * HIT_BLINK_DURATION) {
                    lowerCharSegments[i].a = 1;
                } else {
                    lowerCharSegments[i].a = 0;
                    hitInProgress = false;
                    hitBlinkTime = 0;
                    hits[i] = false;
                }
                SegmentLCD_LowerSegments(lowerCharSegments);
            }
        }
    }
}

/**
 * @brief Gomb állapotváltozás kezelő.
 *
 * Ha a gombot megnyomják, beállítja a megfelelő lövedéket.
 *
 * @param handle A gomb objektum mutatója.
 */
void sl_button_on_change(const sl_button_t *handle) {
    if (&sl_button_btn0 == handle) {
        if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED) {
            if (!first) {
                bullets[pre_sliderDownsc] = true;
            }
        }
    }
}

/**
 * @brief Kacsa kirajzolása a kijelzőn.
 *
 * Véletlenszerű pozícióban megjeleníti a kacsát a kijelzőn.
 */
void drawDuck(void) {
    int prevDuckPosition = 0;
    if (firstDuck) {
        duckPosition = randomTime % MAX_SEGMENTS;
        lowerCharSegments[duckPosition].a = 1;
        SegmentLCD_LowerSegments(lowerCharSegments);
        firstDuck = false;
    } else {
        prevDuckPosition = duckPosition;
        while (prevDuckPosition == duckPosition) {
            duckPosition = randomTime % MAX_SEGMENTS;
        }
        lowerCharSegments[duckPosition].a = 1;
        SegmentLCD_LowerSegments(lowerCharSegments);
    }
}

/**
 * @brief Kacsa eltüntetése a kijelzőről.
 *
 * Törli a kijelzőn megjelenő kacsát.
 */
void deleteDuck(void) {
    lowerCharSegments[duckPosition].a = 0;
    SegmentLCD_LowerSegments(lowerCharSegments);
}

/**
 * @brief A játék fő ciklusa.
 *
 * A játékot futtatja, frissíti a képernyőt és kezeli az időzítéseket, találatokat.
 *
 * @return int A játék visszatérési értéke (jelenleg nem használ).
 */
int game(void) {
    difficulty = start();
    SegmentLCD_Symbol(LCD_SYMBOL_COL10, 1);
    while (duckCounter < MAX_DUCKS) {
        duckCounter++;
        SegmentLCD_Number(score + duckCounter);
        drawDuck();
        msTicks = 0;

        while (msTicks < (-250 * difficulty + 2000) || hitInProgress) {
            bulletDraw();
            hit();
            sliderPos = CAPLESENSE_getSliderPosition();
            sliderDownsc = sliderPos / 16;
            if (sliderDownsc != pre_sliderDownsc && sliderPos != -1) {
                lowerCharSegments[pre_sliderDownsc].d = 0;
                lowerCharSegments[sliderDownsc].d = 1;
                SegmentLCD_LowerSegments(lowerCharSegments);
                pre_sliderDownsc = sliderDownsc;
            } else if (first) {
                pre_sliderDownsc = 0;
                first = false;
                lowerCharSegments[pre_sliderDownsc].d = 1;
                SegmentLCD_LowerSegments(lowerCharSegments);
            }
        }
        deleteDuck();
    }
    game_over();
    return 0;
}
