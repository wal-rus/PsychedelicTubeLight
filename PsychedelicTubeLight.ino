#include "FastLED.h"

// How many leds in your strip?
#define NUM_LEDS 59

#define DATA_PIN 13

#define BUTTON_PIN 1 // Interrupt 1 is pin 3

// Define the array of leds
CRGB leds[NUM_LEDS];

volatile int mode = 0;
volatile boolean interrupt = false;

char wrapLEDIndex(char index) {
  while(index < 0) index += NUM_LEDS;
  index %= NUM_LEDS;
  return index;
}

///////////Basic Generators/////////////////
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

///////////////Lighting programs (Lighters)////////////////////////////
/*
 * Prismatic pulsing as if opal was liquid.
 */
void opalSparkle() {
  opal(1);
}

/*
 * Slow motion version of opalSparkle, kind of like a nebula in a tube.
 */
void opalSlow() {
  opal(0.1);
}

/*
 * Multiple rainbow waves in one tube.
 */
void rainbowWaves() {
  rainbow(5, 0.5);
}

/*
 * One huge pulse of color.
 */
void rainbowPulse() {
  rainbow(1, 0.75);
}

/*
 * Fade entire tube though the spectrum
 */
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

void chasers() {
  const int TRAIL_SIZE = 10;
  
  for (uint8_t tip_pos = 0; tip_pos < NUM_LEDS; tip_pos++) {
    if (interrupt) {
      interrupt = !interrupt;
      return;
    }    
    
    for(uint8_t trail_element = 0; trail_element <= TRAIL_SIZE; trail_element++) {
      const uint8_t led_index = wrapLEDIndex(tip_pos-trail_element);
      const uint8_t led_brightness = 255-(trail_element * (255/TRAIL_SIZE));
      leds[led_index] = CRGB(led_brightness,0,0);
    }
    leds[59] = CRGB(0,0,255);
    FastLED.show();
    delay(10);
  }
}

typedef void (*lightingFunction_t)();

const static int NUM_LIGHTERS = 6;
const lightingFunction_t lighters[] = { &chasers, &opalSparkle, &opalSlow, &rainbowWaves, &rainbowPulse, &rainbowCycle };

void setup() {
  LEDS.addLeds<WS2811, DATA_PIN, BGR>(leds, NUM_LEDS);  
  //attachInterrupt(BUTTON_PIN, update, RISING);
}

void update() {
  fill_solid(&leds[0], NUM_LEDS, CRGB(255, 255, 255));
  FastLED.show();
  delay(100);
  mode++;
  mode %= NUM_LIGHTERS;
  interrupt = true;
}

void loop() {  
  for (int times = 0; times < 127; times++) {
    lighters[mode]();
  }

  update();
}

