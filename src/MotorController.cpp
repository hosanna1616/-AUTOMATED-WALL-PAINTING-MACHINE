/**
 * @file MotorController.cpp
 */

#include "MotorController.h"

void MotorController::begin() {
    pinMode(PIN_WHEEL_MOTOR_IN1, OUTPUT);
    pinMode(PIN_WHEEL_MOTOR_IN2, OUTPUT);
    pinMode(PIN_DRUM_MOTOR_IN1, OUTPUT);
    pinMode(PIN_DRUM_MOTOR_IN2, OUTPUT);

    wheelStop();
    drumStop();
    drumMode_ = DrumMode::IDLE;

    wheelRunning_ = false;
    drumRunning_ = false;
    wheelTurnActive_ = false;
    wheelTurnEndTime_ = 0;

    lastMotorActivityMs_ = millis();
    motorTimeoutTriggered_ = false;
}

void MotorController::update() {
    if (wheelTurnActive_ && millis() >= wheelTurnEndTime_) {
        wheelStop();
        wheelTurnActive_ = false;
        Serial.println(F("[MOTOR] Right turn maneuver complete"));
    }

    if (wheelRunning_ || drumRunning_) {
        refreshMotorActivity();
    }

    checkMotorTimeout();
}

void MotorController::wheelForward() {
    if (motorTimeoutTriggered_) {
        Serial.println(F("[ERROR] Motor timeout active — clear or reset to drive wheel"));
        return;
    }
    if (!wheelRunning_) {
        Serial.println(F("[MOTOR] Wheel motor forward"));
    }
    setWheelPins(HIGH, LOW);
    wheelRunning_ = true;
    wheelTurnActive_ = false;
    refreshMotorActivity();
}

void MotorController::wheelStop() {
    setWheelPins(LOW, LOW);
    wheelRunning_ = false;
    wheelTurnActive_ = false;
}

void MotorController::wheelRightTurnStart() {
    if (motorTimeoutTriggered_) {
        Serial.println(F("[ERROR] Motor timeout active — cannot start turn"));
        return;
    }
    // Right turn: reverse one channel pattern (IN1 LOW, IN2 HIGH) for fixed duration
    setWheelPins(LOW, HIGH);
    wheelRunning_ = true;
    wheelTurnActive_ = true;
    wheelTurnEndTime_ = millis() + TURN_DURATION_MS;
    refreshMotorActivity();
    Serial.println(F("[MOTOR] Right turn maneuver started"));
}

bool MotorController::isWheelTurnActive() const {
    return wheelTurnActive_;
}

void MotorController::drumToggleWrapRelease() {
    if (motorTimeoutTriggered_) {
        Serial.println(F("[ERROR] Motor timeout active — drum disabled"));
        return;
    }

    if (drumMode_ == DrumMode::WRAP) {
        drumMode_ = DrumMode::RELEASE;
        setDrumPins(LOW, HIGH);
        Serial.println(F("[DRUM] State: RELEASE cable"));
    } else {
        drumMode_ = DrumMode::WRAP;
        setDrumPins(HIGH, LOW);
        Serial.println(F("[DRUM] State: WRAP cable"));
    }

    drumRunning_ = true;
    refreshMotorActivity();
}

void MotorController::drumStop() {
    setDrumPins(LOW, LOW);
    drumRunning_ = false;
}

DrumMode MotorController::getDrumMode() const {
    return drumMode_;
}

const char* MotorController::drumModeString() const {
    switch (drumMode_) {
        case DrumMode::WRAP:    return "WRAP";
        case DrumMode::RELEASE: return "RELEASE";
        default:                return "IDLE";
    }
}

bool MotorController::isMotorTimeoutActive() const {
    return motorTimeoutTriggered_;
}

void MotorController::clearMotorTimeout() {
    motorTimeoutTriggered_ = false;
    lastMotorActivityMs_ = millis();
    Serial.println(F("[INFO] Motor timeout cleared"));
}

void MotorController::setWheelPins(bool in1, bool in2) {
    digitalWrite(PIN_WHEEL_MOTOR_IN1, in1 ? HIGH : LOW);
    digitalWrite(PIN_WHEEL_MOTOR_IN2, in2 ? HIGH : LOW);
}

void MotorController::setDrumPins(bool in1, bool in2) {
    digitalWrite(PIN_DRUM_MOTOR_IN1, in1 ? HIGH : LOW);
    digitalWrite(PIN_DRUM_MOTOR_IN2, in2 ? HIGH : LOW);
}

void MotorController::refreshMotorActivity() {
    lastMotorActivityMs_ = millis();
}

void MotorController::checkMotorTimeout() {
    if (!wheelRunning_ && !drumRunning_) {
        lastMotorActivityMs_ = millis();
        return;
    }

    if (millis() - lastMotorActivityMs_ > MOTOR_TIMEOUT_MS) {
        wheelStop();
        drumStop();
        drumMode_ = DrumMode::IDLE;
        motorTimeoutTriggered_ = true;
        Serial.println(F("[ERROR] Motor timeout — all motors stopped"));
    }
}
