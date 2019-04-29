// First SR pins:
const byte DATA_PIN_1 = 7;
const byte LATCH_PIN_1 = 8;
const byte CLOCK_PIN_1 = 9;

// Second SR pins:
const byte DATA_PIN_2 = 10;
const byte LATCH_PIN_2 = 11;
const byte CLOCK_PIN_2 = 12;

// Third SR pins ( LED's ):
const byte DATA_PIN_3 = 2;
const byte LATCH_PIN_3 = 3;
const byte CLOCK_PIN_3 = 4;

// First 7 digit segment pins:
const byte C = 5;
const byte B = 6;
const byte NEGATIVE = 13;

// Sensors:
const byte TEMP_SENSOR = A1;
const byte GAS_SENSOR = A0;

// Value holders:
signed short tempSensor;
unsigned int gasSensor;
signed short temperature;
byte firstDigit;
byte secondDigit;
byte thirdDigit;

// Final temperature value.
double finalResult; 


class Display1 {

    public:
  
    void drawNothing(){
        digitalWrite(C, LOW);
        digitalWrite(B, LOW);
        digitalWrite(NEGATIVE, LOW);
    }
  
  
    void drawOne(){
        digitalWrite(C, HIGH);
        digitalWrite(B, HIGH);
        digitalWrite(NEGATIVE, LOW);
    }
  
  
    void drawNegative(){
        digitalWrite(NEGATIVE, HIGH);
    }
  
   
    Display1(byte value){
        if( value == 1){
            drawOne(); // Display 1 on the display. 
        }
        else if( value == 0 ){
            drawNothing(); // Clear the display.
        }
        else if( value == 10 ){
            drawNegative();
        }
        else{
            Serial.print("Incorrect value for this display!");      
        }
    } // constructor
  
};


class Display2{
  
  public:
  
  void changeLatch(byte value){
      digitalWrite(LATCH_PIN_1, LOW);
      shiftOut(DATA_PIN_1, CLOCK_PIN_1, MSBFIRST, value);
      digitalWrite(LATCH_PIN_1, HIGH);
  }
  
  
  Display2(byte digit){
      switch (digit) {
          case 0: changeLatch(63);
          break;
          case 1: changeLatch(6);
          break;
          case 2: changeLatch(91);
          break;
          case 3: changeLatch(79); 
          break;
          case 4: changeLatch(102); 
          break;
          case 5: changeLatch(109);
          break;
          case 6: changeLatch(125); 
          break;
          case 7: changeLatch(7); 
          break;
          case 8: changeLatch(127); 
          break;
          case 9: changeLatch(111); 
          break;
          default: changeLatch(64);
          break;
      }  
  } // Constructor
}; // Class


class Display3
{
  public:
  
  void changeLatch(byte value){
      digitalWrite(LATCH_PIN_2, LOW);
      shiftOut(DATA_PIN_2, CLOCK_PIN_2, MSBFIRST, value);
      digitalWrite(LATCH_PIN_2, HIGH);
  }
  
  
  Display3(byte digit){
      switch (digit) {
          case 0: changeLatch(0);
          break;
          case 1: changeLatch(6);
          break;
          case 2: changeLatch(91);
          break;
          case 3: changeLatch(79); 
          break;
          case 4: changeLatch(102); 
          break;
          case 5: changeLatch(109);
          break;
          case 6: changeLatch(125); 
          break;
          case 7: changeLatch(7); 
          break;
          case 8: changeLatch(127); 
          break;
          case 9: changeLatch(111); 
          break;
          default: changeLatch(64);
          break;
      }  
  } // Constructor
}; // Class


double getTemperature(){
    tempSensor = analogRead(TEMP_SENSOR);
    float tempC;
  
    tempC = (float)tempSensor / 1024;      // Find percentage of input reading.
    tempC = tempC * 5;                     // Multiply by 5V to get voltage.
    tempC = tempC - 0.5;                   // Subtract the offset.
    tempC = tempC * 100; 

    return tempC;
}


float calculateDanger(){
  // TMP36 Calculation: ( > 55 critical )
  float tempC;
  tempC = getTemperature();

  gasSensor = analogRead(GAS_SENSOR);
  gasSensor -= 150;                   // Some random value for adjusting realistic result.
  
  return gasSensor + tempC;
}


void changeLatchLED(byte value){
    digitalWrite(LATCH_PIN_3, LOW);
    shiftOut(DATA_PIN_3, CLOCK_PIN_3, MSBFIRST, value);
    digitalWrite(LATCH_PIN_3, HIGH);
}


void setup()
{
  pinMode(DATA_PIN_1, OUTPUT);
  pinMode(LATCH_PIN_1, OUTPUT);
  pinMode(CLOCK_PIN_1, OUTPUT);
  
  pinMode(DATA_PIN_2, OUTPUT);
  pinMode(LATCH_PIN_2, OUTPUT);
  pinMode(CLOCK_PIN_2, OUTPUT);
  
  pinMode(DATA_PIN_3, OUTPUT);
  pinMode(LATCH_PIN_3, OUTPUT);
  pinMode(CLOCK_PIN_3, OUTPUT);
  
  pinMode(C, OUTPUT);
  pinMode(B, OUTPUT);
  
  pinMode(TEMP_SENSOR, INPUT);
  pinMode(GAS_SENSOR, INPUT);
  
  pinMode(NEGATIVE, OUTPUT);
}


void loop()
{    
  temperature = (short)getTemperature();
  
  // Disassembling the temperature number on it's digits
  // and passing them on each display.
  firstDigit = ((temperature / 10) / 10) % 10;
  secondDigit = (temperature / 10 ) % 10;
  thirdDigit = temperature % 10 ;
  
  // If the temperature is sub-zero the system does not care about activating
  // and displays only negatives.
  if( temperature < 0 ){
      firstDigit = 10;
      secondDigit = 10;
      thirdDigit = 10;
  }
  
 
  Display1 display1(firstDigit);
  Display2 display2(secondDigit);
  Display3 display3(thirdDigit);
  
  // Some combined result of temperature and gas value.
  finalResult = calculateDanger(); 
    
  if( finalResult <= 89 || temperature < 0){
      changeLatchLED(1); 
  }
  else if( (finalResult >= 90 && finalResult <= 199) || temperature < 25 ){
      changeLatchLED(3);    
  } 
  else if( finalResult >= 200 && finalResult <= 299){
      changeLatchLED(7);       
  }
  else if( finalResult >= 300 && finalResult <= 370 ){
      changeLatchLED(15);       
  }
  else if( finalResult >= 371 && finalResult <= 430 ){
      changeLatchLED(31);       
  }
  else if( finalResult >= 431 ){
      changeLatchLED(63); 
  }    
} // loop