# 🏥 Smart Remote Patient Monitoring System

A full-stack IoT system for real-time remote patient health monitoring — from ESP32 sensor firmware to a cloud backend and a live React dashboard with automated alerts.

![System Architecture](docs/architecture.png)

---

## 📌 Overview

This system continuously reads a patient's **SpO₂**, **heart rate**, and **body temperature** using an ESP32 microcontroller, transmits the data to the cloud over MQTT, and displays live vitals on a web dashboard. If any vital crosses a critical threshold, an automated email alert is sent to the caregiver.

Built as an academic project at Terna Engineering College, Mumbai University.

---

## ✨ Features

- 📡 **Real-time vitals** — SpO₂, heart rate (MAX30102), and body temperature (MLX90614)
- 🖥️ **OLED display** — live readings on SSD1306 at the device level
- ☁️ **Cloud MQTT** — data published to HiveMQ Cloud, consumed by Node.js backend
- 🗄️ **Persistent storage** — all readings stored in PostgreSQL on Railway
- 📊 **React dashboard** — live graphs, historical trends, patient status
- 📧 **Email alerts** — SendGrid triggers notifications when vitals go critical
- 🌐 **REST API** — clean backend API for readings, patients, and alerts

---

## 🏗️ Architecture

```
ESP32 Firmware
  └── Reads sensors (MAX30102, MLX90614)
  └── Displays on SSD1306 OLED
  └── Publishes JSON to HiveMQ Cloud (MQTT)
        │
        ▼
Node.js Backend (Railway)
  └── Subscribes to MQTT topic
  └── Validates and stores readings → PostgreSQL
  └── Triggers SendGrid email on threshold breach
  └── Exposes REST API
        │
        ▼
React Dashboard (Frontend)
  └── Polls REST API for live + historical data
  └── Renders vitals charts and patient status
  └── Shows alert history
```

---

## 🗂️ Project Structure

```
smart-patient-monitor/
├── client/                  # React frontend
│   ├── src/
│   │   ├── components/      # Dashboard, Charts, AlertLog
│   │   ├── pages/
│   │   └── App.jsx
│   └── package.json
│
├── server/                  # Node.js backend
│   ├── src/
│   │   ├── mqtt/            # HiveMQ subscriber
│   │   ├── routes/          # REST API routes
│   │   ├── db/              # PostgreSQL schema & queries
│   │   └── alerts/          # SendGrid email logic
│   ├── .env.example
│   └── package.json
│
├── firmware/                # ESP32 Arduino code
│   ├── main.ino
│   └── config.h             # WiFi, MQTT broker credentials (not committed)
│
├── docs/
│   ├── architecture.png
│   └── screenshots/
│
├── .gitignore
└── README.md
```

---

## 🛠️ Tech Stack

| Layer | Technology |
|---|---|
| Microcontroller | ESP32 |
| Sensors | MAX30102 (SpO₂ + HR), MLX90614 (temp) |
| Display | SSD1306 OLED (I2C) |
| Protocol | MQTT over TLS |
| Message Broker | HiveMQ Cloud |
| Backend | Node.js + Express |
| Database | PostgreSQL |
| Deployment (backend) | Railway |
| Frontend | React |
| Alerts | SendGrid |

---

## ⚙️ Setup

### Prerequisites
- Node.js v18+
- PostgreSQL (or a Railway project)
- HiveMQ Cloud account (free tier works)
- SendGrid account + verified sender email
- Arduino IDE with ESP32 board support

---

### 1. Clone the repo

```bash
git clone https://github.com/yourusername/smart-patient-monitor.git
cd smart-patient-monitor
```

---

### 2. Backend setup

```bash
cd server
npm install
cp .env.example .env
```

Fill in your `.env`:

```env
PORT=5000
DATABASE_URL=your_postgresql_connection_string

MQTT_BROKER=your_hivemq_broker_url
MQTT_PORT=8883
MQTT_USERNAME=your_mqtt_username
MQTT_PASSWORD=your_mqtt_password
MQTT_TOPIC=patient/vitals

SENDGRID_API_KEY=your_sendgrid_api_key
ALERT_EMAIL_FROM=your_verified_sender@example.com
ALERT_EMAIL_TO=caregiver@example.com

SPO2_THRESHOLD=94
HEART_RATE_LOW=50
HEART_RATE_HIGH=120
TEMP_HIGH=38.5
```

Run the backend:

```bash
npm run dev
```

---

### 3. Frontend setup

```bash
cd client
npm install
cp .env.example .env
```

```env
VITE_API_URL=http://localhost:5000
```

```bash
npm run dev
```

---

### 4. Firmware setup

Open `firmware/config.h` and fill in:

```cpp
#define WIFI_SSID       "your_wifi_name"
#define WIFI_PASSWORD   "your_wifi_password"
#define MQTT_BROKER     "your_hivemq_broker_url"
#define MQTT_PORT       8883
#define MQTT_USERNAME   "your_mqtt_username"
#define MQTT_PASSWORD   "your_mqtt_password"
#define MQTT_TOPIC      "patient/vitals"
```

Flash `firmware/main.ino` to the ESP32 via Arduino IDE.

> ⚠️ Never commit `config.h` with real credentials. It is listed in `.gitignore`.

---

## 📡 MQTT Payload Format

The ESP32 publishes the following JSON to the `patient/vitals` topic every 5 seconds:

```json
{
  "spo2": 98,
  "heart_rate": 76,
  "temperature": 36.7,
  "timestamp": 1718000000
}
```

---

## 🖥️ Dashboard Screenshots

| Live Vitals | Alert Log |
|---|---|
| *(add screenshot here)* | *(add screenshot here)* |

---

## 🚨 Alert Logic

An email alert is triggered when any of the following conditions are met:

| Vital | Condition |
|---|---|
| SpO₂ | Below 94% |
| Heart Rate | Below 50 or above 120 bpm |
| Body Temperature | Above 38.5°C |

Thresholds are configurable via environment variables.

---

## 👥 Team

| Name | Role |
|---|---|
| Dhananjay | Firmware, Backend, Deployment |
| Om Devghare | Frontend, Dashboard |
| Disha Mhatre | Sensor integration, Testing |
| Prajakta Mandare | Database schema, API design |

**Guide:** Prof. Prachi Kamble, Terna Engineering College

---

## 📄 License

This project is for academic purposes. Feel free to reference or build upon it with attribution.

---

<p align="center">Made with ☕ at Terna Engineering College, Mumbai</p>
