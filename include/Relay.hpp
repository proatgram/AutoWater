#pragma once
#include <Arduino.h>

/**
 * A class to represent a relay
 *
 */
class Relay {
    public:
        /**
         * @brief Constructs a new `Relay` object
         *
         * @param relayPin The pin to pull to activate the relay
         *
         */
        Relay(int relayPin);

        /**
         * Triggers the relay
         *
         * @param trigger Writes to the relay
         *
         * @return void
         *
         */
        void trigger(PinStatus trigger);

        /**
         * Toggles the relay
         *
         * @return void
         *
         */
        void toggle();
    private:

        int m_relayPin;

        PinStatus m_state;
};
