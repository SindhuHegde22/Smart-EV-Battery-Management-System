# Smart EV Battery Management System (Energy Tag)

An intelligent IoT-enabled Battery Swap and Energy Management Platform designed for Electric Vehicle (EV) infrastructure.  
The system integrates embedded hardware, cloud connectivity, real-time monitoring, and automated energy tracking to improve operational efficiency, safety, and user transparency in EV battery swapping stations.

---

# Project Overview

The rapid growth of Electric Vehicles introduces major challenges in charging downtime, battery monitoring, and swap station management.

This project addresses these challenges through a smart battery swap ecosystem capable of:

- Secure user authentication
- Automated battery slot management
- Dynamic energy-based billing
- Real-time voltage monitoring
- Remote cloud logging and tracking
- Web-based booking and monitoring

The platform combines ESP32-based embedded control with IoT connectivity to simulate a scalable smart charging and swapping infrastructure.

---

# Core Functionalities

## Smart User Authentication

RFID-based authentication is implemented to securely identify users before battery allocation and swapping operations.

## Intelligent Battery Monitoring

A voltage sensing mechanism continuously tracks battery State of Charge (SOC) and enables automated charging status analysis.

## Dynamic Energy-Based Billing

The system calculates battery usage and pricing dynamically based on real-time voltage and charge percentage measurements.

## Real-Time Slot Management

An onboard ESP32 web server provides live monitoring of available battery slots and booking functionality.

## Cloud Data Synchronization

Transaction records, charging information, and battery activity logs are pushed to Google Sheets through cloud integration for transparent monitoring and analytics.

## Automated Swap Control

Relay-controlled switching logic automates battery connection and slot allocation during swap operations.

---

# System Architecture

The complete system integrates:

- ESP32 Wi-Fi Microcontroller
- RFID Authentication Module
- Voltage Sensor Network
- Relay-Based Power Control
- LCD User Interface
- Web-Based Monitoring Portal
- Cloud Logging Infrastructure

The architecture is designed to simulate real-world EV swap station workflows with both hardware and software coordination.

---

## System Architecture Diagram

The following block diagram illustrates the high-level integration of sensors, the ESP32 controller, and the cloud-based data logging system.

![System Block Diagram](assets/system_architecture.png)

---

# Technical Stack

| Category | Technology |
|---|---|
| Embedded Controller | ESP32 |
| Programming Language | Embedded C / C++ |
| Development Environment | Arduino IDE |
| Communication Protocols | SPI, I2C, UART, HTTP |
| Cloud Integration | Google Apps Script / Google Sheets API |
| User Interface | LCD + Web Dashboard |
| Connectivity | Wi-Fi Enabled IoT Communication |

---

# Key Engineering Highlights

- IoT-Based Smart Energy Management
- Real-Time Battery Health Monitoring
- Embedded Hardware + Cloud Integration
- Automated Battery Allocation Logic
- Dynamic Voltage-Based Analytics
- RFID Access Control System
- Remote Monitoring Capability
- Scalable EV Infrastructure Concept

---

# Repository Structure

```text
Smart-EV-Battery-Management-System/
│
├── firmware/
│   └── energy_tag_esp32.ino
│
├── hardware/          # Circuit schematics and hardware design
├── docs/              # Technical reports and presentations
├── assets/            # Images, videos, and UI screenshots
├── README.md
```

---

# Hardware Demonstration

## Embedded Hardware Setup

The hardware implementation integrates ESP32, RFID modules, relays, voltage sensors, and LCD display modules for automated control and monitoring.

![Hardware Connection](assets/hardware_connection.jpg)

---

## LCD Monitoring Interface

The LCD module displays battery information, authentication status, and local server connectivity information.

![LCD Output](assets/lcd_display_output.jpg)

---

## Web-Based Monitoring Portal

The onboard ESP32 server hosts a real-time battery slot booking and monitoring interface.

![Web UI](assets/web_server_ui.png)

---

## Cloud Transaction Logging

Battery transactions and energy usage data are synchronized to cloud storage for transparency and analytics.

![Transaction Log](assets/transaction_log_sheet.png)

---

# Demonstration Video

The demonstration showcases:
- RFID authentication
- Battery swap operation
- Dynamic energy monitoring
- Relay control logic
- Cloud database synchronization
- Real-time web interface interaction

[▶ Watch Project Demonstration](assets/energy_tag_demo.mp4)

---

# Future Enhancements

- AI-Based Battery Health Prediction
- Mobile Application Integration
- Predictive Maintenance Analytics
- GPS-Based Swap Station Mapping
- Fast-Charging Optimization
- Scalable Multi-Station Management

---

# Author

**Sindhu Hegde**
