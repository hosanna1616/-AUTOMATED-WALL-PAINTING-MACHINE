/**
 * @file ServoController.cpp
 */

#include "ServoController.h"

// Servo2 cycle: 0, 45, 90, 135, 180 (per specification)
static const uint8_t SERVO2_POSITIONS[SERVO2_POSITION_COUNT] = {
    0, 45, 90, 135, 180
};

void ServoController::begin() {
    servo1_.attach(PIN_SERVO1);
    servo2_.attach(PIN_SERVO2);

    servo1Angle_ = 0;
    servo2Angle_ = SERVO2_POSITIONS[0];
    servo2Index_ = 0;

    setServoAngle(servo1_, servo1Angle_, 0, SERVO1_MIN_ANGLE, SERVO1_MAX_ANGLE);
    setServoAngle(servo2_, servo2Angle_, SERVO2_POSITIONS[0],
                  SERVO2_MIN_ANGLE, SERVO2_MAX_ANGLE);

    Serial.println(F("[SERVO] Initialized Servo1=0 deg, Servo2=0 deg"));
}

void ServoController::moveServo1ToCeiling() {
    if (setServoAngle(servo1_, servo1Angle_, SERVO1_CEILING_ANGLE,
                      SERVO1_MIN_ANGLE, SERVO1_MAX_ANGLE)) {
        Serial.print(F("[SERVO] Servo1 position: "));
        Serial.print(servo1Angle_);
        Serial.println(F(" deg"));
    } else {
        Serial.println(F("[ERROR] Servo1 angle out of limits"));
    }
}

void ServoController::cycleServo2Position() {
    servo2Index_ = (servo2Index_ + 1) % SERVO2_POSITION_COUNT;
    uint8_t target = SERVO2_POSITIONS[servo2Index_];

    if (setServoAngle(servo2_, servo2Angle_, target,
                      SERVO2_MIN_ANGLE, SERVO2_MAX_ANGLE)) {
        Serial.print(F("[SERVO] Servo2 position: "));
        Serial.print(servo2Angle_);
        Serial.println(F(" deg"));
    } else {
        Serial.println(F("[ERROR] Servo2 angle out of limits"));
        // Revert index on failure
        servo2Index_ = (servo2Index_ + SERVO2_POSITION_COUNT - 1) % SERVO2_POSITION_COUNT;
    }
}

uint8_t ServoController::getServo1Angle() const {
    return servo1Angle_;
}

uint8_t ServoController::getServo2Angle() const {
    return servo2Angle_;
}

uint8_t ServoController::getServo2Index() const {
    return servo2Index_;
}

bool ServoController::setServoAngle(Servo& servo, uint8_t& currentAngle, uint8_t target,
                                    uint8_t minAngle, uint8_t maxAngle) {
    if (target < minAngle || target > maxAngle) {
        return false;
    }

    servo.write(target);
    currentAngle = target;
    return true;
}
