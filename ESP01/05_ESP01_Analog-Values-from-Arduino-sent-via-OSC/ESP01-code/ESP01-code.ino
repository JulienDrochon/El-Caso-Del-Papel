/*---------------------------------------------------------------------------------------------

  Open Sound Control (OSC) library for the ESP8266/ESP32

  Example for sending messages from the ESP8266/ESP32 to a remote computer
  The example is sending "hello, osc." to the address "/test".

  This example code is in the public domain.

  Upload Speed : 115200
  Utiliser osc Shark sur l'ordinateur pour vérifier la bonne reception des message OSC
  https://github.com/hypebeast/OSCShark/releases/tag/v0.0.1
  --------------------------------------------------------------------------------------------- */
#if defined(ESP8266)
#include <ESP8266WiFi.h>
#else
#include <WiFi.h>
#endif
#include <WiFiUdp.h>
#include <OSCMessage.h>
#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library
#include <ESP8266mDNS.h>        // Include the mDNS library

ESP8266WiFiMulti wifiMulti;


WiFiUDP Udp;                                // A UDP instance to let us send and receive packets over UDP
const IPAddress outIp(192, 168, 0, 20);     // remote IP of your computer
const unsigned int outPort = 9999;          // remote port to receive OSC
const unsigned int localPort = 8888;        // local port to listen for OSC packets (actually not used for sending)

int val = 0;
String inString = "";
String stringToInteger;
String ElectrodeIndex = "";
int ElectrodeValue = 0;
int analogValues[6] = {};

void setup() {
  Serial.begin(9600);


  wifiMulti.addAP("SFR-aea0", "QITEL7AX2FEV");   // add Wi-Fi networks you want to connect to
  wifiMulti.addAP("Pedagogie", "CRJ629%fZLm");


  Udp.begin(localPort);
  while (!Serial) {}
}
int j = 0;
const char * stringosc;

void loop() {

  boolean StringReady = false;
  //
  while (Serial.available() > 0 ) {
    char inChar = Serial.read();
    //int testInt = Serial.parseInt();

    if (isDigit(inChar))inString += (char)inChar;
    if (inChar == 'a') {
      analogValues[0] = inString.toInt();
      inString = "";
    }
    if (inChar == 'b') {
      analogValues[1] = inString.toInt();
      inString = "";
    }
    if (inChar == 'c') {
      analogValues[2] = inString.toInt();
      inString = "";
    }
    if (inChar == 'd') {
      analogValues[3] = inString.toInt();
      inString = "";
    }
    if (inChar == 'e') {
      analogValues[4] = inString.toInt();
      inString = "";
    }
    if (inChar == '<') {
      analogValues[5] = inString.toInt();
      inString = "";
      StringReady = true;
    }
    if (StringReady) {
      AnalogSendToOsc();
    }
  }

  delay(10);
}

void AnalogSendToOsc() {

  for (int i = 0; i < 6; i++) {
    String addr_prefix = "/analog";
    String addr_suffix = String(i);
    String completeaddress = addr_prefix + addr_suffix;
    const char * addr = completeaddress.c_str();

    OSCMessage msg(addr);
    msg.add(analogValues[i]);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    delay(10);
  }
  // }
}
