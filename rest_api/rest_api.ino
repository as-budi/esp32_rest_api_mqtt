#include <WiFi.h>
#include <HTTPClient.h>
#include <Arduino_JSON.h>

const char* ssid = "zenfone9";
const char* password = "zxcvbnm9";

String apiUrl = "http://192.168.249.143:8000/api/data";

unsigned long lastTime = 0;
unsigned long timerDelay = 5000;

void get_data(){
  if (WiFi.status() == WL_CONNECTED){
    HTTPClient http;
    http.begin(apiUrl.c_str());
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0){
      Serial.print("HTTP response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    http.end();
  }
  else{
    Serial.println("Sedang tidak terhubung dengan WiFi!!!");
  }
}

void post_data() {
  Serial.println("Posting JSON data to server...");
  if (WiFi.status() == WL_CONNECTED) { 
    HTTPClient http;   
    http.begin(apiUrl.c_str());
    http.addHeader("Content-Type", "application/json");         

    JSONVar myObject;
    myObject["device_id"] = (int) 1;
    myObject["data"] = (int) 25;
    String requestBody = JSON.stringify(myObject);
    Serial.println(requestBody);
    int httpResponseCode = http.POST(requestBody);
    if(httpResponseCode>0){
      String response = http.getString();                       
      Serial.println(httpResponseCode);   
      Serial.println(response);
    }
    else {
      Serial.print("Error occurred while sending HTTP POST: ");
      Serial.println(httpResponseCode);
    }
  }
}

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);
  Serial.println("Sedang melakukan Koneksi dengan WIFi ...");
  while(WiFi.status() != WL_CONNECTED){
    delay(1000);
    Serial.println("...");
  }
  Serial.print("Berhasil terkoneksi ke WiFi dengan IP Address: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if ((millis() - lastTime) > timerDelay){
  get_data();
  post_data();
  lastTime = millis(); 
  }
}
