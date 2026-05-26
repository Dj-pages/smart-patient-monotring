#include "display.h"
#include <Adafruit_SSD1306.h>
#include <WiFi.h>

Adafruit_SSD1306 oled(128, 64, &Wire, -1);

void initDisplay() {
    oled.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    oled.clearDisplay();
    oled.setTextColor(SSD1306_WHITE);
    oled.display();
}

void updateDisplay(VitalReading &r) {
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(0, 0);

    // HR
    oled.print("HR:   ");
    oled.print(r.hr);
    oled.println(" BPM");

    // SpO2 — from backend
    oled.print("SpO2: ");
    if (r.spo2 > 0) {
        oled.print(r.spo2);
        oled.println(" %");
    } else {
        oled.println("-- %");
    }

    // Temperature
    oled.print("Temp: ");
    oled.print(r.temp_c, 1);
    oled.println(" C");

    // WiFi status
    oled.print("WiFi: ");
    oled.println(WiFi.status() == WL_CONNECTED ? "OK" : "NO");

    oled.display();
}
