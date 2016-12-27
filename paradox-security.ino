#include "Paradox.h"
#include "WifiLogger.h"
#include "config.h"
#include "SMSNotify.h"
#include <Simpletimer.h>
SimpleTimer timer;

WifiLogger wifiLogger(Serial2, 115200);

Paradox paradox(Serial1, 9600);

SMSNotify smsNotify(Serial3, 9600, PHONE_NUMBER);
byte armStatus, event, subEvent, partition;

void setup() {
  
  Serial.begin(9600);
  Serial.println("Begin setup");
  
  paradox.begin();
  Serial.println("Paradox setup done");
  
  wifiLogger.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.println("wifiLogger setup done");
 
  smsNotify.begin();
  Serial.println("smsNotifier setup done");

  timer.setInterval(30000, ping);

}

void ping() {
  wifiLogger.log(WIFI_RECEIVER_IP, WIFI_RECEIVER_PORT, 0, 0, 0, 0);
}

void loop() {
  int status = paradox.read(armStatus, event, subEvent, partition);
  if (status == 0) {
    wifiLogger.log(WIFI_RECEIVER_IP, WIFI_RECEIVER_PORT, armStatus, event, subEvent, partition);
    
    if (event == PARADOX_ZONE_IN_ALARM && armStatus == 228) {
      String message = "ALARM! Triggered on zone: ";
    	smsNotify.queueNotification(message + SECURITY_ZONE_NAMES[subEvent]);
    } else if (event == PARADOX_DISARM_AFTER_ALARM) {
      String message = "System disarmed after alarm by user: ";
      smsNotify.queueNotification(message + subEvent);
    } else if (event == PARADOX_ALARM_CANCELED) {
      String message = "Alarm canceled by user: ";
      smsNotify.queueNotification(message + subEvent);
    } else if (event == PARADOX_TROUBLE) {
      String message = "Trouble with system: ";
      smsNotify.queueNotification(message + PARADOX_TROUBLE_CODES[subEvent]); 
    } else if (event == PARADOX_TROUBLE_OK) {
      String message = "Trouble restored: ";
      smsNotify.queueNotification(message + PARADOX_TROUBLE_CODES[subEvent]); 
    }
  } 

  timer.run();
  smsNotify.process();
}
