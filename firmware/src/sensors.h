#pragma once
#include <Arduino.h>
#include "config.h"

typedef struct {
    int      hr;
    int      spo2;       // received from backend
    float    temp_c;
    uint32_t irBuffer[SAMPLE_COUNT];
    uint32_t redBuffer[SAMPLE_COUNT];
    bool     valid;
} VitalReading;

void         initSensors();
VitalReading readSensors();
