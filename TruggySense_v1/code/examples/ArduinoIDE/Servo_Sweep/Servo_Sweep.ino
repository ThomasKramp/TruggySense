/* Sweep
 by BARRAGAN <http://barraganstudio.com> 
 This example code is in the public domain.

 modified 8 Nov 2013
 by Scott Fitzgerald
 http://arduino.cc/en/Tutorial/Sweep
*/ 

#include <Servo.h> 
 
Servo s1;
Servo s2;

 
int pos = 0;    // variable to store the servo position 
 
void setup() 
{ 
  s1.attach(22); 
  s2.attach(33); 
} 
 
void loop() 
{ 
  for(pos = 1000; pos <= 2000; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    s1.writeMicroseconds(pos);
    s2.writeMicroseconds(pos);
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 2000; pos>=1000; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    s1.writeMicroseconds(pos);
    s2.writeMicroseconds(pos);
    delay(15);                       // waits 15ms for the servo to reach the position 
  } 
} 
