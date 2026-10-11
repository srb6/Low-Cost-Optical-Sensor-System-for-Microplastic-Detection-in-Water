# Low-Cost Optical Sensor System for Microplastic Detection in Water

## Project Overview
This project focuses on developing a low-cost optical sensing system to detect potential microplastic contamination in water. The system uses multiple light sources and photodiode sensors to analyze changes in light transmission and scattering.

## Objectives
- Detect potential microplastic contamination in water.
- Measure optical transmission and scattering.
- Analyze sensor readings under different water sample conditions.
- Develop an embedded sensing prototype using the ESP32.

## Hardware Components
- ESP32 microcontroller
- UV LED
- White LED
- Infrared LED
- BPW34 photodiodes
- LM358 operational amplifier
- ADS1115 16-bit ADC
- Water sample chamber
- Supporting electronic components

## Working Principle
The LEDs illuminate the water sample at different wavelengths. Photodiodes measure transmitted and scattered light. The sensor readings are processed by the ESP32 to analyze optical variations between samples.

## Software and Tools
- Arduino IDE
- Embedded C/C++
- ESP32 programming
- Python for data analysis, if applicable

## Repository Structure
- `README.md` — Project overview
- `docs/` — Project report and presentation
- `src/` — ESP32 source code
- `dataset/` — Collected sensor readings, if available

## Project Team
- Sanjeevraj Bommannan M
- Nandita M K
- Sanjay B

## Project Domain
Electronics and Communication Engineering | Embedded Systems | Optical Sensing | Environmental Monitoring

## Disclaimer
The system is an experimental prototype. Its ability to distinguish microplastics from other suspended particles and identify polymer types requires appropriate experimental validation.
