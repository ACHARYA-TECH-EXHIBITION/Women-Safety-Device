#include <SoftwareSerial.h>
#include <TinyGPS++.h>

char incoming_char=0;

#define touch 11
#define led 13
#define default_co_ordinates "13.0837722,77.4820204"
 

SoftwareSerial mySerial(9, 10);
static const int RXPin = 4, TXPin = 3;
static const uint32_t GPSBaud = 9600;

// The TinyGPS++ object
TinyGPSPlus gps;
// The serial connection to the GPS device
SoftwareSerial ss(RXPin, TXPin);

void setup() {
  mySerial.begin(9600); // Setting the baud rate of GSM Module  
  Serial.begin(9600); // Setting the baud rate of Serial Monitor (Arduino)
  Serial.println("Hi initilizing the system.");
  delay(100);
  ss.begin(GPSBaud);
  pinMode(touch,INPUT);
  pinMode(led,OUTPUT);

  Serial.println("In recieving mode.");
  mySerial.println("AT+CNMI=2,2,0,0,0\r"); // AT Command to receive a live SMS
  delay(1000);

}

String data,url="";
int count =1;

void loop() {

  if(digitalRead(touch) == HIGH){
    count = 1;
    Serial.println("Women is safe");
    digitalWrite(led,HIGH);
    delay(1000);
    digitalWrite(led,LOW);

  }
  
  if(count % 30 == 0){
    //send SMS
    Serial.println("Danger");
    sendSms();
    count = 1;

  }
  
  
  while (ss.available() > 0)
    if (gps.encode(ss.read()))
      displayInfo();
  if (millis() > 5000 && gps.charsProcessed() < 10) {
    Serial.println(F("No GPS detected: check wiring."));
    while (true);
  }
}

void sendSms(){
  digitalWrite(led,HIGH);
  mySerial.println("AT+CMGF=1\r"); //Sets the GSM Module in Text Mode
  delay(1000); // Delay of 1 second
  mySerial.println("AT+CMGS=\"+917294889188\"\r"); // Replace x with mobile number
  digitalWrite(led,LOW);
  delay(1000);
  digitalWrite(led,HIGH);
  String mesage = "Hey please help me i am in trouble my location is : ";
  mesage.concat(url);
  mySerial.println(mesage); // The SMS text you want to send
  delay(100);
  mySerial.println((char) 26); // ASCII code of CTRL+Z for saying the end of sms to  the module 
  delay(1000);
  digitalWrite(led,LOW);


  Serial.println("In recieving mode.");
  mySerial.println("AT+CNMI=2,2,0,0,0\r");
}

void displayInfo() {
  //Serial.print(F("Location: "));
  if (gps.location.isValid()) {
    url = "http://www.google.com/maps/place/";
   
    url.concat(String(gps.location.lat(),6));
    url.concat(",");
    url.concat(String(gps.location.lng(),6));
    
    Serial.println(url);
    delay(1000);
    
  } else {
    Serial.print(F("INVALID"));
    url = "http://www.google.com/maps/place/";
    url.concat(default_co_ordinates);
   
  }

  if(mySerial.available() >0) { 
    
    Serial.print(mySerial.read()); 
  }
  
  Serial.println();
  count++;
  //delay(1000);
}
