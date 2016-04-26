// ravebot2000 : compiled succesfully w/Arduino 1.6.5 
#include "functionPointer.h" // needed for timer functions
#define MAX_POWER 255
// speedsSet(left, right); //takes a number between negitive MAX_POWER and positive MAX_POWER 
#define OFFSET 2 // add 1000 right

void setup() {
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  // pingSetup();  // setup pingy pingy
  motorSetup();    // setup motors
  speedsSet(MAX_POWER, -MAX_POWER);
  setInterval(blinkTheLed, 500);
  setTimeout(stopMotors, 2000);
  // setInterval(readoutPing, 1000);
  // setInterval(reactToPing, 1000);
}

void loop() {
  todoChecker(); // ravebot checks his todo list: timer.ino
}

// NOTE: blinkTheLed uses a toggle variable that could be created in various ways
// boolean toggle = 0; // here it is a pesistent on or off state
// NOTE: the toggle above can be modified anywhere in the program: GLOBAL SCOPE
// NOTE: Global scope is dangerous, too easy to lose track of where variables are being modified
// NOTE: limiting veriables to just be accessed by independent functions keeps things safe

void blinkTheLed(){
  static boolean toggle = 0; // 'static' persitent on or off state: set to 0 first time called
  // boolean toggle = 0; // temporary on or off state 
  // NOTE: These two cases access to toggle is limited to blinkTheLed: LOCAL SCOPE
  digitalWrite(13, toggle);
  toggle = !toggle;
}

void readoutPing(){
  long distance = pingGetDistance();
  Serial.print("distance:");
  Serial.println(distance);
}

void reactToPing(){
  int distance = pingGetDistance();
  if(distance < 20){
    speedsSet(200, 200);
    setTimeout(stopMotors, 240); // set motors to stop in X amount of milliseconds
  }
}

void turnTheBot(){
  speedsSet(200, 200);
}

