# Drone Remote Firmware

> NOTE: This is just a fun project for learning purposes, not meant for anything serious.

The Drone Remote repository contains firmware code for `radio control unit` for [drone](https://github.com/the-risk-taker/drone-base) project. It is an Arduino sketch that enables users to control a drone remotely using an Arduino board and a wireless custom communication protocol. The project is written in C++ and requires Arduino Uno, along with a wireless module `nRF24L01` 2.4GHz RF transceivers.

## Getting Started

The project was developed using [Atmel Studio 7](http://www.microchip.com/mplab/avr-support/atmel-studio-7) along with the [Visual Micro](https://www.visualmicro.com/) addon. However, it can also be run using the `Arduino IDE` by adding the necessary files to the project (although it may be necessary to modify the #include paths).
