// Modes: Bullet 1:00, Blitz 3:00, Rapid 10:00
// Increments: OFF, +2, +5, +10
//
// BUTTONS (INPUT_PULLUP → pressed = LOW):
// Mode:     pin 2
// Inc:      pin 3
// Start:    pin 4
// Player A: pin 5
// Player B: pin 6
//
// LCD pins: 7,8,9,10,11,12
// -----------------------------------------------------------------------------

#include <LiquidCrystal.h>

const int PIN_MODE  = 2;
const int PIN_INC   = 3;
const int PIN_START = 4;
const int PIN_A     = 5;
const int PIN_B     = 6;

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int LED_A = A0;
const int LED_B = A1;

// MODE SETTINGS
struct Mode {
  const char* name;
  unsigned long seconds;
};

Mode modes[] = {
  {"Bullet",  60},
  {"Blitz",   180},
  {"Rapid",   600}
};
const int numModes = 3;

const unsigned int increments[] = {0, 2, 5, 10};
const int numIncs = 4;

//CLOCK STATE 
unsigned long timeLeft[2];  // A=0, B=1
int modeIndex = 0;
int incIndex = 0;
int activePlayer = 0;
bool running = false;
unsigned long lastTick = 0;

//DEBOUNCE 
unsigned long lastPressTime[10] = {0};
const unsigned long debounceDelay = 150;

// LONG PRESS (RESET)
unsigned long startPressBegin = 0;
const unsigned long LONG_PRESS = 1700;

// UTIL: print time mm:ss to LCD
void printTime(unsigned long s, int col, int row) {
  lcd.setCursor(col, row);

  char buf[10];
  // Ensures two-digit minutes always
  snprintf(buf, sizeof(buf), "%02lu:%02lu ", s/60, s%60);

  lcd.print(buf);
}

// DISPLAY SETUP SCREEN

void showSetup() {
  lcd.clear();
  lcd.setCursor(0, 0);

  lcd.print(modes[modeIndex].name);
  lcd.print(" ");
  printTime(modes[modeIndex].seconds, 8, 0);

  lcd.setCursor(0, 1);
  lcd.print("Inc:");
  if (increments[incIndex] == 0) lcd.print("OFF");
  else {
    lcd.print("+");
    lcd.print(increments[incIndex]);
  }

  lcd.setCursor(10, 1);
  lcd.print("Start");
}


// START MATCH
void startMatch() {
  timeLeft[0] = modes[modeIndex].seconds;
  timeLeft[1] = modes[modeIndex].seconds;

  activePlayer = 0;
  running = true;

  lastTick = millis();
  updateDisplay();
}


// UPDATE DISPLAY WHILE RUNNING
void updateDisplay() {
  lcd.clear();

  // Player A
lcd.setCursor(0,0);
lcd.print("A:");
printTime(timeLeft[0], 2, 0);

// Player B
lcd.setCursor(9,0);
lcd.print("B:");
printTime(timeLeft[1], 11, 0);   


  lcd.setCursor(0, 1);
  lcd.print(modes[modeIndex].name);
  lcd.print(" ");

  if (increments[incIndex] == 0) lcd.print("I:OFF");
  else {
    lcd.print("I:+");
    lcd.print(increments[incIndex]);
  }

  lcd.setCursor(12, 1);
  lcd.print(running ? "RUN" : "PAU");

  digitalWrite(LED_A, activePlayer == 0);
  digitalWrite(LED_B, activePlayer == 1);
}


// FULL RESET
void fullReset() {
  running = false;
  modeIndex = 0;
  incIndex = 0;

  lcd.clear();
  showSetup();
}

// SAFELY READ BUTTON WITH DEBOUNCE
bool pressed(int pin) {
  return digitalRead(pin) == LOW;
}

bool debounce(int pin) {
  unsigned long t = millis();
  if (t - lastPressTime[pin] >= debounceDelay) {
    lastPressTime[pin] = t;
    return true;
  }
  return false;
}


// FLAG – Times up
void flag(int p) {
  running = false;

  lcd.clear();
  lcd.setCursor(0, 0);
  if (p == 0) lcd.print("Player A Flag!");
  else        lcd.print("Player B Flag!");

  lcd.setCursor(0, 1);
  lcd.print("Match Over");

  digitalWrite(LED_A, LOW);
  digitalWrite(LED_B, LOW);
}

// SETUP
void setup() {
  pinMode(PIN_MODE,  INPUT_PULLUP);
  pinMode(PIN_INC,   INPUT_PULLUP);
  pinMode(PIN_START, INPUT_PULLUP);
  pinMode(PIN_A,     INPUT_PULLUP);
  pinMode(PIN_B,     INPUT_PULLUP);

  pinMode(LED_A, OUTPUT);
  pinMode(LED_B, OUTPUT);

  lcd.begin(16, 2);
  showSetup();
}

// MAIN LOOP

void loop() {
  unsigned long now = millis();

  // MODE (setup only)
  if (!running && pressed(PIN_MODE) && debounce(PIN_MODE)) {
    modeIndex = (modeIndex + 1) % numModes;
    showSetup();
  }

  // INCREMENT (setup only)
  if (!running && pressed(PIN_INC) && debounce(PIN_INC)) {
    incIndex = (incIndex + 1) % numIncs;
    showSetup();
  }

  // START (short/long press)
  if (pressed(PIN_START)) {
    if (startPressBegin == 0) startPressBegin = now;

    if (now - startPressBegin >= LONG_PRESS) {
      fullReset();
      startPressBegin = 0;
      return;
    }
  }
  else {
    if (startPressBegin != 0 && (now - startPressBegin < LONG_PRESS)) {
      if (!running) startMatch();
      else {
        running = !running;
        updateDisplay();
      }
    }
    startPressBegin = 0;
  }

  // PLAYER SWITCHING
  if (running) {

    // Player A ends turn → press A → switch to B
    if (pressed(PIN_A) && debounce(PIN_A) && activePlayer == 0) {
      timeLeft[0] += increments[incIndex];
      activePlayer = 1;
      updateDisplay();
    }

    // Player B ends turn → press B → switch to A
    if (pressed(PIN_B) && debounce(PIN_B) && activePlayer == 1) {
      timeLeft[1] += increments[incIndex];
      activePlayer = 0;
      updateDisplay();
    }

    // TIMER DECREMENT
    if (now - lastTick >= 1000) {
      lastTick = now;

      if (timeLeft[activePlayer] > 0)
        timeLeft[activePlayer]--;

      updateDisplay();

      if (timeLeft[activePlayer] == 0) {
        flag(activePlayer);
      }
    }
  }
}
