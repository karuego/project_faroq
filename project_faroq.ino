#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

// Pin relay
const int lampu = 8;
const int raket = 9;

// Pin LDR
const int ldr = 10;

void setup() {
  Serial.begin(9600);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  pinMode(lampu, OUTPUT);
  pinMode(raket, OUTPUT);
  pinMode(ldr, INPUT_PULLUP);

  // Matikan semua relay (HIGH = mati, karena aktif LOW)
  digitalWrite(lampu, HIGH);
  digitalWrite(raket, HIGH);

  while (!rtc.begin()) {
    Serial.println("RTC tidak ditemukan!");
    delay(1587);
  }

  // Gunakan ini hanya sekali untuk mengatur waktu RTC sesuai waktu komputer
  // rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

  Serial.println("Sistem siap 😁👍👍🏻...");
}

void loop() {
  DateTime now = rtc.now();

  uint8_t jam = now.hour();
  uint8_t menit = now.minute();

  Serial.print("Waktu sekarang: ");
  Serial.print(jam);
  Serial.print(":");
  Serial.println(menit);

  uint8_t gelap = digitalRead(ldr);
  Serial.println(gelap ? "Gelap" : "Terang");

  // Menyala dari jam 18:00 sampai 04:59
  // dan menyala kalau sudah gelap
  if (jam >= 18 || jam < 5) {
    if (gelap)
      digitalWrite(lampu, LOW),
      digitalWrite(raket, LOW);
  } else {
    digitalWrite(lampu, HIGH);
    digitalWrite(raket, HIGH);
  }

  // Periksa setiap 5 detik
  delay(1000);
}
