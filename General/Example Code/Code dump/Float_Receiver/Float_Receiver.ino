#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(9, 10); // CE, CSN pins for NRF24L01
const byte address[6] = "00001";  // Same address as transmitter

float receivedDepth = 0.0;  // Variable to store received depth
unsigned long receivedTime = 0;  // Variable to store received timestamp

void setup() {
  Serial.begin(9600); // Start serial communication

  // Initialize NRF24L01
  radio.begin();
  radio.openReadingPipe(0, address);  // Open reading pipe
  radio.setPALevel(RF24_PA_MIN);  // Set power level
  radio.startListening();  // Set NRF24L01 to receive mode

  Serial.println("Receiver Initialized. Waiting for data...");
}

void loop() {
  if (radio.available()) {
    // Receive depth data
    radio.read(&receivedDepth, sizeof(receivedDepth));
    
    // Receive timestamp data
    radio.read(&receivedTime, sizeof(receivedTime));

    // Print received data to serial monitor
    Serial.print("Received Depth: ");
    Serial.print(receivedDepth);
    Serial.print(" meters, Time: ");
    Serial.print(receivedTime);
    Serial.println(" ms");
  }
}
