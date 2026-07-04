#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

#define OLED_CS   10
#define OLED_DC    9
#define OLED_RST   8

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,
  &SPI, OLED_DC, OLED_RST, OLED_CS);

const int magneticPin = 2;

bool lastStableState = HIGH;
bool lastReadState = HIGH;

unsigned long lastChangeTime = 0;
const unsigned long debounceDelay = 50;

void drawScreen(bool closed) {

  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);

  display.setCursor(35, 0);
  display.println("DOOR");

  display.setCursor(10, 30);

  if (closed) {
    display.println("CLOSED");
  } else {
    display.println("OPEN");
  }

  display.display();
}

void setup() {

  Serial.begin(9600);
  pinMode(magneticPin, INPUT_PULLUP);

  if (!display.begin(SSD1306_SWITCHCAPVCC)) {
    while (1);
  }

  drawScreen(false);
}

void loop() {

  bool reading = digitalRead(magneticPin);

  if (reading != lastReadState) {
    lastChangeTime = millis();
    lastReadState = reading;
  }

  if ((millis() - lastChangeTime) > debounceDelay) {

    if (reading != lastStableState) {
      lastStableState = reading;

      if (lastStableState == LOW) {
        drawScreen(true);
        Serial.println("Door CLOSED");
      } else {
        drawScreen(false);
        Serial.println("Door OPEN");
      }
    }
  }
}
