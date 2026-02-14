#include "Processor.h"
#include <iostream>
#include <chrono>

Processor::Processor(Sensor* sensor)
    : sensor(sensor), running(false) {}

Processor::~Processor() {
    stop();
}

void Processor::start() {
    running = true;
    processorThread = std::thread(&Processor::run, this);
}

void Processor::stop() {
    running = false;
    if (processorThread.joinable())
        processorThread.join();
}

void Processor::run() {
    while(running) {
        int t = sensor->getTemperature();
        int h = sensor->getHumidity();

        std::cout << "Processor read: "
                  << t << "°C "
                  << h << "%"
                  << std::endl;

        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
}
