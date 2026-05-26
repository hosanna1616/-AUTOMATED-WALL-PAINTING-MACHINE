/**
 * @file MotorController.h
 * @brief Wheel and drum DC motor control with timeout protection
 */

#ifndef MOTOR_CONTROLLER_H
#define MOTOR_CONTROLLER_H

#include <Arduino.h>
#include "Config.h"

enum class DrumMode : uint8_t {
    IDLE = 0,
    WRAP,
    RELEASE
};

class MotorController {
public:
    void begin();

    void update();

    // Wheel motor
    void wheelForward();
    void wheelStop();
    void wheelRightTurnStart();
    bool isWheelTurnActive() const;

    // Drum motor
    void drumToggleWrapRelease();
    void drumStop();
    DrumMode getDrumMode() const;
    const char* drumModeString() const;

    bool isMotorTimeoutActive() const;
    void clearMotorTimeout();

private:
    void setWheelPins(bool in1, bool in2);
    void setDrumPins(bool in1, bool in2);
    void refreshMotorActivity();
    void checkMotorTimeout();

    bool wheelRunning_;
    bool drumRunning_;
    bool wheelTurnActive_;
    unsigned long wheelTurnEndTime_;

    DrumMode drumMode_;

    unsigned long lastMotorActivityMs_;
    bool motorTimeoutTriggered_;
};

#endif // MOTOR_CONTROLLER_H
