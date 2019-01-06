#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <ESP8266httpUpdate.h>

const char* ssid = "";
const char* password = "";

#define LED 13
#define RELAY 12
#define BUTTON 0

bool val = LOW;
ESP8266WebServer server(80);

// -----------------------------------------------------------

void sendValue () {
  server.send(200, "text/plain", val ? "on" : "off");
}

void handleOff() {
  val = LOW;
  digitalWrite(RELAY, val);  
  sendValue();
}

void handleOn() {
  val = HIGH;
  digitalWrite(RELAY, val);  
  sendValue();
}

void handleRootPath() {
  sendValue();
}

// ---------------------------------------------------------

String getArg(String name) { //Handler
  String value = "";
  
  for (int i = 0; i < server.args(); i++) {
    if(server.argName(i).equals(name)) {
      value = server.arg(i);
    }
  }
  
  return value;
}

// ---------------------------------------------------------

void handleUpdate() {
  Serial.println("update");
  
  String host = getArg("host");

  if(host.length() < 3) {
    server.send(200, "text/plain", "No");
    return;
  }
  Serial.print("Run update from ");
  Serial.println(host);
  server.send(200, "text/plain", "Yes");
  
  t_httpUpdate_return ret = ESPhttpUpdate.update(host);
  
  switch(ret) {
    case HTTP_UPDATE_FAILED:
      Serial.println("[update] Update failed.");
      break;
    case HTTP_UPDATE_NO_UPDATES:
      Serial.println("[update] Update no Update.");
      break;
    case HTTP_UPDATE_OK:
      Serial.println("[update] Update ok."); // may not called we reboot the ESP
      ESP.reset();
      break;
  }
}

// -----------------------------------------------------------

void setup() {
  // init
  Serial.begin(9600);

  
  pinMode(LED, OUTPUT);
  pinMode(RELAY, OUTPUT);
  // reset
  digitalWrite(RELAY, val);  
  digitalWrite(LED, LOW); // turn on = init

  // connect
  WiFi.begin(ssid, password);
  Serial.print("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.println(WiFi.macAddress());
    delay(250);
    Serial.print(".");
  }

  digitalWrite(LED, HIGH); // turn off led if connected
  Serial.println(WiFi.localIP()); 

/*
  server.on("/update", handleUpdate);
  server.on("/", handleRootPath);
  server.on("/off", handleOff);
  server.on("/on", handleOn);  
  server.begin();
  */
}

// -----------------------------------------------------------

void loop() {
  // server.handleClient(); // Handling of incoming requests

  if (WiFi.status() != WL_CONNECTED) {
    ESP.reset();
  } 
}

  
