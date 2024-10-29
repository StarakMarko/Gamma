#include <SPI.h>
#include <LoRa.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>

TinyGPSPlus gps;
SoftwareSerial gpsSerial(3, 4);

#define SCK 13
#define MISO 12
#define MOSI 11
#define SS 10
#define RST 9
#define DIO0 2

void setup() {
  pinMode(13, OUTPUT);
  Serial.begin(9600);
  gpsSerial.begin(9600);

  if (!LoRa.begin(433E6)) {
    Serial.println("Помилка запуску LoRa!");
    while (1);
  }
  Serial.println("LoRa модуль запущений!");
}

void loop() {
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isValid()) {
    double latitude = gps.location.lat();
    double longitude = gps.location.lng();

    Serial.print("Latitude: ");
    Serial.println(latitude, 6);
    Serial.print("Longitude: ");
    Serial.println(longitude, 6);

    LoRa.beginPacket();
    LoRa.print(latitude, 6);
    LoRa.print(',');
    LoRa.print(longitude, 6);
    LoRa.endPacket();

    delay(10000);
    asm volatile ("jmp 0");
  }
}
