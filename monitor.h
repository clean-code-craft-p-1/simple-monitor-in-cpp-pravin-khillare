#pragma once

// Pure functions for checking each vital
bool isTemperatureOk(float temperature);
bool isPulseOk(float pulseRate);
bool isSpO2Ok(float spo2);

// Main function to check all vitals
int vitalsOk(float temperature, float pulseRate, float spo2);

// Alert function (I/O)
void blinkAlert(const char* message);