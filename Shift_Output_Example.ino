#include "Arduino.h"
#include "Shift_Output.h"

#define LATCH_PIN 0
#define CLOCK_PIN 1
#define DATA_PIN 2
#define NUM_OUTPUTS 22 // every 8 outputs requires a new shift register, so this instance would require 3

Shift_Output shiftOutput(LATCH_PIN, CLOCK_PIN, DATA_PIN, NUM_OUTPUTS);

void setup(){
    Serial.begin(115200);
    shiftOutput.begin();
    shiftOutput.changeUpdateTime(500); // changes update time. Defaults to 1000ms.
                                       // note that the objects will update immediately without update
                                       // update only resends the previously sent data every second to be safe
}

void loop(){

    shiftOutput.write(21, HIGH); // writes the 6th pin of the 3rd shift register high

    shiftOutput.write(3, 6, HIGH); // does the same as the above

    shiftOutput.writeRegister(0, 0b00001011); // writes the output state for the entire shift register
                                                       // would turn pins 1, 2, and 4 high, everything else low
                                                       // 0b00001011 is equivalent to decimal 11, but is in binary for easier comprehension

    shiftOutput.writeAllLow(); // does what it sounds like

    shiftOutput.update(); // leave in main loop. Isn't strictly necessary, but adds some
                       // "safety" in case there is an errant shift out.
                       // gets called every 1000ms by default
}