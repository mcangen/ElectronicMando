#include <Keyboard.h>
#include <Mouse.h>

// Definición de pines
const int Wbutton = 6;
const int Sbutton = 8;
const int Dbutton = 7;
const int Abutton = 9;

const int Xbutton = 10;
const int Bbutton = 14;
const int Ybutton = 15;
const int Zbutton = 16;

const int R1button = 3;
const int L1button = 2;

const int joystickVRx1 = A1;
const int joystickVRy1 = A0;

const int joystickVRx2 = A3;
const int joystickVRy2 = A2;

const int joystickButton1 = 5;
const int joystickButton2 = 4;

const int mouseVelocity = 10;

// Variables para detección de cambios
static bool lastWState = HIGH, lastAState = HIGH, lastSState = HIGH, lastDState = HIGH;
static bool lastXState = HIGH, lastYState = HIGH, lastZState = HIGH, lastBState = HIGH;
static bool joystickState1=HIGH, joystickState2=HIGH;

// Tiempo de referencia para el loop

unsigned long lastTime = 0;

void setup() {
  Serial.begin(115200);

  // Configuración de pines
  pinMode(Wbutton, INPUT_PULLUP);
  pinMode(Abutton, INPUT_PULLUP);
  pinMode(Sbutton, INPUT_PULLUP);
  pinMode(Dbutton, INPUT_PULLUP);

  pinMode(Xbutton, INPUT_PULLUP);
  pinMode(Ybutton, INPUT_PULLUP);
  pinMode(Zbutton, INPUT_PULLUP);
  pinMode(Bbutton, INPUT_PULLUP);

  pinMode(R1button, INPUT_PULLUP);
  pinMode(L1button, INPUT_PULLUP);

  pinMode(joystickButton1, INPUT_PULLUP);
  pinMode(joystickButton2, INPUT_PULLUP);

  // Inicialización de teclado y ratón
  Keyboard.begin();
  Mouse.begin();
}

void loop() {
  if ((millis() - lastTime) > 5) {
    wasdButtonsControl();
    playStationButtonsControl();
    mouseClicksControl();
    joystickMovementControl();
    mouseMovementControl();
    lastTime = millis();
  }
}

bool pressKey(bool currentState, bool lastState, char key[]){
  if (currentState != lastState) {
    if (!currentState) Keyboard.press(key);
    else Keyboard.release(key);
    lastState = currentState;
  }
  return lastState;
}


void wasdButtonsControl() {
  bool currentWState = digitalRead(Wbutton);
  lastWState = pressKey(currentWState, lastWState, 'w');
  
  bool currentAState = digitalRead(Abutton);
  lastAState = pressKey(currentAState, lastAState, 'a');
  

  bool currentSState = digitalRead(Sbutton);
  lastSState = pressKey(currentSState, lastSState, 's');

  bool currentDState = digitalRead(Dbutton);
  lastDState = pressKey(currentDState, lastDState, 'd');
}

void playStationButtonsControl() {
  bool currentXState = digitalRead(Xbutton);
  lastXState = pressKey(currentXState, lastXState, 'x');
  
  bool currentYState = digitalRead(Ybutton);
  lastYState = pressKey(currentYState, lastYState, 'y');

  bool currentZState = digitalRead(Zbutton);
  lastZState = pressKey(currentZState, lastZState, 'z');

  bool currentBState = digitalRead(Bbutton);
  lastBState = pressKey(currentBState, lastBState, 'b');
}

void mouseClicksControl() {
  if (!digitalRead(joystickButton1)) Mouse.press(MOUSE_LEFT);
  else Mouse.release(MOUSE_LEFT);

  if (!digitalRead(joystickButton2)) Mouse.press(MOUSE_RIGHT);
  else Mouse.release(MOUSE_RIGHT);

  if (!digitalRead(L1button)) Keyboard.press('l');
  else Keyboard.release('l');

  if (!digitalRead(R1button)) Keyboard.press('r');
  else Keyboard.release('r');


}

void joystickMovementControl() {
  int VRx = analogRead(joystickVRx1);
  int VRy = analogRead(joystickVRy1);

  // Rango muerto más estricto
  if (VRy > 450 && VRy < 550 && VRx > 450 && VRx < 550) {
    Keyboard.release('w');
    Keyboard.release('a');
    Keyboard.release('s');
    Keyboard.release('d');
    return;
  }

  if (VRx > 550) Keyboard.press('w');
  else if (VRx < 450) Keyboard.press('s');
  else {
    Keyboard.release('w');
    Keyboard.release('s');
  }

  if (VRy > 550) Keyboard.press('d');
  else if (VRy < 450) Keyboard.press('a');
  else {
    Keyboard.release('a');
    Keyboard.release('d');
  }
}


void mouseMovementControl() {
  int VRy = analogRead(joystickVRx2);
  int VRx = analogRead(joystickVRy2);

  int xMove = 0;
  int yMove = 0;

  if (abs(VRx - 512) < 20) xMove = 0; // Filtrar movimientos pequeños
  else xMove = map(VRx, 0, 1023, -mouseVelocity, mouseVelocity);

  if (abs(VRy - 512) < 20) yMove = 0;
  else yMove = map(VRy, 0, 1023, mouseVelocity, -mouseVelocity);

  Mouse.move(xMove, yMove, 0);
}
