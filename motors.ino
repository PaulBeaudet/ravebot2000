// motors.ino ~ Copyright 2015-2016 ~ Paul Beaudet MIT licence - see LICENCE
#define LEFT_SPEED_A  10 // left motor pin 10: speed
#define RIGHT_SPEED_A 9  // right motor pin 9: speed
#define LEFT_SPEED_B  5  // left motor pin for setting speed in other direction
#define RIGHT_SPEED_B 6  // right motor pin for setting speed in other direction
#define LEFT 0
#define RIGHT 1

// --- Basic motor functions ---
void motorSetup(){ // set pin modes for motors
  pinMode(LEFT_SPEED_A, OUTPUT);
  pinMode(RIGHT_SPEED_A, OUTPUT);
  pinMode(LEFT_SPEED_B, OUTPUT);
  pinMode(RIGHT_SPEED_B, OUTPUT);
}

// motors: 0 is stopped, 400 is full speed negitives are reverse speed
// if your motor are going in wrong direction setup forward to false
// then set forward to true when spd < 0
void speedSet(int spd, boolean rightMotor){
  boolean forward = true;  // indication of direction change
  if(spd < 0){             // if reverse direction
    spd = -spd;            // speed actually needs to be positive
    forward = false;       // but direction needs to be reversed
  }
  if (spd > MAX_POWER){spd = MAX_POWER;} // speed can be on greater than 400
  if(rightMotor){
    if(forward){                       // going forward A pin gets set
      analogWrite(RIGHT_SPEED_A, spd); // set A pin to our speed
      analogWrite(RIGHT_SPEED_B, 0);   // set B pin back to low
    } else {                           // going backward B pin gets set
      analogWrite(RIGHT_SPEED_A, 0);
      analogWrite(RIGHT_SPEED_B, spd);
    }
  } else {
    if(forward){                      // going forward A pin gets set
      analogWrite(LEFT_SPEED_A, spd); // set A pin to our speed
      analogWrite(LEFT_SPEED_B, 0);   // set B pin back to low
    } else {                          // going backward B pin gets set
      analogWrite(LEFT_SPEED_A, 0);
      analogWrite(LEFT_SPEED_B, spd);
    }
  }
}

void speedsSet(int leftSpd, int rightSpd){  // calibrated motor heading
  int offset = OFFSET;                      // lazy way TODO: pull from EEPROM
  if (offset > 400) {                       // left offset case: decern weak side on range
    offset = offset - 1000;                 // 1000 is extra amount added to left offsets
    leftSpd = nutralize(leftSpd, offset);   // get offset reletive to direction
  } else {                                  // right tread ofset case
    rightSpd = nutralize(rightSpd, offset); // untouched is the right offset case
  };
  speedSet(leftSpd, LEFT);
  speedSet(rightSpd, RIGHT);
}

// writes offset correctly no matter the direction
int nutralize(int bearing, int offset){
  if(bearing == 0){           // no bearing = no offset
    return 0;                 // motor is stoped! leave it be
  } else if ( bearing < 0 ) { // if the bearing is negitive (reverse)
    return bearing + offset;  // if reverse add offset
  } else {                    // example -400 + 15 = -385
    return bearing - offset;  // if going forward subtract offset
  };                          // example 400 - 15 = 385
} // in basic; same speeds are returned in relitive directions

void stopMotors(){ // simple command to stop the motors
  speedSet(0, LEFT); // set left motor to 0
  speedSet(0, RIGHT); // set right motor to 0
}

