
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>



LiquidCrystal_I2C lcd(0x27,20,4);
boolean states [4] = { 1, 1, 0, 0 }; // start/stop, open/close, east/west, e-stop
boolean prev_states [4] = { 1, 1, 0, 0 }; // start/stop, open/close, east/west, e-stop
char button_states [3] = { 0, 0, 0 }; // previous states, same as above without e-stop.

unsigned long idle_timer;
boolean idle = false;
boolean e_stopped = false;
char out_char;

void setup() {
  lcd.init();
  lcd.backlight();
  
  idle_print();
  
  pinMode(2, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);

  Serial.begin(9600);
  print_state();
}

void loop() {
  unsigned int loop_time = millis ();
  read_buttons();

  check_contradictions();
  
  display_msg();
  
  if (!e_stopped) {
    sendData();
  }
  if ((millis() - idle_timer)/1000 > 3 && !idle) {
    idle_print();
  }

  // Keeping the loop within 10ms
  if ((millis() - loop_time) > 0) {
    delay (millis() - loop_time); 
  }
}

void check_contradictions () {
  lcd.setCursor(0,0);
  // PREVIOUS STATE: train is moving, and doors are closed.
  // UPPDATED STATE: train is moving, command sent to open doors.
  if ((states[0] == 0 && prev_states[0] == 0) && (prev_states[1] == 1 && states[1] == 0)) {
    print_state();
    states[1] = prev_states[1];
    
    lcd.clear();
    Serial.println("COMMAND CONFLICT: CANNOT OPEN DOORS WHILE TRAIN MOVING");
    lcd.print("COMMAND CONFLIT");
    lcd.setCursor(0,1); lcd.print("FIRST STOP TRAIN");
  }
  // PREVIOUS STATE: train is stationary, and doors are open
  // UPDATED STATE: command sent to move train, with open doors
  if ((states[0] == 0 && prev_states[0] == 1) && (prev_states[1] == 0 && prev_states[1] == 0)) {
    print_state();
    states[0] = prev_states[0];
    
    lcd.clear();
    Serial.println("COMMAND CONFLICT: CANNOT START WHILE DOORS OPEN");
    lcd.print("COMMAND CONFLIT");
    lcd.setCursor(0,1); lcd.print("FIRST CLOSE DOORS");
  }
}

void read_buttons() {
  if (!e_stopped) {
    int in_values [4] = { digitalRead (4), digitalRead(6), digitalRead(8), digitalRead(2) };
    int i;
    for (i = 0; i < 3; i = i + 1) {
      if (in_values[i] != button_states[i]) {
        button_states[i] = in_values[i]; 
        if (in_values[i] == LOW) {
          
          idle_timer = millis();
          idle = false;
          
          prev_states[i] = states[i];
          states[i] = !states[i];
        }
      }
    }
  
    if (in_values[3] == LOW) { // E-stop
      states[3] = !states[3];
    }
  }
}
void display_msg (){
  /*
   * Method is called for the printing of the current states and their meanings.
   */
  if (!e_stopped) {
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
        
        if (states[0] == 0) { // TRAIN IS MOVING
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
      if (states[1] != prev_states[1]) {
        
        lcd.clear();
        lcd.setCursor(0,0);
        
        if (states[1] == 0) { //DOORS OPEN
          
          Serial.println("COMMAND: OPEN TRAIN DOORS");
           lcd.print("OPEN DOORS");  
           
        } else { // DOORS CLOSE
          
          Serial.println("COMMAND: CLOSE TRAIN DOORS");
          lcd.print("CLOSE DOORS");
          
        }
        prev_states[1] = states[1];
      }

      // EAST/WEST FUNCTIONALITY
      // FUNCTIONALITY ON BLUE BUTTON
      if (states[2] != prev_states[2]) {
        
        lcd.clear();
        lcd.setCursor(0,0);lcd.print("DIRECTION CHANGE");
        lcd.setCursor(0,1);
        if (states[2] == 0) { // WEST
          Serial.println("COMMAND: DIRECTION WEST");
          
          lcd.print("WEST");
           
        } else { // EAST
          Serial.println("COMMAND: DIRECTION EAST");

          lcd.print("EAST");
          
        }
        prev_states[2] = states[2];
      }
    }
  }
}
void sendData() {
  char SendingChars[6] = {'P', 'R', 'T', 'V', 'X', '\\'}; //ASCII CODES TO SEND VIA A BLUETOOTH MODULE
  char base_char = 'J';
  for (int i = 0; i < 4 ; i++) {
    base_char = (base_char << 1) + states[i];// collects the starting bits
  }
  out_char = base_char; //adds the offset to send any of the above characters 
  for (int j = 0; j < sizeof(SendingChars); j++) {
    if ((out_char & SendingChars[j]) == out_char) {
      transmitBluetooth(out_char); //character to send over
    }
  }
}
void print_state () {
  Serial.print("SS:"); Serial.print(states[0]);
  Serial.print(" OC:"); Serial.print(states[1]);
  Serial.print(" EW:"); Serial.println(states[2]);
}
void idle_print() {
  idle = true;
  lcd.clear();
  lcd.setCursor(0, 0); lcd.print("T2C4 Control Box");
  lcd.setCursor(0,1);
  lcd.print("SS:"); lcd.print(states[0]);
  lcd.print("  OC:"); lcd.print(states[1]);
  lcd.print("  EW:"); lcd.print(states[2]);
}
// DUMMMY FUNCTION ATM
void transmitBluetooth(char toSend) {
  //Serial.println(toSend);
  sendBT(toSend);
}
