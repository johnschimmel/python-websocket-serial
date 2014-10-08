/*
  Software serial multple serial test
 
 Receives from the hardware serial, sends to software serial.
 Receives from software serial, sends to hardware serial.
 
 The circuit: 
 * RX is digital pin 10 (connect to TX of other device)
 * TX is digital pin 11 (connect to RX of other device)
 
 Note:
 Not all pins on the Mega and Mega 2560 support change interrupts, 
 so only the following can be used for RX: 
 10, 11, 12, 13, 50, 51, 52, 53, 62, 63, 64, 65, 66, 67, 68, 69
 
 Not all pins on the Leonardo support change interrupts, 
 so only the following can be used for RX: 
 8, 9, 10, 11, 14 (MISO), 15 (SCK), 16 (MOSI).
 
 created back in the mists of time
 modified 25 May 2012
 by Tom Igoe
 based on Mikal Hart's example
 
 This example code is in the public domain.
 
 */
#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,11); // RX, TX
String incoming = "";
int led = 13;
int ledState = 0;

void setup()  
{


  //  Serial.println("Goodnight moon!");

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);
  mySerial.println("Hello, world?");

  pinMode(led, OUTPUT);


}

void loop() // run over and over
{

  digitalWrite(led, ledState);
  int numBytes = mySerial.available();

  if (numBytes >= 2) {
    
//    mySerial.print("got bytes");
//    mySerial.println(numBytes);
    
    
    int x = mySerial.read(); //read first byte
    int val = mySerial.read(); //read second byte if available;


    if (val == '1') {
      ledState = 1;
//      mySerial.println(val); 
    } else {
      ledState=0;
    }
    delay(1);
  }

}


void sayIt() {
  if (incoming.charAt(0) == 'A') {
    digitalWrite(led, HIGH);    
  } 
  else if (incoming.charAt(0) == 'B') {
    digitalWrite(led, LOW); 
  }
  mySerial.println(incoming);
  incoming = ""; 
}




