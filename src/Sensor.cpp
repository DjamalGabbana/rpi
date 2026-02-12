#include "Sensor.h"
#include <pigpio.h>
#include <iostream>

Sensor::Sensor(int gpioPin, int intervalMs)
    : pin(gpioPin), interval(intervalMs), running(false),
      temperature(0), humidity(0)
{
    if (gpioInitialise() < 0) {
        std::cerr << "Erreur initialisation pigpio" << std::endl;
    }

    // Pas de pull-up interne nécessaire : déjà sur le module
    gpioSetMode(pin, PI_INPUT);

    // Petit délai avant première lecture
    gpioDelay(2000000); // 2 secondes
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
    if (sensorThread.joinable())
        sensorThread.join();
}

float Sensor::getLastTemperature() { return temperature; }
float Sensor::getLastHumidity() { return humidity; }

bool Sensor::readDHT11(int &temperature, int &humidity)
{
    uint8_t data[5] = {0};

    // Start signal
    gpioSetMode(pin, PI_OUTPUT);
    gpioWrite(pin, 0);
    gpioDelay(18000); // 18 ms
    gpioWrite(pin, 1);
    gpioDelay(40);
    gpioSetMode(pin, PI_INPUT);

    // Attendre réponse du DHT
    uint32_t timeout = gpioTick();
    while (gpioRead(pin) == 1) if (gpioTick() - timeout > 100) return false;
    while (gpioRead(pin) == 0);
    while (gpioRead(pin) == 1);

    // Lecture 40 bits
    for (int i = 0; i < 40; i++) {
        while (gpioRead(pin) == 0);
        uint32_t start = gpioTick();
        while (gpioRead(pin) == 1);
        uint32_t duration = gpioTick() - start;

        if (duration > 50)
            data[i / 8] |= (1 << (7 - (i % 8)));
    }

    // Vérification checksum
    if ((data[0] + data[1] + data[2] + data[3]) != data[4])
        return false;

    humidity = data[0];
    temperature = data[2];

    return true;
}

void Sensor::run()
{
    while (running) {
        int t = 0, h = 0;
        bool ok = false;

        // Retry automatique (max 5 tentatives)
        for (int i = 0; i < 5; i++) {
            if (readDHT11(t, h)) {
                ok = true;
                break;
            }
            gpioDelay(2000000); // 2 secondes
        }

        if (ok) {
            temperature = t;
            humidity = h;
            std::cout << "Temp: " << temperature << "°C, Hum: " << humidity << "%" << std::endl;
        } else {
            std::cerr << "Erreur lecture DHT11" << std::endl;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(interval));
    }
}
