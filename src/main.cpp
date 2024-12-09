#include <HardwareSerial.h>
#include <ESP32Servo.h>

#define speed_stop 90  // Stop position

#define CRSF_SERIAL_PORT Serial2  // Using Serial2 on the ESP32
#define CRSF_BAUD_RATE 420000     // CRSF baud rate (400,000 bps)
#define RX_PIN 17                 // Receiver TX connected to ESP32 RX (GPIO17)

#define CRSF_MAX_PACKET_SIZE 64
#define CRSF_FRAMETYPE_RC_CHANNELS_PACKED 0x16

Servo mymotor;  // Servo motor object

int led = D2;  // LED pin
uint16_t rcChannels[16];  // Channel data

// Servo control debouncing variables
const int STABLE_THRESHOLD = 5;  // Required consecutive stable readings
int stableCount = 0;  // Counter to track stable readings
bool motorActive = false;  // State of the motor

void processCRSFPacket(uint8_t *packet, uint8_t length) {
  uint8_t packetType = packet[2];

  if (packetType == CRSF_FRAMETYPE_RC_CHANNELS_PACKED) {
    // Extract 11-bit channel data
    rcChannels[0] = ((packet[3] | packet[4] << 8) & 0x07FF);
    rcChannels[1] = ((packet[4] >> 3 | packet[5] << 5) & 0x07FF);
    rcChannels[2] = ((packet[5] >> 6 | packet[6] << 2 | packet[7] << 10) & 0x07FF);
    rcChannels[3] = ((packet[7] >> 1 | packet[8] << 7) & 0x07FF);
    rcChannels[4] = ((packet[8] >> 4 | packet[9] << 4) & 0x07FF);
    rcChannels[5] = ((packet[9] >> 7 | packet[10] << 1 | packet[11] << 9) & 0x07FF);
    rcChannels[6] = ((packet[11] >> 2 | packet[12] << 6) & 0x07FF);
    rcChannels[7] = ((packet[12] >> 5 | packet[13] << 3) & 0x07FF);

    // Print channel values
    Serial.print("Channels: ");
    for (int i = 0; i < 16; i++) {
      Serial.printf("%2d: %4d ", i, rcChannels[i]);
    }
    Serial.println();

    // Debounce logic for channel 7 (Anticlockwise only)
    if (rcChannels[7] >= 1000) {
      if (!motorActive) {
        stableCount++;
        if (stableCount >= STABLE_THRESHOLD) {
          mymotor.write(60);  // Rotate motor anticlockwise (position 0)
          digitalWrite(led, HIGH);  // Turn on LED
          motorActive = true;
          stableCount = 0;  // Reset counter
        }
      }
    } else {
      if (motorActive) {
        stableCount++;
        if (stableCount >= STABLE_THRESHOLD) {
          mymotor.write(90);  // Stop motor
          digitalWrite(led, LOW);  // Turn off LED
          motorActive = false;
          stableCount = 0;  // Reset counter
        }
      }
    }
  }
}

void setup() {
  Serial.begin(115200);  // Initialize serial communication
  CRSF_SERIAL_PORT.begin(CRSF_BAUD_RATE, SERIAL_8N1, RX_PIN);
  mymotor.attach(12);  // Attach servo to GPIO12
  pinMode(led, OUTPUT);  // Set LED pin as output

  Serial.println("CRSF Receiver Initialized.");
}

void loop() {
  static uint8_t packet[CRSF_MAX_PACKET_SIZE];
  static uint8_t packetIndex = 0;

  while (CRSF_SERIAL_PORT.available()) {
    uint8_t incomingByte = CRSF_SERIAL_PORT.read();

    if (packetIndex == 0 && incomingByte != 0xEE && incomingByte != 0xC8) {
      continue;  // Skip invalid bytes
    }

    packet[packetIndex++] = incomingByte;

    if (packetIndex >= 2 && packetIndex == packet[1] + 2) {
      processCRSFPacket(packet, packetIndex);
      packetIndex = 0;  // Reset for next packet
    }

    if (packetIndex >= CRSF_MAX_PACKET_SIZE) {
      packetIndex = 0;  // Prevent buffer overflow
    }
  }
}
