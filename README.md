**Project Title:** UART Data Transmission with Real-Time Speed Measurement

**Description:**

This project implements firmware for data transmission between a PC and an MCU via UART (Universal Asynchronous Receiver-Transmitter) at a baud rate of 2400.
The goal is to transmit a predefined text message from the PC to the MCU, store it in EEPROM memory, and then retrieve the data from the MCU and send it back to the PC.
The system calculates and displays the real-time transmission speed (in bits per second) during both sending and receiving operations


**Key Features:**

PC to MCU Data Transmission:  Sends a predefined text message from the PC to the MCU over UART.
EEPROM Storage: The MCU stores the received data byte-by-byte in its EEPROM memory.
MCU to PC Data Reception: The MCU retrieves the stored data from EEPROM and sends it back to the PC.
Real-Time Speed Measurement: Both sending and receiving operations are monitored with real-time calculation of transmission speed, shown in bits per second (bps).
Embedded C/C++ Implementation: The firmware is written in embedded C/C++ for ATmega328P (or similar MCU) using UART communication.


**Tools and Technologies Used:**
MCU: ATmega328P 
Programming Language: Embedded C/C++
PC-side Programming: Python 
UART Communication: 2400 baud rate
Data Storage: EEPROM 
