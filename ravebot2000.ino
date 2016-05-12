// ravebot2000 : compiled succesfully w/Arduino 1.6.5 
#include <IRLib.h> // https://learn.adafruit.com/using-an-infrared-library/overview
#include <IRLibMatch.h>
#include <IRLibRData.h>
#include <IRLibTimer.h>
#include "functionPointer.h" // needed for timer functions

/*
 *  ~ Constants ~ never changing numbers
 * NOTE: both #define and const can create Constants
 * there are valid technical reasons for using either
 * most people use only one or the other out of lazyness/habit
 * when in doubt use const
 */
#define MAX_POWER 255
// EXAMPLE -> speedsSet(left, right); //takes a number between negitive MAX_POWER and positive MAX_POWER
#define OFFSET 2    // add 1000 for a right trim offset
#define LED_PIN 13  // Built in Arduino LED
// IR codes for our remote
const long ONWARD       = 0xFF18E7; //<-in base 16 ~16718055<- in base 10
const long STOP         = 0xFF38C7; // 16726215
const long BACKNUP      = 0xFF4AB5; // 16730805
const long RIGHT        = 0xFF5AA5; // 16734885
const long LEFT         = 0xFF10EF; // 16716015
const long REMOTETOGGLE = 0xFFE21D; // 16769565

/*
 * NOTE about IR sensor: Bill had to make sure eletromagnetic waves from
 * motors were sheilded from IR sensor Jumpers.
 * (he did this by moving them away from motors)
 * an effective way to do this is to wrap the 3 IR jumpers in aluminum foil
 * ... not sure if it would need to be grounded or not..
 * but a floating wrap would be better than nothing to block EM waves
 */

#define IR_PIN 2
IRrecv My_Receiver(IR_PIN); // Create a receiver object to listen on pin
IRdecode My_Decoder;        // Create a decoder object

// ----------- MAIN FUNCTIONS (all the important stuf) --------------------------
void setup() {
  // --- seting stuff up ---
  Serial.begin(9600);              // establish serial speed between arduino & computer
  pinMode(LED_PIN, OUTPUT);        // make sure LED understands its role
  pingSetup();                     // setup pingy pingy
  motorSetup();                    // setup motors
  fullSpeedAhead();                // Explore forward to start our journey
  My_Receiver.enableIRIn();        // turn on IR sensor
  // ----- setting actions into motion -------------------
  setInterval(blinkTheLed, 500);   // blinks LED
  // setTimeout(fullSpeedBackwards, 5000);
  setTimeout(stopMotors, 10000); // stop motors in a couple of seconds
  // setInterval(readoutPing, 1000); // serial readout of ping
  setInterval(reactToPing, 1000);  // reaction to ping
}

void loop() {    // Place to continually check what needs to be done
  remoteReact(); // react to events from an IR remote
  todoChecker(); // ravebot checks todo list, needed for setIterval and setTimeout: timer.ino
}
//-----------(done important stuff, implementation details below)------------------------

/*
 * NOTES ABOUT VARIABLES: blinkTheLed uses a toggle variable that could be created in various ways
 * The toggle outside of the function could be modified anywhere in the program: GLOBAL SCOPE
 * Global scope is dangerous, too easy to lose track of where variables are being modified
 * limiting veriables to just be accessed by independent functions keeps things safe
 */
// boolean toggle = 0; // here toggle is a globally availible pesistent on or off state
void blinkTheLed(){
  static boolean toggle = 0; // 'static' persitent on or off state: set to 0 first time called
  // NOTE: static data can be subsequently be changed like a global variable only by this function
  // boolean toggle = 0;     // temp state: 0 would be assigned to toggle like new every time function called 
  // NOTE: Both cases inside of function access to toggle is limited to blinkTheLed: LOCAL SCOPE
  
  digitalWrite(LED_PIN, toggle); // write our pin to binary value last toggled. HIGH = 1 / LOW = 0
  toggle = !toggle;              // toggle our toggle value 1->0 or 0->1
}

void readoutPing(){
  long distance = pingGetDistance();   // read distance from ping sensor
  Serial.print("distance:");           // print a contextual string to serial
  Serial.println(distance);            // print distance value and create a new line
}

void reactToPing(){
  int distance = pingGetDistance();
  if(distance < 20){
    speedsSet(MAX_POWER, -MAX_POWER); // take a turn
    setTimeout(fullSpeedAhead, 240);      // set motors to stop in X amount of milliseconds
  }
}

// some functions for directions
void fullSpeedAhead(){    speedsSet(MAX_POWER, MAX_POWER);}
void fullSpeedBackwards(){speedsSet(-MAX_POWER, -MAX_POWER);}
void turnTheBot(){        speedsSet(-MAX_POWER, MAX_POWER);}

// using the IR remote (UNTESTED)
void remoteReact(){                            // react to IR remote commands
  static boolean usingRemote = false;          // detects whether remote is being used or not
 
  if(My_Receiver.GetResults(&My_Decoder)){     // if IR reciever returns results
    My_Decoder.decode();                       // decode those results
    My_Decoder.DumpResults();                  // dump result?
    if(usingRemote){                           // when using remote
      if(My_Decoder.value == ONWARD){          // 1? button
        speedsSet(MAX_POWER, MAX_POWER);       // forward march!
      } else if (My_Decoder.value == STOP){    // 5? button
        stopMotors();                          // Arret! (stop)
      } else if (My_Decoder.value == RIGHT){   // 6? button
        speedsSet(MAX_POWER, (MAX_POWER/2));   // right here right now (right turn)
      } else if (My_Decoder.value == LEFT){    // 4? button
        speedsSet((MAX_POWER/2), MAX_POWER);   // to the left to the left
      } else if (My_Decoder.value == BACKNUP){ // 8? button
        speedsSet(-MAX_POWER, -MAX_POWER);     // Back the train up
      }
      setTimeout(stopMotors, 250);             // set motors to stop in x amount of milliseconds
    }
    if(My_Decoder.value == REMOTETOGGLE){      // whether using remote or not check if we want to
      usingRemote = !usingRemote;              // toggle using remote from previous state
      if(usingRemote){stopMotors();}           // make sure motors are stopped when taking control
    }
  }
  My_Receiver.resume();                        //Restart the receiver
}


