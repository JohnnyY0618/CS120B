# CS120B Spring 2021
## Introduction to Embedded Systems
### Lab 1
Zybooks Reading and activity, so not lab available!

### Lab 2： Intro to AVR Toolchain
1. Garage open at night-- A garage door sensor connects to PA0 (1 means door open), and a light sensor connects to PA1 (1 means light is sensed). Write a program that illuminates an LED connected to PB0 (1 means illuminate) if the garage door is open at night.
   Hints: PA0 = garage door sensor (input), PA1 = light sensor (input), PB0 = LED (output)
2. Port A's pins 3 to 0, each connect to a parking space sensor, 1 meaning a car is parked in the space, of a four-space parking lot. Write a program that outputs in binary on port C the number of available spaces (Hint: declare a variable "unsigned char cntavail;" you can assign a number to a port as follows:
PORTC = cntavail;).
3. Extend the previous program to still write the available spaces number, but only to PC3..PC0, and to set PC7 to 1 if the lot is full.
4. (**Challenge**) An amusement park kid ride cart has three seats, with 8-bit weight sensors connected to ports A, B, and C (measuring from 0-255 kilograms). Set PD0 to 1 if the cart's total passenger weight exceeds the maximum of 140 kg. Also, the cart must be balanced: Set port PD1 to 1 if the difference between A and C exceeds 80 kg.  The remaining 6 bits on D should display an approximation of the total combined weight.
    <u>Hint</u>: You don’t have enough bits to represent the complete value; you only need to represent the most significant bits.
    <u>Interesting note</u>: Disneyland recently redid their "It's a Small World" ride because the average passenger weight has increased over the years, causing more boats to get stuck on the bottom.

### Lab 3： Bit manipulation (Vehicle sensors)
1. Count the number of 1s on ports A and B and output that number on port C. 
2. A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full). A series of LEDs connected to PC5..PC0 should light to graphically indicate the fuel level. If the fuel level is 1 or 2, PC5 lights. If the level is 3 or 4, PC5 and PC4 light. Level 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 lights PC5..PC1. 13-15 lights PC5..PC0. Also, PC6 connects to a "Low fuel" icon, which should light if the level is 4 or less.  (The example below shows the display for a fuel level of 3).   
3. In addition to the above, PA4 is 1 if a key is in the ignition, PA5 is one if a driver is seated, and PA6 is 1 if the driver's seatbelt is fastened. PC7 should light a "Fasten seatbelt" icon if a key is in the ignition, the driver is seated, but the belt is not fastened. 
4. (**Challenge**): Read an 8-bit value on PA7..PA0 and write that value on PB3..PB0PC7..PC4. That is to say,  take the upper nibble of PINA and map it to the lower nibble of PORTB, likewise take the lower nibble of PINA and map it to the upper nibble of PORTC (PA7 -> PB3, PA6 -> PB2, … PA1 -> PC5, PA0 -> PC4).
5. (**Challenge**): A car's passenger seat weight sensor outputs a 9-bit value (ranging from 0 to 511) and connects to input PD7...PD0PB0 on the microcontroller. If the weight is equal to or above 70 pounds, the airbag should be enabled by setting PB1 to 1. If the weight is above 5 but below 70, the airbag should be disabled and an "Airbag disabled" icon should light by setting PB2 to 1. (Neither B1 nor B2 should be set if the weight is 5 or less, as there is no passenger).

### Lab 4： State machines (Digital door lock)
1. PB0 and PB1 each connect to an LED, and PB0's LED is initially on. Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED, staying that way after button release. Pressing the button again turns off PB1's LED and turns on PB0's LED. 
2. PB0 and PB1 each connect to an LED, and PB0's LED is initially on. Pressing a button connected to PA0 turns off PB0's LED and turns on PB1's LED, staying that way after button release. Pressing the button again turns off PB1's LED and turns on PB0's LED. 
3. A household has a digital combination deadbolt lock system on the doorway. The system has buttons on a keypad. Button **'X'** connects to PA0, **'Y'** to PA1, and **'#** to PA2. Pressing and releasing '#', then pressing 'Y', should unlock the door by setting PB0 to 1. Any other sequence fails to unlock. Pressing a button from inside the house (PA7) locks the door (PB0=0). For debugging purposes, give each state a number, and always write the current state to PORTC (consider using the enum state variable). Also, be sure to check that only one button is pressed at a time. 
4. (**Challenge**) Extend the above door so that it can also be locked by entering the earlier code.
5. (**Challenge**) Extend the above door to require the 4-button sequence #-X-Y-X rather than the earlier 2-button sequence. To avoid excessive states, store the correct button sequence in an array, and use a looping SM.  

