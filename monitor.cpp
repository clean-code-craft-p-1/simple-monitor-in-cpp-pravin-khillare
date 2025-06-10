#include "./monitor.h"
#include <thread>
#include <chrono>
#include <iostream>
#include <vector>
#include <algorithm>
#include <string>
#include <map>

using namespace std::chrono;
using std::cout, std::flush, std::this_thread::sleep_for;

// Language support
enum class Language { EN, DE };
Language currentLanguage = Language::EN;

// Message keys
enum class MsgKey {
    TempCritical, PulseCritical, SpO2Critical,
    WarnHypo, WarnHyper, WarnBrady, WarnTachy, WarnHypox,
};

// Message table
const std::map<Language, std::map<MsgKey, std::string>> messages = {
    { Language::EN, {
        { MsgKey::TempCritical, "Temperature is critical!" },
        { MsgKey::PulseCritical, "Pulse Rate is out of range!" },
        { MsgKey::SpO2Critical, "Oxygen Saturation out of range!" },
        { MsgKey::WarnHypo, "Warning: Approaching hypothermia" },
        { MsgKey::WarnHyper, "Warning: Approaching hyperthermia" },
        { MsgKey::WarnBrady, "Warning: Approaching bradycardia" },
        { MsgKey::WarnTachy, "Warning: Approaching tachycardia" },
        { MsgKey::WarnHypox, "Warning: Approaching hypoxemia" }
    }},
    { Language::DE, {
        { MsgKey::TempCritical, "Temperatur ist kritisch!" },
        { MsgKey::PulseCritical, "Pulsrate ist außerhalb des Bereichs!" },
        { MsgKey::SpO2Critical, "Sauerstoffsättigung außerhalb des Bereichs!" },
        { MsgKey::WarnHypo, "Warnung: Annähernde Unterkühlung" },
        { MsgKey::WarnHyper, "Warnung: Annähernde Hyperthermie" },
        { MsgKey::WarnBrady, "Warnung: Annähernde Bradykardie" },
        { MsgKey::WarnTachy, "Warnung: Annähernde Tachykardie" },
        { MsgKey::WarnHypox, "Warnung: Annähernde Hypoxämie" }
    }}
};

std::string getMsg(MsgKey key) {
    return messages.at(currentLanguage).at(key);
}

// Unit support
enum class TempUnit { F, C };

float toFahrenheit(float value, TempUnit unit) {
    return (unit == TempUnit::C) ? (value * 9.0f / 5.0f + 32.0f) : value;
}

// Pure functions for vital checks (in Fahrenheit)
bool isTemperatureOk(float temperatureF) { return (temperatureF >= 95 && temperatureF <= 102); }
bool isPulseOk(float pulseRate) { return (pulseRate >= 60 && pulseRate <= 100); }
bool isSpO2Ok(float spo2) { return (spo2 >= 90); }

// Early warning helpers
void printWarning(MsgKey warningKey) {
#ifndef UNIT_TEST
    cout << getMsg(warningKey) << "\n";
#endif
}

void checkTemperatureWarning(float temperatureF) {
    const float lower = 95.0f, upper = 102.0f;
    float tol = 0.015f * upper;
    if (temperatureF >= lower && temperatureF < lower + tol) {
        printWarning(MsgKey::WarnHypo);
    } else if (temperatureF > upper - tol && temperatureF <= upper) {
        printWarning(MsgKey::WarnHyper);
    }
}

void checkPulseWarning(float pulseRate) {
    const float lower = 60.0f, upper = 100.0f;
    float tol = 0.015f * upper;
    if (pulseRate >= lower && pulseRate < lower + tol) {
        printWarning(MsgKey::WarnBrady);
    } else if (pulseRate > upper - tol && pulseRate <= upper) {
        printWarning(MsgKey::WarnTachy);
    }
}

void checkSpO2Warning(float spo2) {
    const float lower = 90.0f;
    float tol = 0.015f * 100.0f;
    if (spo2 >= lower && spo2 < lower + tol) {
        printWarning(MsgKey::WarnHypox);
    }
}

void blinkAlert(MsgKey key) {
#ifndef UNIT_TEST
    cout << getMsg(key) << "\n";
    for (int i = 0; i < 6; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
    cout << "\r  \r" << flush;
#endif
}

// Helper function to check a single vital and warning
int checkVital(bool (*check)(float), float value, MsgKey criticalKey, void (*warn)(float)) {
    if (warn) warn(value);
    if (!check(value)) {
        blinkAlert(criticalKey);
        return 0;
    }
    return 1;
}

// Main function: accepts temperature unit
int vitalsOk(float temperature, float pulseRate, float spo2, TempUnit tempUnit = TempUnit::F) {
    float temperatureF = toFahrenheit(temperature, tempUnit);

    struct Vital {
        bool (*check)(float);
        float value;
        MsgKey criticalKey;
        void (*warn)(float);
    };

    std::vector<Vital> vitals = {
        { isTemperatureOk, temperatureF, MsgKey::TempCritical, checkTemperatureWarning },
        { isPulseOk, pulseRate, MsgKey::PulseCritical, checkPulseWarning },
        { isSpO2Ok, spo2, MsgKey::SpO2Critical, checkSpO2Warning }
    };

    for (const auto& v : vitals) {
        if (!checkVital(v.check, v.value, v.criticalKey, v.warn)) {
            return 0;
        }
    }
    return 1;
}