# Remote Control Light Switch

An embedded systems project that allows you to control a light switch remotely using your iPhone by utilizing Apple's IR pulsing facial recognition feature or any standard IR remote control. The system uses an Arduino Nano with an ATMega328p microprocessor to control a servo motor that physically toggles a light switch.

## Overview

This project provides a simple and reliable way to control household light switches remotely. The system can be activated from anywhere in a room using any standard remote control within a range of 20-25 feet. When activated, a servo motor physically toggles the light switch, and an LED indicates successful signal reception.

## Features

- Works with any standard IR remote control
- Operating range up to 20 feet
- Visual feedback via LED indicator
- Easy installation using Command Strips
- 9V battery powered
- Immune to common household IR interference (lamps, LCD TV, Wi-Fi)

## Hardware Components

- Arduino Nano (ATMega328p microcontroller)
- SG90 servo motor
- KSOP4838 IR sensor (38 kHz)
- 9V battery
- Red LED indicator
- Custom control arm and mount housing
- Breadboard
- Command Strips for mounting

## Technical Specifications

- IR Sensor Operating Frequency: 38 kHz
- PWM Configuration:
  - PWM_TOP: 39999
  - Servo Min Position: 1999
  - Servo Max Position: 3999
- Servo Range: ~45 degrees rotation
- Signal Processing Tolerance: 100

## Installation

1. Mount the housing unit near the target light switch using Command Strips
2. Align the servo arm with the light switch
3. Connect the 9V battery
4. Test the system using any IR remote control

## Code Structure

The main functionality is implemented in `lightSwitch.c` and includes:

- IR signal processing
- Servo motor control using PWM
- LED indicator management
- Interrupt handling for PWM state updates

Key functions:
```c
uint16_t readIRSignal(void)     // Read and process IR sensor input
void flickUp(void)              // Rotate servo clockwise
void flickDown(void)            // Rotate servo counter-clockwise
void servo_set(uint16_t deg, uint16_t max_deg)  // Set servo to specific angle
```

## System Operation

1. The IR sensor continuously monitors for remote control signals
2. When a valid signal is detected:
   - The LED indicator lights up
   - The servo motor rotates to toggle the switch
   - The system waits for 1 second before accepting new input
   - The LED turns off

## Dependencies

- AVR/IO library
- util/delay library
- avr/interrupt library
- stdbool library

## Building and Flashing

You'll need an AVR development environment set up with the following:
- AVR-GCC compiler
- AVRDUDE for flashing
- A programmer compatible with Arduino Nano

## Authors

- Zachary Mineiko
- Dawson Crowley

## Course Information

Developed for ECE:3360:0001 Embedded Systems 2024 at The University of Iowa

## Acknowledgments

Based on servo motor PWM implementation techniques from:
- Gupta, K. (2020) - "Servo Motor Embedded C code: Using Timers, PWM OF AVR MCU"
