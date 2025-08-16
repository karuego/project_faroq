#include <Wire.h>
#include "RTClib.h"

RTC_DS3231 rtc;

// Pin relay
const int relay_lampu = 8;
const int relay_raket = 9;

// Pin LDR
const int ldr_waktu = 10;
const int ldr_lampu = 11;

// Pin untuk notifikasi
const int buzzer = 12;

void setup() {
  Serial.begin(9600);

  pinMode(13, OUTPUT);
  digitalWrite(13, LOW);

  pinMode(relay_lampu, OUTPUT);
  pinMode(relay_raket, OUTPUT);
  pinMode(ldr_waktu, INPUT_PULLUP);
  pinMode(ldr_lampu, INPUT_PULLUP);
  pinMode(buzzer, OUTPUT);

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
  // Periksa setiap 1 detik
  delay(1000);

  DateTime now = rtc.now();
  uint8_t jam = now.hour();
  uint8_t menit = now.minute();

  Serial.print("Waktu sekarang: ");
  Serial.print(jam);
  Serial.print(":");
  Serial.println(menit);

  uint8_t terang = !digitalRead(ldr_waktu);
  uint8_t aktif = !digitalRead(ldr_lampu);

  Serial.println(terang ? "Terang": "Gelap");
  Serial.print("Lampu UV ");
  Serial.println(aktif ? "Aktif" : "Non-Aktif");

  // Menyala hanya dari pukul 18:00 sampai 04:59
  if (jam >= 5 || jam < 18) {
    digitalWrite(relay_lampu, HIGH);
    digitalWrite(relay_raket, HIGH);
    return;
  }

  // tidak akan menyala kalau masih terang
  if (terang) return;

  // Menyalakan elektrokusi dan lampu UV
  digitalWrite(relay_lampu, LOW);
  digitalWrite(relay_raket, LOW);
  delay(300);

  // Lampu berhasil dinyalakan
  if (aktif && !digitalRead(relay_lampu)) {
    Serial.println("Lampu berhasil dinyalakan");
    return;
  }

  // Lampu gagal menyala
  Serial.println("Lampu gagal dinyalakan");

  // Menyalakan notifikasi
  for (uint8_t i = 0; i < 5; i++) {
    delay(200);
    digitalWrite(buzzer, 1);
    delay(200);
    digitalWrite(buzzer, 0);
  }
}
