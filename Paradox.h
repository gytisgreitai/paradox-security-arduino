#include "Arduino.h"

#define PARADOX_ZONE_IN_ALARM 36
#define PARADOX_DISARM_AFTER_ALARM 32
#define PARADOX_ALARM_CANCELED 33
#define PARADOX_TROUBLE 44
#define PARADOX_TROUBLE_OK 45

const String PARADOX_TROUBLE_CODES[] = {
  "None"
  "AC failure",
  "Battery failure",
  "Auxiliary current overload",
  "Bell current overload",
  "Bell disconnected",
  "Clock loss",
  "Fire loop trouble",
  "Monitoring station phone 1 failed",
  "Monitoring station phone 2 failed",
  "Fail to communicate with voice report",
  "RF jamming",
  "GMS RF jamming",
  "GSM no service",
  "GSM supervision lost",
  "IP receiver 1 failed (GPRS)",
  "IP receiver 2 failed (GPRS)",
  "IP module no service",
  "IP module supervision loss",
  "IP receiver 1 failed (IP)",
  "IP receiver 2 failed (IP)"
};

class Paradox {
public:

	Paradox(HardwareSerial &serial, uint32_t baud);
	void begin();
	int read(byte &armStatus, byte &event, byte &subEvent, byte &partition);

private:
	HardwareSerial *paradoxSerial;
	char serialData[38];
	byte serialIndex = 0;
  uint32_t failedCount = 0;
  uint32_t baudRate;
	void flushBuffer(void);
  void reset(void);
};
