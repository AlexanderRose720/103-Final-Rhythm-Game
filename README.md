# 103-Final-Rhythm-Game
Rhythm game made for Adafruit Circuit Playground Express board for ENGR 103 final

*Dependencies*

CPX Board and Library from Adafruit: [Link](https://github.com/adafruit/Adafruit_CircuitPlayground)
AsyncDelay Library: [Link](https://github.com/stevemarple/AsyncDelay)

**Rules:** <br>
LEDs will move down the left and right side, you must press the corresponding button as it gets to the bottom. Additionally the board will
count down from three and you must shake the board on zero. Your score will be tallied up and displayed a a percentage at the end.

**Additional Features:** <br>
In the main menu, the right button will take you to the settings menu where you can change the game color (left button) and the game 
volume (right button).

The switch will always act as a return until you get to the main menu where it will change the difficulty.

**Inputs:** <br>
Buttons which just use true or false
Accelerometer which read -8 to 8 g's. in the code it just reads as true over a certain threshold.

**Outputs:** <br>
LEDs
Speaker

