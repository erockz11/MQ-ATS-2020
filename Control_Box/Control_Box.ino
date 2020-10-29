#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

/* The following Global variables control the state of the control box, the methods that is changing
 *  these values are: 
 *  - read_buttons
 *  - check_contradictions
 */
boolean states [4] = { 0, 0, 0, 0 }; // stop/start, west/east, open/close, e-stop
boolean prev_states [4] = { 0, 0, 0, 0 }; // stop/start, close/open, west/east, e-stop
char button_states [3] = { 0, 0, 0 }; // previous states, same as above without e-stop.

// the following Global variables control the features of the program, this includes the loop timer,
// and the e-stop feature.
unsigned long idle_timer;
boolean idle = false;
boolean e_stopped = false;

/* the following Global variables control the information for the bluetooth. Received_value, represents
 * the value we receive from the bluetooth. out_char represents the value we send over the bluetooth.
*/
char received_value = ' ';
char out_char;

void setup() {

  // the setting up of the LCD display
  lcd.init();
  lcd.backlight();

  // Method is printing the value of the group's name, and the current states of the control box
  idle_print();

  // The following commandments, are setting the pins, for the Arduino
  pinMode(2, INPUT_PULLUP); // red
  pinMode(4, INPUT_PULLUP); // Amber
  pinMode(6, INPUT_PULLUP); // green
  pinMode(10, INPUT_PULLUP); // blue - was 8 now 10
  pinMode(3, OUTPUT); // red
  pinMode(5, OUTPUT); // Amber
  pinMode(7, OUTPUT); // green
  pinMode(11, OUTPUT); // blue
  
  // These digitalwrites, are setting all the buttons, making sure that they are off
  digitalWrite(3, LOW);
  digitalWrite(5, LOW);
  digitalWrite(7, LOW);
  digitalWrite(11, LOW); // was 9 now 11

  // setting up the serial output panel
  Serial.begin(9600);
  //setupBT(); // <-- we cannot uncomment this until M4, fixes the bluetooth connections.
  print_state();
}

void loop() {
  // Calculates the current time to milli-seconds, this is for the feature making sure the 
  // loop count sends data at a rate of 10ms. Check 
  unsigned long loop_time = millis ();
  
  // Updating Button Value inputs
  read_buttons();
  check_contradictions();

  // Read_in bluetooth values
  //received_value = readBT(); // <-- Same as above, cannot implement util resolution by M4

  // Display messages
  // This funciton displays the meanings of the chaning of the states. For different messages
  // change the strings.
  display_msg(); 
 
  // the condition, contains the variable that represents the e-stop. If pressed, the button will 
  // not eter the condition.
  if (!e_stopped) {
    sendData();

  }
  // When the Control box's states have not been changed. After three seconds, the control
  if ((millis() - idle_timer)/1000 > 3 && !idle) {
    idle_print();
  }
  
  // Keeping the loop within 10ms
  // If there is a problem in the lab with the code remove these two lines
//  if ((10-(millis() - loop_time)) > 0) {
//    delay ((10-(millis() - loop_time))/1000); 
//  }
}

/* the following method is use to check the states commanded by the user, and the previous
 *  states before the button press. This is for the purpose of making sure that the 
 *  commands do not conflict with the expressed usability of the train. Ιf there are 
 *  conflicts on a command, the state of that command will be rolled-back to its previous
 *  state. 
 */
void check_contradictions () {
  // just some screen stuff
  lcd.setCursor(0,0);
  
  // PREVIOUS STATE: train is moving (states[0]), and doors are closed (states[2]).
  // UPPDATED STATE: train is moving (states[0]), command sent to open doors (states[2]).
  if ((states[0] == 1 && prev_states[0] == 1) && (prev_states[2] == 0 && states[2] == 1)) {
    print_state(); // printing the current state to the serial output
    // If true, then conflict on the state for the doors, will be rolled back.
    states[2] = prev_states[2]; // revert to the previous state.

    // Displaying of the message
    lcd.clear();
    Serial.println("COMMAND CONFLICT: CANNOT OPEN DOORS WHILE TRAIN MOVING");
    lcd.print("COMMAND CONFLICT");
    lcd.setCursor(0,1); lcd.print("FIRST STOP TRAIN");
  }
  // PREVIOUS STATE: train is stationary (states[0]), and doors are open (states[2])
  // UPDATED STATE: command sent to move train (states[0]), with open doors (states[2])
  if ((states[0] == 1 && prev_states[0] == 0) && (prev_states[2] == 1 && prev_states[2] == 1)) {
    print_state(); // printing the current state to the serial output
    states[0] = prev_states[0]; // if true, then conflict on the state for the movement, will be
    // rolled back to the previous state.

    //Displaying of the message
    lcd.clear();
    Serial.println("COMMAND CONFLICT: CANNOT START WHILE DOORS OPEN");
    lcd.print("COMMAND CONFLICT");
    lcd.setCursor(0,1); lcd.print("FIRST CLOSE DOORS");
  }
}

