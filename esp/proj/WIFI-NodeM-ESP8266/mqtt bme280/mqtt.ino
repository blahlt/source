#include <Wire.h>
#include <ESP8266WiFi.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <ESP8266WebServer.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

Adafruit_BME280 bme; // I2C

// Replace with your network details
const char* ssid = "YOUR_SSID";
const char* password = "YOUR_PASSWORD";
float h, t, p, pin, dp;
char temperatureFString[6];
char dpString[6];
char humidityString[6];
char pressureString[7];
char pressureInchString[6];

String lastMessage;

ESP8266WebServer server(80);

WiFiClient wifiClient;
PubSubClient client(wifiClient);


#define mqtt_server "mqtt.beebotte.com"
#define TOKEN "token:token_YOUR_TOKEN"
#define CHANNEL "YOUR_CHANNEL_NAME"

#define TEMP_RESOURCE "Temperature"
#define HUMID_RESOURCE "Humidity"
#define DEWPOINT_RESOURCE "Dewpoint"
#define PRESSURE_RESOURCE "Pressure"

const char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
char id[17]; // Identifier for the MQTT connection - will set it randomly

const char * generateID()
{
  randomSeed(analogRead(0));
  int i = 0;
  for(i = 0; i < sizeof(id) - 1; i++) {
    id[i] = chars[random(sizeof(chars))];
  }
  id[sizeof(id) -1] = '\0';

  return id;
}

void httpHandleRoot();
void httpHandleNotFound();

// only runs once on boot
void setup() {
  // Initializing serial port for debugging purposes
  Serial.begin(115200);
  delay(10);
  Wire.begin(D3, D4);
  Wire.setClock(100000);
  // Connecting to WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  
  // Starting the web server
  server.begin();
  Serial.println("Web server running. Waiting for the ESP IP...");
  delay(10000); // 10 sec. wait
  
  // Printing the ESP IP address
  Serial.println(WiFi.localIP());
  Serial.println(F("BME280 test"));

  if (!bme.begin()) {
    Serial.println("Could not find a valid BME280 sensor, check wiring and restart!");
    lastMessage = String("BME280...[FAIL]. Please reset!");
    while (1);
  } else {
    Serial.println("BME280...[OK]");
    lastMessage = String("BME280...[OK]");
  }

  Serial.print("Reading temp/hum data...");
  getWeather();
  Serial.println("[OK]");

  Serial.print("HTTP server starting...");
  server.on("/", httpHandleRoot);
  server.onNotFound(httpHandleNotFound);
  server.begin();
  Serial.println("[OK]");
  
  client.setServer(mqtt_server, 1883);
}

long lastReconnect = 0;

bool reconnect() {
  if (client.connected()) {
    return true;
  }

  long now = millis();
  if (lastReconnect == 0 || now - lastReconnect > 5*1000) { // every 5sec
    lastReconnect = now;
    Serial.print("Attempting MQTT connection...");
    lastMessage = String("Attempting MQTT connection...");
  
    // If you do not want to use a username and password, change next line to
    // if (client.connect("ESP8266Client")) {
    if (client.connect(generateID(), TOKEN, "")) {
      lastMessage = String("MQTT connected!");
      Serial.println("Connected to Beebotte MQTT");
      lastReconnect = 0;
      return true;
    } else {
      lastMessage = String("MQTT connect failed.");
      Serial.print("failed, rc=");
      Serial.println(client.state());
      return false;
    }
  } else {
    return false;
  }
}

void getWeather() {
    h = bme.readHumidity();
    t = bme.readTemperature();
    //t = t*1.8+32.0; // convert C to F
    dp = t-0.36*(100.0-h);
    
    p = bme.readPressure()/100.0F;
    pin = 0.02953*p;
    dtostrf(t, 5, 1, temperatureFString);
    dtostrf(h, 5, 1, humidityString);
    dtostrf(p, 6, 1, pressureString);
    dtostrf(pin, 5, 2, pressureInchString);
    dtostrf(dp, 5, 1, dpString);
    delay(100);
}

long lastMsg = 0;


// publishes data to the specified resource
void publish(const char* resource, float data, bool persist)
{
  StaticJsonBuffer<128> jsonOutBuffer;
  JsonObject& root = jsonOutBuffer.createObject();
  root["channel"] = CHANNEL;
  root["resource"] = resource;
  if (persist) {
    root["write"] = true;
  }
  root["data"] = data;

  // Now print the JSON into a char buffer
  char buffer[128];
  root.printTo(buffer, sizeof(buffer));

  // Create the topic to publish to
  char topic[64];
  sprintf(topic, "%s/%s", CHANNEL, resource);

  // Now publish the char buffer to Beebotte
  client.publish(topic, buffer);
}

bool postDataNow = false;

void postData() {
  if (!postDataNow) {
    return;
  }
  
  if (!reconnect())
  {
    return;
  }
  client.loop();

  getWeather();

  Serial.print("New temperature:");
  Serial.println(temperatureFString);
  //client.publish(temperature_topic, temperatureFString, true);
  publish(TEMP_RESOURCE, t, true);
  
  Serial.print("New humidity:");
  Serial.println(humidityString);
  //client.publish(humidity_topic, humidityString, true);
  publish(HUMID_RESOURCE, h, true);

  Serial.print("New dewpoint:");
  Serial.println(dpString);
  publish(DEWPOINT_RESOURCE, dp, true);
  
  Serial.print("New pressure:");
  Serial.println(pressureString);
  publish(PRESSURE_RESOURCE, p, true);

  postDataNow = false;
}

void httpHandleRoot() {

  String html = String("<!DOCTYPE HTML><html><head><META HTTP-EQUIV=\"refresh\" CONTENT=\"15\"></head><body>")
    + "<h1>ESP8266 Weather Web Server 2</h1><table border=\"2\" width=\"456\" cellpadding=\"10\"><tbody><tr><td>"
    + "<h3>Temperature = " + temperatureFString + "&deg;C</h3>"
    + "<h3>Humidity = " + humidityString + "%</h3>"
    + "<h3>Approx. Dew Point = " + dpString + "&deg;C</h3>"
    + "<h3>Pressure = " + pressureString + "hPa (" + pressureInchString + "Inch)</h3>"
    + "<h3>Message: " + lastMessage + "</h3>"
    + "<h3>Post after: " + (180000 - (millis() - lastMsg)) + "ms<h3>"
    + "</td></tr></tbody></table></body></html>";
    
  server.send(200, "text/html", html);  
}

void httpHandleNotFound(){
  server.send(404, "text/plain", "404: Not found");
}

void loop() {
  server.handleClient();
  long now = millis();
  if (lastMsg == 0 || now - lastMsg > 3*60*1000) { // every 3mins
    lastMsg = now;
    postDataNow = true;
  }
  postData();
}