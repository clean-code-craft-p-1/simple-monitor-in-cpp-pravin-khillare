#include "./monitor.h"
#include <thread>
#include <chrono>
#include <iostream>

using namespace std::chrono;
using std::cout, std::flush, std::this_thread::sleep_for;

bool isTemperatureOk(float temperature) { return (temperature >= 95 && temperature <= 102); }
bool isPulseOk(float pulseRate) { return (pulseRate >= 60 && pulseRate <= 100); }
bool isSpO2Ok(float spo2) { return (spo2 >= 90); }

void blinkAlert(const char* message) {
#ifndef UNIT_TEST
    cout << message << "\n";
    for (int i = 0; i < 6; ++i) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
    cout << "\r  \r" << flush;
#endif
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    struct Vital {
        bool (*check)(float);
        float value;
        const char* message;
    };

    Vital vitals[] = {
        { isTemperatureOk, temperature, "Temperature is critical!" },
        { isPulseOk, pulseRate, "Pulse Rate is out of range!" },
        { isSpO2Ok, spo2, "Oxygen Saturation out of range!" }
    };

    for (const auto& v : vitals) {
        if (!v.check(v.value)) {
            blinkAlert(v.message);
            return 0;
        }
    }
    return 1;
}
