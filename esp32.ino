#include <WiFi.h>
#include <HTTPClient.h>

#define BUTTON_PIN 1

const char *ssid = "{ssid}";
const char *password = "{password}";

const String token = "{token}";
const String pageId = "{pageId}";
const String componentId = "{componentId}";

void setup() {
  WiFi.begin(ssid, password);

  Serial.begin(115200);
  Serial.print("Connecting...");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("Connected to Wi-Fi");

  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  static int lastState = HIGH;

  int state = digitalRead(BUTTON_PIN);
  if (state == LOW && lastState == HIGH) {
    sendStatus("degraded_performance","This system is used. Please wait until it is finished.");
  }
  if (state == HIGH && lastState == LOW) {
    sendStatus("operational","Can be used this system.");
  }
  lastState = state;
  delay(100);
}

void sendStatus(String message, String description) { 
  HTTPClient client;

  String url = "https://api.statuspage.io/v1/pages/" + pageId + "/components/" + componentId;
  client.begin(url);
  client.addHeader("Content-Type", "application/json");
  client.addHeader("Authorization", "OAuth" + apiKey);

  // JSON形式のデータを作成
  String jsonPayload = "{\"component\": {\"status\": \"" + message + "\", \"description\": \"" + description + "\"}}";

  int httpResponseCode = client.PATCH(jsonPayload);

  if (httpResponseCode > 0) {
    Serial.println("HTTP Response Code: " + String(httpResponseCode));
    String response = client.getString();
    Serial.println("Response: " + response);
  } else {
    Serial.println("Error on sending POST: " + String(client.errorToString(httpResponseCode).c_str()));
  }

  client.end();
}
