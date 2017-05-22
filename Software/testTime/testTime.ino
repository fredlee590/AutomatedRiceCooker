#include <TimeLib.h>

#define TIME_MSG_LEN 11 // time sync to PC is HEADER followed by Unix time_t as ten ASCII digits
#define TIME_HEADER 'T' // Header tag for serial time sync message
#define TIME_REQUEST 7 // ASCII bell character requests a time sync message 

#define JUDGEMENT_DAY 22
#define JUDGEMENT_MONTH 5
#define JUDGEMENT_YEAR 2017

// Start serial monitor and enter a string like this
// T1262347200 //noon Jan 1 2010
// at time of writing, Judgement Day is on 1495455132 seconds from Jan 1 1970 (epoch)

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
}

void processSyncMessage() {
  // if time sync available from serial port, update time and return true
  while (Serial.available() >= TIME_MSG_LEN ) { // time message consists of header & 10 ASCII digits
    char c = Serial.read() ;
    Serial.print(c);
    if ( c == TIME_HEADER ) {
      time_t pctime = 0;
      for (int i = 0; i < TIME_MSG_LEN - 1; i++) {
        c = Serial.read();
        if ( c >= '0' && c <= '9') {
          pctime = (10 * pctime) + (c - '0') ; // convert digits to a number
        }
      }
      setTime(pctime); // Sync Arduino clock to the time received on the serial port
    }
  }
}

void digitalClockDisplay() {
  // digital clock display of the time
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
  Serial.print(" ");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(month());
  Serial.print(" ");
  Serial.print(year());
  Serial.println();
}

void printDigits(int digits) {
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void checkJudgement() {
  if(year() == JUDGEMENT_YEAR &&
    month() == JUDGEMENT_MONTH &&
    day() == JUDGEMENT_DAY) {
      String judgement_str = "IT IS JUDGEMENT DAY " + String(JUDGEMENT_MONTH) + "/" + String(JUDGEMENT_DAY) + "/" + String(JUDGEMENT_YEAR);
      Serial.println(judgement_str);
  }
}
 

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    processSyncMessage(); // this should set the time
  }

  if (timeStatus() == timeNotSet) {
    Serial.println("waiting for sync message");
  } else {
    digitalClockDisplay();
    checkJudgement();
  }
  delay(1000);
}
