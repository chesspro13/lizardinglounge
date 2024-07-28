#include <WiFiManager.h>
#include <ArduinoJson.h>

WebServer server(80);
JsonDocument json;
char buffer[1024];

// UV Lamp control
bool UV_LAMP = false;
int UV_PIN = 1;

// Heat Lamp control
bool HEAT_LAMP = false;
int HEAT_PIN = 2;

void setup()
{
  Serial.begin(115200);
  delay(100);

  WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
  // it is a good practice to make sure your code sets wifi mode how you want it.

  // WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
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
  res = wm.autoConnect("AutoConnectAP", "password"); // password protected ap

  if (!res)
  {
    Serial.println("Failed to connect");
    ESP.restart();
  }
  else
  {
    // if you get here you have connected to the WiFi
    Serial.println("Lizarding Lounge Active");
  }

  setupApi();
}

void loop()
{
  server.handleClient();
}

bool turnOnUV()
{
  Serial.println("Turning on UV Lamp");
  UV_LAMP = true;
  return true;
}

bool turnOffUV()
{
  Serial.println("Turning off UV Lamp");
  UV_LAMP = false;
  return true;
}

bool turnOnHeat()
{
  Serial.println("Turning on Heat Lamp");
  HEAT_LAMP = true;
  return true;
}

bool turnOffHeat()
{
  Serial.println("Turning off Heat Lamp");
  HEAT_LAMP = false;
  return true;
}

void getAll()
{
  json.clear();
  json["uv"] = UV_LAMP;
  json["heat"] = HEAT_LAMP;
  serializeJson(json, buffer);
  server.send(200, "application/json", buffer);
}

void getUV()
{
  json.clear();
  json["uv"] = UV_LAMP;
  serializeJson(json, buffer);
  server.send(200, "application/json", buffer);
}

void getHeat()
{
  json.clear();
  json["heat"] = HEAT_LAMP;
  serializeJson(json, buffer);
  server.send(200, "application/json", buffer);
}

void enableAll()
{
  turnOnUV();
  turnOnHeat();
  server.send(200, "application/json", "{}");
}

void enableUV()
{
  turnOnUV();
  server.send(200, "application/json", "{}");
}

void enableHeat()
{
  turnOnHeat();
  server.send(200, "application/json", "{}");
}

void disableAll()
{
  turnOffUV();
  turnOffHeat();
  server.send(200, "application/json", "{}");
}

void disableUV()
{
  turnOffUV();
  server.send(200, "application/json", "{}");
}

void disableHeat()
{
  turnOffHeat();
  server.send(200, "application/json", "{}");
}


bool jsonFailure(String body)
{
  json.clear();
  DeserializationError error = deserializeJson(json, body);
  if (error)
  {
    Serial.print("deserializeJson() returned ");
    Serial.println(error.c_str());
    server.send(500, "application/json", "{\"result\":\"json failure\"}");
    return true;
  }
  return false;
}

void setupApi()
{
  server.on("/get_all", HTTP_GET, getAll);
  server.on("/get_uv", HTTP_GET, getUV);
  server.on("/get_heat", HTTP_GET, getHeat);

  server.on("/enable_all", HTTP_POST, enableAll);
  server.on("/enable_uv", HTTP_POST, enableUV);
  server.on("/enable_heat", HTTP_POST, enableHeat);

  server.on("/disable_all", HTTP_POST, disableAll);
  server.on("/disable_uv", HTTP_POST, disableUV);
  server.on("/disable_heat", HTTP_POST, disableHeat);

  server.begin();
}
