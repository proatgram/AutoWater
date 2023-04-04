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

void (*Timed::m_function)() = nullptr;


volatile bool Timed::m_interruptHit = false;

unsigned long long int Timed::m_targetNumber = 0;

void Timed::initializeClock(unsigned int milliseconds) {
    // Turns off interrupts to configure them.
    cli();

    // Sets the desired clock / oscillator to use
    // for the RTC
    // 0x00 Internal 32768 Hz
    // 0x01 Internal 1024 Hz
    // 0x02 32768 Mz from XOSC32K or from external clock from TOSC1
    // 0x03 External clock from EXTCLK pin
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
    
    // Clock 0 in RTC.CLKSEL is 32,768Mz
    // Prescalar of 256
    // Max seconds that can store is 512 (512000 milliseconds)
    unsigned long long int targetCompareValue = static_cast<uint16_t>((32768 / (256 * (1000.0 / milliseconds))) - 1);

    if (targetCompareValue < 65535) {
        // Sets the Compare Match (CMP) value
        RTC.CMP = targetCompareValue;
        sei();
        return;
    }
    m_targetNumber = targetCompareValue - 65535;
    RTC.CMP = 65535;
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
    // Sets the interrupt clear bit in the interrupt flags register
    RTC.INTFLAGS |= 0x02;

    // Resets the count
    RTC.CNT = 0;
}

void Timed::addCallback(void (*func)()) {
    m_function = func;
}

bool Timed::check() {
    if (m_interruptHit) {
        if (m_targetNumber == 0) {
            if (m_function != nullptr) {
                m_function();
            }
            clearInterrupt();
            resetClock();
            return true;
        }
        clearInterrupt();
        resetClock();
        if (m_targetNumber <= 65535) {
            RTC.CMP = static_cast<uint16_t>(m_targetNumber);
            startClock();
            return false;
        }
        RTC.CMP = 65535;
        m_targetNumber -= 65535;
        startClock();
    }
    return false;
}

void Timed::setInterrupt() {
    m_interruptHit = true;
}

void Timed::clearInterrupt() {
    m_interruptHit = false;
}

ISR(RTC_CNT_vect) {
    Timed::setInterrupt();
    Timed::stopClock();
}
