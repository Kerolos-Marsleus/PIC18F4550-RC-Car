# PIC18F4550 RC Car

A 4-mode robot car built bare-metal on a **PIC18F4550**: register-level C, no vendor libraries, layered MCAL / HAL / APP architecture.
First project of the ITI Embedded Systems program.

| Mode | Name | Sensors | Status |
|---|---|---|---|
| 1 | Manual (Bluetooth) | — | ✅ Working |
| 2 | Obstacle avoidance | HC-SR04 on a servo | 🔧 Redesign in progress (servo scanning) |
| 3 | Line follower | 3× IR (TCRT5000) | ✅ Working (jitter tuning pending) |
| 4 | Maze solver (line maze, left-hand rule) | 3× IR | ✅ Basic version |

---

## Hardware

| Part | Model | Notes |
|---|---|---|
| MCU | PIC18F4550 | 16 MHz crystal (HS), Fcy = 4 MHz |
| Motor driver | L293D | 2 DC motors, differential drive |
| Motors | 2× TT gear motor | Start threshold ≈ 45% duty |
| Distance sensor | HC-SR04 ultrasonic | Mounted on the servo |
| Servo | SG90 (or compatible) | Sweeps the ultrasonic sensor |
| Line sensors | 3× TCRT5000 / LM393 modules | Digital output, **1 = black** |
| Bluetooth | HC-05 / HC-06 | UART |
| Battery | 2× 18650 Li-ion (7.4 V) | |
| Regulator | LM2596 buck, set to 5.0 V | Replaced the 7805 (brown-out resets with the servo) |
| Programmer | PICkit 3 | ICSP |

---

## Pin Map

| Pin | Direction | Connected to | Function |
|---|---|---|---|
| RA0 | Out | LED + resistor | Boot / reset indicator (lights once at power-up) |
| RA1 | Out | LED + resistor | Heartbeat (toggles every 500 ms; stops if firmware freezes) |
| RB0 | In | HC-SR04 ECHO | Echo pulse, timed with Timer1 |
| RB1 | Out | Servo signal | 50 Hz pulse, 500–2500 µs, generated with Timer3 |
| RB6 / RB7 | — | PICkit 3 (PGC / PGD) | **Reserved for programming, do not use** |
| RC1 | Out | L293D EN2 (pin 9) | PWM2 (CCP2) → right motor speed |
| RC2 | Out | L293D EN1 (pin 1) | PWM1 (CCP1) → left motor speed |
| RC6 | Out | Bluetooth RX | UART TX |
| RC7 | In | Bluetooth TX | UART RX |
| RD0 | Out | L293D IN1 (pin 2) | Left motor direction |
| RD1 | Out | L293D IN2 (pin 7) | Left motor direction |
| RD2 | Out | L293D IN3 (pin 10) | Right motor direction |
| RD3 | Out | L293D IN4 (pin 15) | Right motor direction |
| RD4 | Out | HC-SR04 TRIG | 10 µs trigger pulse |
| RD5 | In | IR sensor — Right | 1 = black |
| RD6 | In | IR sensor — Middle | 1 = black |
| RD7 | In | IR sensor — Left | 1 = black |
| MCLR (pin 1) | In | 10 kΩ to 5 V | Reset pin pull-up |

> **Verify** the EN1/EN2 ↔ left/right mapping against the wiring on your board.
> Pins to avoid for new features: RB6/RB7 (ICSP), RC4/RC5 (USB-only on this chip), RA4 (open-drain).

### Power

```
Battery 7.4 V ──┬── LM2596 (5.0 V) ──► 5 V rail: PIC, Bluetooth, sensors, L293D VCC1 (pin 16), servo
                └────────────────────► L293D VCC2 (pin 8) — motor power
GND ─────────────────────────────────── shared by everything (LM2596 IN− / OUT− are common)
```

- 470 µF capacitor across the servo's 5 V / GND.
- 100 nF ceramic capacitor at each PIC VDD/VSS pair (pins 11/12 and 31/32).
- The servo's 5 V is wired directly from the LM2596 output, not through breadboard contacts.

---

## Peripheral and Timer Usage

