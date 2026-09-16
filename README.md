# Smart Urban Flooding and Waterlogging Management System

## Overview

This project is an IoT-based Smart Urban Flooding and Waterlogging Management System designed to monitor water levels and rainfall conditions in real time.

The system uses an ESP8266 NodeMCU, a US-100 ultrasonic sensor, rain sensor, relay module, and motor/pump. It provides monitoring through a Blynk dashboard and sends email alerts when the water level reaches the defined threshold.

## Features

- Real-time water-level monitoring
- Rain detection
- Automatic flood/waterlogging detection
- Automatic motor/pump control using a relay
- Blynk dashboard for monitoring
- Email alerts when the motor is activated
- IoT-based monitoring and control

## Hardware Components

- ESP8266 NodeMCU
- US-100 Ultrasonic Sensor
- Rain Sensor
- Relay Module
- Water Pump/Motor
- Jumper Wires
- Power Supply

## Software and Technologies

- Arduino IDE
- Embedded C/C++
- ESP8266
- Blynk IoT
- SMTP Email Service

## Project Structure

```text
FloodNode-ESP32/
│
├── FloodNode_ESP8266.ino
├── myBlynk.h
├── myEmail.h
└── README.md
