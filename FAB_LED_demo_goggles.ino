////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
/// Fast Adressable Bitbang LED Library example
/// Copyright (c)2018 Dan Truong
///
/// Fiber optic goggle illumination with 1m cycle rotating color. The goggles
/// are built with a fiber optic surround and one GRB LED on each end. This
/// program keeps the 2 LEDs colors sync'ed.
///
////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////
#include <FAB_LED.h>

/// This parameter says how many LEDs will be lit up in your strip.
const uint8_t numPixels = 2;
/// This gives a single color's max brightness. The LED will always be twice
/// as bright as maxBrightness because wheel keeps one of the colors full on.
const uint8_t minBrightness = 0;
const uint8_t maxBrightness = 255;

// We define the color with the LED strip declaration to match so that it
// can be changed in that one spot for the whole program.
// ws2812b uses grb. apa102 uses rgb, etc.
#define color grb
//ws2812b<B,4>  strip; // AtTiny85 port B4 - pin 4
ws2812b<D,6>  strip; // Uno port D6 - pin 6

// In memory copy of the LED strip
color pixels[numPixels];

// The reference color to put in  the strip
static color mainColor = {maxBrightness, maxBrightness, minBrightness};

////////////////////////////////////////////////////////////////////////////////
// Helper: Sets a single color over the whole strip
////////////////////////////////////////////////////////////////////////////////
static inline void setColor(color mainColor, uint8_t numPixels, color * pixelPt)
{
  uint8_t i;
  for(i = 0; i< numPixels; i++)
    pixelPt[i] = mainColor;
}


////////////////////////////////////////////////////////////////////////////////
// Helper: Rotates the colors. Instead of a rainbow with a max brightness of 255
// this wheel always has a brightness of 512, so instead of RGB rotation it's
// like Cyan-Yellow-Magenta rotations.
////////////////////////////////////////////////////////////////////////////////
static inline void wheel(color & c)
{
  // change to the next color loop when g reaches max
  if ((c.r == maxBrightness) && (c.g != maxBrightness)) {
    c.g++;
    c.b--;
  }
  // change to the next color loop when b reaches max
  if ((c.g == maxBrightness) && (c.b != maxBrightness)) {
    c.b++;
    c.r--;
  }
  // We don't need to check r because it's done above!
  if (c.b == maxBrightness) {
    c.r++;
    c.g--;
  }
}



////////////////////////////////////////////////////////////////////////////////
/// Setup: Automatically called once when the board boots.
////////////////////////////////////////////////////////////////////////////////
void setup()
{
  // Let's just erase the LED strip of any rogue data, and go past the last LED
  strip.clear(numPixels+1);
}

////////////////////////////////////////////////////////////////////////////////
/// Main loop: Automatically called repeatedly after setup has run once.
////////////////////////////////////////////////////////////////////////////////
void loop()
{
  // Copy the color in the in-memory pixel array
  setColor(mainColor,  numPixels,  pixels);

  // Send the pixel array to display on the LED strip
  strip.draw(numPixels, pixels);

  // Pick the next color
  wheel(mainColor);

  // Sleep: About 1min full cycle -> 235/1000 * 255 = 60s
  delay(235);
}
