/* ArKom Blynk 
 * Thai Baan Soft
 * (c) 2019
 */
#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
SoftwareSerial swSer(D5, D6);  // RX,TX

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "your Token B2";

// Your WiFi credentials.
// Set password to "" for open networks.
char ssid[] = "wisoot";
char pass[] = "parrot66";

BlynkTimer timer;
int count = 0;
bool Connected2Blynk = false;

// This function sends Arduino's up time every second to Virtual Pin (5).
// In the app, Widget's reading frequency should be set to PUSH. This means
// that you define how often to send data to Blynk App.
void myTimerEvent()
{
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  int r = random(100);
  Blynk.virtualWrite(V0, r);
  Serial.println(r);
}

void WiFiConnect()
{
  WiFi.mode(WIFI_OFF);
  delay(100);
  WiFi.mode(WIFI_STA);
  delay(100);
  WiFi.begin(ssid, pass);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());    
}

void setup()
{
  Serial.begin(115200);
  WiFiConnect();
  Blynk.config(auth);  // in place of Blynk.begin(auth, ssid, pass);
  Blynk.connect(3333);  // timeout set to 10 seconds and then continue without Blynk
  while (Blynk.connect() == false) {
    // Wait until connected
  }
  Serial.println("Connected to Blynk server");
  timer.setInterval(4000L, CheckConnection); // check if still connected every 11 seconds 
}

void CheckConnection(){
  if (WiFi.status() != WL_CONNECTED) {
    WiFiConnect();
  }
  if (count>5) {
    Serial.println("reset");
    swSer.println("reset");
    count = 0;
    delay(5000);    
  } else {
    Connected2Blynk = Blynk.connected();
    if(!Connected2Blynk){
      count++;
      Serial.println("Not connected to Blynk server"); 
      Blynk.connect(3333);  // timeout set to 10 seconds and then continue without Blynk  
    }
    else{
      myTimerEvent();     
    }  
  }
}

void loop() {
  if(Connected2Blynk){
    Blynk.run();
  }
  timer.run();
}
