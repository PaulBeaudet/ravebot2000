// low level functions for using our ping sensor

#define TRIGGER 4
#define ECHO 12

void pingSetup(){ // set-up our trigger and echo pins
  pinMode(TRIGGER, OUTPUT);
  pinMode(ECHO, INPUT);
}

long pingGetDistance(){                // return duration of ping signal
  delayMicroseconds(2);                // wait to send ping
  digitalWrite(TRIGGER, HIGH);         // send of our ping
  delayMicroseconds(2);                // wait to not send ping
  digitalWrite(TRIGGER, LOW);          // STOP sending the ping
  long duration = pulseIn(ECHO, HIGH); // get pulse of ping
  return duration / 29 / 2;            // devide by the magic numbers
}

