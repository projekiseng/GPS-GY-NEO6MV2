#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <HardwareSerial.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

// PIN OLED - ESP32
// VCC - 5V /VIN
// GND - GND
// SCL - 22
// SDA - 21

// PIN GPS - ESP32
// RX - 32
// TX - 33

#define OLED_RESET -1
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

TinyGPSPlus gps;
HardwareSerial gpsSerial(2);  // UART2

void setup() {
  Serial.begin(115200);
  gpsSerial.begin(9600, SERIAL_8N1, 32, 33); // RX=32, TX=33

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("OLED gagal"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Menunggu GPS...");
  display.display();
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  display.clearDisplay();
  display.setCursor(0, 0);
  if (gps.location.isValid()) {
    display.print("Lat: ");
    display.println(gps.location.lat(), 6);
    display.print("Lng: ");
    display.println(gps.location.lng(), 6);
  } else {
    display.println("GPS belum fix...");
  }

  display.print("Satelit: ");
  display.println(gps.satellites.value());

  display.display();
}
