#include "Relay.hpp"

Relay::Relay(int relayPin) :
    m_relayPin(m_relayPin)
{
    pinMode(relayPin, OUTPUT);
}

void Relay::trigger(PinStatus trigger) {
    m_state = trigger;
    digitalWrite(m_relayPin, trigger);
}

void Relay::toggle() {
    m_state = m_state ? LOW : HIGH;
    digitalWrite(m_relayPin, m_state);
}
