#include "ESP8266.h"
#include <ArduinoJson.h>
#include <dht.h>
#include "EmonLib.h"                   // Include Emon Library

#define SSID        "MYA-L11"
#define PASSWORD    "12345678"
#define HOST_NAME   "104.198.163.92"
#define HOST_PORT   (80)
String api_token = "nQZ5ZAvYYsQo6gtzzNWY9eVheqFIV7ScFsqm7qLAPGXnwKmGSJOyEd0WLgMCHQmV";


#define dht11SensorPin A4
#define mq2SensorPin A5
#define moistureSensorPin A2
#define powerSensorPin 3
#define heaterSwitch 4
#define fanSwitch 5
byte flowSensorPin = 2;

// Hall-effect flow sensor outputs approximately 4.5 pulses per second per
// litre/minute of flow.
float calibrationFactor = 9.0;

volatile byte pulseCount;

float flowRate;
unsigned int flowMilliLitres;
unsigned long totalMilliLitres;
unsigned long totalWatts;

unsigned long oldTime;
const double sendIntervals = 1000.00;

ESP8266 wifi(Serial3, 115200);
dht DHT;
EnergyMonitor emon1;                   // Create an instance


void connectToWiFi();

String sendHTTPRequest(String uri, String query_parameters = "", String method = "GET");

DynamicJsonDocument updateAndGetInstr(String parameters = "");

void manageActuator(DynamicJsonDocument json);

void initializeFlowSensor();

void setup(void) {
    Serial.begin(57600);
    Serial.print("setup begin\r\n");

    connectToWiFi();

    pinMode(heaterSwitch, OUTPUT);
    pinMode(fanSwitch, OUTPUT);
    digitalWrite(heaterSwitch, HIGH);
    digitalWrite(fanSwitch, HIGH);

    emon1.current(powerSensorPin, 11.1111);
    initializeFlowSensor();
    totalWatts = 0;

    Serial.print("setup end\r\n");
}

void loop(void) {

    totalWatts += powerSensorValue();

    if ((millis() - oldTime) > sendIntervals) {

        flowMilliLitres = flowSensor();

        oldTime = millis();

        dht t = tempHumid();

        String param = "temperature=";
        param += t.temperature;
        param += "&humidity=";
        param += t.humidity;
        param += "&air_quality=";
        param += gasSensorValue();
        param += "&soil_moisture=";
        param += moistureSensorValue();
        param += "&power_used=";
        param += totalWatts;
        param += "&water_used=";
        param += flowMilliLitres;
        param += "&send_interval=";
        param += sendIntervals;

        totalMilliLitres += flowMilliLitres;

//        Serial.println(param);
//        Serial.print("Sensation: ");
//        Serial.println(totalMilliLitres);
        manageActuator(updateAndGetInstr(param));
        //Serial.println(json_resp["status"]);


        // Reset the pulse counter so we can start incrementing again
        pulseCount = 0;
        totalWatts = 0;
        flowMilliLitres = 0;

        attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALLING);
    }
}

void initializeFlowSensor() {
    pinMode(flowSensorPin, INPUT_PULLUP);

    pulseCount = 0;
    flowRate = 0.0;
    flowMilliLitres = 0;
    totalMilliLitres = 0;
    oldTime = 0;

    attachInterrupt(digitalPinToInterrupt(flowSensorPin), pulseCounter, FALLING);
}

/*
Insterrupt Service Routine
 */
void pulseCounter() {
    // Increment the pulse counter
    pulseCount++;
}


     
