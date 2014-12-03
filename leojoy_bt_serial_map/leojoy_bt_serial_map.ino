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
 
 uint8_t leftStickX : 8;  // Each of the analog stick digit2ues can range from 0 to 255
 uint8_t leftStickY : 8;  //  0 is fully left or up
 uint8_t rightStickX : 8; //  255 is fully right or down 
 uint8_t rightStickY : 8; //  128 is centered.
 // Important - analogRead(pin) returns a 10 bit digit2ue, so if you're getting strange
 //  results from analogRead, you may need to do (analogRead(pin) >> 2) to get good data
 } dataForController_t;
 
 
 */


//#include <SoftwareSerial.h>

//SoftwareSerial Serial1(10,11); // RX, TX
//unsigned int controllerInputdigit2=0;  // Max digit2ue is 65535
char incomingByte;
char controllerInput = '0';         // throw away previous integerdigit2ue
int controllerInputdigit2 = 0;         // throw away previous integerdigit2ue

String combinedData = "";
int byteCount = 0;
int led = 13;
int ledState = 0;

#define numberOfDigits 3
char theNumberString[numberOfDigits + 1]; // used to atoi joyVal value
int joyVal;


boolean triangleOn = false;
boolean circleOn = false;
boolean squareOn = false;
boolean crossOn = false;
boolean dpadUpOn = false;
boolean dpadDownOn = false;
boolean dpadLeftOn = false;
boolean dpadRightOn = false;
boolean l1On = false;
boolean l2On = false;
boolean l3On = false;
boolean r1On = false;
boolean r2On = false;
boolean r3On = false;
boolean selectOn = false;
boolean startOn = false;
boolean homeOn = false;
int leftStickX = 128;
int leftStickY = 128;
int rightStickX = 128;
int rightStickY = 128;

void setup(){
  pinMode(led, OUTPUT);

  // set the data rate for the SoftwareSerial port
  Serial1.begin(115200);  
}

void loop(){

  while(Serial1.available()) {

    int numBytes = Serial1.available();

    if (numBytes >= 2) {


      int x = Serial1.read(); //read first byte
      int digit0 = Serial1.read(); //read second byte if available;
      int digit1 = Serial1.read(); //read second byte if available;
      int digit2 = Serial1.read(); //read second byte if available;
      theNumberString[0] = digit0;
      theNumberString[1] = digit1;
      theNumberString[2] = digit2;
      theNumberString[3] = 0x00;

      //  'triangle': 'T',
      //  'circle': 'O',
      //  'square': 'S',
      //  'cross': 'X',
      //  'dpadUp': 'U',
      //  'dpadDown': 'D',
      //  'dpadLeft': 'F',
      //  'dpadRight': 'G',
      //  'l1': '[',
      //  'l2': '{',
      //  'l3': '<',
      //  'r1': ']',
      //  'r2': '}',
      //  'r3': '>',    
      //  'select': 'Z',
      //  'start': 'Y',
      //  'home': 'P',

      if (x == '*') {
        Serial1.flush();
        forceReset(); 
      } 
      else if (x == 'T') {
        triangleOn = (digit2 == '1');  
      } 
      else if (x == 'O') {
        circleOn = (digit2 == '1');  
      } 
      else if (x == 'S') {
        squareOn = (digit2 == '1'); 
      } 
      else if (x == 'X') {
        crossOn = (digit2 == '1'); 
      } 
      else if (x == 'U') {
        dpadUpOn = (digit2 == '1'); 
      } 
      else if (x == 'D') {
        dpadDownOn = (digit2 == '1'); 
      } 
      else if (x == 'F') {
        dpadLeftOn = (digit2 == '1'); 
      } 
      else if (x == 'G') {
        dpadRightOn = (digit2 == '1'); 
      } 
      else if (x == '[') {
        l1On = (digit2 == '1'); 
      } 
      else if (x == '{') {
        l2On = (digit2 == '1'); 
      } 
      else if (x == '<') {
        l3On = (digit2 == '1'); 
      } 
      else if (x == ']') {
        r1On = (digit2 == '1'); 
      } 
      else if (x == '}') {
        r2On = (digit2 == '1'); 
      } 
      else if (x == '>') {
        r3On = (digit2 == '1'); 
      } 
      else if (x == 'Z') {
        selectOn = (digit2 == '1'); 
      } 
      else if (x == 'Y') {
        startOn = (digit2 == '1'); 
      } 
      else if (x == 'P') {
        homeOn = (digit2 == '1'); 
      }
      else if (x == 'L') {
        joyVal = atoi(theNumberString);
        leftStickX = joyVal; //left stick X
        Serial1.println("joyval");
        Serial1.println(int(joyVal));

      } 
      else if (x == 'l') {
        joyVal = atoi(theNumberString);        
        leftStickY = joyVal; //left stick Y
      }
      else if (x == 'R') {
        joyVal = atoi(theNumberString);
        rightStickX = joyVal; //left stick X
        Serial1.println("joyval");
        Serial1.println(joyVal);
      } 
      else if (x == 'r') {
        joyVal = atoi(theNumberString);
        rightStickY = joyVal; //left stick Y
      }

      ledState = (digit2 == '1');



    }
    break;

  }

  // Always be getting fresh data
  dataForController_t controllerData = getControllerData();

  // Then send out the data over the USB connection
  // Joystick.set(controllerData) also works.
  Joystick.setControllerData(controllerData);

  digitalWrite(led, ledState);
  delay(50);

}






dataForController_t getControllerData(void){  
  // Set up a place for our controller data
  //  Use the getBlankDataForController() function, since
  //  just declaring a fresh dataForController_t tends
  //  to get you one filled with junk from other, random
  //  digit2ues that were in those memory locations before
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
  controllerData.l2On = l2On;
  controllerData.l3On = l3On;
  controllerData.r1On = r1On;
  controllerData.r2On = r2On;
  controllerData.r3On = r3On;
  controllerData.selectOn = selectOn;
  controllerData.startOn = startOn;
  controllerData.homeOn = homeOn;

  // Set the analog sticks
  //  Since analogRead(pin) returns a 10 bit digit2ue,
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

void forceReset() {
  triangleOn = false;
  circleOn = false;
  squareOn = false;
  crossOn = false;
  dpadUpOn = false;
  dpadDownOn = false;
  dpadLeftOn = false;
  dpadRightOn = false;
  l1On = false;
  l2On = false;
  l3On = false;
  r1On = false;
  r2On = false;
  r3On = false;
  selectOn = false;
  startOn = false;
  homeOn = false;
  leftStickX = 128;
  leftStickY = 128;
  rightStickX = 128;
  rightStickY = 128;
}



























