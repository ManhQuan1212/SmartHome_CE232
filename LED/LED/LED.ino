#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ArduinoJson.h>
#include <BH1750.h>
#include <Wire.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

#define FIREBASE_HOST "smartdoor-3769e-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "9dNc24Ut1y81UxKgQAio19SjVhgaC3rBqQog39vK"
#define WIFI_SSID "Quan"   //tên wifi
#define WIFI_PASSWORD "quanprovip" //password wifi

String Led_Button= "";
String LedAuto = "";
BH1750 lightMeter(0x23);
int led = D3;
void setup(){
   Serial.begin(9600);
   delay(1000);
   pinMode(LED_BUILTIN, OUTPUT);
   pinMode(led, OUTPUT);
   WiFi.begin(WIFI_SSID, WIFI_PASSWORD); //kết nối với wifi
   Serial.print("Connecting to ");
   Serial.print(WIFI_SSID);
   while (WiFi.status() != WL_CONNECTED) {
     Serial.print(".");
     delay(500);
   }
   Serial.println();
   Serial.print("Connected to ");
   Serial.println(WIFI_SSID);
   Serial.print("IP Address is : ");
   Serial.println(WiFi.localIP()); //In ra địa chỉ IP
   Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); // Kết nối tới firebase
   Wire.begin();
   if (lightMeter.begin()) {
    Serial.println(F("BH1750 initialised"));
    }
   else {
    Serial.println(F("Error initialising BH1750"));
    }

    Serial.println(F("BH1750 Test begin"));

}

void loop() 
{
  uint16_t lux = lightMeter.readLightLevel();
  Serial.print("Light: ");
  Serial.print(lux);
  Serial.println(" lx");
  delay(1000);
  
  Led_Button = Firebase.getString("Led_Button");
  if (Led_Button == "ON") { // Kiểm tra nếu trạng thái đèn là ON
     Serial.println("Led Turned ON");
     digitalWrite(LED_BUILTIN, LOW); // make bultin led ON
     digitalWrite(led, HIGH); // make external led ON
   } else if (Led_Button == "OFF") { // Kiểm tra nếu trạng thái đèn là OFF
     Serial.println("Led Turned OFF");
     digitalWrite(LED_BUILTIN, HIGH); // make bultin led OFF
     digitalWrite(led, LOW); // make external led OFF
   } else {
     Serial.println("Wrong Credential! Please send ON/OFF");
   }
  LedAuto = Firebase.getString("Led_Auto");
  if (LedAuto == "ON") { // Kiểm tra nếu trạng thái đèn là ON
   if(lux>2){
     Serial.println("Led Turned OFF");
     digitalWrite(LED_BUILTIN, HIGH); // make bultin led OFF
     digitalWrite(led, LOW); // make external led OFF
   }
   else{
    Serial.println("Led Turned ON");
     digitalWrite(LED_BUILTIN, LOW); // make bultin led ON
     digitalWrite(led, HIGH); // make external led ON
   }
    }
}
