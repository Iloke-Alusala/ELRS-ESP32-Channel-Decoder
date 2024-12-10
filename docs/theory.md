# Theory: Decoding the CRSF Protocol

## Overview
Crossfire (CRSF) is a lightweight and efficient protocol developed by Team BlackSheep (TBS) for transmitting RC channel data, telemetry, and control signals between a transmitter and receiver. It is commonly used in high-performance RC systems, including ExpressLRS (ELRS), which leverages the CRSF protocol for low-latency, long-range communication.

---

## CRSF Protocol Packet Structure
The CRSF protocol organizes data into packets to efficiently transmit multiple RC channel values, telemetry, and other information. Each packet has a consistent format to ensure efficient communication.

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
When transmitting RC channel data, the CRSF protocol encodes up to 16 channels into a compact payload using **11-bit resolution** per channel. These channel values represent the positions of RC controls (e.g., throttle, yaw, pitch, roll) and auxiliary switches.

#### **Structure of RC Channel Data Payload**
| **Field**       | **Description**                                  |
|------------------|-------------------------------------------------|
| **Channels 1-2** | Encoded as 11-bit values, packed into 3 bytes.  |
| **Channels 3-4** | Encoded as 11-bit values, packed into 3 bytes.  |
| **...**          | Continues for all 16 channels.                  |

---

## Extracting RC Channel Data
The RC channel data is extracted from the packet payload by processing the 11-bit encoded values. Each channel's 11 bits are spread across adjacent bytes, requiring bitwise operations to isolate and decode them.

### **Steps for Decoding:**
1. **Verify Packet Type:**
   - Ensure the packet type is `0x16` (RC channel data).
   - This ensures the payload contains the encoded channel values.

2. **Extract Channel Values:**
   - Use bitwise shifts and masks to decode the 11-bit values for each channel.
   - The process involves combining bits from adjacent bytes.

3. **Iterate Through All Channels:**
   - Repeat the decoding process for all 16 channels in the payload.

---

### **Example of Channel Data Encoding and Decoding**

#### Payload Byte Mapping for First Four Channels:
| **Channel** | **Bits Used**                     | **Bytes Used**  |
|-------------|-----------------------------------|-----------------|
| **1**       | Bits 0–10                        | Bytes 3–4       |
| **2**       | Bits 11–21                       | Bytes 4–5       |
| **3**       | Bits 22–32                       | Bytes 5–6       |
| **4**       | Bits 33–43                       | Bytes 6–7       |

#### Example Code to Decode First Two Channels:
```cpp
// Extract Channel 1 (Bits 0–10)
rcChannels[0] = ((packet[3] | (packet[4] << 8)) & 0x07FF);

// Extract Channel 2 (Bits 11–21)
rcChannels[1] = (((packet[4] >> 3) | (packet[5] << 5)) & 0x07FF);
```

### **Checksum**
The checksum ensures packet integrity by validating the Address, Length, Type, and Payload fields. If the checksum fails, the packet is discarded.

I hope you enjoyed this documentation (hehe)
