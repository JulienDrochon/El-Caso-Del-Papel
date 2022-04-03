String inString = "";
int analogValues[6] = {};
char analogSplitters[6] = {'a', 'b', 'c', 'd', 'e', '<'};

void ProximitySendToOsc(int indElectrode[], int valElectrode[]) {
  for (byte i = 0; i < sizeof(indElectrode); i++) {
    String addr_prefix = "/proximity/";
    String addr_suffix = String(indElectrode[i]);
    String completeaddress = addr_prefix + addr_suffix;
    const char * addr = completeaddress.c_str();

    OSCMessage msg(addr);
    msg.add(valElectrode[i]);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    delay(10);
  }
}

void ToggleSendToOsc(String indElectrode, int valElectrode) {
  String addr_prefix = "/toggle/";
  String addr_suffix = indElectrode;
  String completeaddress = addr_prefix + addr_suffix;
  const char * addr = completeaddress.c_str();

  OSCMessage msg(addr);
  msg.add(valElectrode);
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty();
  delay(10);
}

void AnalogSendToOsc(int valAnalog[]) {
  for (int i = 0; i < sizeof(valAnalog); i++) {
    String addr_prefix = "/analog";
    String addr_suffix = String(i);
    String completeaddress = addr_prefix + addr_suffix;
    const char * addr = completeaddress.c_str();

    OSCMessage msg(addr);
    msg.add(valAnalog[i]);
    Udp.beginPacket(outIp, outPort);
    msg.send(Udp);
    Udp.endPacket();
    msg.empty();
    delay(10);
  }

}

void receiveFromSerial(){
  
  if (StringReady) {
    AnalogSendToOsc();
    ToggleSendToOsc();
    ProximitySendToOsc();
  }
}
