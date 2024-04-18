# WACIE

Impact echo project

- Arduino IDE Version : 1.8.18
- ESP32 Version : 2.0.11
- ESP32 Board : DOIT ESP32 DEVKIT V1

## Python Libraries to install

- [ ] pip install pyserial

- [ ] pip install kivy

- [ ] pip install kivy-garden

- [ ] garden install matplotlib

## BLE_Get_MAC

This sketch is uploaded first on the receiver to get the device MAC address and be used by the WACIE controller.

## BLE_Receiver

This sketch is uploaded on the receiver MCU(esp32) then connected to a computer/laptop for software application.

## WACIE

This sketch is for the WACIE controller that is responsible for getting data of the vibration and transmitting data to the receiver.

### src

This folder contains the servo library for WACIE controller.

