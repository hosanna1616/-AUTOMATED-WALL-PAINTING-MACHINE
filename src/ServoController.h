/**
 * @file ServoController.h
 * @brief Servo1 (ceiling position) and Servo2 (angle cycle) control
 */

#ifndef SERVO_CONTROLLER_H
#define SERVO_CONTROLLER_H

#include <Arduino.h>
#include <Servo.h>
#include "Config.h"

class ServoController {
public:
    void begin();

    void moveServo1ToCeiling();
    void cycleServo2Position();

    uint8_t getServo1Angle() const;
    uint8_t getServo2Angle() const;
    uint8_t getServo2Index() const;

private:
    bool setServoAngle(Servo& servo, uint8_t& currentAngle, uint8_t target,
                       uint8_t minAngle, uint8_t maxAngle);

    Servo servo1_;
    Servo servo2_;

    uint8_t servo1Angle_;
    uint8_t servo2Angle_;
    uint8_t servo2Index_;
};

#endif // SERVO_CONTROLLER_H
