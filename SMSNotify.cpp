#include "SMSNotify.h"

SMSNotify * SMSNotify::instance;

SMSNotify::SMSNotify(HardwareSerial &serial, uint32_t baud, String phone): gsmSerial(&serial)
{
	baudRate = baud;
  phoneNumber = phone;
  instance = this;
}

void SMSNotify::begin() {
	gsmSerial->begin(baudRate);
  timer.setInterval(1500, processQueue);
}


void SMSNotify::queueNotification(String message) {
	queue.push(message);
}

void SMSNotify::process() {
	timer.run();
}

void SMSNotify::processQueue() {
	if (!instance->queue.isEmpty()) {
		if (instance->sendState == 0) {
      instance->gsmSerial->write("AT+CMGF=1\r\n");
      String cmgs = "AT+CMGS=\"";
      cmgs = cmgs + instance->phoneNumber + "\"\r\n";
      instance->gsmSerial->print(cmgs);
			instance->sendState = 1;
		}
		else if (instance->sendState == 1) {
		   instance->sendState = 2;
		}
		else if (instance->sendState == 2) {
      String message = instance->queue.peek();
      char data[message.length() + 1];
      message.toCharArray(data, message.length() + 1);
      instance->gsmSerial->write(data);
			instance->sendState = 3;
		} else if (instance->sendState == 3) {
      instance->gsmSerial->write((char)26);
			instance->sendState = 4;
			instance->queue.pop();
		} else if (instance->sendState > 3) {
       // sleep for 4 cycles.
       instance->sendState++;
       if( instance->sendState > 7) {
          instance->sendState = 0;
       }
		}
	}
}
