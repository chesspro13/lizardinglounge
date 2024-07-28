#include <WiFiManager.h>
#include <ArduinoJson.h>

WebServer server(80);
JsonDocument json;
char buffer[1024];

void setup() {
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
  WiFiManager wm;

  // reset settings - wipe stored credentials for testing
  // these are stored by the esp library
  // wm.resetSettings();

  // Automatically connect using saved credentials,
  // if connection fails, it starts an access point with the specified name ( "AutoConnectAP"),
  // if empty will auto generate SSID, if password is blank it will be anonymous AP (wm.autoConnect())
  // then goes into a blocking loop awaiting configuration and will return success result

  bool res;
  // res = wm.autoConnect(); // auto generated AP name from chipid
  // res = wm.autoConnect("AutoConnectAP"); // anonymous ap
  res = wm.autoConnect("AutoConnectAP","password"); // password protected ap

  if(!res) {
      Serial.println("Failed to connect");
      ESP.restart();
  } 
  else {
      //if you get here you have connected to the WiFi    
      Serial.println("Connected...yeey :)");
  }

  setupApi();
}

void loop() {
  server.handleClient();
}

void handlePost() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }
  String body = server.arg("plain");
  json.clear();
  DeserializationError error = deserializeJson(json, body);
  if (error){
    Serial.print("deserializeJson() returned ");
    Serial.println(error.c_str());
    return;
  }
  const char* sensor = json["a"];
  Serial.println(sensor);

  // Respond to the client
  server.send(200, "application/json", "{}");
}

void handleGet() {
  if (server.hasArg("plain") == false) {
    //handle error here
  }
  String body = server.arg("plain");
  // deserializeJson(jsonDocument, body);
  createJson("Success");
  // Respond to the client
  server.send(200, "application/json", buffer);
}

void createJson(char *status) {  
  json.clear();
  json["status"] = status;
  serializeJson(json, buffer);  
}

void setupApi() {
  server.on("/getStatus", HTTP_GET, handleGet);
  server.on("/setStatus", HTTP_POST, handlePost);
 
  // start server
  server.begin();
}


