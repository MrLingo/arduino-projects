#include <LiquidCrystal.h>

// Pins:
const byte LATCH_PIN = 6;
const byte CLOCK_PIN = 7;
const byte DATA_PIN = 5;

const byte RS = 13; 
const byte EN = 12; 
const byte D4 = 11;
const byte D5 = 10;
const byte D6 = 9;
const byte D7 = 8;

const byte START_SWITCH = 0;
const byte TIME_POTENTIOMETER = A0;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

// Value holders:
short poVal = 0;
short eachLEDDelay = 0;
bool switchState = 0;

// DIP Switch value holders:
bool val1 = 0;
bool val2 = 0;
bool val3 = 0;
bool val4 = 0;

// 595 values:
// 1, 3, 7, 15, 31, 63, 127, 255

void changeLatch(short value, int delayVal){
    digitalWrite(LATCH_PIN, LOW);
    shiftOut(DATA_PIN, CLOCK_PIN, LSBFIRST, value);
    digitalWrite(LATCH_PIN, HIGH);
    delay(delayVal); 
}


void setup(){
    lcd.begin(16,2);
  
    // Setting each pin of DIP Switch to INPUT.
    pinMode(1, INPUT);
    pinMode(2, INPUT);
    pinMode(3, INPUT);
    pinMode(4, INPUT);  
    pinMode(LATCH_PIN, OUTPUT);
    pinMode(CLOCK_PIN, OUTPUT);
    pinMode(DATA_PIN, OUTPUT);  
    pinMode(START_SWITCH, INPUT);
    pinMode(TIME_POTENTIOMETER, INPUT);   
}


void loop(){
    // Each choice corresponds to: Low, Medium, High and Very high temp. ( MHz )
    val1 = digitalRead(1);
    val2 = digitalRead(2);
    val3 = digitalRead(3);
    val4 = digitalRead(4);
  
    lcd.setCursor(0,0);
  
    if( val1 == HIGH ){  
        lcd.print("LOW          ");
        lcd.setCursor(0,1);
        lcd.print("               ");
    }
    else if( val2 == HIGH ){ 
        lcd.print("MEDIUM       ");
        lcd.setCursor(0,1);
        lcd.print("               ");
    }
    else if( val3 == HIGH ){
        lcd.print("HIGH         ");
        lcd.setCursor(0,1);
        lcd.print("               ");
    }
    else if( val4 == HIGH ){
        lcd.print("VERY HIGH    ");
        lcd.setCursor(0,1);
        lcd.print("               ");
    }
    else {        
        lcd.print("Please choose");
        lcd.setCursor(0,1);
        lcd.print("degree of heat!");
    }      

  
    poVal = analogRead(TIME_POTENTIOMETER);
  
    // Extend the time value the potentiometer gives. ( Too short )
    eachLEDDelay = (poVal * 40) / 8;
    switchState = digitalRead(START_SWITCH);
   
    // Inverse the state of the switch, since it starts with 'HIGH'.
    // Only start the process if some degree of heat is selected.
    if( switchState == LOW && ( val1 || val2 || val3 || val4 )){  
        changeLatch(1, eachLEDDelay);
        changeLatch(3, eachLEDDelay);
        changeLatch(7, eachLEDDelay);
        changeLatch(15, eachLEDDelay);
    
        changeLatch(31, eachLEDDelay);
        changeLatch(63, eachLEDDelay);
        changeLatch(127, eachLEDDelay);
        changeLatch(255, eachLEDDelay);
    
        // Notify that the heating stopped.
        for(int i = 0; i < 10; i++){
            changeLatch(0, 500);
            changeLatch(255, 500);
             
            lcd.setCursor(0,0); 
            lcd.print("Finished!      ");
            lcd.setCursor(0,1);
            lcd.print("               ");          
        } // for  
    } // if    
} // void
