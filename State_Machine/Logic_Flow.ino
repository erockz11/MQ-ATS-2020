int currentSpeed;
int redCounter = 0;

void colourSwitch(uint8_t colourState) {
  switch (colourState) {
    case BLUE:
      spin(stopV);
      currentSpeed = stopV;
      emergency = true;
      tStatus = 'Z';
      break;
    case RED:
      if(pColourState != RED) { 
        spin(stopV);
        redCounter++;
        switch (operationState) {
          case HALTED:
              if(command == 'X') {
              directionState=WEST;
              currentSpeed = slowR;
              spin(slowR);
            }
            else if (command == '\\') {
              directionState = EAST;
              currentSpeed = slowS;
              spin(slowS);
            }
            operationState = MOVING;
            break;
          case MOVING:
            currentSpeed = stopV;
            spin(stopV);
            if (command == 'P') {
              directionState = WEST;
            }
            else if (command == 'R') {
              directionState = WEST;
            }
            else if (command == 'T') {
              directionState = EAST;
            }
            else if (command == 'V') {
              directionState = EAST;
            }
            operationState = HALTED;
            break;
        }
      }
        if (redCounter > 0) {
          switch (directionState) {
            case EAST:
            currentSpeed = slowS;
            spin(slowS);
            break;
            case WEST:
            currentSpeed = slowR;
            spin(slowR);
            break;
          }
        }
      
        pColourState = RED;
        tStatus = pCommand;
      break;

    case GREEN:
      if (greenCounter == 0 && pColourState != GREEN) {
        switch (directionState) {
          case EAST:
            currentSpeed = maxS;
            break;
          case WEST:
            currentSpeed = maxR;
            break;
        }
          greenCounter++;
      }
      else if (greenCounter > 0 && pColourState != GREEN) {
        switch (directionState) {
          case EAST:
            currentSpeed = slowS;
            break;
          case WEST:
            currentSpeed = slowR;
            break;
          greenCounter = 0;
        }
      }
      spin(currentSpeed);
      pColourState = GREEN;
      break;
    case YELLOW:
      if (pColourState != YELLOW) {
        switch (directionState) {
          case EAST:
            spin(slowR);
            directionState = WEST;
            tStatus = 'X';
            break;
          case WEST:
            spin(slowS);
            directionState = EAST;
            tStatus = '\\';
            break;
        }
        pColourState = YELLOW;
      }
      break;

    case BLACK:
      redCounter = 0;
      if (directionState == EAST) {
        spin(maxS);
      }
      else if (directionState = WEST) {
        spin(maxR);
      }
      
      break;
}
}

void checkForUpdates() {
  if (pColourState != colourState || (pCommand != ' ' && pCommand != command)) {
    eStop();
    colourSwitch(colourState);
    pCommand = command;
  }
  sendBT(tStatus);
}

void eStop() {
  if (command == 'Z') {
    spin(stopV);
    emergency = true;
    tStatus = command;
    sendBT(tStatus);
    loop();
  }
}
