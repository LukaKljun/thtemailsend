#include "DHTesp.h" // Click here to get the library: http://librarymanager/All#DHTesp
#include <Ticker.h>

#ifndef ESP32
#pragma message(THIS EXAMPLE IS FOR ESP32 ONLY!)
#error Select ESP32 board.
#endif

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
#define BUTTON 4

void setup(){
    Serial.begin(115200);
    pinMode(BUTTON, INPUT_PULLUP);
    dht.setup(dhtPin, DHTesp::DHT11);
}
 void loop(){
    if(digitalRead(BUTTON) == LOW){
        Serial.println("Button pressed");
    }
    float Temperatura = dht.getTemperature();
    Serial.println("Temperatura je " + String(Temperatura) + "°C");
    delay(10000);
 }