| Peripheral | Used by | Configuration |
|---|---|---|
| Timer0 | `timer0.c`: system tick | 8-bit, prescaler 1:16, preload 6 → interrupt every **1 ms** |
| Timer1 | `ultrasonic.c`: echo timing | measures ECHO pulse width |
| Timer2 | `pwm.c`: motor PWM time base | PR2 = 199, prescaler 1:16 → **1250 Hz** |
| Timer3 | `servo.c`: servo pulse | 16-bit, prescaler 1:2 (0.5 µs/count), alternating HIGH / LOW alarms |
| CCP1 / CCP2 | `pwm.c` | PWM mode, left / right motor speed |
| EUSART | `uart.c` | Bluetooth commands, RX interrupt + overrun (OERR) recovery |

### Interrupts

One ISR in `main.c` (priorities disabled). Each source checks **enable && flag** before calling its handler:

| Source | Flag / Enable | Handler |
|---|---|---|
| UART RX | `RCIF` / `RCIE` | stores byte in `rx_data`, sets `rx_flag` |
| Timer0 | `TMR0IF` / `TMR0IE` | `Timer0_Handler()` → `ms_ticks++` |
| Timer3 | `TMR3IF` / `TMR3IE` | `Servo_Handler()` → toggles RB1, reloads Timer3 |

---

## Software Architecture

```
APP    main.c            boot, Bluetooth command parser, mode dispatcher, heartbeat
       obstacle.c        Mode 2: obstacle avoidance
       line_follower.c   Mode 3: line follower + Line_Handler() (shared with maze)
       maze.c            Mode 4: maze solver (left-hand rule)
─────────────────────────────────────────────────────────────────────────────
HAL    motor.c           direction patterns + speed, Motor_Reset(), Motor_TurnSpeed()
       ultrasonic.c      distance in cm (or US_NO_ECHO)
       ir_sensor.c       3 sensors → 3-bit pattern (L M R)
       servo.c           Servo_SetPulseUs(), owns Timer3
─────────────────────────────────────────────────────────────────────────────
MCAL   uart.c  pwm.c  timer0.c  timer1.c
       configure.h       config bits, _XTAL_FREQ = 16 MHz
```

**Rules**
- Layers only call **downward** (APP → HAL → MCAL).
- Every `*_Run()` function does **one step and returns**. No blocking loops, so Bluetooth commands (e.g. `D` = stop) are always handled.
- Every mode change calls `Motor_Reset()`, so the motors start each mode in a known state.
- Variables shared with an ISR are `volatile`. Multi-byte reads/writes are protected by briefly disabling that interrupt (8-bit CPU).

---

## Bluetooth Commands

| Key | Action |
|---|---|
| `F` / `B` / `L` / `R` | Forward / Back / Left / Right |
| `G` / `I` | Forward-left / Forward-right (diagonal) |
| `H` / `J` | Back-left / Back-right (diagonal) |
| `S` | Stop |
| `0`–`9` | Speed = digit × 10 % (kept across modes) |
| `q` | Speed 100 % |
| `U` | Mode 2: Obstacle avoidance |
| `V` | Mode 3: Line follower |
| `X` | Mode 4: Maze |
| `D`, `W`, `w`, `u`, `v`, `x` | Back to Mode 1 (manual) + stop |

---

## Build and Flash

- **IDE:** VS Code + MPLAB extension
- **Compiler:** Microchip XC8
- **Programmer:** PICkit 3 via ICSP (MCLR, VDD, GND, PGD = RB7, PGC = RB6)

If the programmer reports *"No Device Found"*: check the ICSP connector and the board's power. The PICkit supplies only ~30 mA.

---

## Roadmap

- [x] Manual Bluetooth driving with diagonals and speed control
- [x] Line follower with graduated turns
- [x] Maze solver (left-hand rule)
- [x] Timer0 1 ms system tick + heartbeat LED
- [x] Servo driver on Timer3
- [x] LM2596 power supply (fixed brown-out resets)
- [ ] **Calibrate servo positions (−60°, −30°, 0°, +30°, +60°)** ← next
- [ ] Mode 2: non-blocking scan state machine (servo + ultrasonic + Timer0)
- [ ] Mode 2: steer toward the most open direction
- [ ] Stop the car if the Bluetooth link is lost (timeout on Timer0)
- [ ] Battery voltage monitor (ADC)
- [ ] Line follower: PD control
- [ ] Maze: detect the finish square