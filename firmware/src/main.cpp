#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>
#include "config.h"
#include "sensors.h"
#include "display.h"

WiFiClient   wifiClient;
PubSubClient mqtt(wifiClient);

// Global SpO2 updated by MQTT callback
volatile int latestSpo2 = 0;

// ── MQTT Callback — receives SpO2 from backend ─
void mqttCallback(char* topic, byte* payload, unsigned int length) {
    // Parse incoming SpO2 value
    char msg[32];
    memcpy(msg, payload, length);
    msg[length] = '\0';

    StaticJsonDocument<64> doc;
    deserializeJson(doc, msg);

    if (doc.containsKey("spo2")) {
        latestSpo2 = doc["spo2"].as<int>();
        Serial.printf("SpO2 received from backend: %d%%\n", latestSpo2);
    }
}

// ── WiFi Connect ──────────────────────────────
void connectWiFi() {
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("Connecting WiFi");
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
    Serial.println(" Connected");
}

// ── MQTT Connect ──────────────────────────────
void connectMQTT() {
    mqtt.setServer(MQTT_BROKER, MQTT_PORT);
    mqtt.setCallback(mqttCallback);
    while (!mqtt.connected()) {
        Serial.print("Connecting MQTT...");
        if (mqtt.connect(DEVICE_ID)) {
            Serial.println("OK");
            // Subscribe to SpO2 topic
            mqtt.subscribe(MQTT_TOPIC_SUB);
            Serial.println("Subscribed to SpO2 topic");
        } else {
            delay(3000);
        }
    }
}

// ── Send Raw Data to Backend ──────────────────
void sendReading(VitalReading &r) {
    if (WiFi.status() != WL_CONNECTED) connectWiFi();
    if (!mqtt.connected()) {
        connectMQTT();
    }
    mqtt.loop();

    DynamicJsonDocument doc(2048);
    doc["device_id"]  = DEVICE_ID;
    doc["patient_id"] = PATIENT_ID;
    doc["hr"]         = r.hr;
    doc["temp_c"]     = r.temp_c;

    // Raw samples for backend SpO2 calculation
    JsonArray ir  = doc.createNestedArray("ir");
    JsonArray red = doc.createNestedArray("red");
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        ir.add(r.irBuffer[i]);
        red.add(r.redBuffer[i]);
    }

    char payload[2048];
    serializeJson(doc, payload);
    bool sent = mqtt.publish(MQTT_TOPIC_PUB, payload);

    if (sent) {
        Serial.printf("Sent → HR:%d Temp:%.1fC SpO2:%d%%\n",
            r.hr, r.temp_c, latestSpo2);
    } else {
        Serial.println("Publish failed");
    }
}

// ── Setup ─────────────────────────────────────
void setup() {
    Serial.begin(115200);
    initSensors();
    initDisplay();
    connectWiFi();
    connectMQTT();
    Serial.println("System ready");
}

// ── Main Loop ─────────────────────────────────
void loop() {
    unsigned long start = millis();

    // Keep MQTT alive
    mqtt.loop();

    // Read sensors
    VitalReading reading = readSensors();

    // Inject latest SpO2 from backend
    reading.spo2 = latestSpo2;

    // Update OLED
    updateDisplay(reading);

    // Send to backend if finger detected
    if (reading.valid && reading.hr > 0) {
        sendReading(reading);
    } else {
        Serial.println("Waiting for finger...");
    }

    // Wait to complete interval
    unsigned long elapsed = millis() - start;
    if (elapsed < READ_INTERVAL_MS) {
        delay(READ_INTERVAL_MS - elapsed);
    }
}
