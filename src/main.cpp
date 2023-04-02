/*
Copyright (c) 2023 Timothy Hutchins

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/

#include <Arduino.h>
#include "Relay.hpp"

Relay relay(3);

ISR(RTC_CNT_vect) {
    Serial.println("Counter Inturrupt");
    RTC.INTFLAGS |= 0x02;
    RTC.CNT = 0;
}

void setup() {
    Serial.begin(115200);
    // Disable interupts while we configure them
    noInterrupts();

    // Sets the desired clock / oscillator to use
    // for the RTC
    // See 22.4.1.1
    RTC.CLKSEL = 0x00;
    
    // First sets the RTC to run in standby
    // Sets the prescalar
    // And then sets the frequency correction bit
    // See 22.13.1
    RTC.CTRLA = 0b10000000 | (0x8 << 3) | 0x04;

    // Sets the type of interupt to use
    // Compare Match Interrupt (CMP) (bit two)
    // See 22.13.3
    RTC.INTCTRL = 0x02;

    // Sets the Compare Match (CMP) value
    RTC.CMP = 127;

    Serial.println("Setup done");

    // Enabled interrupts again
    interrupts();

    // Tries to set the enable bit for the RTC
    while((RTC.CTRLA & 0x01) != 0x01) {
        RTC.CTRLA |= 0x01;
    }
}

void loop() {

}
