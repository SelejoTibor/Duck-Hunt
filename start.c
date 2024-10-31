#include <caplesense.h>
#include <em_gpio.h>
#include <segmentlcd.h>

#define PB1_PORT    gpioPortB
#define PB1_PIN     10

int start()
{
  int difficulty = 1;
  while(GPIO->P[PB1_PORT].DIN & (1 << PB1_PIN))
    {
      int sliderPos = CAPLESENSE_getSliderPosition();
      if( -1 != sliderPos)
        {
          difficulty = (sliderPos/12);
          SegmentLCD_Number((sliderPos/12)+1);
      }
  }
  return difficulty;
}
