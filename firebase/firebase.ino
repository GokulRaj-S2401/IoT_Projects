#include<WiFi.h>
#include<FirebaseESP32.h>
#include<addons/TokenHelper.h>
#include<addons/RTDBHelper.h>

#define API_KEY "AIzaSyCcrSfDbe2WNNEnWEhERGlt7p181eWyuDQ"
#define DATABASE_URL "moisture-57e47-default-rtdb.firebaseio.com/"

#define USER_EMAIL "leva@gmail.com"
#define USER_PASSWORD "google@123"

#define WIFI_SSID "hello"
#define WIFI_PASSWORD "123456789"

float soilMoisture =89;
int status = 0;

FirebaseData f;
FirebaseAuth auth;
FirebaseConfig config;

void setup() {
  Serial.begin(9600);
  WiFi.begin(WIFI_SSID,WIFI_PASSWORD);
  while(WiFi.status() != WL_CONNECTED){
    Serial.print('.');
    delay(3000);
  }
  Serial.println("Connected with IP");
  Serial.println(WiFi.localIP());

  config.api_key = API_KEY;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  config.database_url = DATABASE_URL;
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config,&auth);
  Firebase.reconnectWiFi(true);
  Firebase.setDoubleDigits(5);

}

void loop() {
  if(Firebase.ready() ){
    soilMoisture = ( 100.00 - ( (analogRead(34) / 1023.00) * 100.00 ) );
    Firebase.setFloat(f,F("/test/moisture_percentage"),soilMoisture);
    if(soilMoisture<=35){
    Firebase.set(f,F("/test/status"),"Dry");
    }
    else if(soilMoisture<=60 && soilMoisture>35){
    Firebase.set(f,F("/test/status"),"Medium");
    }
    else{
    Firebase.set(f,F("/test/status"),"Wet");
    }
  }
    delay(5000);
}
