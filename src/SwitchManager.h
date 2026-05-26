/**
 * @file SwitchManager.h
 * @brief Debounced switch reading with edge detection for SW1–SW5
 */

#ifndef SWITCH_MANAGER_H
#define SWITCH_MANAGER_H

#include <Arduino.h>
#include "Config.h"

class SwitchManager {
public:
    void begin();
    void update();

    /** True while switch is stably pressed (LOW with pull-up) */
    bool isPressed(uint8_t switchIndex) const;

    /**
     * Consumes a one-shot press event (stable transition to pressed).
     * Returns true once per valid press; clears until released and pressed again.
     */
    bool wasPressed(uint8_t switchIndex);

    /** True on stable release edge (for diagnostics) */
    bool wasReleased(uint8_t switchIndex);

private:
    static const uint8_t PIN_MAP[SWITCH_COUNT];

    bool     rawState_[SWITCH_COUNT];
    bool     debouncedState_[SWITCH_COUNT];
    bool     lastDebouncedState_[SWITCH_COUNT];
    bool     pressEvent_[SWITCH_COUNT];
    bool     releaseEvent_[SWITCH_COUNT];
    unsigned long lastDebounceTime_[SWITCH_COUNT];

    void debounceSwitch(uint8_t index);
};

#endif // SWITCH_MANAGER_H
