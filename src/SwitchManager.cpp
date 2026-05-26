/**
 * @file SwitchManager.cpp
 */

#include "SwitchManager.h"

const uint8_t SwitchManager::PIN_MAP[SWITCH_COUNT] = {
    PIN_SW1, PIN_SW2, PIN_SW3, PIN_SW4, PIN_SW5
};

void SwitchManager::begin() {
    for (uint8_t i = 0; i < SWITCH_COUNT; i++) {
        pinMode(PIN_MAP[i], INPUT_PULLUP);
        rawState_[i] = HIGH;
        debouncedState_[i] = HIGH;
        lastDebouncedState_[i] = HIGH;
        pressEvent_[i] = false;
        releaseEvent_[i] = false;
        lastDebounceTime_[i] = 0;
    }
}

void SwitchManager::update() {
    for (uint8_t i = 0; i < SWITCH_COUNT; i++) {
        debounceSwitch(i);
    }
}

void SwitchManager::debounceSwitch(uint8_t index) {
    bool reading = digitalRead(PIN_MAP[index]);

    if (reading != rawState_[index]) {
        lastDebounceTime_[index] = millis();
        rawState_[index] = reading;
    }

    if ((millis() - lastDebounceTime_[index]) > DEBOUNCE_DELAY_MS) {
        if (reading != debouncedState_[index]) {
            debouncedState_[index] = reading;

            // Pressed = LOW with INPUT_PULLUP
            if (debouncedState_[index] == LOW && lastDebouncedState_[index] == HIGH) {
                pressEvent_[index] = true;
            }
            if (debouncedState_[index] == HIGH && lastDebouncedState_[index] == LOW) {
                releaseEvent_[index] = true;
            }

            lastDebouncedState_[index] = debouncedState_[index];
        }
    }
}

bool SwitchManager::isPressed(uint8_t switchIndex) const {
    if (switchIndex >= SWITCH_COUNT) {
        return false;
    }
    return debouncedState_[switchIndex] == LOW;
}

bool SwitchManager::wasPressed(uint8_t switchIndex) {
    if (switchIndex >= SWITCH_COUNT) {
        return false;
    }
    if (pressEvent_[switchIndex]) {
        pressEvent_[switchIndex] = false;
        return true;
    }
    return false;
}

bool SwitchManager::wasReleased(uint8_t switchIndex) {
    if (switchIndex >= SWITCH_COUNT) {
        return false;
    }
    if (releaseEvent_[switchIndex]) {
        releaseEvent_[switchIndex] = false;
        return true;
    }
    return false;
}
