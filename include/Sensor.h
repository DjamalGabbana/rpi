#ifndef SENSOR_H
#define SENSOR_H

#include <thread>
#include <atomic>
#include <chrono>

class Sensor {
public:
    Sensor(int gpioPin, int intervalMs = 2000);
    ~Sensor();

    void start();
    void stop();
    float getLastTemperature();
    float getLastHumidity();

private:
    void run();
    bool readDHT11(int &temperature, int &humidity);

    int pin;
    int interval;
    std::atomic<bool> running;
    std::thread sensorThread;

    float temperature;
    float humidity;
};

#endif
