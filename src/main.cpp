#include "Sensor.h"
#include "Processor.h"
#include <thread>
#include <chrono>

int main()
{
    Sensor sensor(4);
    Processor processor(&sensor);

    sensor.start();
    processor.start();

    while(true)
        std::this_thread::sleep_for(std::chrono::seconds(10));

    return 0;
}
