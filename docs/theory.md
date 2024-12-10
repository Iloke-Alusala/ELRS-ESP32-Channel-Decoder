### **Theory Documentation (docs/theory.md)**

# Theory: Decoding the CRSF Protocol

## Overview
Crossfire (CRSF) is a lightweight and efficient protocol developed by Team BlackSheep (TBS) for transmitting RC channel data, telemetry, and control signals between a transmitter and receiver.

---

## CRSF Protocol Packet Structure
The CRSF protocol organizes data into packets to efficiently transmit multiple RC channel values, telemetry, and other information. Each packet follows a specific format:

### **Packet Structure**
| **Field**       | **Size (Bytes)** | **Description**                                   |
|------------------|------------------|---------------------------------------------------|
| **Address**      | 1                | Identifies the packet sender (e.g., receiver or transmitter). |
| **Length**       | 1                | Specifies the total packet size (excluding the checksum). |
| **Type**         | 1                | Defines the type of data in the packet (e.g., RC channels, telemetry). |
| **Payload**      | Variable         | Contains the actual data (e.g., channel values or telemetry). |
| **Checksum**     | 1                | Ensures data integrity by validating the packet contents. |

---

### **RC Channel Data Packet**
When transmitting RC channel data, the CRSF protocol encodes up to 16 channels into a compact payload using **11-bit resolution** per channel, as channel values range from 0 to 2000. 11-bit resolution allows you to cater for values from 0 - 2047.

#### **Structure of RC Channel Data Payload**
| **Field**       | **Description**                                  |
|------------------|-------------------------------------------------|
| **Channels 1-2** | Encoded as 11-bit values, packed into 3 bytes.  |
| **Channels 3-4** | Encoded as 11-bit values, packed into 3 bytes.  |
| **...**          | Continue for all 16 channels.                  |

---

### **Decoding RC Channel Data**
To decode the RC channel values, the payload is processed using bitwise operations to extract the 11-bit values for each channel. The steps are as follows:
1. Read the incoming packet and verify its type (e.g., `0x16` for RC channel data).
2. Extract the 11-bit channel values by:
   - Combining adjacent bytes.
   - Applying bitwise shifts and masks to isolate each channel.

---

## Example of RC Channel Data Encoding
Given a payload of 22 bytes for RC channel data:
- Channels 1–2 are stored across bytes 3–5.
- Channels 3–4 are stored across bytes 6–8.
- The pattern continues for all 16 channels.

#### Example Decoding for First 2 Channels:
```cpp
// Example in C++
rcChannels[0] = ((packet[3] | (packet[4] << 8)) & 0x07FF);
rcChannels[1] = (((packet[4] >> 3) | (packet[5] << 5)) & 0x07FF);


### Decoding Process
1. Read the incoming CRSF packet from the receiver.
2. Verify the packet type to ensure it contains RC channel data.
3. Extract 11-bit channel values using bitwise masking and shifts.
4. Output the channel values for use in applications.

This decoding method enables precise control and data monitoring for advanced RC applications.
```
### **Checksum**
The checksum ensures packet integrity by validating the Address, Length, Type, and Payload fields. If the checksum fails, the packet is discarded.
