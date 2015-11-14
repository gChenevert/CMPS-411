#include <ax12.h>
#include <BioloidController.h>

// Establish Wire Slave Receiver
#include <Wire.h>

// Include bots movements
#include "dance.h"
//#include "walk1.h"
#include "multiStep.h"

BioloidController bioloid = BioloidController(1000000);

//#define frames 3
int idx = 0;

int time = 0; 
int i = 500; // Interpolate speed
int incomingByte = 0;

void setup(){
  
  // Begin communications between Pi and ArbotiX at address 8
  Wire.begin(8);
 // Serial.begin(38400);
  
  // Load initial Stand up pose
  bioloid.loadPose(move1);
  bioloid.readPose();
  bioloid.interpolateSetup(i);
  while(bioloid.interpolating > 0){
    bioloid.interpolateStep();
    delay(3);
  } 
}

void loop(){
  // For Serial communication
  //if(Serial.available() > 0) {
    Serial.println("Please begin communication with Pi: ");
    Serial.print("\n");
    
    Wire.onReceive(receiveEvent);
    delay(100);
    // incomingByte = 4;
     Serial.print("I received from the Pi: ");
     Serial.print(incomingByte);
     
     if(incomingByte == 1){
       while(time != 5){
         walk();
       // }
         time++;
       }     
    }
    if(incomingByte == 2){
      while(time != 2){
         counting();
         time++;
      }
    }

   if(incomingByte == 3){
      while(time != 3){
         turn();
         time++;
      }
   }

  if(incomingByte == 4){
    while(time != 2){
        tilt();
        time++;
    }
  }  
  
  // This is the original 
 //bioloid.playSeq(seq5);
 //while(bioloid.playing){
 //bioloid.play();
 //}


 // }
  Serial.print("\n"); 
  time = 0;
}


void walk(){
  #define frames 3
  bioloid.playSeq(seq5);
  while(bioloid.playing){
    bioloid.play();
  }
}

void counting(){
  #define frames 15
   bioloid.playSeq(Count);
   while(bioloid.playing){
   bioloid.play();
 }
}

void turn(){
  #define frames 5
  bioloid.playSeq(turnSeq);
  while(bioloid.playing){
    bioloid.play();
  }
}

void tilt(){
  #define frames 2
  bioloid.playSeq(leanSeq);
  while(bioloid.playing){
  bioloid.play();
  }
}

// function that executes whenever data is received from master
// this function is registered as an event, see setup()
void receiveEvent(int howMany)
{
  while(1 < Wire.available()) // loop through all but the last
  {
    int c = Wire.read(); // receive byte as a character
   // Serial.print(c);         // print the character
  }
  incomingByte = Wire.read();    // receive byte as an integer
  Serial.println(incomingByte);         // print the integer
}
