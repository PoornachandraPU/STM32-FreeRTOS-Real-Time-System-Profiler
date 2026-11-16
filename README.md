# Real-Time System Profiler using STM32 and FreeRTOS

## 1. Project Overview

The **Real-Time System Profiler** is an embedded firmware project developed using Embedded C, an STM32F4 microcontroller and FreeRTOS.

The project implements a USB CDC-based command-line interface that allows a user to interact with the embedded system through a PC terminal. It uses multiple RTOS tasks to handle command processing and periodic LED indication.

The project explores real-time task scheduling, inter-task communication, hardware timer configuration and embedded system monitoring.

## 2. Key Features

- FreeRTOS-based multitasking.
- USB CDC virtual serial communication.
- Command-line interface for interacting with the microcontroller.
- Queue-based communication for receiving USB commands.
- Periodic heartbeat LED indication.
- System uptime reporting.
- Task information and runtime-statistics interface.
- LED control through terminal commands.

## 3. System Architecture

The firmware uses two primary application tasks:

| Task | Responsibility |
|---|---|
| `cliTask` | Receives and processes user commands |
| `heartbeatTask` | Toggles the heartbeat LED periodically |

A FreeRTOS message queue transfers received USB data to the CLI task.

### Communication Flow

PC Terminal → USB CDC → Receive Callback → FreeRTOS Queue → CLI Task → Command Processing → Response through USB CDC.

### Heartbeat Flow

FreeRTOS Scheduler → Heartbeat Task → GPIO Toggle (PD13) → Delay → Repeat.

## 4. Available CLI Commands

| Command | Function |
|---|---|
| `HELP` | Displays available commands |
| `UPTIME` | Displays system uptime |
| `TASKS` | Requests task-related information |
| `MEM` | Requests memory/task information |
| `ON` | Turns the controlled LED on |
| `OFF` | Turns the controlled LED off |

## 5. Technologies Used

- **Microcontroller:** STM32F4 series
- **Programming Language:** Embedded C
- **RTOS:** FreeRTOS
- **RTOS API:** CMSIS-RTOS2
- **IDE:** STM32CubeIDE
- **Configuration Tool:** STM32CubeMX
- **Communication:** USB CDC
- **Peripherals:** GPIO, TIM2 hardware timer

## 6. Important RTOS Concepts Implemented

- Task creation and scheduling
- Task priorities and delays
- Message queues
- Blocking task operations
- Runtime-statistics configuration
- Task information and stack monitoring APIs

## 7. Hardware and Software Requirements

### Hardware
- Compatible STM32F4 development board with USB device support
- USB cable
- PC

### Software
- STM32CubeIDE
- STM32CubeMX
- FreeRTOS
- USB CDC middleware
- Serial terminal application

## 8. Build Instructions

1. Clone this repository.
2. Open the project in STM32CubeIDE.
3. Allow the IDE to recognize and configure the project.
4. Build the project.
5. Flash the firmware to a compatible STM32F4 development board.
6. Connect the USB device interface to a PC.
7. Open the corresponding virtual COM port using a terminal application.

The USB connection, command handling and hardware behaviour require validation on the target board.

## 9. Testing Status

The firmware has been compiled successfully in STM32CubeIDE with **zero compilation errors and zero warnings** after the project configuration updates.

Hardware-level testing is pending because the target development board was unavailable during development.

USB communication, command execution, LED behaviour and runtime-statistics measurements have therefore not yet been physically validated.

## 10. Future Improvements

- Validate USB CDC communication on physical hardware.
- Verify task runtime-statistics measurements.
- Improve command parsing and error handling.
- Add more system-monitoring commands.
- Add robust queue and task creation error handling.
- Validate stack and heap monitoring on hardware.

## 11. Learning Outcomes

This project provides practical experience with:

- Embedded firmware development using STM32.
- RTOS-based application architecture.
- Task scheduling and inter-task communication.
- USB device communication.
- Hardware timer configuration.
- Embedded debugging and firmware integration.
