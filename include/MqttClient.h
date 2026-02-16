#pragma once
#include <mqtt/async_client.h>

class MqttClient {
public:
    MqttClient(const std::string& server, const std::string& clientId);
    void publish(int temp, int hum);

private:
    mqtt::async_client client;
};
