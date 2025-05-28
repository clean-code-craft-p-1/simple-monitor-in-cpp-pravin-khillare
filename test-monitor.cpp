#include <gtest/gtest.h>
#include "./monitor.h"

TEST(Monitor, TemperatureOutOfRange) {
    ASSERT_FALSE(vitalsOk(94.9, 70, 98));   // Too low
    ASSERT_FALSE(vitalsOk(102.1, 70, 98));  // Too high
    ASSERT_TRUE(isTemperatureOk(95));
    ASSERT_TRUE(isTemperatureOk(102));
    ASSERT_FALSE(isTemperatureOk(94.9));
    ASSERT_FALSE(isTemperatureOk(102.1));
}

TEST(Monitor, PulseOutOfRange) {
    ASSERT_FALSE(vitalsOk(98, 59, 98));    // Too low
    ASSERT_FALSE(vitalsOk(98, 101, 98));   // Too high
    ASSERT_TRUE(isPulseOk(60));
    ASSERT_TRUE(isPulseOk(100));
    ASSERT_FALSE(isPulseOk(59));
    ASSERT_FALSE(isPulseOk(101));
}

TEST(Monitor, SpO2OutOfRange) {
    ASSERT_FALSE(vitalsOk(98, 70, 89));    // Too low
    ASSERT_TRUE(isSpO2Ok(90));
    ASSERT_FALSE(isSpO2Ok(89.9));
}

TEST(Monitor, AllVitalsOk) {
    ASSERT_TRUE(vitalsOk(98.1, 70, 98));
    ASSERT_TRUE(vitalsOk(95, 60, 90));     // Edge values
    ASSERT_TRUE(vitalsOk(102, 100, 90));   // Edge values
}

TEST(Monitor, MultipleVitalsOutOfRange) {
    ASSERT_FALSE(vitalsOk(94, 101, 89));   // All out of range
    ASSERT_FALSE(vitalsOk(103, 59, 88));   // All out of range
}