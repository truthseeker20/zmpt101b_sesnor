/*
  ZMPT101B - AC Voltage sensor
  Calculate Voltage for 12-bit ADC
*/
#include <Arduino.h> //comment this line, in case you are using arduino ide
// Declare variables to store sensor values and results
const int sensor_pin = 34;
const int led = 2;
double sensorValue = 0;
int val[100];   // Array to store sensor values
int max_v = 0;
double VmaxD = 0;  // Max voltage
double Veff = 0;   // Resulting voltage

// Setup function: Initializes the program
void setup() {
  Serial.begin(115200);  // Initialize serial communication
  pinMode(led, OUTPUT);
  pinMode(sensor_pin,INPUT);
}

// Loop function: Main program logic runs repeatedly
void loop() {
  // Read and process sensor values
  for (int i = 0; i < 100; i++) {
    sensorValue = analogRead(sensor_pin);  // Read analog sensor value
    if (analogRead(sensor_pin) > 2048) {
      val[i] = sensorValue;  // Store sensor value in the array if it's greater than 2050
    } else {
      val[i] = 0;  // Otherwise, set the value to 0
    }  // Short delay for stability
    delay(1);
  }

  // Find the maximum sensor value in the array
  max_v = 0;
  for (int i = 0; i < 100; i++) {
    if (val[i] > max_v) {
      max_v = val[i];  // Update max_v if a higher value is found
    }
    val[i] = 0;  // Reset the array element to 0
  }
  Serial.print("ADC PIN:");
  Serial.println(max_v);
  // Calculate effective voltage based on the maximum sensor value
  if (max_v != 0) {
    VmaxD = max_v;  // Set VmaxD to the maximum sensor value
    // Calculate effective voltage (RMS) from VmaxD
    Veff = (VmaxD*311)/(4095*sqrt(2));
  } else {
    Veff = 0;  // If no maximum value, set Veff to 0
  }

  // Print the calculated voltage to the serial monitor
  Serial.print("Voltage: ");
  Serial.println(Veff);
  if (Veff < 160 ) { //Veff < 0.7*220
    Serial.println("Bulb OFF");
    digitalWrite(led, LOW);
    Serial.println("");
  } else {
    Serial.println("Bulb ON");
    digitalWrite(led, HIGH);
    Serial.println("");
  }

  VmaxD = 0;  // Reset VmaxD for the next iteration

  delay(0.1);  // Delay for 3 seconds before the next loop
}