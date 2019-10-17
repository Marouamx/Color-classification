#include <Servo.h>

#define SensorPin 1 //Analog Pin
#define redPin 3 //Red LED Pin
#define greenPin 4 //Green LED Pin
#define bluePin 5 //Blue LED Pin

#define trigger 6
#define echo 12


// Declaring all servos
Servo servoA;
Servo servoB;
Servo servoC;
Servo servoD;


int counter = 0 ;
bool Ready = false;

// for storing the colors
char color ;

int r ;
int g ;
int b ; 

void setup() {


  pinMode(redPin,OUTPUT);
  pinMode(greenPin,OUTPUT);
  pinMode(bluePin,OUTPUT);

  pinMode(12,INPUT); // for the sensor
  pinMode(6,OUTPUT); // for the sensor 

  // Attaching and initializing all servos
  servoA.attach(7);
  servoB.attach(8);
  servoC.attach(9);
  servoD.attach(10);

  // Setting up the ARM

  restorePosition();


  //Beging thr serial communication at 9600 baud rate
  Serial.begin(9600);
  
  // For Assuring communication with BT device
  Serial.write(2);
  Serial.write(2);

}

void loop() {

  //Waiting for a signal to start
  if(Ready){
    
  // iterate 3 times to complete the task
  

      color = getColor();
      grip(true); // open the grip
      moveArmTo(154,74,38,10,10,10);
      grip(false); // close the grip
      
      moveArmTo(164,10,90,10,10,10);
      delay(400);
      
      //sending the color to the phone 
      //for more effects |for testing
      Serial.write(color);

      //placing the color in its appropriat place
      PlaceColorAs(color);
      delay(100);
      grip(true);
      delay(400);

      //restore the default position
      restorePosition();
      Ready = false;
    }
 
}



void moveArmTo(int b,int c,int d,int t1,int t2,int t3){

          writeTo(servoB,b,t1); // rotate the arm
          delay(400);
          
          writeTo(servoD,d,t3); // move up/down
          delay(400);
          
          writeTo(servoC,c,t2); // move forward/backward
          delay(400);
          
}

void grip(bool g){

  if(g) writeTo(servoA,60,1);
  else  writeTo(servoA,0,6);
  
  
}


void PlaceColorAs(char c){

switch(c){

  //NOTE: the values entered in the functions 
  //      must be changed after testing
  
  case 'r': moveArmTo(107,20,75,10,7,7);
          break;

  case 'g': moveArmTo(70,20,68,10,10,10);
          break;

  case 'b': moveArmTo(45,48,90,10,10,10);
          break;        
  }
  
  grip(true); // for releasing the object
  
}

void restorePosition(){
  
     writeTo(servoB,90,2); // rotate the arm
          
     writeTo(servoC,10,2); // move forward/backward
          
     writeTo(servoD,90,2); // move up/down

  }


// alternative function to write to servo

void writeTo(Servo sv,int angle,int t){
  int p = sv.read();
  if((p-angle)< 0){

    for(int i=p;i<= angle; i++){
    sv.write(i);
    delay(t);}
  }
  else{
    
    for(int i=p ;i>=angle; i--){
    sv.write(i);
    delay(t);}
    
    }
  }

// function for getting the color

char getColor(){
  
  char c ;
  
  turnOn(redPin);
  delay(50); 
  r = analogRead(SensorPin);

  turnOn(greenPin);
  delay(50); 
  g = analogRead(SensorPin);

  turnOn(bluePin);
  delay(50); 
  b = analogRead(SensorPin);
  
  //turn off the last led
  digitalWrite(bluePin,LOW); //turn off the last led

  if (r>g && r>b) c = 'r' ;
  if (g>r && g>b) c = 'g' ;
  if (b>r && b>g) c = 'b' ;
  
  return c;
  }
  

void turnOn(int l){
  digitalWrite(redPin,LOW);
  digitalWrite(greenPin,LOW);
  digitalWrite(bluePin,LOW); 
  
  digitalWrite(l,HIGH);}


float CheckForObject(){
  
  digitalWrite(trigger, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigger, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigger, LOW);
   
  // distance in centimeters
  float distance = pulseIn(echo, HIGH) / 58;
   return distance;
  }

void serialEvent() {
  // wait for a signal from the phone 
  // to start the arm 
  if(Serial.available()>0){
  char inChar = (char) Serial.read();

  if(inChar == 'R') Ready = true ;
    
    }
  }
