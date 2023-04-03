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

#pragma once

#include <Arduino.h>
#include <avr/interrupt.h>

class Timed {
    public:
        
        Timed() = delete;

        ~Timed() = delete;

        Timed(const Timed& other) = delete;

        Timed(Timed&& other) = delete;

        static void initializeClock(unsigned int milliseconds);

        static void startClock();

        static void stopClock();

        static void resetClock();

        static void addCallback(void *func);

        static bool check();

        static void setInterrupt();

        static void clearInterrupt();

    private:
        
        static void *m_function;

        volatile static bool m_interruptHit;

        static unsigned int m_numberRepeat;

        static unsigned int m_targetRepeats;
};
