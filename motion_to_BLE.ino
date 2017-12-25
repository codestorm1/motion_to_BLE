//  SerialIn_SerialOut_HM-10_01
//
//  Uses hardware serial to talk to the host computer and AltSoftSerial for communication with the bluetooth module
//
//  What ever is entered in the serial monitor is sent to the connected device
//  Anything received from the connected device is copied to the serial monitor
//  Does not send line endings to the HM-10
//
//  Pins
//  BT VCC to Arduino 5V out. 
//  BT GND to GND
//  Arduino D8 (SS RX) - BT TX no need voltage divider 
//  Arduino D9 (SS TX) - BT RX through a voltage divider (5v to 3.3v)
//

#include <Arduino.h>
#include <SPI.h>
#include <Debounce.h>
#include <AltSoftSerial.h>

//#if SOFTWARE_SERIAL_AVAILABLE
//  #include <SoftwareSerial.h>
//#endif

AltSoftSerial BTserial; 
// https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html
 
int rate = 9600;
char c=' ';
boolean NL = true;
int motionPin = 2;
int motionLed = 12;

// Instantiate a Debounce object with a millisecond debounce time
Debounce debouncer = Debounce(10 * 1000 , motionPin);

void setup()
{
    Serial.begin(rate);
    Serial.print("Sketch:   ");   Serial.println(__FILE__);
    Serial.print("Uploaded: ");   Serial.println(__DATE__);
    Serial.println(" ");
 
    BTserial.begin(rate);  
    Serial.print("BTserial started at ");
    Serial.println(rate);
    pinMode(motionPin, INPUT);
}

void recordChange(int motionStatus) {
  char onOff = motionStatus == HIGH ? '1' : '0';
  digitalWrite (motionLed, motionStatus);
  Serial.print("[Send] Motion ");
  Serial.println(onOff);
  Serial.flush();
  BTserial.write(onOff);
}

void loop() {
  long sensor = digitalRead(motionPin);
  bool changed = debouncer.update();
  if (changed) {
    Serial.println("Change detected");
    recordChange(debouncer.read());
  }
}
 
