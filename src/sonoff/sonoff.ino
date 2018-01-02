#include <Espiot.h>

Espiot espiot;


// APP
String appV = "1.0.1";
String SENSOR1 = "RELEY"; // BMP180, HTU21, DHT11

#define RELEY 12
#define BUILTINLED 13

#define BUTTON 0


void setup() { //------------------------------------------------
  Serial.begin(115200);
  Serial.println("Setup ...");
  delay(300);

  espiot.init(appV);
  espiot.SENSOR = SENSOR1;

  pinMode(RELEY, OUTPUT);
  pinMode(BUILTINLED, OUTPUT);

  digitalWrite(RELEY, LOW);
  digitalWrite(BUILTINLED, HIGH);

  espiot.server.on("/switch/on", HTTP_GET, []() {
    espiot.blink();
    Serial.println("/switch/on ...");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["state"] = "on";

    digitalWrite(RELEY, HIGH);
    digitalWrite(BUILTINLED, LOW);

    String content;
    root.printTo(content);
    espiot.server.send(200, "application/json", content);
  });

  espiot.server.on("/switch/off", HTTP_GET, []() {
    espiot.blink();
    Serial.println("/switch/off ...");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["state"] = "off";

    digitalWrite(RELEY, LOW);
    digitalWrite(BUILTINLED, HIGH);

    String content;
    root.printTo(content);
    espiot.server.send(200, "application/json", content);
  });

  espiot.server.on("/switch/state", HTTP_GET, []() {
    espiot.blink();
    Serial.println("/switch/state ...");
    DynamicJsonBuffer jsonBuffer;
    JsonObject &root = jsonBuffer.createObject();

    root["id"] = espiot.getDeviceId();
    root["name"] = espiot.deviceName;

    if (RELEY == LOW)
      root["state"] = "off";
    else
      root["state"] = "onf";

    String content;
    root.printTo(content);
    espiot.server.send(200, "application/json", content);
  });




} //--

// -----------------------------------------------------------------------------
// loop ------------------------------------------------------------------------
// -----------------------------------------------------------------------------
void loop() {
  yield();

  int buttonState = digitalRead(BUTTON);

  espiot.loop();

  // button pressed
  if (buttonState == LOW) {
    Serial.println(F("\nButton pressed..."));
    Serial.print("WiFi status=" + String(WiFi.status()));
    if (digitalRead(RELEY) == HIGH) {
      digitalWrite(RELEY, LOW);
      digitalWrite(BUILTINLED, HIGH);
    } else {
      digitalWrite(RELEY, HIGH);
      digitalWrite(BUILTINLED, LOW);
    }
    delay(300);
  }


} //---------------------------------------------------------------
