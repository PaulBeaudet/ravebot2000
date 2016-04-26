// motors.ino ~ Copyright 2015-2016 ~ Paul Beaudet MIT licence - see LICENCE
#define SPD_L_PIN 10 // left motor pin 10: speed
#define SPD_R_PIN 9  // right motor pin 9: speed 
#define SPD_L_MOTOR OCR1A // left motor pin 10 register? speed
#define SPD_R_MOTOR OCR1B // right motor pin 9 register? speed 
#define DIR_L_MOTOR 8  // left motor pin for setting direction 
#define DIR_R_MOTOR 7  // right motor pin for setting direction
#define LEFT 0
#define RIGHT 1

// ### Basic motion functions ###

void motorSetup(){ // set pin modes for motors
  pinMode(SPD_L_PIN, OUTPUT);
  pinMode(SPD_R_PIN, OUTPUT);
  pinMode(DIR_L_MOTOR, OUTPUT);
  pinMode(DIR_R_MOTOR, OUTPUT);
  // wish I could tell you what following lines mean, included in zumo library
  // for 328p and 34u4 which are prodominately the arduinos we are concered about
  // TCCR1A = 0b10100000; // Timer 1 configuration: prescaler: clockI/O /1
  // TCCR1B = 0b00010001; // outputs enabled / phase-correct PWM
  // ICR1 = 400;          // top of 400 / PWM calc: 16Mhz / 1 / 2 / 400 =  20khz
}

// motors: 0 is stopped, 400 is full speed negitives are reverse speed
void speedSet(int spd, boolean rightMotor){
  boolean motorDirection = LOW; // indication of direction change
  if(spd < 0){             // if reverse direction
    spd = -spd;            // speed actually needs to be positive
    motorDirection = HIGH; // but direction needs to be reversed
  }
  if (spd > MAX_POWER){spd = MAX_POWER;} // speed can be on greater than 400
  if(rightMotor){
    // OCR1B = spd;
    analogWrite(SPD_R_PIN, spd);
    digitalWrite(DIR_R_MOTOR, motorDirection);
  } else {
    // OCR1A = spd;                  // manually set register? to speed
    analogWrite(SPD_L_PIN, spd); 
    digitalWrite(DIR_L_MOTOR, motorDirection); // set motor direction
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

