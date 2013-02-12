#ifndef SM5100BLite_h
#define SM5100BLite_h

#include <Arduino.h>
#include <SoftwareSerial.h>

#define STATE_NONE 		0
#define STATE_ON		1
#define STATE_INITIALIZED	2
#define STATE_REGISTERED	4

#define BUF_LENGTH 64

class SM5100BGSMLite
{
  private:
    SoftwareSerial *sm5100b;
    int onOffPin;
    char *pin;
    byte state;
    byte requestModem(const char *command, uint16_t timeout, boolean check, char *buf);
    byte getsTimeout(char *buf, uint16_t timeout);

  public:
    SM5100BGSMLite(SoftwareSerial *modemSerial, char *pin);
    SM5100BGSMLite(SoftwareSerial *modemSerial);
    boolean isRegistered();
    void reset();
    void init();
    void sendSMS(char *number, char *message);
    void checkNetwork();
    void send(char *buf);
    char *receive(char *buf);
};

#endif