### Lab 5: Using the ATMega1284 microcontroller
1. (From an earlier lab) A car has a fuel-level sensor that sets PA3..PA0 to a value between 0 (empty) and 15 (full). A series of LEDs connected to PC5..PC0 should light to graphically indicate the fuel level. If the fuel level is 1 or 2, PC5 lights. If the level is 3 or 4, PC5 and PC4 light. 5-6 lights PC5..PC3. 7-9 lights PC5..PC2. 10-12 lights PC5..C1. 13-15 lights PC5..PC0. Also, PC6 connects to a "Low fuel" icon, which should light if the level is 4 or less. Use buttons on PA3..PA0 and mimic the fuel-level sensor with presses. 
2. (From an earlier lab) Buttons are connected to PA0 and PA1. Output for PORTC is initially 7. Pressing PA0 increments PORTC (stopping at 9). Pressing PA1 decrements PORTC (stopping at 0). If both buttons are depressed (even if not initially simultaneously), PORTC resets to 0. If a reset occurs, both buttons should be fully released before additional increments or decrements are allowed to happen. Use LEDs (and resistors) on PORTC. Use a state machine (not synchronous) captured in C.
    **Note**: Make sure that one button press causes only one increment or decrement respectively. Pressing and holding a button should **<u>NOT</u>** continually increment or decrement the counter.
3. (**Challenge**) Create your own festive lights display with 6 LEDs connected to port PB5..PB0, lighting in some attractive sequence. Pressing the button on PA0 changes the lights to the next configuration in the sequence.  Use a state machine (not synchronous) captured in C. 

### Lab 6： Timer SynchSMs
1. Create a synchSM to blink three LEDs connected to PB0, PB1, and PB2 in sequence, 1 second each. Implement that synchSM in C using the method defined in class. In addition to demoing your program, you will need to show that your code adheres entirely to the method with no variations.
    - To clarify and match the video, the lights should turn on in the following sequence:
       - PB0,PB1,PB2,PB0,PB1,PB2,PB0,PB1,PB2…
2. Create a simple light game that requires pressing a button on PA0 while the middle of three LEDs on PB0, PB1, and PB2 is lit. The LEDs light for 300 ms each in sequence. When the button is pressed, the currently lit LED stays lit. Pressing the button again restarts the game.
    - To clarify and match the video, the lights should turn on in the following sequence:
      - PB0,PB1,PB2,PB1,PB0,PB1,PB2,PB1,PB0…
3. (**Challenge**) (from an earlier lab) Buttons are connected to PA0 and PA1. Output for PORTB is initially **7**. Pressing PA0 increments PORTB once (stopping at 9). Pressing PA1 decrements PORTB once (stopping at 0). If both buttons are depressed (even if not initially simultaneously), PORTB resets to 0. **Now that we have timing**, only check to see if a button has been pressed every 100 ms. Additionally, if a button is held, then the count should continue to increment (or decrement) at a rate of once per second. 

### Lab 7： ADC
1. Make sure your breadboard is wired according to the prelab. The potentiometer is used to adjust the voltage supplied to the microcontroller for ADC . Design a system that reads the 10-bit ADC result from the ADC register, and displays the result on a bank of 10 LEDs.    
    **Hints**:
    - Assuming the breadboard has been wired according to the prelab photo, display the lower 8 bits on port B, and the upper 2 bits on port D.
    - Use a “short” variable to hold the ADC result.
    - Use bit shifting and casting to align the proper bits to the proper ports. For example:
        - ```
          unsigned shoart my_short = 0xABCD;
          unsigned char my_char = (char)my_short; // my_char = 0xCD
          my_char = (char)(my_short >> 4); // my_char = 0xBC
          ```       