void read_buttons() {
  // The following method, has to do with the management of the buttons, including the LEDs, and
  // the reading of the buttons.
  func_leds();
  if (!e_stopped) {
    int in_values [4] = { digitalRead (4), digitalRead(6), digitalRead(10), digitalRead(2) };
    int i;

    // loops through the first three buttons. not including the e-stop button.
    for (i = 0; i < 3; i = i + 1) {
      if (in_values[i] != button_states[i]) {
        button_states[i] = in_values[i]; 
        if (in_values[i] == LOW) {
          
          // This is for the method 'loop()'. This will reset the idle timer
          idle_timer = millis();
          idle = false;

          // The storing of the previous states, and the update of the new states.
          prev_states[i] = states[i];
          states[i] = !states[i];
        }
      }
    }
  
    if (in_values[3] == LOW) { // E-stop is special
      states[3] = !states[3];
    }
  }
}
void display_msg (){
  /*
   * Method is called for the printing of the current states and their meanings.
   */
  if (!e_stopped) {
   //  if (received_value == ' ') { //checks if we are receiving any data from state machine
      
      if (states[3] == 1) { // E-STOPP Engaged.
        e_stopped = true;
    
        Serial.println("COMMAND: EMERGENCY STOP");
        Serial.println("------------------------");
        Serial.println("Execute power recycle");
  
        lcd.clear();
        lcd.setCursor(0,0); lcd.print("EMERGENCY STOP");
        lcd.setCursor(0,1); lcd.print("Power Recycle");
        
      } else {
  
        // START/STOP FUNCTIONALITY
        // FUNCTIONALITY ON AMBER BUTTON
        if (states[0] != prev_states[0]) {
          
          lcd.clear();
          lcd.setCursor(0,0);
          
          if (states[0] == 1) { // TRAIN IS MOVING
            Serial.println("COMMAND: START TRAIN");
            lcd.print("START TRAIN");
            
          } else { // STOPPING THE TRAIN
            Serial.println("COMMAND: STOP TRAIN");
            lcd.print("STOP TRAIN");
            
          }
          prev_states[0] = states[0];
        }
        
        // OPEN/CLOSE FUNCTIONALITY
        // FUNCTIONALITY ON GREEN BUTTON
        if (states[2] != prev_states[2]) {
          
          lcd.clear();
          lcd.setCursor(0,0);
          
          if (states[2] == 1) { //DOORS OPEN
            
            Serial.println("COMMAND: OPEN TRAIN DOORS");
             lcd.print("OPEN DOORS");  
             
          } else { // DOORS CLOSE
            
            Serial.println("COMMAND: CLOSE TRAIN DOORS");
            lcd.print("CLOSE DOORS");
            
          }
          prev_states[2] = states[2];
        }
  
        // EAST/WEST FUNCTIONALITY
        // FUNCTIONALITY ON BLUE BUTTON
        if (states[1] != prev_states[1]) {
          
          lcd.clear();
          lcd.setCursor(0,0);lcd.print("DIRECTION CHANGE");
          lcd.setCursor(0,1);
          if (states[1] == 1) { // WEST
            Serial.println("COMMAND: DIRECTION WEST");
            
            lcd.print("WEST");
             
          } else { // EAST
            Serial.println("COMMAND: DIRECTION EAST");
  
            lcd.print("EAST");
            
          }
          prev_states[1] = states[1];
        }
      }
//    } else {
//      print_code(received_value);
//    }
  }
}

void sendData() {
  // the following method sends the data to the bluetooth.
  char SendingChars[6] = {'P', 'R', 'T', 'V', 'X', '\\'}; //ASCII CODES TO SEND VIA A BLUETOOTH MODULE
  char base_char = 'P';
  // this is the creating of the char value
  for (int i = 0; i < 4 ; i++) { // setting of the first 4 bits of the ASCII char
    base_char = (base_char << 1) + states[i];// collects the starting bits
  }
  // adding the offset, is setting the top 4 bits of the ASCII char
  out_char = base_char + 80; //adds the offset to send any of the above characters 

  // checking that the char is an actual valid char.
  for (int j = 0; j < sizeof(SendingChars); j++) {
    if ((out_char & SendingChars[j]) == out_char) {
      sendBT(out_char); //character to send over
    }
  }
}
void print_code (char code) {
  if (code != out_char) {
    Serial.println(code);
  }
}
void print_state () {
  Serial.print("SS:"); Serial.print(states[0]);
  Serial.print("OC:"); Serial.print(states[1]);
  Serial.print("EW:"); Serial.println(states[2]);
}
void idle_print() {
  idle = true;
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("T2C4 Control Box");
  lcd.setCursor(0,1);
  lcd.print("SS:"); lcd.print(states[0]);
  lcd.print(" EW:"); lcd.print(states[1]);
  lcd.print("  OC:"); lcd.print(states[2]);
}

void func_leds(){
  if (digitalRead(4) == LOW){
    digitalWrite(5,HIGH);
  } else{
    digitalWrite(5,LOW);
  }

  if (digitalRead(6) == LOW){
    digitalWrite(7,HIGH);
  } else{
    digitalWrite(7,LOW);
  }
  
  if (digitalRead(2) == LOW){
    digitalWrite(3,HIGH);
  } else{
    digitalWrite(3,LOW);
  }

    if (digitalRead(10) == LOW){
    digitalWrite(11,HIGH);
  } else{
    digitalWrite(11,LOW);
  }
}
