/*
  Keyboard Message test
  For the Adafruit Neo Trinkey
  Sends a text string when a one touchpad is pressed, opens up Notepad if the other is pressed
*/

#include <Adafruit_NeoPixel.h>
#include "Adafruit_FreeTouch.h"
#include "Keyboard.h"

// Create the neopixel strip with the built in definitions NUM_NEOPIXEL and PIN_NEOPIXEL
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_NEOPIXEL, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// Create the two touch pads on pins 1 and 2:
Adafruit_FreeTouch qt_1 = Adafruit_FreeTouch(1, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);
Adafruit_FreeTouch qt_2 = Adafruit_FreeTouch(2, OVERSAMPLE_4, RESISTOR_50K, FREQ_MODE_NONE);

#define NUM_LAYERS 3
#define EXTERNAL_MACROS
//#define EN_LAYERS

uint8_t current_layer = 0;
uint8_t current_macro = 0;
uint32_t current_color = 0xFF0000;
uint8_t color_wheel = 0;
bool color_direction = true;// false == down
#ifndef EXTERNAL_MACROS
const char *macros[NUM_LAYERS][4] = {
  {"macroa1","macroa2","macroa3","macrosa4"},
  {"macrob1","macrob2","macrob3","macrob4"},
  {"macroc1","macroc2","macroc3","macroc4"}
};
#else
#include "macros.h"
#endif

bool Touch1State = false;
bool Touch2State = false;
bool previousTouch1State = false;   // for checking the state of touch 1
bool previousTouch2State = false;   // for checking the state of touch 1

#ifdef EN_LAYERS
bool TouchCombState = false;
bool previousTouchCombState = false;   // for checking the state of touch 1
#endif

void setup() {
  Serial.begin(9600);
  //while (!Serial) delay(10);
  
  strip.begin(); // start pixels
  strip.setBrightness(20); // not too bright!
  strip.show(); // Initialize all pixels to 'off'

  if (! qt_1.begin())  
    Serial.println("Failed to begin qt on pin 1");
  if (! qt_2.begin())  
    Serial.println("Failed to begin qt on pin 2");
    
  // initialize control over the keyboard:
  Keyboard.begin();

  // set initial macro indicator (led) color
  strip.setPixelColor(current_macro, current_color);
  
}

void loop() {
  
  // measure the captouches
  uint16_t touch1 = qt_1.measure();
  uint16_t touch2 = qt_2.measure();
  // whether we think they are being touched
  
  // print out the touch readings
  if( touch1 > 500 || touch2 > 500 )
  {
    Serial.print("QT 1: "); Serial.print(touch1);
    Serial.print("\t\tQT 2: "); Serial.println(touch2);
  }

#ifdef EN_LAYERS
  // if both buttons touched together
  if( touch1 > 500 && touch2 > 500 )
  {
    // set flag to true
    TouchCombState = true;
    Serial.println("Both Buttons Pressed");
  } 
  else if( touch1 < 250 && touch2 < 250 && TouchCombState )
  {
    // set flag to true
    TouchCombState = false;
    Serial.println("Both Buttons Released");
  } 
#endif
  // If the first pad is touched
  if (touch1 > 500 && !TouchCombState) {
    Touch1State = true;  // is touched
    Serial.println("Button 1 Pressed");
  } else if(touch1 < 250 && Touch1State)  {
    // turn it off
    Touch1State = false;  // is not touched      
    Serial.println("Button 1 Released");
  }

  // If the second pad is touched
  if (touch2 > 500  && !TouchCombState) {
    Touch2State = true;  // is touched
    Serial.println("Button 2 Pressed");
  } else if(touch2 < 250 && Touch2State)  {
    // turn it off
    Touch2State = false;  // is not touched
    Serial.println("Button 2 Released");
  }

#ifdef EN_LAYERS
  // if both buttons are pressed and they weren't before...
  if(TouchCombState && !previousTouchCombState)
  {
    current_layer = current_layer + 1;    
    if( current_layer > NUM_LAYERS ) current_layer = 0;
    current_macro = 0;
    Serial.print("Changing layer to: "); Serial.println(current_layer);
  } 
#endif    
  // If we are now touching #1, and we weren't before...
  if (Touch1State && !previousTouch1State) {
  //  touch1counter++; // increment the touch counter
    // type out a message
    Serial.print("Typing macro: "); Serial.print(current_macro);
    Serial.print(" On Layer: "); Serial.println(current_layer);
    Keyboard.print(macros[current_layer][current_macro]);
  }  
  // If we are now touching #2, and we weren't before...
  if (Touch2State && !previousTouch2State) {
    strip.setPixelColor(current_macro, 0x00);
    ++current_macro;
    if( current_macro > 3) current_macro = 0;
    strip.setPixelColor(current_macro, current_color);
    Serial.print("switching Macro to "); Serial.println(current_macro);
    // change color here?
  }
  // save the current touch states for comparison next time:
  previousTouch1State = Touch1State;
  previousTouch2State = Touch2State;
  previousTouchCombState = TouchCombState;

    // set color and show pixels
  rotate_color();
  delay(10);  // and pause a moment
}

void rotate_color(void)
{
  // clear pixels
  for(int i=0;i<4;i++)
  {
    strip.setPixelColor(i, 0x00);
  }

#ifdef EN_LAYERS
  // set pixel color
  if(current_layer == 0)
  {
    current_color = strip.Color(255,0,color_wheel);
  }
  else if(current_layer == 1)
  {
    current_color = strip.Color(0,255,color_wheel);
  }
  else if(current_layer == 2)
  {
    current_color = strip.Color(color_wheel,0,255);
  }
#else
  current_color = strip.Color(0,255,color_wheel);
#endif

  // whether we are going up or down
  if( color_direction )
  {
    //going up, increasing color
    color_wheel = color_wheel + 1;
    // paste end of color, switching direction
    if(color_wheel > 220) color_direction = false;
  } else {
    //going down, decreasing color
    color_wheel = color_wheel - 1;
    // paste end of color in the other direction, switch direction
    if(color_wheel == 20) color_direction = true;
  }

  strip.setPixelColor(current_macro, current_color);
  strip.show(); // display pixel changes
}
