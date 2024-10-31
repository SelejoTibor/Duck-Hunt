#include <segmentlcd.h>
#include <sys/_stdint.h>
#include "segmentlcd_individual.h"
#include "caplesense.h"
#include "sl_simple_button_instances.h"
#include "start.h"

SegmentLCD_LowerCharSegments_TypeDef lowerCharSegments[SEGMENT_LCD_NUM_OF_LOWER_CHARS];

volatile uint32_t msTicks; /* counts 1ms timeTicks */
volatile uint32_t randomTime;
volatile uint32_t bulletTime[4];
volatile bool bullets[4];
bool first;
bool firstDuck;
int sliderPos;
int sliderDownsc;
int pre_sliderDownsc;
int difficulty;
int duckCounter;
int score;
int p;

void SysTick_Handler(void)
{
  msTicks++;       /* increment counter necessary in Delay()*/
  randomTime++;
  for(int i = 0; i < 4; i++)
    {
      if(true == bullets[i])
        {
          bulletTime[i]++;
        }
    }
}

void game_init(void)
{
  sliderDownsc=0;
  pre_sliderDownsc=0;
  duckCounter = 0;
  score = 0;
  first = true;
  firstDuck = true;
  for(int i = 0; i < 4; i++ )
    {
      bulletTime[i] = 0;
      bullets[i] = false;
    }
}


/***************************************************************************//**
 * @brief Delays number of msTick Systicks (typically 1 ms)
 * @param dlyTicks Number of ticks to delay
 ******************************************************************************/
/*
void Delay(int dlyTicks)
{
  uint32_t curTicks;

  curTicks = msTicks;
  while ((msTicks - curTicks) < dlyTicks) ;
}
 */

void bulletDraw(void)
{
  for(int i = 0; i < 4; i++ )
    {
      if(true == bullets[i])
        {
          if(bulletTime[i] <= (uint32_t)((-250*difficulty+2000)/4))
            {
              lowerCharSegments[i].p = 1;
              lowerCharSegments[i].j = 0;
            }
          else if((bulletTime[i] > (uint32_t)((-250*difficulty+2000)/4)) && bulletTime[i] <= (uint32_t)((-250*difficulty+2000)/2))
            {
              lowerCharSegments[i].p = 0;
              lowerCharSegments[i].j = 1;
            }
          else if((bulletTime[i] > (uint32_t)((-250*difficulty+2000)/2)))
            {
              lowerCharSegments[i].p = 0;
              lowerCharSegments[i].j = 0;
              bullets[i] = false;
            }
        }
      if(false == bullets[i])
        {
          bulletTime[i] = 0;
        }
    }
  SegmentLCD_LowerSegments(lowerCharSegments);
}

void hit(void){
  //TODO: vijúviju (kacssa villog)
}

void sl_button_on_change(const sl_button_t *handle)
{
  if (&sl_button_btn0 == handle)
    {
      if (sl_button_get_state(handle) == SL_SIMPLE_BUTTON_PRESSED)
        {
          if(!first)
            {
              bullets[pre_sliderDownsc] = true;
            }
        }
      else
        {
          return;
        }
    }
}

void drawDuck(void)
{
  int q = 0;
  if(firstDuck)
    {
      p = randomTime % 4;
      lowerCharSegments[p].a = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
      firstDuck = false;
    }
  else
    {
      q = p;
      while(q == p)
        {
          p = randomTime % 4;
        }
      lowerCharSegments[p].a = 1;
      SegmentLCD_LowerSegments(lowerCharSegments);
    }
}

void deleteDuck(void)
{
  lowerCharSegments[p].a = 0;
  SegmentLCD_LowerSegments(lowerCharSegments);
}


int game()
{
  difficulty = start();
  while(duckCounter<25)
    {
      /*Random kacsa és nehézség */
      duckCounter++;
      SegmentLCD_Number(score+duckCounter);
      drawDuck();
      msTicks = 0;
      while(msTicks<-250*difficulty+2000)
        {
          bulletDraw();
          sliderPos = CAPLESENSE_getSliderPosition();
          sliderDownsc = sliderPos/16;
          if((first || sliderDownsc!=pre_sliderDownsc)&&sliderPos!=-1)
            {
              first = false;
              lowerCharSegments[pre_sliderDownsc].d = 0;
              lowerCharSegments[sliderDownsc].d = 1;
              SegmentLCD_LowerSegments(lowerCharSegments);
              pre_sliderDownsc=sliderDownsc;
            }
        }
      deleteDuck();
    }
  return 0;
}


