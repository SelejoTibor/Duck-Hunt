/**
 * @file game_over.c
 * @brief Játék vége kijelzés.
 *
 * A játék végén törli a kijelzőt, és ezzel jelzi a befejezést.
 */

#include "segmentlcd_individual.h"

/**
 * @brief Játék vége.
 *
 * A kijelző tisztításával jelzi a játék végét.
 */

extern SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];

void game_over(void)
{
  for(int i =0; i < 4; i++)
    {
      lowerCharSegments[i].raw = 0;
      SegmentLCD_LowerSegments(lowerCharSegments);
    }
}