2. A **photoresistor** is a resistor whose resistance varies based on how much light the photoresistor detects. An additional resistor needs to be connected in parallel with the photoresistor. Without the additional resistor, results from ADC will be too small to observe. A 330 ohm resistor is chosen because it is common in the lab kits. Connect the photoresistor to the microcontroller according to the diagram below.    
    **NOTE**: With the photoresistor connected in this way, more light equals higher ADC values.
   Revise exercise 1 by replacing the potentiometer with a photoresistor and 330 ohm resistor. Take note of the highest ADC value displayed (MAX) when the photoresistor is exposed to light, and the lowest ADC value displayed (MIN) when the photoresistor is deprived of all light. These values will be used for the remaining lab exercises.
3. Design a system where an LED is illuminated only if enough light is detected from the photo resistor. Criteria:
   - a. If the result of the ADC is >= MAX/2, the LED is illuminated.
   - b. If the result of the ADC is < MAX/2, the LED is turned off. 
**Note**: MAX is the highest ADC value observed from exercise 2 of the lab.
4. (**Challenge**) Design a system, using a bank of eight LEDs, where the number of LEDs illuminated is a representation of how much light is detected. For example, when more light is detected, more LEDs are illuminated. Criteria:
   The LEDs should illuminated in sequence from 0 to 7, based on the amount of light detected by the photoresistor.  
**Hints**:
   - Use the MAX ADC value observed from exercise 2 as the highest amount of light detectable by the photoresistor. Divide that number by eight to determine the thresholds for each LED.
### Lab 8: PWM (Music player)
1. Using the ATmega1284’s PWM functionality, design a system that uses three buttons to select one of three tones to be generated on the speaker. When a button is pressed, the tone mapped to it is generated on the speaker. Criteria:
    - Use the tones C4, D4, and E4 from the table in the introduction section.
    - When a button is pressed and held, the tone mapped to it is generated on the speaker.
    - When more than one button is pressed simultaneously, the speaker remains silent.
    - When no buttons are pressed, the speaker remains silent.
2. Using the ATmega1284’s PWM functionality, design a system where the notes: C4, D, E, F, G, A, B, and C5,  from the table at the top of the lab, can be generated on the speaker by scaling up or down the eight note scale. Three buttons are used to control the system. One button toggles sound on/off. The other two buttons scale up, or down, the eight note scale. Criteria:
    - The system should scale up/down one note per button press.
    - When scaling down, the system should not scale below a C.
    - When scaling up, the system should not scale above a C. 
**Hints**: Breaking the system into multiple synchSMs could make this part easier.

3. (**Challenge**) Using the ATmega1284’s built in PWM functionality, design a system where a short, five-second melody, is played when a button is pressed. NOTE: The melody must be somewhat complex (scaling from C to B is NOT complex). Criteria:
    - When the button is pressed, the melody should play until completion
    - Pressing the button again in the middle of the melody should do nothing
    - If the button is pressed and held, when the melody finishes, it should not repeat until the button is released and pressed again 
**Hints**: One approach is to use three arrays. One array holds the sequence of notes for the melody. Another array holds the times that each note is held. The final array holds the down times between adjacent notes.

### Lab 9: Concurrent synchSMs (Noise maker device)
1. Connect LEDs to PB0, PB1, PB2, and PB3.
    - In one state machine (ThreeLEDsSM), output to a shared variable (threeLEDs) the following behavior: set only bit 0 to 1, then only bit 1, then only bit 2 in sequence for 1 second each.
    - In a second state machine (BlinkingLEDSM), output to a shared variable (blinkingLED) the following behavior: set bit 3 to 1 for 1 second, then 0 for 1 second.
    - In a third state machine (CombineLEDsSM), combine both shared variables and output to the PORTB. 
Note: only one SM is writing to outputs. Do this for the rest of the quarter.
2. Modify the above example so the threeLEDs light for 300 ms, while blinkingLED’s LED still blinks 1 second on and 1 second off.
3. To the previous exercise's implementation, connect your speaker's red wire to PB4 and black wire to ground. Add a third task that toggles PB4 on for 2 ms and off for 2 ms as long as a switch on PA2 is in the on position. **Don’t use the PWM for this task**.
4. (**Challenge**) Extend the previous exercise to allow a user to adjust the sound frequency up or down using buttons connected to PA0 (up) and PA1 (down). Using our 1 ms timer abstraction, the fastest you'll be able to pulse is 1 ms on and 1 ms off, meaning 500 Hz. **Again: Don’t use the PWM for this task**. Hint: You'll probably want to introduce another synchSM that polls the buttons and sets a global variable storing the current frequency that in turn is read by the frequency generator task. 
