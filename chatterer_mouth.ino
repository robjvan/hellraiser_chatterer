/// Hellraiser Chatterer Preliminary Code

/// This code makes the following assumptions:
/// - your photodiode is connected to analog pin 0
/// - your motor is connected to digital pin 3
/// - you are powering the motor from a driver circuit or IC

const int sensorPin = A0;
const int motorPin = 3;
int lightLevel;
int lastLightLevel = -1;
int lightLevelDelta;

void setup() {
  Serial.begin(9600);
  pinMode(motorPin, OUTPUT);
  pinMode(sensorPin, INPUT);
}

void loop() {
  /// Grab current light level
  lightLevel = analogRead(sensorPin);
  // lightLevel = map(analogRead(sensorPin), 0, 1023, 0, 100); // Try mapping the value to 0-100 if there are problems
  if (Serial.available) {
    Serial.println("DEBUG - Current light level = " + lightLevel);
  }

  /// If we are running this loop for the first time, we won't have a lastLightLevel to compare against
  if (lastLightLevel == -1) {
    /// Set lastLightLevel to current light level for next loop
    lastLightLevel = lightLevel;
  } else {
    /// Check if there is a large difference between the light level and lastLightLevel
    /// This signifies someone walking by and blocking light or someone shining a light source onto the sensor
    /// We will also check if the light level has gone up or down to avoid firing when ambient light levels go up (light switched on)

    lightLevelDelta = lastLightLevel - lightLevel;

    /// The light levels are measured as values between 0 and 1024
    /// A positive difference means our current light level is lower than our last light level, or something is blocking the light    

    if (lightLevelDelta > 0) { // If you want to lower the sensitivity to light level changes, change this zero to a higher number      
      /// Fire the mouth motor for 15s
      if (Serial.available()) {
        Serial.println("DEBUG - lastLightLevel = " + lastLightLevel);
        Serial.println("DEBUG - Light level changed, running motor for 15 seconds...");
      }
      digitalWrite(motorPin, HIGH);
      delay(15000);  // wait 15 seconds before cutting power to the motor
      digitalWrite(motorPin, LOW);
    } else {
      if (Serial.available) {
        Serial.println("DEBUG - No change in light levels, ignoring...");
      }
    }
  }
}