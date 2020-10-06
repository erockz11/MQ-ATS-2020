// Unsure on how to receive direction info from motor code.
// Waiting for specific e-stop method. using default stop for motion.
// Will send/interpret door status once door functions have been written.
// Interpreting/sending T2C4 control box signals between UNO/MEGA.
//
// MEGA Bluetooth module attempts to establish connection with UNO Bluetooth module
// by directly connecthing to its address.
 
void setup() {
    Serial.begin(9600); //pins 0 and 1 as RX, TX (assumption. pins need to be sorted)

    Serial.print("AT+IMME1");
    delay(1000);
    Serial.print("AT+ROLE1"); //master (assuming mega is master)
    delay(1000);
    Serial.print("AT+CON[]"); //replace [] with slave hm10 address
    delay(1000);
}

// If connection is established, the module will actively listen for the following
// signals listed.

void loop() {
  if(Serial.available()){
    char c = Serial.read();

    if (c == 'P'){  //STOP, EAST, DOOR CLOSE
    if (motorSpeed != 0)
      moveChange(0);
    if (direction != 0)   // will change to appropriate data type
      directionChange(0);
    // insert doors here and all other cases
    }
    
    if (c == 'R'){  //STOP, EAST, DOOR OPEN
    if (motorSpeed != 0)
      moveChange(0);
    if (direction != 0)
      directionChange(0);
      }
    
    if (c == 'T'){  //STOP, WEST, DOOR CLOSE
    if (motorSpeed != 0)
      moveChange(0);
    if (direction != 1)
      directionChange(1);
      }
    
    if (c == 'V'){  //STOP, WEST, DOOR OPEN
    if (motorSpeed != 0)
      moveChange(0);
    if (direction != 1)
      directionChange(1);
      }
    
    if (c == 0x5C){ // hex for '\'  //START, WEST, DOOR CLOSE,
    if (motorSpeed != 1)
      moveChange(1);
    if (direction != 1)
      directionChange(1);
      }
    
    if (c == 'X'){  //START, EAST, DOOR CLOSE
    if (motorSpeed != 1)
      moveChange(1);
    if (direction != 0)
      directionChange(0);
      }

    if (c == 'Y'){  //START, EAST, DOOR CLOSE, E-STOP ENGAGED
    //still waiting for code for e-stop. will use regular stop code for now: colorRed()
    if (motorSpeed != 1)
      moveChange(1);
    
      }

    Serial.println(c);
    }
}

void moveChange(int s) { //0 for STOP, 1 for START, 2 for possible e-stop
  if (s == 0){
    colorRed(); //function in motor code. colorRed(): motorSpeed of A and B set to 0.
  }
  else if (s == 1){
    colorGreen(); //speed up
  }
  else {  //assume stop on invalid input
    Serial.println("Error on move. Stopping train.");
    colorRed();
  }
}

void directionChange(int d){ //0 for EAST, 1 for WEST
  if (d == 0){  
  //unsure on how to change directions relating to motor code. must include stopping at station before direction change
  }
  else if (d == 1){
  }
  else
    Serial.println("Error changing direction.");
}