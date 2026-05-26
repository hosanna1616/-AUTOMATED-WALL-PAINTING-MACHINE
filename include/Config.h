/**
 * @file Config.h
 * @brief Pin map, timing, and angle constants for Automated Wall Painting Machine
 * @project Automated Wall Painting Machine — Final Year Project
 * @board   Arduino UNO (ATmega328P)
 */

#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ---------------------------------------------------------------------------
// Serial
// ---------------------------------------------------------------------------
#define SERIAL_BAUD_RATE        9600

// ---------------------------------------------------------------------------
// Switch pins (INPUT_PULLUP — active LOW when pressed)
// SW6 is NOT connected to Arduino (pump wired directly to power)
// ---------------------------------------------------------------------------
#define PIN_SW1                 2
#define PIN_SW2                 3
#define PIN_SW3                 4
#define PIN_SW4                 5
#define PIN_SW5                 6

#define SWITCH_COUNT            5

// ---------------------------------------------------------------------------
// Servo pins
// ---------------------------------------------------------------------------
#define PIN_SERVO1              9
#define PIN_SERVO2              10

// ---------------------------------------------------------------------------
// Motor driver pins (dual H-bridge / L298N style IN1/IN2 per channel)
// ---------------------------------------------------------------------------
#define PIN_WHEEL_MOTOR_IN1     7
#define PIN_WHEEL_MOTOR_IN2     8
#define PIN_DRUM_MOTOR_IN1      11
#define PIN_DRUM_MOTOR_IN2      12

// ---------------------------------------------------------------------------
// Debounce and timing (ms)
// ---------------------------------------------------------------------------
#define DEBOUNCE_DELAY_MS       50
#define MOTOR_TIMEOUT_MS        10000   // Max continuous run without refresh
#define TURN_DURATION_MS        1500    // Right-turn maneuver duration
#define STARTUP_DELAY_MS        500

// ---------------------------------------------------------------------------
// Servo angles (degrees)
// ---------------------------------------------------------------------------
#define SERVO1_CEILING_ANGLE    90
#define SERVO1_MIN_ANGLE        0
#define SERVO1_MAX_ANGLE        180

#define SERVO2_POSITION_COUNT   5
#define SERVO2_MIN_ANGLE        0
#define SERVO2_MAX_ANGLE        180

// ---------------------------------------------------------------------------
// Switch indices (for SwitchManager arrays)
// ---------------------------------------------------------------------------
#define SW_IDX_1                0
#define SW_IDX_2                1
#define SW_IDX_3                2
#define SW_IDX_4                3
#define SW_IDX_5                4

#endif // CONFIG_H
