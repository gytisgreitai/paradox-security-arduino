#include "WifiLogger.h"
#include "ESP8266.h"

WifiLogger::WifiLogger(HardwareSerial &serial, uint32_t baud)
{
  wifi = new ESP8266(serial, baud);
  failCounter = 0;
}

void WifiLogger::begin(String ssid, String password) {
  wifi->setOprToStation();
  while(!wifi->joinAP(ssid, password)) {
    delay(500);
  }
  wifi->disableMUX();
}

void WifiLogger::log(String host, uint32_t port, byte armStatus, byte event, byte subEvent, byte partition) {
  uint8_t buffer[4] = {armStatus, event, subEvent, partition};
	if (!wifi->registerUDP(host, port)) {
    failCounter = failCounter + 1;
	} else {
    failCounter = 0;
    wifi->send(&buffer[0], 4);
    wifi->unregisterUDP();
	}

  if (failCounter > 10) {
    wifi->restart();
    failCounter = 0;
  }
}

