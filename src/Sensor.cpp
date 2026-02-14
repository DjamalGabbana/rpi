#include "Sensor.h"
#include <pigpio.h>
#include <iostream>

Sensor::Sensor(int gpioPin, int intervalMs)
    : pin(gpioPin), interval(intervalMs),
      running(false), temperature(0), humidity(0)
{
    if (gpioInitialise() < 0)
        std::cerr << "Erreur initialisation pigpio" << std::endl;
}

Sensor::~Sensor() {
    stop();
    gpioTerminate();
}

void Sensor::start() {
    running = true;
    sensorThread = std::thread(&Sensor::run, this);
}

void Sensor::stop() {
    running = false;
    if(sensorThread.joinable())
        sensorThread.join();
}

int Sensor::getTemperature() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return temperature;
}

int Sensor::getHumidity() {
    std::lock_guard<std::mutex> lock(dataMutex);
    return humidity;
}

void Sensor::run() {
    while(running) {

        int t = 0;
        int h = 0;

        if(readDHT11(t,h)) {
            std::lock_guard<std::mutex> lock(dataMutex);
            temperature = t;
            humidity = h;
            std::cout << "Thread read: "
                      << temperature << "°C "
                      << humidity << "%" << std::endl;
        }

        gpioDelay(interval * 1000);
    }
}

bool Sensor::readDHT11(int &temperature, int &humidity)
{
    uint8_t data[5] = {0};

    gpioSetMode(pin, PI_OUTPUT);
    gpioWrite(pin, 0);
    gpioDelay(18000);
    gpioWrite(pin, 1);
    gpioDelay(40);
    gpioSetMode(pin, PI_INPUT);

    uint32_t startTick = gpioTick();

    while (gpioRead(pin) == 1)
        if (gpioTick() - startTick > 100) return false;

    startTick = gpioTick();
    while (gpioRead(pin) == 0)
        if (gpioTick() - startTick > 100) return false;

    startTick = gpioTick();
    while (gpioRead(pin) == 1)
        if (gpioTick() - startTick > 100) return false;

    for (int i = 0; i < 40; i++) {

        startTick = gpioTick();
        while (gpioRead(pin) == 0)
            if (gpioTick() - startTick > 100) return false;

        uint32_t pulseStart = gpioTick();

        startTick = gpioTick();
        while (gpioRead(pin) == 1)
            if (gpioTick() - startTick > 100) return false;

        uint32_t duration = gpioTick() - pulseStart;

        if (duration > 50)
            data[i/8] |= (1 << (7 - (i%8)));
    }

    if ((data[0] + data[1] + data[2] + data[3]) != data[4])
        return false;

    humidity = data[0];
    temperature = data[2];

    return true;
}
