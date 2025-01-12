#include <WiFi.h>
#include <HTTPClient.h>

#define BUTTON_PIN 1

const char *ssid = "{ssid}";
const char *password = "{password}";

const String token = "{token}";


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
  static int lastState = LOW;

  int state = digitalRead(BUTTON_PIN);
  if (state == LOW && lastState == HIGH) {
    sendStatus("TRUE");
  }
  if (state == HIGH && lastState == LOW) {
    sendStatus("FALSE");
  }
  lastState = state;
  delay(100);
}

void sendStatus(String message) { 
  HTTPClient client;

  if (message == TRUE) {
    const updateData = {
      component: {
        status: status,
        description: description,
      },
    };
  }

  String url = "https://api.statuspage.io/v1/pages/" + pageId + "/components/" + componentId ;
  client.begin(url);
  client.addHeader("Content-Type", "application/x-www-form-urlencoded");
  client.addHeader("Authorization", "Bearer " + token);

  String query = "message=" + message;
  client.POST(query);

  String body = client.getString();
  Serial.println("Sent the message");
  Serial.println(body);
  client.end();
}
