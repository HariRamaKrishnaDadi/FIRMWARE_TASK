"""
In this PC side code:-

we will send the text data to the MCU.
we will recieve the data from the MCU.
we will display the received data and real-time transmission speed.
"""
import serial
import time

text_data = """Finance Minister Arun Jaitley Tuesday hit out at former RBI governor Raghuram Rajan for predicting that the next banking crisis would be triggered by MSME lending, saying postmortem is easier than taking action when it was required.
               Rajan, who had as the chief economist at IMF warned of impending financial crisis of 2008, in a note to a parliamentary committee warned against ambitious credit targets and loan waivers, saying that they could be the sources of next banking crisis.
               Government should focus on sources of the next crisis, not just the last one.In particular, government should refrain from setting ambitious credit targets or waiving loans. Credit targets are sometimes achieved by abandoning appropriate due diligence,
               creating the environment for future NPAs," Rajan said in the note." Both MUDRA loans as well as the Kisan Credit Card, while popular, have to be examined more closely for potential credit risk. Rajan, who was RBI governor for three years till September 2016, is currently."""

ser = serial.Serial('COM3', 2400, timeout=1)

def transmit_data_to_mcu(data):
    byte_total = 0
    transmission_start = time.time()
    for byte in data:
        ser.write(byte.encode())
        byte_total += 1
    transmission_end = time.time()
    transmission_rate = (byte_total * 8) / (transmission_end - transmission_start)
    print(f"Transmission Speed: {transmission_rate} bps")

def receive_data_from_mcu():
    full_text = ""
    receive_start = time.time()
    byte_total = 0

    while len(full_text) < len(text_data):
        if ser.in_waiting > 0:
            incoming_char = ser.read().decode()
            full_text += incoming_char
            byte_total += 1
            print(incoming_char, end='')
    receive_end = time.time()
    reception_rate = (byte_total * 8) / (receive_end - receive_start)
    print(f"\nReception Speed: {reception_rate} bps")
    print("Received Text:", full_text)

# Sending data to MCU
transmit_data_to_mcu(text_data)

# Receiving data from MCU
receive_data_from_mcu()

ser.close()




