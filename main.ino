/**
 * @file main.ino
 * @brief Automated Wall Painting Machine — main control loop
 *
 * Switch-controlled wall/ceiling painting rig.
 * SW6 is NOT connected to Arduino (pump on direct power switch).
 */

#include "Config.h"
#include "SwitchManager.h"
#include "MotorController.h"
#include "ServoController.h"

SwitchManager  switches;
MotorController motors;
ServoController servos;

void printStartupBanner() {
    Serial.println(F("========================================"));
    Serial.println(F(" Automated Wall Painting Machine"));
    Serial.println(F(" Arduino UNO — System Startup"));
    Serial.println(F("========================================"));
    Serial.println(F("[INFO] SW6: pump (not on MCU — direct power)"));
    Serial.print(F("[INFO] Debounce: "));
    Serial.print(DEBOUNCE_DELAY_MS);
    Serial.println(F(" ms"));
    Serial.print(F("[INFO] Motor timeout: "));
    Serial.print(MOTOR_TIMEOUT_MS);
    Serial.println(F(" ms"));
    Serial.println(F("[INFO] Ready — SW1..SW5 active"));
}

void handleSwitch1() {
    if (motors.isMotorTimeoutActive()) {
        return;
    }

    if (switches.wasPressed(SW_IDX_1)) {
        Serial.println(F("[SWITCH] SW1 pressed"));
    }

    if (switches.isPressed(SW_IDX_1)) {
        if (!motors.isWheelTurnActive()) {
            motors.wheelForward();
        }
    } else {
        if (!motors.isWheelTurnActive()) {
            motors.wheelStop();
        }
        if (switches.wasReleased(SW_IDX_1)) {
            Serial.println(F("[MOTOR] Wheel motor stopped"));
        }
    }
}

void handleSwitch2() {
    if (switches.wasPressed(SW_IDX_2)) {
        Serial.println(F("[SWITCH] SW2 pressed"));
        servos.moveServo1ToCeiling();
    }
}

void handleSwitch3() {
    if (switches.wasPressed(SW_IDX_3)) {
        Serial.println(F("[SWITCH] SW3 pressed"));
        servos.cycleServo2Position();
    }
}

void handleSwitch4() {
    if (switches.wasPressed(SW_IDX_4)) {
        Serial.println(F("[SWITCH] SW4 pressed"));
        if (!switches.isPressed(SW_IDX_1)) {
            motors.wheelRightTurnStart();
            Serial.println(F("[MOTOR] Wheel right turn action"));
        } else {
            Serial.println(F("[WARN] SW1 active — turn ignored"));
        }
    }
}

void handleSwitch5() {
    if (switches.wasPressed(SW_IDX_5)) {
        Serial.println(F("[SWITCH] SW5 pressed"));
        motors.drumToggleWrapRelease();
        Serial.print(F("[DRUM] Mode: "));
        Serial.println(motors.drumModeString());
    }
}

void setup() {
    Serial.begin(SERIAL_BAUD_RATE);
    delay(STARTUP_DELAY_MS);

    switches.begin();
    motors.begin();
    servos.begin();

    printStartupBanner();
}

void loop() {
    switches.update();
    motors.update();

    handleSwitch1();
    handleSwitch2();
    handleSwitch3();
    handleSwitch4();
    handleSwitch5();
}
