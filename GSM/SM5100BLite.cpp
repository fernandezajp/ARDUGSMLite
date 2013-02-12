#include "SM5100BGSMLite.h"

SM5100BGSMLite::SM5100BGSMLite(SoftwareSerial *sm5100b, char *pin)
{
  state = STATE_NONE;
  this->sm5100b = sm5100b;
  this->pin = pin;
  sm5100b->begin(9600);
}

SM5100BGSMLite::SM5100BGSMLite(SoftwareSerial *sm5100b)
{
  state = STATE_NONE;
  this->sm5100b = sm5100b;
  this->pin = NULL;
  sm5100b->begin(9600);
}

boolean SM5100BGSMLite::isRegistered() {
  return (state & STATE_REGISTERED);
}

byte SM5100BGSMLite::requestModem(const char *command, uint16_t timeout, boolean check, char *buf) {

  byte count = 0;
  char *found = 0;
  
  *buf = 0;
  Serial.println(command);
  sm5100b->flush();
  sm5100b->print(command);
  sm5100b->print("\r\n");
  count = getsTimeout(buf, timeout);
  
  if (count) {
    if (check) {
      found = strstr(buf, "\r\nOK\r\n");
      if (!found) {
	Serial.println("error buffer: ");
	Serial.print(buf);  
      }
      else
        Serial.println("OK");
    }
    else {
      Serial.print(buf);  
    }
  }
  else {
    Serial.println("->no answer");
  }
  return count;
}

byte SM5100BGSMLite::getsTimeout(char *buf, uint16_t timeout) {
  int count = 0;
  long timeIsOut = 0;
  char c;
  *buf = 0;
  timeIsOut = millis() + timeout;
  while (timeIsOut > millis() && count < (BUF_LENGTH - 1)) {  
    if (sm5100b->available()) {
      count++;
      c = sm5100b->read();
      *buf++ = c;
      timeIsOut = millis() + timeout;
    }
  }
  if (count != 0) {
    *buf = 0;
    count++;
  }
  return count;
}

void SM5100BGSMLite::init() {
  Serial.println("Initializing GSM ...");
  Serial.println("waiting for 10 secs");
  delay(10000);
  char buf[BUF_LENGTH];
  
  requestModem("AT+IPR=9600", 200, true, buf);
  //if (this->pin!=NULL) {
  //  char cmdbuf[16] = "AT+CPIN=";
  //  strcat(cmdbuf, pin);
  //  requestModem(cmdbuf, 15000, true, buf);
  //}
  
  //requestModem("AT+COPS=1,2,73404", 200, true, buf);
  requestModem("AT+CMEE=2", 200, true, buf);
  requestModem("AT+CMGF=1", 200, true, buf);
  state |= STATE_INITIALIZED;
  Serial.println("Ready.");
}

void SM5100BGSMLite::sendSMS(char *number, char *message) {
  char buf[BUF_LENGTH];
  char cmdbuf[30] = "AT+CMGS=\"";
  Serial.println("sending SMS ...");
  requestModem("AT+CMGF=1", 5000, true, buf);
  strcat(cmdbuf, number);
  strcat(cmdbuf, "\"");
  requestModem(cmdbuf, 5000, false, buf);
  sm5100b->print(message);
  sm5100b->write(0x1a);
  getsTimeout(buf, 2000);
  Serial.println(buf);
  Serial.println("ready");
}

void SM5100BGSMLite::checkNetwork() {
  char buf[BUF_LENGTH];
  char *found = 0;

  Serial.println("Cheching network ...");
  requestModem("AT+CREG?", 5000, false, buf);

  found = strstr(buf, "+CREG: 0,1");

  if (found) {
    state |= STATE_REGISTERED;
	Serial.println("registered");
  }
  else {
    state &= ~STATE_REGISTERED;
	Serial.println("no GSM Signal");
  }
}

void SM5100BGSMLite::send(char *buf) {
  sm5100b->print(buf);
}

char *SM5100BGSMLite::receive(char *buf) {
  getsTimeout(buf, 2000);
  return buf;
}
