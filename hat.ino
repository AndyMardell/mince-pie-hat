// Include Libraries

#include <Servo.h>
#include "pitches.h"
#include <Adafruit_NeoPixel.h>

// Setup Servo

Servo servo;

// Setup Pin Numbers

int buttonPin = D1;
int songPin = D4;
int servoPin = D5;
int ringPin = D2;

// Setup Variables

int debounce = 500;
int servoAngle = 0;
int playing = 0;

// Setup Neopixel
Adafruit_NeoPixel strip = Adafruit_NeoPixel(24, ringPin, NEO_GRB + NEO_KHZ400);
int colPos;

// Init Function

void setup() {

  // Attach interupt to button, call playTune()
  attachInterrupt(buttonPin, playPause, RISING);

  // Set Pin Modes
  pinMode(buttonPin, INPUT);
  pinMode(songPin, OUTPUT);
  servo.attach(servoPin);

  // Serial Init
  Serial.begin(115200);
  Serial.println("Ready");

  // Set all pixels off
  strip.begin();
  strip.show();

}

// Main Program

void loop() {

  // Write servo angle
  servo.write(servoAngle);
  
  // Add 10deg to servo angle
  servoAngle = servoAngle + 10;
  Serial.println("Moved +10 degrees");

  // If 90deg, reset angle
  if(servoAngle >= 90) {
    servoAngle = 0;
  }

  // Neopixel
  uint16_t i, j;

  if(colPos >= 255) {
    colPos = 0;
  }
  
  for(i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel((i+colPos) & 255));
  }
  
  strip.show();
  colPos = colPos + 15;
  delay(30);

}

// Play/Pause function

void playPause() {
  
  if(playing == 0) {
    digitalWrite(songPin, HIGH);
    delay(debounce);
    playing = 1;
  } else {
    digitalWrite(songPin, LOW);
    delay(debounce);
    playing = 0;
  }

}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

