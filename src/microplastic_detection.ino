
#include <Wire.h>
#include <Adafruit_ADS1X15.h>

// Microplastic Optical Sensor
// ESP32 + ADS1115 + UV/White/IR LEDs
// No TinyML used

Adafruit_ADS1115 ads;

// LED pins
const int UV_LED    = 25;
const int WHITE_LED = 26;
const int IR_LED    = 27;

// ADS1115 channels
// A0 = Transmission photodiode amplifier output
// A1 = Scattering photodiode amplifier output
const int PD_TRANSMISSION = 0;
const int PD_SCATTERING   = 1;

const int NUM_SAMPLES = 20;
const int SETTLE_TIME_MS = 100;

struct OpticalReading {
  float transmission;
  float scattering;
};

struct Baseline {
  OpticalReading uv;
  OpticalReading white;
  OpticalReading ir;
  bool valid = false;
};

Baseline cleanWater;

float readChannelAverage(int channel) {
  long total = 0;

  for (int i = 0; i < NUM_SAMPLES; i++) {
    total += ads.readADC_SingleEnded(channel);
    delay(5);
  }

  return (float)total / NUM_SAMPLES;
}

OpticalReading measureLED(int ledPin) {
  // Ensure only one LED is active
  digitalWrite(UV_LED, LOW);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(IR_LED, LOW);

  delay(50);

  // Take dark readings
  float darkT = readChannelAverage(PD_TRANSMISSION);
  float darkS = readChannelAverage(PD_SCATTERING);

  digitalWrite(ledPin, HIGH);
  delay(SETTLE_TIME_MS);

  float lightT = readChannelAverage(PD_TRANSMISSION);
  float lightS = readChannelAverage(PD_SCATTERING);

  digitalWrite(ledPin, LOW);

  OpticalReading result;
  result.transmission = lightT - darkT;
  result.scattering = lightS - darkS;

  return result;
}

void printReading(const char* name, OpticalReading r) {
  Serial.print(name);
  Serial.print(",");
  Serial.print(r.transmission, 2);
  Serial.print(",");
  Serial.println(r.scattering, 2);
}

void captureBaseline() {
  Serial.println("Capturing clean-water baseline...");
  Serial.println("Keep the clean-water sample in the chamber.");

  cleanWater.uv = measureLED(UV_LED);
  cleanWater.white = measureLED(WHITE_LED);
  cleanWater.ir = measureLED(IR_LED);
  cleanWater.valid = true;

  Serial.println("Baseline saved in RAM.");
}

void printDifference(const char* name,
                     OpticalReading current,
                     OpticalReading reference) {
  Serial.print(name);
  Serial.print("_delta_T,");
  Serial.println(current.transmission - reference.transmission, 2);

  Serial.print(name);
  Serial.print("_delta_S,");
  Serial.println(current.scattering - reference.scattering, 2);
}

void setup() {
  Serial.begin(115200);
  Wire.begin();

  pinMode(UV_LED, OUTPUT);
  pinMode(WHITE_LED, OUTPUT);
  pinMode(IR_LED, OUTPUT);

  digitalWrite(UV_LED, LOW);
  digitalWrite(WHITE_LED, LOW);
  digitalWrite(IR_LED, LOW);

  if (!ads.begin()) {
    Serial.println("ERROR: ADS1115 not detected.");
    Serial.println("Check SDA, SCL, VCC, GND and I2C wiring.");

    while (true) {
      delay(1000);
    }
  }

  // ADS1115 input range: +/-4.096 V
  ads.setGain(GAIN_ONE);

  Serial.println("Microplastic Optical Sensor Started");
  Serial.println("ESP32 + ADS1115");
  Serial.println("Enter B in Serial Monitor to capture baseline.");
  Serial.println("Format: wavelength,transmission,scattering");
}

void loop() {
  if (Serial.available()) {
    char command = Serial.read();

    if (command == 'B' || command == 'b') {
      captureBaseline();
    }
  }

  OpticalReading uv = measureLED(UV_LED);
  OpticalReading white = measureLED(WHITE_LED);
  OpticalReading ir = measureLED(IR_LED);

  Serial.println();
  Serial.println("--- Optical Sensor Readings ---");
  Serial.println("Channel,Transmission,Scattering");

  printReading("UV", uv);
  printReading("WHITE", white);
  printReading("IR", ir);

  if (cleanWater.valid) {
    Serial.println("--- Difference From Baseline ---");

    printDifference("UV", uv, cleanWater.uv);
    printDifference("WHITE", white, cleanWater.white);
    printDifference("IR", ir, cleanWater.ir);

    Serial.println(
      "NOTE: Differences indicate optical changes, "
      "not confirmed microplastic detection."
    );
  } else {
    Serial.println(
      "Capture a clean-water baseline using command B."
    );
  }

  delay(1000);
}
