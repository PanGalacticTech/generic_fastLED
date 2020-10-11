/* ~~~~ Fast LED Playground ~~~~

    For testing fastLED library Functions


     Pan Galactic Tech
     11/10/2020

*/

#include <FastLED.h>

#include "globals.h"

#include <autoDelay.h>       // Copy of my own library available @:   https://github.com/PanGalacticTech/autoDelay_Library
#include <buttonObject.h>    // Copy of my own library available @:   https://github.com/PanGalacticTech/buttonObject_Library


FASTLED_USING_NAMESPACE      // I do not Know what This line does



#if defined(FASTLED_VERSION) && (FASTLED_VERSION < 3001000)
#warning "Requires FastLED 3.1 or later; check github for latest code."
#endif

#define DATA_PIN    2                   // Data pin connection to LED string
//#define CLK_PIN   4                      // Not required for 3 pin LED strings

// Uncomment one of these lines depending on LED driver used

#define LED_TYPE    WS2812B                // This must be set to match the specific LED driver used
//#define LED_TYPE WS2811_400               //This must be set to match the specific LED driver used
//#define LED_TYPE    NEOPIXEL              // This must be set to match the specific LED driver used


#define COLOR_ORDER GRB                   // try RGB if RED & GREEN are switched



#define NUM_LEDS 20                           //Number of LED in string         


CRGB ledString[NUM_LEDS];                       // Create fastLED Object

//CRGBArray<NUM_LEDS> ledString;                 // Alternative Method

//uint8_t numberOfLEDs = NUM_LEDS               // alternative methods, depreciated for now

//CRGB leds[NUM_LEDS];                            // alternative methods, depreciated for now

//CRGBSet A(MUN_LEDS, 21);                        // alternative methods, depreciated for now

autoDelay ledDelay;   // Object to manage delay functions for writing data to the LED string

autoDelay animationDelay;  // Delay object to manage the animation for the turn signal

autoDelay demoDelay;  // Delay to change the program state as a demo as no buttonPress yet

uint32_t ledWriteDelay = 10;   // Delay value to control how often the led string is updated   (100ms would update 10 times a second, 200ms updates 5 times a second etc)

uint32_t turnSignalDelay = 30;  // Time delay between turn signal animations

uint32_t switchStates = 7;    // time in seconds to wait between switching states as a demo function





// button Object

#define BUTTON_PIN 4

//#define BUTTON_PULL_HIGH 1    // These are defined in buttonObject.h file
//#define BUTTON_PULL_LOW 0


buttonObject button(BUTTON_PIN, BUTTON_PULL_HIGH);    // Set up instance of buttonObject

// N.B. I havnt tested this function in this program yet, but the library works very well




//@@@@@@@@@@@\\\ Options ///@@@@@@@@@@@@@@@@@@@


uint8_t maxBrightness = 30;                      // Define the max brightness of all LEDs. 0 - 255. 20-50 reccomended for testing: MUST use seperate 5v supply for full brightness operation! Do not use USB power!

//   N.B. on power consumption:
// At full brightness each LED can draw 60 mA

// 60 mA * 126 = 7560 mA = 7.6 A !
// including the 14 extra = 8.4 A
// These LEDs can draw a decent amount of current and require a decent PSU. 10A @ 5v Recommended


int8_t colourSelect = 0;                        // selectes the initial LED colour. 0 = skyBlue, 1 = offwhite, 2 = yellowOrange.

//@@@@@@@@@@@\\\ Options ///@@@@@@@@@@@@@@@@@@@



unsigned long updateDisplayDelay = 1000;  //   (will also be used to slow down printing to the serial monitor during testing)
unsigned long lastDisplayUpdate;        // save the time of the last update






// Some global variables to hold individual RGB values

uint8_t red;
uint8_t green;
uint8_t blue;









// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Setup ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void setup() {


  // Set up and get controller ready


  // tell FastLED about the LED strip configuration
  FastLED.addLeds<LED_TYPE, DATA_PIN, COLOR_ORDER>(ledString, NUM_LEDS).setCorrection(TypicalLEDStrip);            // This sets up our LED string object


  // set master brightness control
  FastLED.setBrightness(maxBrightness);                                                                        // Sets the brightness for the entire string



  button.begin();                                                                                               // Sets up button Object




  // currentColour = skyBlue;                                                                                 // Preset the starting LED colour, can be changed later in program

  currentColour = offWhite;

  for (int i = 0; i < NUM_LEDS; i++) {                                                                          // loop could be used to reset all LEDs to black on startup. Currently depreciated
    ledString[i] = CRGB::Black;
  }

  FastLED.show();

}




// States

// The state Machine has 2 states,

// TURN_SIGNAL_ON
// HEADLIGHT_ON

enum operationMode {     // There are more simple ways to do this, but this makes it easier to expand the state machine and add additional features

  HEADLIGHT_ON = 0,
  TURN_SIGNAL_ON = 1

};


uint8_t currentState = 1;
uint8_t lastState = 0;        // Pre emptive bug fix, if I had more time I could make this neater, but make these different just to make sure it definatly works on startup



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Main Loop ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~


void loop() {


// Button Stuff

  button.buttonLoop(750);   // long int passed controls the time required for a long press. (defaults to 1 second)

  // calling button master returns global variables:

  //pressCount
  //longPressCount
  //releaseCount
  //longPress - bool
  //shortPress - bool

  if (button.shortPress || demoDelay.secondsDelay(switchStates)) {                         // || (or) case here uses a delay to cycle states instead of just waiting for a button press. Remove from || to change to button only

    if (currentState == 0) {
      currentState = 1;
    } else {
      currentState = 0;
    }
    button.buttonReset();     // .buttonReset method resets longPress & shortPress variables once action has been taken
  }



// State Machine

  if (currentState != lastState) {     // If the current State is different from the previous State
    solidColour(blackout);                 // If state has changed set all LEDs black (helps smooth transitions between states)
    
    if (currentState == 0) {           // and that state is 0, then change all the LEDS to white. This only needs to happen when the state changes
      solidColour(offWhite);
    }
    lastState = currentState;      // Save the current state into the lastState for comparason in the next loop
  }

  if (currentState == 1) {               //if we are in "turn Signal Mode" this is a dynamic effect, and so needs to be run in every loop to work.
    turnSignal(yellowOrange);
  }




// fastLED loop Function

  if (ledDelay.millisDelay(ledWriteDelay)) {  // Some delay in between sending data to LED string to avoid flashing/ glitches
    FastLED.show();
  }


}








// ANIMATIONS/ LED FUNCTIONS

// SOLID COLOUR

void solidColour(savedColour inputColour) {                                     // Turns all the LEDs a solid colour

  // to set all LEDS a solid colour
  for (int i = 0; i < NUM_LEDS; i++) {
    ledString[i] = CRGB(inputColour.r, inputColour.g, inputColour.b);                  // print the RGB colour to that segment
  }
}




// MOVING CHASE EFFECT

uint8_t adv = 0;   // variable used to "advance" the animation

void turnSignal(savedColour inputColour) {                                       // Chasing Turn Signal Effect

  if (animationDelay.millisDelay(turnSignalDelay)) {

    
    ledString[adv] =  CRGB(inputColour.r, inputColour.g, inputColour.b);

    adv++;                                                                                 // Advance the led down the string
  }



  if (adv >= NUM_LEDS+2) {                                                                     // If we reach the end of the string (or a specified number of LEDs in)
    adv = 0;                                                                                 // Reset the counter
                                                                      // Turn all the LEDS black ready for another run   
  }



}
