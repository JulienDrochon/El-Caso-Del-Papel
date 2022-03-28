#include <SoftwareSerial.h>
SoftwareSerial mySerial(4, 5); // RX, TX

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(5000);
}

void loop() {
  mySerial.print(analogRead(A0));
  mySerial.print("a");
  mySerial.print(analogRead(A1));
  mySerial.print("b");
  mySerial.print(analogRead(A2));
  mySerial.print("c");
  mySerial.print(analogRead(A3));
  mySerial.print("d");
  mySerial.print(analogRead(A4));
  mySerial.print("e");
  mySerial.print(analogRead(A5));
  mySerial.print("<");
  delay(100);
}
