
#include "Arduino.h"
#include "ESP8266.h"

class WifiLogger {
public:
	WifiLogger(HardwareSerial &serial, uint32_t baud);
	void begin(String ssid, String password);
	void log(String host, uint32_t port, byte armStatus, byte event, byte subEvent, byte partition);
private:
  int failCounter;
	ESP8266 *wifi;
};
