# STM_Not_Generated_HAL

This repository contains STM32 bare-metal and MCAL-style driver implementations developed without relying on STM32CubeMX generated application code.

The project structure and coding style are inspired by AUTOSAR-like layered architecture and MCAL development concepts.

Unlike HAL-generated projects, this repository focuses on low-level peripheral understanding, register-level programming, modular driver development, and embedded software architecture concepts.

Repository Link:
https://github.com/Cap593/STM_Not_Generated_HAL

---

# Project Goals

- Understanding STM32 peripherals at register level
- Learning MCAL-style driver development
- Practicing AUTOSAR-like layered software architecture
- Building reusable embedded software modules
- Understanding bootloader and application separation
- Developing interrupt-driven and DMA-based communication systems

---

# Covered Peripherals and Concepts

## System Timer
- System tick timer configuration
- Periodic task execution
- Basic task scheduling concepts
- LED tasking demonstration

## Bootloader
- Minimal bootloader implementation
- Application image validation using CRC
- Bootloader to application jump sequence
- Vector table relocation basics

## GPIO
- Bare-metal GPIO driver
- GPIO input/output handling
- GPIO external interrupt configuration

## UART
- UART bare-metal driver
- UART interrupt-based communication
- UART command-line interface (CLI)
- UART ring buffer implementation
- UART DMA-based communication

---

# Software Architecture

The repository follows an AUTOSAR-inspired modular structure:

- MCAL-like peripheral drivers
- Separation between application and low-level drivers
- Reusable module-based architecture
- Hardware abstraction concepts

---

# Development Approach

- Register-level programming
- Minimal dependency on generated code
- Focus on embedded software fundamentals
- Modular and reusable driver implementation
- AI-assisted development and learning workflow

---

# AI Usage

All projects in this repository are developed with the assistance of AI tools for:
- Concept understanding
- Debugging support
- Driver implementation guidance
- Architecture discussions
- Learning acceleration
- Code review and experimentation

The primary goal is educational learning, experimentation, and improving embedded software development skills.

---

# Tools and Environment

- STM32CubeIDE
- STM32 Reference Manuals
- ARM Cortex-M Architecture
- Git & SourceTree

---

# Hardware Used

- STM32F407 Discovery Board

---

# Learning Focus Areas

This repository is useful for learning:

- Embedded C programming
- Bare-metal firmware development
- Interrupt handling
- DMA concepts
- Bootloader design
- Embedded software architecture
- AUTOSAR-inspired MCAL concepts
- Low-level debugging and peripheral understanding

---

# Disclaimer

This repository is primarily developed for learning, experimentation, and embedded systems practice purposes.
Some modules may continue to evolve with additional features and improvements.
