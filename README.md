# DEPRECATED
Though not Arduino, but there is a really good alternative http://github.com/paradoxAlarmInterface/pai

# paradox-security-arduino
Arduino code to send SMS notifications and events via WIFI for Paradox Home Security system


## What does it do?
Using [Arduino Mega](https://www.arduino.cc/en/Main/ArduinoBoardMega2560) attached to [Paradox Home Security System Megallan](http://www.paradox.com/Products/default.asp?CATID=4) this can send SMS messages and log events from Security System via WIFI

Similar commercial products cost > 110€, with Arduino this can be achieved with around ~15€

## How does it work?
Arduino mega is attached to Paradox Serial interface. 

Events received via Serial interface are processed and sent via UDP over WIFI to a Raspeberry Pi (you can use any other hardware) which are forwared to AWS DynamoDB and later can be viewed via Android App

Critical events - such as alarms, fire alarms, or any trouble events are sent via GSM SMS

##### RaspberryPi code that sends events to [DymamoDB can be found here](https://github.com/gytisgreitai/paradox-security-service)
##### Android application where you can [view events can be found here](https://github.com/gytisgreitai/paradox-mobile)

## Hardware used
* Cheap Chinese clone of Arduino Mega - $7.63
* ESP8266 wifi module (Delivery of events over WIFI via UDP) - $2.98
* SIM800L GPRS GSM Module (Critical event delivery over GSM via SMS) - $5.22
* LM2596 Power Supply Buck converter (for SIM800L module) - $1.5
* [Donation to Arduino](https://www.arduino.cc/en/main/donate) - $10

## Wiring
I've used Arduino Mega because it has 4 HardwareSerial ports. While in theory all of this should work with SoftwareSerial, I couldn't manage to read Paradox Serial Interface with SoftwareSerial on Arduino Pro Mini, so it was easier and less hassle to setup Arduino Mega

* Serial is used for uploading code and debug messages
* Serial1 is attached to Paradox Serial Interface
* Serial2 is attached to ESP8266
* Serial3 is attached to SIM800L

## How to use this?
Create config.h file from sample config.h.sample, wire everything up and attach this to Paradox Serial interface.

Everything is powered from Paradox Serial Interface (which gives ~13v) - it has enough juice to power SIM800L, which is really power hungry (make sure you get the voltage right with LM2596, I had to use 4v)

## Notes on code
SMS delivery is implemented directly via AT commands by using queue, so expect a 5-10 second delay between actual event and sms delivery.

Wifi should work almost instantly.

I also epxerienced random reading fails from Paradox Serial Interface, therefore there is a safety switch which restarts HardwareSerial after 3 consecutive failed packet decode attempts

Why DynamoDB ? Cause it is free, and you can simply leave your raspberry pi on DMZ

Why not wire directly everything to RaspberryPi? My raspberry does a lot of other things (NAS, Motion processing, etc.), also Paradox Security System has a backup power, so critical events will be always delivered via SMS.

## Credits
This work is based on wonderful post from Martin Harizanov [Interfacing with Paradox home security system attempt 2](https://harizanov.com/2014/07/interfacing-with-paradox-home-security-system-attempt-2/)

