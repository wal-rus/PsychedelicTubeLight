#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 60

#define DATA_PIN 13

#define BUTTON_PIN 1 // Interrupt 1 is pin 3

// Define the array of leds
CRGB leds[NUM_LEDS];

volatile int mode = 0;
volatile boolean interrupt = false;

void setup() {
  LEDS.addLeds<WS2811, DATA_PIN, GBR>(leds, NUM_LEDS);  
  //attachInterrupt(BUTTON_PIN, update, RISING);
}

/*
 * Prismatic pulsing as if opal was liquid.
 */
#define OPAL_SPARKLE 0

void opalSparkle() {
  opal(1);
}

/*
 * Slow motion version of opalSparkle, kind of like a nebula in a tube.
 */
#define OPAL_SLOW 1 
 
void opalSlow() {
  opal(0.1);
}

/*
 * Similar in apprence to the gemstone opal
 */
 
void opal(float flow) {
  for (float c = 0; c <= 255; c += flow) {
    if (interrupt) {
      interrupt = !interrupt;
      return;
    }    
    
    for (int i = 0; i < NUM_LEDS; i++) {  
      leds[i] = CHSV((int)(c + i * 32) % 255, 255, 255);
    }
    
    FastLED.show();
  }      
}

/*
 * Multiple rainbow waves in one tube.
 */
#define RAINBOW_WAVES 2

void rainbowWaves() {
  rainbow(5, 0.5);
}

/*
 * One huge pulse of color.
 */
#define RAINBOW_PULSE 3
 
void rainbowPulse() {
  rainbow(1, 0.75);
}

/*
 * Makes a rainbow.
 */
void rainbow(float scale, float flow) {
  for (float c = 0; c <= 255; c += flow) {
    if (interrupt) {
      interrupt = !interrupt;
      return;
    }      

    for (int i = 0; i < NUM_LEDS; i++) {  
      leds[i] = CHSV((int)(c + i * scale) % 255, 255, 255);      
    }
    
    FastLED.show();
  }      
}

/*
 * Fade entire tube though the spectrum
 */
#define RAINBOW_CYCLE 4
 
void rainbowCycle() {
  for (int c = 0; c <= 255; c++) {
    if (interrupt) {
      interrupt = !interrupt;
      return;
    }      

    for (int i = 0; i < NUM_LEDS; i++) {  
      leds[i] = CHSV(c, 255, 255);      
    }
    
    FastLED.show();
    
    delay(50);
  }      
}

/*
 * Blink between red, blue and green very fast.
 */
#define BAD_ACID 5
 
void badAcid() {
  int blinkDelay = 7;
  
  if (interrupt) {
    interrupt = !interrupt;
    return;
  }      

  for (int i = 0; i < NUM_LEDS; i++) {  
    leds[i] = CRGB(255, 0, 0);      
  }
    
  FastLED.show();
  
  delay(blinkDelay);
  
  for (int i = 0; i < NUM_LEDS; i++) {  
    leds[i] = CRGB(0, 255, 0);      
  }
    
  FastLED.show();
  
  delay(blinkDelay);
  
  for (int i = 0; i < NUM_LEDS; i++) {  
    leds[i] = CRGB(0, 0, 255);      
  }
    
  FastLED.show();  
  
  delay(blinkDelay);
}

#define FLASH_BLUE 6

void flashBlue() {
  flashColor(0x00, 0x00, 0xFF, 250);
}

/*
 * Flash a color.
 */
 
void flashColor(byte r, byte g, byte b, int duration) {
  if (interrupt) {
    interrupt = !interrupt;
    return;
  }      

  for (int i = 0; i < NUM_LEDS; i++) {  
    leds[i] = CRGB(b, g, r);      
  }
    
  FastLED.show();
  
  delay(duration);
  
  for (int i = 0; i < NUM_LEDS; i++) {  
    leds[i] = CRGB(0, 0, 0);      
  }
    
  FastLED.show();
  
  delay(duration);  
}

void fadeWhite() {
  for (int i = 0; i < NUM_LEDS; i++) {  
    leds[i] = CRGB(0, 0, 0);      
  }
    
  FastLED.show();
  
  for (int brightness = 0; brightness < 256; brightness++) {
    for (int i = 0; i < NUM_LEDS; i++) {  
      leds[i] = CRGB(brightness, brightness, brightness);      
    }

    FastLED.show();    
    delay(5);
  }

  for (int brightness = 255; brightness >= 0; brightness--) {
    for (int i = 0; i < NUM_LEDS; i++) {  
      leds[i] = CRGB(brightness, brightness, brightness);      
    }

    FastLED.show();    
    delay(5);
  }
}

void update() {
  fill_solid(&leds[0], NUM_LEDS, CRGB(255, 255, 255));
  FastLED.show();
  delay(100);
  mode++;
  mode %= 7;
  interrupt = true;
}

void loop() {  
  for (int times = 0; times < 127; times++) {
    if (mode == OPAL_SPARKLE) {    
      opalSparkle();
    }
    else if (mode == OPAL_SLOW) {
      opalSlow();
    }
    else if (mode == RAINBOW_WAVES) {
      rainbowWaves();
    }
    else if (mode == RAINBOW_PULSE) {
      rainbowPulse();
    }
    else if (mode == RAINBOW_CYCLE) {
      rainbowCycle();
    }
    else if (mode == BAD_ACID) {
     badAcid();
    }  
    else if (mode == FLASH_BLUE) {
      flashBlue();
    }    
  }

  update();
}

