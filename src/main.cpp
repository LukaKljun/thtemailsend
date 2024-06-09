#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
#include <Ticker.h>
#include <Arduino.h>
#include "WiFi.h"
#include <mqtt_client.h>
#include <esp_log.h>

#ifndef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif

esp_mqtt_client_config_t konfiguracija;
esp_mqtt_client_handle_t klient;

void kaj_se_dogaja(void *event_handler_arg, esp_event_base_t event_base, int32_t event_id, void *event_data){
  Serial.println("Nekaj se je zgodilo.");
}

/**************************************************************/
/* Example how to read DHT sensors from an ESP32 using multi- */
/* tasking.                                                   */
/* This example depends on the Ticker library to wake up      */
/* the task every 20 seconds                                  */
/**************************************************************/

DHTesp dht;

void tempTask(void *pvParameters);
bool getTemperature();
void triggerGetTemp();

/** Task handle for the light value read task */
TaskHandle_t tempTaskHandle = NULL;
/** Ticker for temperature reading */
Ticker tempTicker;
/** Comfort profile */
ComfortState cf;
/** Flag if task should run */
bool tasksEnabled = false;
/** Pin number for DHT11 data pin */
int dhtPin = 33;
int cas = 0;
#define BUTTON 4

void setup(){
    Serial.begin(115200);
    WiFi.begin("iLuka", "23456789");
    delay(6000);

    konfiguracija.client_id = "esp32_luka";
    konfiguracija.host = "nether.mojvegovc.si";
    konfiguracija.port = 1883;
    konfiguracija.username = "VegovaLjubljana";
    konfiguracija.password = "RSOv";
    konfiguracija.lwt_msg = "esp has disconnectLu";
    konfiguracija.lwt_topic = "banana";
    konfiguracija.lwt_retain = true;
    konfiguracija.lwt_msg_len = sizeof konfiguracija.lwt_msg;
    pinMode(BUTTON, INPUT_PULLUP);
    dht.setup(dhtPin, DHTesp::DHT11);

    klient = esp_mqtt_client_init(&konfiguracija);
    bool started = esp_mqtt_client_start(klient);
  
    esp_mqtt_client_register_event(klient, MQTT_EVENT_ANY, kaj_se_dogaja , NULL);
    char message[] = "esp_ connected.";
    int a = esp_mqtt_client_publish(klient, "banana", message, 0, 0, 1);
    log_i("Publihs je vrnil kodo %d.", a);

    esp_mqtt_client_subscribe(klient, "banana", 0);
}
 void loop(){
     if(digitalRead(BUTTON) == LOW){
        
            float Temperatura = dht.getTemperature();
            String message = "Temperatura je " + String(Temperatura) + " °C";
            char messageArray[message.length() + 1];
            message.toCharArray(messageArray, message.length() + 1);
            int a = esp_mqtt_client_publish(klient, "banana", messageArray, 0, 0, 1);
            log_i("Publihs je vrnil kodo %d.", a);
            cas = 10;
            delay (10000);
        
    
 }
 }
