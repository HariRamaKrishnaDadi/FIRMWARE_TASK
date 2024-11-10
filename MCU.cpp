/**This code handles:

--> Receiving data from the PC via UART.
--> Storing each received byte in EEPROM.
--> Sending stored data back to the PC.
--> Calculating and sending the real-time transmission speed during both receiving and sending operations. **/

// Assuming we are using ATmega328P

#include <avr/io.h>
#include <avr/eeprom.h>
#include <util/delay.h>
#include <stdio.h>

// Defining MACRO's 

#define F_CPU 16000000UL  
#define BAUD 2400
#define UART_UBRR F_CPU/16/BAUD-1
#define TEXT_SIZE 1000

// A UART class for handling UART transmission and reception.

class SerialComm {
public:
    SerialComm(unsigned int ubrr_value) {
        // we are Setting baud rate here
        UBRR0H = (unsigned char)(ubrr_value >> 8);
        UBRR0L = (unsigned char)ubrr_value;
        // Enabling receiver and transmitter
        UCSR0B = (1 << RXEN0) | (1 << TXEN0);
        // Setting frame format: 8 data bits, 1 stop bit
        UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    }

    void transmitByte(uint8_t byte) {
        while (!(UCSR0A & (1 << UDRE0)));  
        UDR0 = byte;
    }

    uint8_t receiveByte() {
        while (!(UCSR0A & (1 << RXC0)));  
        return UDR0;
    }

    void transmitString(const char *text) {
        while (*text) {
            transmitByte(*text++);
        }
    }
};

// An EEPROM class for managing data storage and retrieval.

class MemoryStorage {
public:
    static void writeMemory(uint16_t address, uint8_t data) {
        eeprom_update_byte((uint8_t *)address, data);
    }

    static uint8_t readMemory(uint16_t address) {
        return eeprom_read_byte((uint8_t *)address);
    }
};

// A SpeedMeter class to measure the transmission speed.

class DataRateMonitor {
private:
    uint32_t startClock;
    uint32_t byteCounter;

public:
    DataRateMonitor() : startClock(0), byteCounter(0) {}
    // initializes the timer and byte counter.
    void begin() {
        startClock = millis();
        byteCounter = 0;
    }
    // increments the byte count after each received or transmitted byte.
    void countByte() {
        byteCounter++;
    }
    // computes the speed in bits per second.
    uint32_t calculateDataRate() {
        uint32_t endClock = millis();
        uint32_t timeDifference = endClock - startClock;
        return (byteCounter * 8 * 1000) / timeDifference;  
    }
    // sends the calculated speed to the PC through the UART instance.
    void displayRate(SerialComm &serial) {
        char output[20];
        sprintf(output, "Speed: %lu bps\n", calculateDataRate());
        serial.transmitString(output);
    }
};

// reads data byte-by-byte from the PC, stores it in EEPROM, and calculates speed.
void receiveAndSaveData(SerialComm &serial, DataRateMonitor &monitor) {
    monitor.begin();
    for (uint16_t i = 0; i < TEXT_SIZE; i++) {
        uint8_t byteReceived = serial.receiveByte();
        MemoryStorage::writeMemory(i, byteReceived);
        monitor.countByte();
    }
    monitor.displayRate(serial);
}

// retrieves data from EEPROM and sends it to the PC, calculating speed during this process.
void loadDataAndTransmit(SerialComm &serial, DataRateMonitor &monitor) {
    monitor.begin();
    for (uint16_t i = 0; i < TEXT_SIZE; i++) {
        uint8_t byteData = MemoryStorage::readMemory(i);
        serial.transmitByte(byteData);
        monitor.countByte();
    }
    monitor.displayRate(serial);
}

int main() {  

    SerialComm serial(UART_UBRR);
    DataRateMonitor monitor;

    // Receiving data from PC and storing in EEPROM
    receiveAndSaveData(serial, monitor);

    // Retrieving data from EEPROM and sending back to PC
    loadDataAndTransmit(serial, monitor);

    return 0;
}
