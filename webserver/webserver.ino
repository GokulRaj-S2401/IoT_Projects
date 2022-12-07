#include<WiFi.h>

const char* ssid = "hello";
const char* password = "123456789";

WiFiServer server(80);

String header = "";

String buttonState = "";

void setup() {
    Serial.begin(115200);
    WiFi.begin(ssid,password);
    pinMode(13,OUTPUT);
    Serial.print("connecting");
    while(WiFi.status() != WL_CONNECTED ){
      delay(500);
      Serial.print(".");
    }
    Serial.println("WiFi connectd");
    Serial.println(WiFi.localIP());
    server.begin();
}

void loop() {
    WiFiClient client = server.available();
    if(client){
      String currentLine = "";
        while(client.connected()){
        if(client.available()){
          char c = client.read();
          header += c;
          if(c == '\n'){
          Serial.println("client connectd");

            if(currentLine.length() == 0 ){
          Serial.println("client ");

              client.println("HTTP/1.1 200/ OK");
              client.println("Content-type:text/html");
              client.println("Connection: close");
              client.println();
              if(header.indexOf("GET /13/on") >= 0){
                digitalWrite(13,HIGH);
                buttonState = "ON";
              }
              if(header.indexOf("GET /13/off") >= 0){
                digitalWrite(13,LOW);
                buttonState = "OFF";
              }
             client.println("<html><body><div align='center' ><h1> Webserver ESP32 </h1>");
            if(buttonState =="OFF"){
             client.println("<a href='/13/on' >OFF</a>");
            }
            else{
             client.println("<a href='/13/off' >ON</a>");
            }
            client.println("</div></body></html>");
            client.println();
            break;
          }
          else{
            currentLine="";
          }
        }else if(c != '\r'){
          currentLine += c;
        }
    }
  }
  header = "";
    client.stop(); 
}
}