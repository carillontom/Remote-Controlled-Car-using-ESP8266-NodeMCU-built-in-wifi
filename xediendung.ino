#define ENA   14          // Enable/speed motors Right        GPIO14(D5)
#define ENB   12          // Enable/speed motors Left         GPIO12(D6)
#define IN_1  15          // L298N in1 motors Right           GPIO15(D8)
#define IN_2  13          // L298N in2 motors Right           GPIO13(D7)
#define IN_3  2           // L298N in3 motors Left            GPIO2(D4)
#define IN_4  0           // L298N in4 motors Left            GPIO0(D3)

#include <ESP8266WiFi.h>
#include <WiFiClient.h> 
#include <ESP8266WebServer.h>

String command;             
int speedCar = 800;         // 400 - 1023.
int speed_Coeff = 3;
unsigned long lastCommandTime = 0;
unsigned long commandTimeout = 500;  //Time in milliseconds to wait before stopping 


const char* ssid = "Niggaseeker123";
ESP8266WebServer server(80);

void setup() {
 
 pinMode(ENA, OUTPUT);
 pinMode(ENB, OUTPUT);  
 pinMode(IN_1, OUTPUT);
 pinMode(IN_2, OUTPUT);
 pinMode(IN_3, OUTPUT);
 pinMode(IN_4, OUTPUT); 
  
  Serial.begin(115200);
  
// Connecting to WiFi

  WiFi.mode(WIFI_AP);
  WiFi.softAP(ssid);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
 
 //starting WEB-server 
     server.on ( "/", HTTP_handleRoot );
     server.onNotFound ( HTTP_handleRoot );
     server.begin();    
}

void goAhead(){ 

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goBack(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goRight(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goLeft(){

      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goAheadRight(){
      
      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);
 
      digitalWrite(IN_3, HIGH);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
   }

void goAheadLeft(){
      
      digitalWrite(IN_1, HIGH);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void goBackRight(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar/speed_Coeff);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, HIGH);
      analogWrite(ENB, speedCar);
  }

void goBackLeft(){ 

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, HIGH);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
  }

void stopRobot(){  

      digitalWrite(IN_1, LOW);
      digitalWrite(IN_2, LOW);
      analogWrite(ENA, speedCar);

      digitalWrite(IN_3, LOW);
      digitalWrite(IN_4, LOW);
      analogWrite(ENB, speedCar);
}

void loop() {
  server.handleClient();

  if (server.hasArg("State")) {
    command = server.arg("State");
    lastCommandTime = millis();  //Update time of last command received

    char cmd = command.charAt(0);  // get the first character
    switch (cmd) {
      case 'F': goAhead(); break;
      case 'B': goBack(); break;
      case 'L': goLeft(); break;
      case 'R': goRight(); break;
      case 'I': goAheadRight(); break;
      case 'G': goAheadLeft(); break;
      case 'J': goBackRight(); break;
      case 'H': goBackLeft(); break;
      case 'S': stopRobot(); break;

      case '0': speedCar = 400; break;
      case '1': speedCar = 470; break;
      case '2': speedCar = 540; break;
      case '3': speedCar = 610; break;
      case '4': speedCar = 680; break;
      case '5': speedCar = 750; break;
      case '6': speedCar = 820; break;
      case '7': speedCar = 890; break;
      case '8': speedCar = 960; break;
      case '9': speedCar = 1023; break;

      default: break; // Unknown command= do nothing
    }
  }

  // Stop robot if no command received for more than 'commandTimeout'
  if (millis() - lastCommandTime > commandTimeout) {
    stopRobot();
  }
}


void HTTP_handleRoot(void) {

if( server.hasArg("State") ){
       Serial.println(server.arg("State"));
  }
  server.send ( 200, "text/html", "" );
  delay(1);
}
