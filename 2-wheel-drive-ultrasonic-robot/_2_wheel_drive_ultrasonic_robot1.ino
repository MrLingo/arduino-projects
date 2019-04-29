// Pins.
const byte TRIG = 10;
const byte ECHO = 9;
const byte MOTOR_CONTROL = 3;
const byte RGB1_GREEN = 4;
const byte RGB1_RED = 7;
const byte RGB2_GREEN = 12;
const byte RGB2_RED = 13;

// Value holders.
unsigned long duration;
unsigned short cm;

void setup()
{
    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
    pinMode(MOTOR_CONTROL, OUTPUT);
    pinMode(RGB1_GREEN, OUTPUT);
    pinMode(RGB1_RED, OUTPUT);
    pinMode(RGB2_GREEN, OUTPUT);
    pinMode(RGB2_RED, OUTPUT);
}


void loop()
{
  // Ensuring a clean HIGH pulse by starting with LOW.
  // The sensor initiates when a HIGH signal for about 10μs is given.
  digitalWrite(TRIG, LOW);
  delayMicroseconds(5);
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  
  // Wait for the pin to become HIGH ( Recieve the pulse )
  // and starts timing.
  duration = pulseIn(ECHO, HIGH);
  
  cm = (duration/2) / 29.1; 
  
  if( cm <= 120){
      analogWrite(MOTOR_CONTROL, 0);    
      digitalWrite(RGB1_RED, HIGH);
      digitalWrite(RGB2_RED, HIGH);
      digitalWrite(RGB1_GREEN, LOW);
      digitalWrite(RGB2_GREEN, LOW);   
  }
  else {
      analogWrite(MOTOR_CONTROL, 250);   
      digitalWrite(RGB1_GREEN, HIGH);
      digitalWrite(RGB2_GREEN, HIGH);
      digitalWrite(RGB1_RED, LOW);
      digitalWrite(RGB2_RED, LOW);
  }
  
}