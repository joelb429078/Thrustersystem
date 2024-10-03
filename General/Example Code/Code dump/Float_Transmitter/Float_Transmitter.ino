#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <MS5837.h>  // BAR30 sensor library
#include <EEPROM.h>  // EEPROM library
#include <AccelStepper.h>  // Stepper motor library for NEMA 17

// Define pins for sonar and NEMA 17 stepper motor (A4988 driver)
#define TRIGGER_PIN 17
#define ECHO_PIN 16
#define STEP_PIN 25
#define DIR_PIN 26

MS5837 bar30;
RF24 radio(4, 5); // CE, CSN pins for NRF24L01
const byte address[6] = "00001";
unsigned long previousMillis = 0;
const int interval = 5000; // 5 seconds
bool reachedBottom = false;
int eeAddress = 0; // EEPROM address for storing depth data

// Stepper motor setup
AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  Wire.begin();

  // Initialize BAR30 sensor
  if (!bar30.init()) while (1);  // Halt if sensor fails to initialize
  bar30.setModel(MS5837::MS5837_30BA);
  bar30.setFluidDensity(997); // Freshwater

  // Initialize NRF24L01
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();

  // Initialize EEPROM and sonar pins
  EEPROM.begin(512);
  pinMode(TRIGGER_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  // Set up NEMA 17 motor
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Every 5 seconds, collect depth data and store it in EEPROM
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    
    // Read depth from BAR30 sensor
    bar30.read();
    float depth = bar30.depth();  // Depth in meters

    // Store depth and timestamp in EEPROM
    EEPROM.put(eeAddress, depth);
    eeAddress += sizeof(float);
    EEPROM.put(eeAddress, currentMillis);
    eeAddress += sizeof(unsigned long);
    EEPROM.commit();

    // Sonar distance
    digitalWrite(TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(TRIGGER_PIN, LOW);
    unsigned long duration = pulseIn(ECHO_PIN, HIGH);
    unsigned int sonarDistance = duration * 0.034 / 2; // Distance in cm

    // Detect bottom and start ascending
    if (!reachedBottom && sonarDistance < 20) {
      reachedBottom = true; // Bottom detected, start ascent
      stepper.setSpeed(500); // Positive speed for ascent
    }

    // Move stepper motor (up or down depending on the situation)
    if (reachedBottom) {
      stepper.runSpeed(); // Keep ascending until sonar detects the top
      if (sonarDistance > 100) {  // If the float has reached the surface
        reachedBottom = false; // Reset for next cycle
        stepper.setSpeed(-500); // Set motor to descend for the next cycle

        // Transmit stored depth data via NRF24L01
        int readAddress = 0;
        while (readAddress < eeAddress) {
          float storedDepth;
          unsigned long storedTimestamp;
          EEPROM.get(readAddress, storedDepth);
          readAddress += sizeof(float);
          EEPROM.get(readAddress, storedTimestamp);
          readAddress += sizeof(unsigned long);

          radio.write(&storedDepth, sizeof(storedDepth));         // Send depth
          radio.write(&storedTimestamp, sizeof(storedTimestamp)); // Send timestamp
        }
        eeAddress = 0; // Reset EEPROM for next cycle
        EEPROM.commit();
      }
    } else {
      // Descend until sonar detects the bottom again
      stepper.setSpeed(-500); // Negative speed for downward motion
      stepper.runSpeed(); // Keep descending
    }
  }
}
