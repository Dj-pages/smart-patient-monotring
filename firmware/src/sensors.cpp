#include "sensors.h"
#include "config.h"
#include <Wire.h>
#include "MAX30105.h"
#include "spo2_algorithm.h"
#include <Adafruit_MLX90614.h>

MAX30105          particleSensor;
Adafruit_MLX90614 mlx;

void initSensors() {
    Wire.begin(I2C_SDA, I2C_SCL);
    particleSensor.begin(Wire, I2C_SPEED_FAST);

    byte ledBrightness = 0x1F;
    byte sampleAverage = 4;
    byte ledMode       = 2;
    int  sampleRate    = 400;
    int  pulseWidth    = 411;
    int  adcRange      = 4096;

    particleSensor.setup(ledBrightness, sampleAverage,
                         ledMode, sampleRate,
                         pulseWidth, adcRange);
    mlx.begin();
    Serial.println("Sensors ready");
}

VitalReading readSensors() {
    VitalReading r = {0, 0, 0.0, {}, {}, false};

    // Collect samples
    for (int i = 0; i < SAMPLE_COUNT; i++) {
        while (!particleSensor.available())
            particleSensor.check();
        r.redBuffer[i] = particleSensor.getRed();
        r.irBuffer[i]  = particleSensor.getIR();
        particleSensor.nextSample();
    }

    // No finger check
    if (r.irBuffer[SAMPLE_COUNT-1] < 30000) {
        Serial.println("No finger detected");
        return r;
    }

    // Calculate HR only on ESP32
    int8_t validSPO2, validHR;
    int32_t spo2, hr;
    maxim_heart_rate_and_oxygen_saturation(
        r.irBuffer, SAMPLE_COUNT, r.redBuffer,
        &spo2, &validSPO2,
        &hr,   &validHR
    );

    // Read temperature
    float t1 = mlx.readObjectTempC();
    float t2 = mlx.readObjectTempC();
    float t3 = mlx.readObjectTempC();
    r.temp_c = (t1 + t2 + t3) / 3.0;

    // Validate HR and temp
    if (validHR == 1 && hr >= 40 && hr <= 180) r.hr = hr;
    if (r.temp_c < 30.0 || r.temp_c > 42.0)   r.temp_c = 0;

    // spo2 starts at 0 — will be updated by backend callback
    r.spo2 = 0;
    r.valid = (r.hr > 0);
    return r;
}
