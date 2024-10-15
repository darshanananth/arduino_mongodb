#include <WiFi.h>
#include <HTTPClient.h>
#include <ArduinoJson.h>

const char* ssid = "galaxy";
const char* password = "12345678";


const char* apiKey = "URbanV5hM4ut6ELAwj8wv5rVwbFPFkikKw3roK4sQKLDYiynJKVum1uUbZNJDDI9";
const char* collection = "accounts";
const char* database = "sample_analytics";
const char* dataSource = "Cluster0";

void setup() {
  Serial.begin(9600);

  // Connect to Wi-Fi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}

void loop() {
  // Create a Wi-Fi client and HTTP client objects
  WiFiClientSecure client;
  HTTPClient http;

  // Construct the request URL
  String url = "https://ap-south-1.aws.data.mongodb-api.com/app/data-zsjvk/endpoint/data/v1/action/findOne";

  // Construct the request payload
  DynamicJsonDocument payload(256);
  payload["collection"] = collection;
  payload["database"] = database;
  payload["dataSource"] = dataSource;
//  payload["projection"]["_id"] = 1;

  // Serialize the payload to a string
  String payloadStr;
  serializeJson(payload, payloadStr);

  // Set the request headers
  http.begin(client, url);
  http.addHeader("Content-Type", "application/json");
  http.addHeader("api-key", apiKey);
  http.addHeader("Access-Control-Request-Headers", "*");
  // Send the POST request with the payload
  int httpCode = http.POST(payloadStr);

  // Check the response code
  if (httpCode == HTTP_CODE_OK) {
    // Read the response
    String response = http.getString();
    Serial.print("Got response");
    // Parse the JSON response
    DynamicJsonDocument jsonDoc(1024);
    DeserializationError error = deserializeJson(jsonDoc, response);
    if (error) {
      Serial.print("JSON parsing error: ");
      Serial.println(error.c_str());
    } else {
      // Process the JSON data
      JsonObject root = jsonDoc.as<JsonObject>();
      // Access the fields in the JSON response
      // Example: String value = root["field_name"].as<String>();
      //         int value = root["field_name"].as<int>();
    }
  } else {
    Serial.print("HTTP request failed with error code: ");
    Serial.println(httpCode);
  }

  // Close the connection
  http.end();

  delay(5000); // Wait for 5 seconds before sending the next request
}
