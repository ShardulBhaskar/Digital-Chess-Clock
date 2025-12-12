⏱️ Arduino Chess Clock
A fully functional chess clock supporting Bullet, Blitz, Rapid time controls with optional increments.
________________________________________
📝 Project Overview
This project implements a digital chess clock using an Arduino UNO, a 16×2 LCD, and push buttons.
It supports:
•	Bullet (1 min)
•	Blitz (3 min)
•	Rapid (10 min)
•	Increments (+0, +2, +5, +10 seconds per move)
The clock behaves similarly to commercial chess clocks, but is open-source, low-cost, and fully customizable.
________________________________________
🎯 Problem Statement
Chess players often need a reliable timer for casual or tournament-style games. Commercial clocks can be expensive, and building your own provides hands-on experience with:
•	Real-time embedded systems
•	Debouncing
•	LCD interfacing
•	Input handling
•	Timing algorithms
This project solves the need for an affordable, customizable chess clock.
________________________________________
✅ Features
Core Functionalities
•	Three time controls (1/3/10 minutes)
•	Per-move increments: OFF, +2, +5, +10 seconds
•	Accurate countdown using millis()
•	Player A / Player B switching
•	Start/Pause and Long-Press Reset
•	LCD shows both timers simultaneously
•	Active player indicator LEDs 
________________________________________
🧰 Components Used
Component	Qty	Purpose
Arduino UNO	1	Main controller
16×2 LCD	1	Displays both timers
Pushbuttons	5	Mode, Inc, Start, Player A, Player B
10k Potentiometer	1	LCD contrast
Jumper Wires	—	Connections
Breadboard	1	Hardware layout
LEDs 	2	Active player indicators
220Ω resistors 	2	LED current limiting
________________________________________
🧠 System Design
Logic Flow 
1.	Setup Mode
o	Mode button cycles: Bullet → Blitz → Rapid
o	Increment button cycles: OFF → +2 → +5 → +10
o	Start begins game
2.	Game Running
o	Player A starts
o	Pressing A → adds increment → switches to B
o	Pressing B → adds increment → switches to A
o	Every 1000ms → active clock decrements
3.	Game End
o	When timer hits zero → display "Flag" screen
4.	Reset
o	Long press Start (1.7s)
________________________________________
🔧 Implementation Notes
Assembly
•	LCD wired in 4-bit mode
•	Buttons wired using INPUT_PULLUP (pin → button → GND)
•	LED indicators optional
Problems Faced & Fixes
•	Unresponsive buttons → corrected wiring using INPUT_PULLUP
•	LCD showing ghost numbers → fixed by padding output & adjusting cursor position
•	Switching only once → fixed debounce + proper button logic
•	LCD blank → contrast adjustment and proper RW grounding
________________________________________
💻 Final Code 
👉 Full code is included in:
https://github.com/ShardulBhaskar/Digital-Chess-Clock/blob/main/Chess-Clock.ino
________________________________________
📷 Results
Working Features
•	All three time modes tested
•	Increment applied correctly
•	Smooth switching between players
•	Clear MM:SS display
•	Reset works as expected
________________________________________
🚀 Future Improvements
•	Add Bronstein or Fischer delay
•	Add buzzer/sound alerts
•	Battery-powered portable version
•	Use I2C LCD to reduce wiring
•	Add rotary encoder for time selection
•	Save last settings in EEPROM
•	Add graphical OLED instead of LCD
________________________________________
📜 License
MIT License — use freely for learning or personal projects.
________________________________________
🤝 Contributing
Pull requests are welcome!
Feel free to improve time controls, UI, or add new modes.
