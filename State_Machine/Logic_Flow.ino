void colourSwitch(uint8_t colourState) {
  switch (colourState) {

    case BLUE:
      spin(stopV);
      emergency = true;
      tStatus = 'Z';
      break;

    case RED:
      eStop();
      spin(stopV);
      if (pColourState != RED) {
        switch (operationState) {
          case HALTED:
            if (command == 'X' && directionState == EAST) {
              directionState = WEST;
              delay(5000);
              spin(slowR);
            }
            else if (command == '\\' && directionState == WEST) {
              directionState = EAST;
              delay(5000);
              spin(slowS);
            }
            operationState = MOVING;
            break;
          case MOVING:
            if (command == 'P' && directionState == EAST) {
              directionState = WEST;
            }
            else if (command == 'R' && directionState == EAST) {
              directionState = WEST;
            }
            else if (command == 'T' && directionState == WEST) {
              directionState = EAST;
            }
            else if (command == 'V' && directionState == WEST) {
              directionState = EAST;
            }
            operationState = HALTED;
            break;
        }
        pColourState = RED;
        tStatus = pCommand;
      }
      break;

    case GREEN:
      eStop();
      if (greenCounter == 0) {
        switch (directionState) {
          case EAST:
            spin(maxS);
            break;
          case WEST:
            spin(maxR);
            break;
        }
        if (pColourState != GREEN) {
          greenCounter++;
        }
      }
      else if (greenCounter == 1) {
        switch (directionState) {
          case EAST:
            spin(slowS);
            break;
          case WEST:
            spin(slowR);
            break;
        }
        if (pColourState != GREEN) {
          greenCounter = 0;
        }
      }
      pColourState = GREEN;
      break;
    case YELLOW:
      eStop();
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
      eStop();
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
    tStatus = 'Z';
    sendBT(tStatus);
    loop();
  }
}
