// Global Variables Go Here.





//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ Colour Variables ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Custom Colour Creator:
// use savedColour Constructor to craete custom colours that can be passed to the display

struct savedColour {             // data structure to save named RGB values that could be passed to functions
  uint8_t r;
  uint8_t g;
  uint8_t b;
};


// Pick and Name Colours here, then add them to the colourArray to make them easily accessable

savedColour  skyBlue = { 0, 170, 255};    // Data structure for "skyBlue" colour as an RGB value

savedColour  offWhite = { 160, 255, 200};      // data structure for an offwhite colour, all LEDs on max (these figures chave been calibrated to produce a cleaner white)

savedColour  yellowOrange = {255, 160, 0};     // Holds Orange Colour as a nice contrast to the blue

savedColour  blackout = {0 , 0 , 0};          // data structure holding "black" - all LEDS off.

savedColour  currentColour;                  // data structure to hold the current LED colour


savedColour colourArray[3] = {skyBlue, offWhite, yellowOrange};     // array to hold different colour data structures. Makes it easy to cycle through them

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~







//'''''''''''' Input Variables ''''''''''''''''''''''''''

// Can be used for switches or switchable options

bool demoMode = false;                              // demo mode for animations // Defaults to OFF

int8_t demoPin = 15;                              // Short this pin to GND on boot up to force demo mode

int8_t demoPinValue;                                // Stores the value of demopin on boot up.


//'''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''''
