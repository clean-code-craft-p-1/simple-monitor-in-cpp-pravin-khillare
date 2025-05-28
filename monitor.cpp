#include "./monitor.h"
#include <assert.h>
#include <thread>
#include <chrono>
#include <iostream>
using std::cout, std::flush, std::this_thread::sleep_for, std::chrono::seconds;

/**
 * Checks if the provided vital signs are within the acceptable range.
 *
 * @param temperature The body temperature of the patient in degrees Fahrenheit.
 * @param pulseRate The pulse rate of the patient in beats per minute.
 * @param spo2 The blood oxygen saturation level of the patient in percentage.
 * @return int Returns 1 if all vital signs are within the acceptable range.
 *             Returns 0 if any of the vital signs are critical or out of range.
 * 
 * This function also outputs a message and a blinking indicator if any of the vital signs
 * are out of the normal range, indicating a critical condition.
 */

// Helper function to avoid code duplication
void blinkAlert(const char* message) {
    cout << message << "\n";
    for (int i = 0; i < 6; i++) {
        cout << "\r* " << flush;
        sleep_for(seconds(1));
        cout << "\r *" << flush;
        sleep_for(seconds(1));
    }
    cout << "\r  \r" << flush; // Clear the blinking indicator after done
}

int vitalsOk(float temperature, float pulseRate, float spo2) {
    if (temperature > 102 || temperature < 95) {
        blinkAlert("Temperature is critical!");
        return 0;
    } else if (pulseRate < 60 || pulseRate > 100) {
        blinkAlert("Pulse Rate is out of range!");
        return 0;
    } else if (spo2 < 90) {
        blinkAlert("Oxygen Saturation out of range!");
        return 0;
    }
    return 1;
}