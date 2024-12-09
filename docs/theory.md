### **Theory Documentation (docs/theory.md)**

# Theory: Decoding the ELRS Protocol

## Overview
ExpressLRS (ELRS) is a high-performance RC link protocol designed for long-range and low-latency communication. It utilizes the Crossfire (CRSF) protocol to transmit control data from the transmitter to the receiver.

### Key Concepts
- **Chirp Spread Spectrum (CSS):**
  ELRS uses CSS modulation for long-range communication with high resilience to interference.
- **Crossfire (CRSF) Protocol:**
  A lightweight protocol that encodes multiple RC channel values into compact packets.

## ELRS Protocol Structure
ELRS transmits data in packets with the following structure:
- **Header (2 bytes):**
  - Identifies the start of a packet and specifies the packet type.
- **Channel Data (22 bytes):**
  - Contains 16 RC channels, each encoded as an 11-bit value.
- **Flags and Metadata (1 byte):**
  - Includes telemetry and control flags.
- **Checksum (1 byte):**
  - Ensures data integrity.

### Channel Data Encoding
The 16 RC channels are encoded as **11-bit values** and packed into 22 bytes. The data is extracted using bitwise operations:
- **Channel 1:** Bits 0–10
- **Channel 2:** Bits 11–21
- **Channel 3:** Bits 22–32
- ...
- **Channel 16:** Bits 165–175

### Decoding Process
1. Read the incoming CRSF packet from the receiver.
2. Verify the packet type to ensure it contains RC channel data.
3. Extract 11-bit channel values using bitwise masking and shifts.
4. Output the channel values for use in applications.

This decoding method enables precise control and data monitoring for advanced RC applications.
