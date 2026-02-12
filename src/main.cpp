#include "Sensor.h"
#include <iostream>
#include <csignal>

Sensor* sensor;

void signalHandler(int signum) {
    std::cout << "Arrêt du programme..." << std::endl;
    if (sensor) sensor->stop();
    exit(signum);
}

int main() {
    signal(SIGINT, signalHandler); // Ctrl+C

    sensor = new Sensor(4); // GPIO4 = pin 7
    sensor->start();

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return 0;
}
