#include "Sensor.h"
#include "MqttClient.h"
#include <unistd.h>

int main()
{
    Sensor sensor(4);
    sensor.start();

    MqttClient mqtt("tcp://192.168.1.25:1883", "RPi3_DHT11");

    while(true)
    {
        int t = (int)sensor.getTemperature();
        int h = (int)sensor.getHumidity();

        if(t != 0 && h != 0)
        {
            mqtt.publish(t,h);
        }

        sleep(5);
    }
}
