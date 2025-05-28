#include "./monitor.h"
#include <thread>
#include <chrono>
#include <iostream>
using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

// Pure functions
bool isTemperatureOk(float temperature) {
    return (temperature >= 95 && temperature <= 102);
}
bool isPulseOk(float pulseRate) {
    return (pulseRate >= 60 && pulseRate <= 100);
}
bool isSpO2Ok(float spo2) {
    return (spo2 >= 90);
}

// I/O function
void blinkAlert(const char* message) {
#ifdef UNIT_TEST
    (void)message;
#else
    cout << message << "\n";
    for (int i = 0; i < 6; i++) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
    cout << "\r  \r" << flush;
#endif
}

// Main function
int vitalsOk(float temperature, float pulseRate, float spo2) {
    if (!isTemperatureOk(temperature)) {
        blinkAlert("Temperature is critical!");
        return 0;
    }
    if (!isPulseOk(pulseRate)) {
        blinkAlert("Pulse Rate is out of range!");
        return 0;
    }
    if (!isSpO2Ok(spo2)) {
        blinkAlert("Oxygen Saturation out of range!");
        return 0;
    }
    return 1;
}