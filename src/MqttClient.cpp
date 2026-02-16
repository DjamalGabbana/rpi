#include "MqttClient.h"

MqttClient::MqttClient(const std::string& server, const std::string& clientId)
    : client(server, clientId)
{
    mqtt::connect_options connOpts;
    connOpts.set_clean_session(true);
    connOpts.set_user_name("admin");
    connOpts.set_password("admin");

    try {
        client.connect(connOpts)->wait();
        std::cout << "MQTT connected" << std::endl;
    }
    catch (const mqtt::exception& exc) {
        std::cerr << "MQTT connect failed: " << exc.what() << std::endl;
    }
}


void MqttClient::publish(int temp, int hum)
{
    std::string payload =
        "{\"temperature\":" + std::to_string(temp) +
        ",\"humidity\":" + std::to_string(hum) + "}";

    auto msg = mqtt::make_message("rpi3/dht11", payload);
    msg->set_qos(1);

    client.publish(msg);
}
