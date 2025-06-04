# 103-Final-Rhythm-Game<br>
Rhythm game made for Adafruit Circuit Playground Express board for ENGR 103 final<br>
<br>
*Dependencies*<br>
<br>
CPX Board and Library from Adafruit: [Link](https://github.com/adafruit/Adafruit_CircuitPlayground)<br>
AsyncDelay Library: [Link](https://github.com/stevemarple/AsyncDelay)<br>
<br>
**Rules:** <br>
LEDs will move down the left and right side, you must press the corresponding button as it gets to the bottom. Additionally the board will<br>
count down from three and you must shake the board on zero. Your score will be tallied up and displayed a a percentage at the end.<br>
<br>
**Additional Features:** <br>
In the main menu, the right button will take you to the settings menu where you can change the game color (left button) and the game <br>
volume (right button).<br>
<br>
The switch will always act as a return until you get to the main menu where it will change the difficulty.<br>
<br>
**Inputs:** <br>
Buttons which just use true or false <br>
Accelerometer which read -8 to 8 g's. in the code it just reads as true over a certain threshold. <br>
<br>
**Outputs:** <br>
LEDs <br>
Speaker <br>

