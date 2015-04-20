#include <SPI.h>
#include <Ethernet.h>

// assign a MAC address for the ethernet controller.
// fill in your address here:
byte mac[] = { 
   0x90, 0xA2, 0xDA, 0x0F, 0x98, 0xAD };
// fill in an available IP address on your network here,

//TMP36 Pin Variables
int tempSensor = 0; 
float tempVal = 0; // initiate the temperature value to be uploaded

// LED Slots
int rLED = 3;
int yLED = 5;
int gLED = 6;
int bLED = 9;

// initialize the library instance:
EthernetClient client;

char server[] = "data.sparkfun.com";

unsigned long lastConnectionTime = 0;          // last time you connected to the server, in milliseconds
boolean lastConnected = false;                 // state of the connection last time through the main loop
const unsigned long postingInterval = 20*1000;  // delay between updates, in milliseconds

void setup() {
  // start serial port:
  Serial.begin(9600);
    
  //LED setup
  pinMode(rLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  pinMode(gLED, OUTPUT);
  pinMode(bLED, OUTPUT);
    
  // give the ethernet module time to boot up:
  delay(1000);
  // start the Ethernet connection using a fixed IP address and DNS server:
  Ethernet.begin(mac);
}

void loop() {
 //getting the voltage reading from the temperature sensor
 int reading = analogRead(tempSensor);  
 
 // converting that reading to voltage
 float voltage = reading * 5.0;
 voltage /= 1024.0; 
 
 float temperatureC = (voltage - 0.5) * 100 ; 
 
 // now convert to Fahrenheit
 float temperatureF = (temperatureC * 9.0 / 5.0) + 32.0;
 tempVal = temperatureF;
 
 if (tempVal >= 90){
   // Too hot!! produce may rot quickly
   digitalWrite(rLED, 255);
   delay(1000);
   digitalWrite(rLED, 0);
   delay(1000);
   digitalWrite(rLED, 255);
   digitalWrite(rLED, 0);
   digitalWrite(yLED, 0);
   digitalWrite(gLED, 0);
   digitalWrite(bLED, 0);
   

 }
 
  if (tempVal < 90 && tempVal >= 80){
   digitalWrite(rLED, 255);
   digitalWrite(yLED, 0);
   digitalWrite(gLED, 0);
   digitalWrite(bLED, 0); 
 
   
 }
 
  if (tempVal < 80 && tempVal >= 70){
   digitalWrite(rLED, 128);
   digitalWrite(yLED, 128);
   digitalWrite(gLED, 0);
   digitalWrite(bLED, 0); 

 }
 
 if (tempVal < 70 && tempVal >= 60){
   digitalWrite(rLED, 0);
   digitalWrite(yLED, 255);
   digitalWrite(gLED, 0);
   digitalWrite(bLED, 0);
   

 }
 
 if (tempVal < 60 && tempVal >= 40){
   digitalWrite(rLED, 0);
   digitalWrite(yLED, 128);
   digitalWrite(gLED, 128);
   digitalWrite(bLED, 0);
   

 }
 
 if (tempVal < 40 && tempVal >= 35){
   digitalWrite(rLED, 0);
   digitalWrite(yLED, 0);
   digitalWrite(gLED, 255);
   digitalWrite(bLED, 0);
   
 }
 
 if (tempVal < 35 && tempVal >= 32){
   digitalWrite(rLED, 0);
   digitalWrite(yLED, 0);
   digitalWrite(gLED, 128);
   digitalWrite(bLED, 128);
   
 }
 
 if (tempVal < 32 && tempVal >= 30){
   digitalWrite(rLED, 0);
   digitalWrite(yLED, 0);
   digitalWrite(gLED, 0);
   digitalWrite(bLED, 255);
   
 }
 
 if (tempVal < 30){
   // Too cold
   digitalWrite(bLED, 255);
   delay(1000);
   digitalWrite(bLED, 0);
   delay(1000);
   digitalWrite(bLED, 255);
   delay(1000);
   digitalWrite(bLED, 0);
   
 }
 

  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting.");
    client.stop();
  }

  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
    client.println("Waiting 15 minutes...");
    delay(3000);
    client.println("...Done.");
  }

  lastConnected = client.connected();
}

void httpRequest() {
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // Private Key = A18JmryXYAiyDzMa4AYZ
    // Public Key = LQ37ad4Yy8Fx3YrKmZQ0
    client.print("GET /input/LQ37ad4Yy8Fx3YrKmZQ0?private_key=A18JmryXYAiyDzMa4AYZ&temperature=");
    client.print(tempVal);
    client.println(" HTTP/1.1");
    client.println("Host: data.sparkfun.com");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
  }
}
