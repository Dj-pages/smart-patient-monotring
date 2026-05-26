#pragma once

// ── WiFi ──────────────────────────────────────
#define WIFI_SSID      "DJ"
#define WIFI_PASSWORD  "32187654"

// ── MQTT ──────────────────────────────────────
#define MQTT_BROKER    "broker.hivemq.com"
#define MQTT_PORT      1883

// ESP32 publishes raw data here
#define MQTT_TOPIC_PUB "rpm/vitals/patient_001"

// ESP32 subscribes to receive SpO2 from backend
#define MQTT_TOPIC_SUB "rpm/spo2/patient_001"

// ── Device Identity ───────────────────────────
#define DEVICE_ID      "device_esp32_01"
#define PATIENT_ID     "patient_001"

// ── I2C Pins ──────────────────────────────────
#define I2C_SDA        21
#define I2C_SCL        22

// ── Reading interval ──────────────────────────
#define READ_INTERVAL_MS  500

// ── Sample count ──────────────────────────────
#define SAMPLE_COUNT   100
