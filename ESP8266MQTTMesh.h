#ifndef _ESP8266MQTTMESH_H_
#define _ESP8266MQTTMESH_H_

#if  ! defined(ESP8266MESHMQTT_DISABLE_OTA)
    #define HAS_OTA 1
    //By default we support OTA
    #define MQTT_MAX_PACKET_SIZE (1024 + 128)
#else
    #define HAS_OTA 0
#endif

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <FS.h>
#include <functional>

#define TOPIC_LEN 32

class ESP8266MQTTMesh {
private:
    const char   **networks;
    const char   *network_password;
    const char   *mesh_password;
    const char   *base_ssid;
    const char   *mqtt_server;
    const int    mqtt_port;
    const int    mesh_port;
    const char   *inTopic;
    const char   *outTopic;

    WiFiClient espClient;
    WiFiServer espServer;
    PubSubClient mqttClient;
    char mySSID[16];
    long lastMsg = 0;
    char msg[50];
    int value = 0;
    bool meshConnect = false;
    unsigned long lastReconnect = 0;
    bool AP_ready = false;
    std::function<void(String topic, String msg)> callback;

    bool connected() { return (WiFi.status() == WL_CONNECTED); }
    void die() { while(1) {} }

    bool match_bssid(const char *bssid);
    void connect();
    void connect_mqtt();
    void setup_AP();
    int read_subdomain(const char *fileName);
    void assign_subdomain();
    void send_bssids(IPAddress ip);
    void handle_client_connection(WiFiClient client);
    void parse_message(const char *topic, const char *msg);
    void mqtt_callback(char* topic, byte* payload, unsigned int length);
    void send_message(IPAddress ip, String msg);
    void broadcast_message(String msg);
    void handle_ota(const String cmd, const String msg);
    void read_until(File &f, char *buf, char delim, int len);
public:
    ESP8266MQTTMesh(const char **networks, const char *network_password, const char *mesh_password,
                    const char *base_ssid, const char *mqtt_server, int mqtt_port, int mesh_port,
                    const char *inTopic, const char *outTopic);
    void setCallback(std::function<void(String topic, String msg)> _callback);
    void setup();
    void loop();
    void publish(String subtopic, String msg);
    static void keyValue(const String data, char separator, String &key, String &value);
};
#endif //_ESP8266MQTTMESH_H_
