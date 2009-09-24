#define F_CPU 16000000UL

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>

/*
 * Initializes the control-registers of the MCU to enable PWM operation
 * on PB1-PB3
 */
void init_pwm(void)
{
  /* Set Timer/Counter 1 Control register A to 10100001
   * That means: Non-Inverting PWM Mode (Bits 7-4)
   *             Phase correct PWM with 8bit resolution (Bits 1-0)
   */
  TCCR1A = (1<<COM1A1) | (1<<COM1B1) | (1<<WGM10);

  /* Set Timer/Counter 1 Control register B to 00001001
   * That means: Fast PWM with 8bit resolution (Bits 4-3)
                 Clock is system clock with no prescaling (Bits 2-0)
   */
  TCCR1B = (0<<WGM12) | (1<<CS10);

  /* Set Compare registers to 0, turning the LEDs off */
  OCR1AH = 0;
  OCR1AL = 0;
  OCR1BH = 0;
  OCR1BL = 0;

  /* Disable all interrupts on Timer/Counter 1 */
  TIMSK = 0;

  /* Set Timer/Counter 2 Control register to 01100001
   * That means: Non-Inverting PWM Mode (bits 5-4)
   *             Phase correct PWM Mode (bits 6,3)
                 Clock is system clock with no prescaling (Bits 2-0)
   */
  TCCR2 = (1<<WGM20) | (1<<COM21) | (1<<CS20);

  /* Set Compare register to 0, turning the LED off */
  OCR2 = 0;

  /* Disable all interrupts on Timer/Counter 2 */
  TIMSK = 0;                 

  /* Configure the pin direction to be output */
  DDRB |= (1<<PB2) | (1<<PB1) | (1<<PB3);
}

// The code from here is mostly testing/for fun and will be replaced
#define STEPS 128
#define DELAY 20

int main(void)
{
  PORTB = 0;
  init_pwm();
  double rs, gs, bs, rt, gt, bt;
  uint8_t r, g, b, i;
  srand(34432);
  rs = 0; gs = 0; bs = 0;
  while (1)
  {
    rt = (rand() * 255.0) / RAND_MAX; gt = (rand() * 255.0) / RAND_MAX; bt = (rand() * 255.0) / RAND_MAX;
    for (i = 0; i < STEPS; i++)
    {
      r = rs + ((rt - rs) * i) / (STEPS-1);
      g = gs + ((gt - gs) * i) / (STEPS-1);
      b = bs + ((bt - bs) * i) / (STEPS-1);
      OCR1AL = r;
      OCR1BL = g;
      OCR2 = b;
      _delay_ms(DELAY);
    }
    rs = rt; gs = gt; bs = bt;
  }
  return 0;
}
