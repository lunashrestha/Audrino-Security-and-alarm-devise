#include <LiquidCrystal.h>
#include <Keypad.h>

#define buzzer 7
#define trigPin 8
#define echoPin 9
long duration;
int distance, initialDistance, currentDistance;
String password="1234";
String tempPassword;
boolean flagme = false; // State of the alarm
boolean activateAlarm = false;
boolean alarmActivated = false;
boolean enteredPassword; // State of the entered password to stop the alarm
boolean passChangeMode = false;
boolean passChange = false;
boolean setupMode = true;
boolean calibrated = false;
char pressedKey;
const byte ROWS = 4; 
const byte COLS = 4; 

//map buttons on keypad to symbols
char keyMap[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
byte rowPins[ROWS] = {13, 14, 15, 16}; //Row pinouts of the keypad
byte colPins[COLS] = {17, 18, 19, 20}; //Column pinouts of the keypad
Keypad myKeypad = Keypad( makeKeymap(keyMap), rowPins, colPins, ROWS, COLS); 
LiquidCrystal lcd(1, 2, 4, 5, 6, 7);


void setup() { 
  // set up 16x2 LCD	
  lcd.begin(16,2); 
  pinMode(buzzer, OUTPUT); //set buzzer as Output
  pinMode(echoPin, INPUT); //set echopin as input; sensor receives the signal
  pinMode(trigPin, OUTPUT); //set trigPin as output; sensor transmitter sends signal
}

void loop() {
	if (activateAlarm){  //checks if initialDistance has been measured; if yes, countdown before Activation
		if (calibrated) {
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Alarm activates in");
			int tiktok = 5;
			while (ticktok != 0){				//dramatic countdown begins for 5 seconds
				lcd.setCursor(7,1);			
				lcd.print(ticktok);
				tone(buzzer, 900, 100);
				ticktok--;
				delay(1000);
			}
			alarmActivated = true;
			activateAlarm = false;			
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Alarm Activated!");
		}
		else { 								//you have to calibrate initialDistance from reset option before 'first' Activation
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("Not calibrated");
			lcd.setCursor(0,1);
			lcd.print("Reset first!");
			activateAlarm = false;
		}
	}
	if (alarmActivated) {						//ativates buzzer if something comes near
		currentDistance = getDistance() + 10;
		if (currentDistance < initialDistance){
			lcd.clear();	
			tone(buzzer, 900);					//annoying 900 Hz buzz for eternity
			enterPassword();					//should enter correct password to stop buzzer					
		}
	}
	if (!alarmActivated) {						//if alarm not activated, shows option to activate/password change/recalibrate distance
		if (setupMode){
			lcd.clear();
			lcd.setCursor(0,0);
			lcd.print("A - Activate Alarm");
			lcd.setCurson(0,1);
			lcd.print("B - Options");	
			setupMode = false;
		}
		pressedKey = myKeypad.getKey();			//gets user keypad input
		if (pressedKey == 'A'){					//if A selected, activates countdown to alarm Activation
			tone(buzzer, 1000, 200);
			activateAlarm = true;
		}			
		else if (pressedKey == 'B') {			//shows option to Password change or Reset everything
			lcd.clear();
			tone(buzzer, 1000, 100);
			lcd.setCursor(0,0);
			lcd.print("A - Change Pass");
			lcd.setCursor(0,1);
			lcd.print("B - Reset");			
			pressedKey = myKeypad.getKey();
			if (pressedKey == 'A') {			//A for password change
				lcd.clear();
				int a=1;
				tone(buzzer, 1000, 100);
				lcd.setCursor(0,0);
				lcd.print("Current Password");
				lcd.setCursor(0,1);
				lcd.print(">");
				passChange = true;
				while (passChange) {
				pressedKey = myKeypad.getKey();
					if (pressedKey == NO_KEY) {  //runs when no key is pressed
						if (pressedKey == '0' || pressedKey == '1' || pressedKey == '2' || pressedKey == '3' ||
							pressedKey == '4' || pressedKey == '5' || pressedKey == '6' || pressedKey == '7' ||
							pressedKey == '8' || pressedKey == '9' ) {
							tempPassword += pressedKey;  //store pressed key to tempPassword
							tone(buzzer, 2000, 100);
							lcd.setCursor(a,1);			// password enter cursor set ex. 12_	
							lcd.print(pressedKey);		//display entered key
							delay(1000);				//wait 1 second
							lcd.print("*");				//hide password
							a++;							
						}
					}
					if (a > 5 || pressedKey == '#') {	//if password entered more than 4 key or # is pressed, again enter password from first
						tempPassword = "";				//clear temporary password to empty
						a=1;
						lcd.clear();						
						lcd.setCursor(0,0);
						lcd.print("Current Password");
						lcd.setCursor(0,1);
						lcd.print(">");
					}
					if (pressedKey = '*') {				//if * pressed
						a=1;
						tone(buzzer, 2000, 100);
						if (password == tempPassword) {	//check if entered password matches password and proceed to enter new password
							lcd.clear();
							lcd.setCursor(0,0);
							lcd.print("New Password:");
							lcd.setCursor(0,1);
							lcd.print(">");
							while(passChangeMode) {
								pressedKey = myKeypad.getKey();
								if (pressedKey != NO_KEY){
									if (pressedKey == '0' || pressedKey == '1' || pressedKey == '2' || pressedKey == '3' ||
										pressedKey == '4' || pressedKey == '5' || pressedKey == '6' || pressedKey == '7' ||
										pressedKey == '8' || pressedKey == '9' ) {
										tempPassword += pressedKey;
										tone(buzzer, 2000, 100);
										lcd.setCursor(a,1);
										lcd.print(pressedKey);										
										delay(1000);
										lcd.print("*");
										a++;										
									}
								}
								if (a > 5 || pressedKey == '#') {
									tempPassword = "";
									a=1;
									tone(buzzer, 2000, 100);
									lcd.clear();
									lcd.setCursor(0,0);
									lcd.print("New Password:");
									lcd.setCursor(0,1);
									lcd.print(">");
								}
								if ( pressedKey == '*') {
									a=1;
									tone(buzzer, 2000, 100);
									password = tempPassword;
									passChangeMode = false;
									passChange = false;
									setupMode = true;
								}            
							}
						}
					}
					
				}
			}
			else if (pressedKey = 'B'){   //reset password+ultrasonic sensor calibration
				reset = true;								
				while (reset){
					lcd.clear();
					lcd.setCursor(0,0);
					lcd.print("Reset Confirm?");
					lcd.setCursor(0,1);
					lcd.print("A - Yes B - No");
					pressedKey = myKeypad.getKey();
					if (pressedKey == NO_KEY) {
						if (pressedKey == 'A') {	//if options to reset confirmed						
							enterPassword();		//enter current password					
							password = "1234";		//if current password correct, set password to default 1234
							lcd.clear();
							lcd.setCursor(0,0);
							lcd.print("Recalibration");
							lcd.setCursor(0,1);
							lcd.print("Starting..");
							lcd.clear();
							int tiktok = 5;
							while (tiktok != 0) { 	//dramatic countdown begins to move aside and clear the space opposite the sensor
								lcd.setCursor(7,1);
								lcd.print(tiktok);
								delay(1000);
								tiktok--;
							}
							initialDistance = calibrateSensor(); //measure the distance and set initialDistance value
							lcd.clear();
							tone(buzzer, 2000, 100);
							lcd.setCursor(0,0);
							lcd.print("Reset Done!");							
							reset = false;
							delay(2000);												
						} 
						if (pressedKey != 'B' && pressedKey !='A') {	//if any other key beside A and B is pressed
							lcd.clear();
							tone(buzzer, 2000, 100);
							lcd.setCursor(0,0);
							lcd.print("Invalid input");
							delay(2000);
						}
						if (pressedKey == 'B') {				//if you do not want to reset
							tone(buzz, 2000, 100);
							lcd.clear();
							lcd.setCursor(0,0);
							lcd.print("No change");
							delay(2000);
							reset = false;
						}
					}
				}
			}
				
			}  		
		}			
		
	}
void enterPassword() {		//function to get user input password and check if it matches password; if not you are stuck here forever
  int k=1;
  tempPassword = "";	//tempPassword value empty
  flagme = true;
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(" Enter Password ");
  lcd.setCursor(0,1);
  lcd.print(">");
      while(flagme) {
      pressedKey = myKeypad.getKey();
      if (pressedKey != NO_KEY){
        if (pressedKey == '0' || pressedKey == '1' || pressedKey == '2' || pressedKey == '3' ||
            pressedKey == '4' || pressedKey == '5' || pressedKey == '6' || pressedKey == '7' ||
            pressedKey == '8' || pressedKey == '9' ) {
          tempPassword += pressedKey;
          lcd.setCursor(k,1);
		  lcd.print(pressedKey);
		  delay(1000);
          lcd.print("*");
          k++;
        }
      }
      if (k > 5 || pressedKey == '#') {		//if entered more than 4 keys or # is pressed, again enter password from beginning
        tempPassword = "";
        k=1;
        lcd.clear();
        lcd.setCursor(0,0);
        lcd.print(" Enter Password ");
        lcd.setCursor(0,1);
        lcd.print(">");
      }
      if ( pressedKey == '*') {				//* is pressed to confirm password, check if password matches and get out of this function
        if ( tempPassword == password ) {
          flagme = false;
          alarmActivated = false;
          noTone(buzzer);
          setupMode = true; 
        }
        else if (tempPassword != password) {	//if password is invalid, enter password from beginning
          lcd.clear();
		  lcd.setCursor(0,0);
          lcd.print("Wrong Password!");
          delay(2000);
          lcd.clear();
          lcd.setCursor(0,0);
          lcd.print(" Enter Password ");
          lcd.setCursor(0,1);
          lcd.print(">");
        }
      }    
    }
}
// reset initial distance for the Ultrasonic sensor
long calibrateSensor() {
	calibrated = true;
	distance = getDistance() + 10; //+10 cm for any error uncertainties
	return distance;	
}
//measure distance from the ultrasonic sensor
long getDistance(){
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH); //time to receive the signal
  distance = duration*0.034/2;		//sound travels at 0.034 cm/Âµs, divide by 2 to get distance before wave bouncing back
  return distance;
}

