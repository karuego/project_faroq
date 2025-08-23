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

  // Cek lampu UV
  cek_uv();

  Serial.println("Sistem siap 😁👍👍🏻...");
}

void loop() {
  // Jeda selama 1 detik
  delay(1000);

  DateTime now = rtc.now();
  uint8_t jam = now.hour();
  uint8_t menit = now.minute();

  Serial.print("Waktu sekarang: ");
  Serial.print(jam);
  Serial.print(":");
  Serial.println(menit);

  uint8_t terang = !digitalRead(ldr_waktu);
  // Serial.println(terang ? "Terang": "Gelap");

  // Sistem akan mati jika waktu berada direntang 05:00 sampai dengan 17:59
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
}

// Memeriksa lampu UV
void cek_uv() {
  // Menyalakan lampu UV
  digitalWrite(relay_lampu, LOW);
  delay(3000);

  // Memeriksa LDR
  uint8_t aktif = !digitalRead(ldr_lampu);

  Serial.print(F("Lampu UV "));

  if (aktif) {
    Serial.println(F("berfungsi dengan baik!"));
    digitalWrite(relay_lampu, HIGH);
    return;
  }

  Serial.println(F("tidak berfungsi / rusak !"));
  Serial.println(F("Silahkan periksa sistem dan lakukan reset pada board Arduino!"));

  // Menyalakan notifikasi selamanya
  const int buzz_delay = 5;
  while (1) {
    for (int freq = 500; freq <= 1000; freq += 10) {
      tone(buzzer, freq);
      delay(buzz_delay);
    }
    // noTone(buzzer);

    for (int freq = 51000; freq >= 500; freq -= 10) {
      tone(buzzer, freq);
      delay(buzz_delay);
    }
    // noTone(buzzer);
  }
}