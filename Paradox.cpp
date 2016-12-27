#include "Paradox.h"

Paradox::Paradox(HardwareSerial &serial, uint32_t baud): paradoxSerial(&serial)
{
  baudRate = baud;
}

void Paradox::begin() {
	paradoxSerial->begin(baudRate);
  paradoxSerial->flush();
}

int Paradox::read(byte &armStatus, byte &event, byte &subEvent, byte &partition) {
	if (paradoxSerial->available() < 37) {
		return -1;
	}
    serialIndex = 0;
    while(serialIndex < 37) { // Paradox packet is 37 bytes 
    
        serialData[serialIndex++] = paradoxSerial->read();
    }
    serialData[++serialIndex] = 0x00; // Make it print-friendly

    bool isData = (serialData[0] & 0xF0) == 0xE0;
    if (!isData) {
      failedCount++;
      if (failedCount > 3) {
        reset();
      } else {
        flushBuffer();
      }
    } else {
      armStatus=serialData[0];
      event=serialData[7];
      subEvent=serialData[8];
      partition=serialData[9];
    }
    return isData ? 0 : 1;
}

void Paradox::flushBuffer() {
	while (paradoxSerial->available()) {
    paradoxSerial->read();
	}
}


void Paradox::reset() {
  paradoxSerial->end();
  delay(200);
  paradoxSerial->begin(baudRate);
  delay(200);
  paradoxSerial->flush();
}


