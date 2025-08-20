#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// WiFi credentials
const char* ssid = "hi";
const char* password = "12345678";

// Entry sensor (Ultrasonic #1)
const int trigPin1 = 5;   // D1
const int echoPin1 = 4;   // D2

// Exit sensor (Ultrasonic #2)
const int trigPin2 = 14;  // D5
const int echoPin2 = 12;  // D6

#define SOUND_VELOCITY 0.034
#define THRESHOLD 10   // cm detection
#define MAX_CAPACITY 8

int count = 0;
bool entryDetected = false;
bool exitDetected = false;

ESP8266WebServer server(80);

// Measure distance
float getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH, 30000); 
  if (duration == 0) return -1;  // No echo
  return (duration * SOUND_VELOCITY) / 2;
}

// Endpoint: return current count
void handleCount() {
  server.send(200, "text/plain", String(count));
}

void setup() {
  Serial.begin(9600);

  pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ Connected to WiFi!");
  Serial.print("📡 ESP IP Address: ");
  Serial.println(WiFi.localIP());

  server.on("/count", handleCount);
  server.begin();
  Serial.println("🌐 HTTP server started");
}

void loop() {
  server.handleClient();

  float distance1 = getDistance(trigPin1, echoPin1);
  float distance2 = getDistance(trigPin2, echoPin2);

  // Entry
  if (distance1 > 0 && distance1 < THRESHOLD) {
    if (!entryDetected) {
        if (count < MAX_CAPACITY) {
            count++;
            entryDetected = true;
            Serial.println("🚗 Car entered! Count = " + String(count));
        } else {
            Serial.println("⚠️ Parking Full!");
        }
    }
  } else entryDetected = false;

  // Exit
  if (distance2 > 0 && distance2 < THRESHOLD) {
    if (!exitDetected) {
        if (count > 0) count--;
        exitDetected = true;
        Serial.println("↩️ Car exited! Count = " + String(count));
    }
  } else exitDetected = false;

  delay(200);
}
