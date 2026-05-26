# Flowcharts — Automated Wall Painting Machine

## Main Program Flow

```mermaid
flowchart TD
    A([Power On]) --> B[setup]
    B --> C[Init Serial 9600]
    C --> D[SwitchManager.begin]
    D --> E[MotorController.begin]
    E --> F[ServoController.begin]
    F --> G[Print startup banner]
    G --> H([loop])

    H --> I[switches.update]
    I --> J[motors.update]
    J --> K[handleSwitch1]
    K --> L[handleSwitch2]
    L --> M[handleSwitch3]
    M --> N[handleSwitch4]
    N --> O[handleSwitch5]
    O --> H
```

## Switch Debounce Flow

```mermaid
flowchart TD
    A[Read raw pin] --> B{Changed from last raw?}
    B -->|Yes| C[Reset debounce timer]
    B -->|No| D{Elapsed > DEBOUNCE_DELAY_MS?}
    C --> A
    D -->|No| E[Wait next loop]
    D -->|Yes| F{Debounced state changed?}
    F -->|No| E
    F -->|Yes| G{LOW edge?}
    G -->|Yes| H[Set pressEvent]
    G -->|No| I{HIGH edge?}
    I -->|Yes| J[Set releaseEvent]
    I -->|No| E
    H --> E
    J --> E
```

## SW1 — Wheel Forward (Held)

```mermaid
flowchart TD
    A[SW1 wasPressed?] -->|Yes| B[Log SW1 pressed]
    A --> C{SW1 isPressed?}
    B --> C
    C -->|Yes| D{Turn active?}
    D -->|No| E[wheelForward]
    D -->|Yes| F[Skip forward]
    C -->|No| G{Turn active?}
    G -->|No| H[wheelStop]
    G -->|No| I[wasReleased?]
    H --> I
    I -->|Yes| J[Log motor stopped]
```

## SW2 — Servo1 Ceiling Position

```mermaid
flowchart TD
    A{SW2 wasPressed?} -->|Yes| B[Log SW2]
    B --> C[moveServo1ToCeiling 90°]
    C --> D{Angle valid?}
    D -->|Yes| E[Serial: Servo1 90°]
    D -->|No| F[Serial: ERROR]
    A -->|No| G[End]
    E --> G
    F --> G
```

## SW3 — Servo2 Cycle

```mermaid
flowchart TD
    A{SW3 wasPressed?} -->|Yes| B[index = index + 1 mod 5]
    B --> C[target = POSITIONS index]
    C --> D[Write servo2]
    D --> E[Serial angle]
    A -->|No| F[End]
    E --> F
```

Positions: **0 → 45 → 90 → 135 → 180 → 0**

## SW4 — Right Turn

```mermaid
flowchart TD
    A{SW4 wasPressed?} -->|Yes| B[Log SW4]
    B --> C{SW1 pressed?}
    C -->|Yes| D[WARN ignore turn]
    C -->|No| E[wheelRightTurnStart]
    E --> F[IN1 LOW IN2 HIGH]
    F --> G[Start turn timer]
    G --> H{motors.update: time elapsed?}
    H -->|Yes| I[wheelStop + log complete]
    H -->|No| J[Continue turn]
    A -->|No| K[End]
    D --> K
    I --> K
    J --> K
```

## SW5 — Drum Wrap / Release

```mermaid
flowchart TD
    A{SW5 wasPressed?} -->|Yes| B[Log SW5]
    B --> C{Current mode WRAP?}
    C -->|Yes| D[Set RELEASE + motor pins]
    C -->|No| E[Set WRAP + motor pins]
    D --> F[Serial DRUM state]
    E --> F
    A -->|No| G[End]
    F --> G
```

## Motor Timeout Safety

```mermaid
flowchart TD
    A[motors.update] --> B{Wheel or drum running?}
    B -->|Yes| C[refreshMotorActivity]
    B -->|No| D[Reset idle timer]
    C --> E{Elapsed > MOTOR_TIMEOUT_MS?}
    D --> F[End]
    E -->|Yes| G[Stop all motors]
    G --> H[Set timeout flag]
    H --> I[Serial ERROR]
    E -->|No| F
```

## SW6 — Pump (External to Firmware)

```mermaid
flowchart LR
    A[12V +] --> B{SW6}
    B -->|ON| C[Pump +]
    B -->|OFF| D[Open]
    E[12V -] --> F[Pump -]
    C --> F
```

## System Data Flow

```
Operator → SW1..SW5 → SwitchManager → main.ino handlers
                              ↓
                    MotorController / ServoController
                              ↓
                    H-bridge / Servo signals → Mechanics

Operator → SW6 → 12V → Pump (parallel path, no MCU)
```

## State Machine Diagram (Drum)

```mermaid
stateDiagram-v2
    [*] --> Idle: Power on
    Idle --> Wrap: SW5 press
    Wrap --> Release: SW5 press
    Release --> Wrap: SW5 press

    note right of Idle
        First SW5 from IDLE
        enters WRAP
    end note
```

## State Machine Diagram (Servo2 Index)

```mermaid
stateDiagram-v2
    [*] --> S0: angle 0°
    S0 --> S45: SW3
    S45 --> S90: SW3
    S90 --> S135: SW3
    S135 --> S180: SW3
    S180 --> S0: SW3
```
