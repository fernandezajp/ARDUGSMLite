#include <Arduino.h>
#include <SoftwareSerial.h>
#include "SM5100BGSMLite.h"

#define VERBOSE

#define RXPINGSM     2
#define TXPINGSM     3
#define PIN          "1234"  // Replace this number

// Spreadtrum GSM Shield
SoftwareSerial GSMSerialport(RXPINGSM, TXPINGSM);
SM5100BGSMLite gsmModule(&GSMSerialport);

void setup() {                
  // initialize softserial gsm
  Serial.begin(9600);
  GSMSerialport.begin(9600);
  
  gsmModule.init();
  
  while (!gsmModule.isRegistered()) {
    Serial.println("searching for provider...");
    gsmModule.checkNetwork();
    delay(3000);
  }
  
  //at this point the module is registered
  //then we can send a sms
  
  //gsmModule.sendSMS("<destination number here>", "hola");
}

void loop() {
  // your program
}
