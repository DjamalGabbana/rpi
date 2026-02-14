#pragma once
#include "Sensor.h"
#include <thread>
#include <atomic>

class Processor {
public:
    Processor(Sensor* sensor);
    ~Processor();

    void start();
    void stop();

private:
    void run();

    Sensor* sensor;
    std::atomic<bool> running;
    std::thread processorThread;
};
