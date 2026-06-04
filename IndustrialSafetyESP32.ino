#include <WiFi.h>
#include <WebServer.h>

const char* ssid = "Industrial-Safety";
const char* password = "12345678";

WebServer server(80);

// Sensors
#define TEMP_PIN 34
#define TRIG_PIN 5
#define ECHO_PIN 18
#define IR_PIN 19

float temperature;
int distanceCM;
bool intrusion;

void readSensors()
{
    // LM35

    int sensorValue = analogRead(TEMP_PIN);

    float voltage =
        sensorValue * (3.3 / 4095.0);

    temperature =
        voltage * 100.0;

    // Ultrasonic

    digitalWrite(TRIG_PIN, LOW);
    delayMicroseconds(2);

    digitalWrite(TRIG_PIN, HIGH);
    delayMicroseconds(10);

    digitalWrite(TRIG_PIN, LOW);

    long duration =
        pulseIn(ECHO_PIN, HIGH);

    distanceCM =
        duration * 0.034 / 2;

    // IR

    intrusion =
        digitalRead(IR_PIN);
}

String getStatus()
{
    if(temperature > 45)
        return "HIGH TEMP";

    if(distanceCM < 10)
        return "OBJECT ALERT";

    if(intrusion)
        return "INTRUSION";

    return "NORMAL";
}

void handleData()
{
    readSensors();

    String json = "{";

    json += "\"temperature\":";
    json += String(temperature,1);
    json += ",";

    json += "\"distance\":";
    json += String(distanceCM);
    json += ",";

    json += "\"intrusion\":";
    json += intrusion ? "true" : "false";
    json += ",";

    json += "\"status\":\"";
    json += getStatus();
    json += "\"";

    json += "}";

    server.send(
        200,
        "application/json",
        json
    );
}

void setup()
{
    Serial.begin(115200);

    pinMode(TRIG_PIN, OUTPUT);
    pinMode(ECHO_PIN, INPUT);
    pinMode(IR_PIN, INPUT);

    WiFi.softAP(
        ssid,
        password
    );

    Serial.println();
    Serial.println("WiFi Started");
    Serial.println(WiFi.softAPIP());

    server.on("/data", handleData);

    server.begin();
}

void loop()
{
    server.handleClient();
}