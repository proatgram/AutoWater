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

#include "TimedCall.hpp"

void *Timed::m_function = nullptr;


volatile bool Timed::m_interruptHit = false;

unsigned int Timed::m_numberRepeat = 0;

unsigned int Timed::m_targetRepeats = 0;

void Timed::initializeClock(unsigned int milliseconds) {
    cli();

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
    
    Serial.println((1024 / (256 * (1000.0 / milliseconds))) - 1);

    uint16_t targetCompareValue = static_cast<uint16_t>((1024 / (256 * (1000.0 / milliseconds))) - 1);

    Serial.println(targetCompareValue);

    // Sets the Compare Match (CMP) value
    RTC.CMP = targetCompareValue;

    sei();
}

void Timed::startClock() {
    // Tries to set the enable bit for the RTC to ON
    while((RTC.CTRLA & 0x01) != 0x01) {
        RTC.CTRLA |= 0x01;
    }
}

void Timed::stopClock() {
    // Tries to set the enable bit for the RTC to off
    while((RTC.CTRLA & 0x01) == 0x01) {
        RTC.CTRLA &= (RTC.CTRLA ^ 0x01);
    }
    RTC.INTFLAGS |= 0x02;
    RTC.CNT = 0;
}

void Timed::resetClock() {

    RTC.INTFLAGS |= 0x02;
    RTC.CNT = 0;
}

void Timed::addCallback(void *func) {
    m_function = func;
}

bool Timed::check() {
    if (m_targetRepeats == m_numberRepeat) {
        if (m_function != nullptr) {
            ((void(*)())m_function)();
        }
        return true;
    }
    resetClock();
    m_numberRepeat++;
    return false;
}

void Timed::setInterrupt() {
    m_interruptHit = true;
}

void Timed::clearInterrupt() {
    m_interruptHit = false;
}

ISR(RTC_CNT_vect) {
    Serial.println("Counter Inturrupt");
    Timed::setInterrupt();
    Timed::stopClock();
}