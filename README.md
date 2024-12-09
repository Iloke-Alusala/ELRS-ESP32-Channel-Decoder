# ELRS Channel Decoder Using ESP32

A project demonstrating how to decode the ExpressLRS (ELRS) protocol using an ESP32 to extract all RC channel values transmitted from an ELRS receiver. This forms the backbone of advanced RC applications, including autonomous control and hardware integration.

## Features
- Decode Crossfire (CRSF) protocol packets from ELRS receivers.
- Extract all 16 RC channel values in real-time.
- Easily integrate with various applications, from servo motor control to custom robotics.

## Use Case
This decoder was used to transform an ordinary FPV drone into a delivery drone by mapping specific channel values to control payload mechanisms. See the [Use Case Documentation](docs/use_case.md) for more details.
![FPV Drone Video](assets/demo_video.gif)

## Theory
ELRS transmits RC data using the Crossfire (CRSF) protocol. This project decodes the protocol's structure to access channel values. See the [Theory Documentation](docs/theory.md) for an in-depth explanation.

## Hardware Requirements
- ESP32
- ELRS Receiver
- Breadboard and connecting wires

## Wiring Diagram
![Wiring Diagram](assets/wiring_diagram.png)

## Quickstart
1. Clone this repository:
   ```bash
   git clone https://github.com/yourusername/elrs-channel-decoder.git
