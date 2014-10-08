/*
  LeoJoy Sample Code
 Alan Chatham - 2013
 unojoy.com
 
 Hardware - Attach buttons to any of the pins 2-12, A4 and A5
 to ground, and see different controller buttons
 be pressed.
 Analog stick controls can be enabled by removing
 some comments at the bottom, since it's way
 easier to test just with some wire and not have
 to go drag out some pots.
 
 
 This code is in the public domain
 */

/* Usage of LeoJoy
 The dataForController_t struct is the core of the library.
 You'll create an instance of that and manipulate it,
 then use the Joystick.setControllerData function to send that data out.
 
 The two main functions that LeoJoy! adds are
 - Joystick.setControllerData(dataForController_t)
 This actually updates the USB controller. Edit the dataForController_t struct
 as much as you like before pushing it out the USB interface with this function.
 
 - getBlankDataForController()
 This utility function returns a dataForController_t object, with no buttons pressed
 and both of the analog sticks centered. Handy!
 
 So, basically, you'll do something like this:
 
 void loop(void){
 // Set up an empty controller data struct
 dataForController_t joystickData = getBlankDataForController();
 // Set some of the buttons
 joystickData.squareOn = someBooleanInput();
 // Send the data over USB to the PC
 Joystick.setControllerData(joystickData);
 }
 
 The fields of the dataForController_t stuct are below.
 The struct is hard-coded into the USB communications library of LeoJoy,
 so it's not designed to be changed.
 
 typedef struct dataForController_t
 {
 uint8_t triangleOn : 1;  // Each of these member variables
 uint8_t circleOn : 1;    //  control if a button is off or on
 uint8_t squareOn : 1;    // For the buttons, 
 uint8_t crossOn : 1;     //  0 is off
 uint8_t l1On : 1;        //  1 is on
 uint8_t l2On : 1;        
 uint8_t l3On : 1;        // The : 1 here just tells the compiler
 uint8_t r1On : 1;        //  to only have 1 bit for each variable.
 //  This saves a lot of space for our type!
 uint8_t r2On : 1;
 uint8_t r3On : 1;
 uint8_t selectOn : 1;
 uint8_t startOn : 1;
 uint8_t homeOn : 1;
 uint8_t dpadLeftOn : 1;
 uint8_t dpadUpOn : 1;
 uint8_t dpadRightOn : 1;
 
 uint8_t dpadDownOn : 1;
 uint8_t padding : 7;     // We end with 7 bytes of padding to make sure we get our data aligned in bytes
 
 uint8_t leftStickX : 8;  // Each of the analog stick values can range from 0 to 255
 uint8_t leftStickY : 8;  //  0 is fully left or up
 uint8_t rightStickX : 8; //  255 is fully right or down 
 uint8_t rightStickY : 8; //  128 is centered.
 // Important - analogRead(pin) returns a 10 bit value, so if you're getting strange
 //  results from analogRead, you may need to do (analogRead(pin) >> 2) to get good data
 } dataForController_t;
 
 
 */


#include <SoftwareSerial.h>

SoftwareSerial mySerial(10,11); // RX, TX
//unsigned int controllerInputVal=0;  // Max value is 65535
char incomingByte;
char controllerInput = '0';         // throw away previous integerValue
int controllerInputVal = 0;         // throw away previous integerValue

String combinedData = "";
int byteCount = 0;
int led = 13;
int ledState = 0;

boolean triangleOn = false;
boolean circleOn = false;
boolean squareOn = false;
boolean crossOn = false;
boolean dpadUpOn = false;
boolean dpadDownOn = false;
boolean dpadLeftOn = false;
boolean dpadRightOn = false;
boolean l1On = false;
boolean r1On = false;
boolean selectOn = false;
boolean startOn = false;
boolean homeOn = false;
int leftStickX = 128;
int leftStickY = 128;
int rightStickX = 128;
int rightStickY = 128;

void setup(){
  //  setupPins();

  pinMode(led, OUTPUT);

  // set the data rate for the SoftwareSerial port
  mySerial.begin(115200);  
  //  establishContact();  // send a byte to establish contact until receiver responds 
}

void loop(){
  // Always be getting fresh data
  //  dataForController_t controllerData = getControllerData();

  // Then send out the data over the USB connection
  // Joystick.set(controllerData) also works.
  //  Joystick.setControllerData(controllerData);

  // read incoming serial (softserial)

  digitalWrite(led, ledState);
  while (mySerial.available() > 0)
  {

    digitalWrite(led, HIGH);
    char received = mySerial.read();

    if (received == '\r') continue;
    if (received == '\n')
    {
      setController(controllerInput, controllerInputVal);
      combinedData = ""; // Clear recieved buffer
      byteCount = 0;  
      break;

    } 
    else {
      if (byteCount == 0) {
        controllerInput = received;
        byteCount++;    
      } 
      else {
        controllerInputVal = (int) received -48;
        byteCount++;
        //        combinedData += received; 
      }
    }


  }

}


void setController(char input, int value) {
//  mySerial.print("inside set");
//  mySerial.print(value);
  if (input == 'A' && value == 1) {
    ledState = 1;
  } 
  else {
    ledState = 0;
  }

  //  digitalWrite(led,LOW);
  //  mySerial.print("controller val: ");
  //  mySerial.println(input);
  //  mySerial.print("value: ");
  //  mySerial.println(value);
  //  String tmpStrVal = (String) input.charAt(1);
  //  mySerial.println(tmpStrVal.toInt());


}





dataForController_t getControllerData(void){  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  values that were in those memory locations before
  dataForController_t controllerData = getBlankDataForController();
  controllerData.triangleOn = triangleOn;
  controllerData.circleOn = circleOn;
  controllerData.squareOn = squareOn;
  controllerData.crossOn = crossOn;
  controllerData.dpadUpOn = dpadUpOn;
  controllerData.dpadDownOn = dpadDownOn;
  controllerData.dpadLeftOn = dpadLeftOn;
  controllerData.dpadRightOn = dpadRightOn;
  controllerData.l1On = l1On;
  controllerData.r1On = r1On;
  controllerData.selectOn = selectOn;
  controllerData.selectOn = selectOn;
  controllerData.selectOn = selectOn;

  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit value,
  //  we need to perform a bit shift operation to
  //  lose the 2 least significant bits and get an
  //  8 bit number that we can use  // analogRead(A0) >> 2;
  controllerData.leftStickX = leftStickX;
  controllerData.leftStickY = leftStickY;
  controllerData.rightStickX = rightStickX;
  controllerData.rightStickY = rightStickY;

  // And return the data!
  return controllerData;
}

// void setupPins(void){
//   // Set all the digital pins as inputs
//   // with the pull-up enabled, except for the 
//   // two serial line pins
//   for (int i = 2; i <= 7; i++){
//     pinMode(i, INPUT);
//     digitalWrite(i, HIGH);
//   }
//   pinMode(A4, INPUT);
//   digitalWrite(A4, HIGH);
//   pinMode(A5, INPUT);
//   digitalWrite(A5, HIGH);
// }











