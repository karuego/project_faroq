#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

// Pin relay
const int relay_lampu = 8;
const int relay_raket = 9;

// Pin LDR
const int ldr_waktu = 10;
const int ldr_lampu = 11;

void setup() {
  Serial.begin(9600);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  pinMode(relay_lampu, OUTPUT);
  pinMode(relay_raket, OUTPUT);
  pinMode(ldr_waktu, INPUT_PULLUP);
  pinMode(ldr_lampu, INPUT_PULLUP);

  // Matikan semua relay (HIGH = mati, karena aktif LOW)
  digitalWrite(relay_lampu, HIGH);
  digitalWrite(relay_raket, HIGH);

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

  uint8_t gelap = digitalRead(ldr_waktu);
  uint8_t nonaktif = digitalRead(ldr_lampu);

  Serial.println(gelap ? "Gelap" : "Terang");
  Serial.print("Lampu UV ");
  Serial.println(nonaktif ? "Non-Aktif" : "Aktif");

  // Menyala dari jam 18:00 sampai 04:59
  if (jam >= 18 || jam < 5) {
    // tidak akan menyala kalau belum gelap
    if (!gelap) goto _;
    digitalWrite(relay_lampu, LOW),
    digitalWrite(relay_raket, LOW);
  } else {
    digitalWrite(relay_lampu, HIGH);
    digitalWrite(relay_raket, HIGH);
  }

  // Periksa setiap 1 detik
  _: delay(1000);
}
