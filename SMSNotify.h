#include "Arduino.h"
#include "ESP8266.h"
#include <QueueList.h>
#include <Simpletimer.h>

class SMSNotify {
  static void processQueue();
  static SMSNotify * instance;
public:
	SMSNotify(HardwareSerial &serial, uint32_t baud, String phone);
	void begin();
	void queueNotification(String message);
	void process();
private:
	byte sendState;
	uint32_t baudRate;
  String phoneNumber;
	HardwareSerial *gsmSerial;
	QueueList <String> queue;
	SimpleTimer timer;
};
