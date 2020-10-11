/*
 *  Functions to Cycle & Change Colours
 * 
 * 
 * 
 */


void cycleColour(){    // Test Function to Cycle Colour of countdown clock periodically

  

    colourSelect++;                                     // increment current colour - cycle through colours
    if (colourSelect > 2) {                              // reset when reach the end of array
      colourSelect = 0;
    }


     // Just a way of testing the colour changing function through each cycle
    currentColour =  colourArray[colourSelect];            // pass currentColour the array containing data structure containing
    // colour data. Can be used in response to other events
    // in response to other events. e.g. when value turns negative
    // to positive.

  
}
