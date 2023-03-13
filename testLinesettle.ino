#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
//#include <HCSR04.h>//old sonic sensor library
#include <Wire.h>
#include <Adafruit_Sensor.h>
//#include <Adafruit_HMC5883L.h> check wether it is possible to include this(magnatometer library)
#include <NewPing.h>

#define COLLISION_DISTANCE 52
#define MAX_SPEED 180
#define  REFERENCE_DISTANCE 30

#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters)
#define NUM_READINGS 5 // Number of readings to take for each distance measurement

#define SCREEN_WIDTH 128 // OLED display width,  in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// declare an SSD1306 display object connected to I2C
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

///////////////////////////////////////////////////////////////////// color sensor variables//////////////////
/*int S0 = 2; //pin names
int MZS1 = 3;
int MZS2 = 4;
int MZS3 = 5;
int MZout = 6;*/

int MZredFrequency = 0;
int MZgreenFrequency = 0;
int MZblueFrequency = 0;
//////////////////////////////////////////////////////////////////////////////////////////////////////////////
char MZcurentDerection = "r" //at the green squre
int MZsteps = 0;//used to traverse one edge of squre
int MZextraSteps = 0;// used to check the line (for extra length)
char MZmode;//mode of line
char MZjunctionType;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////

const int ProxSensor_0=A0;//right
const int ProxSensor_1=A1;
const int ProxSensor_2=A2;
const int ProxSensor_3=A3;
const int ProxSensor_4=A4;
const int ProxSensor_5=A5;
const int ProxSensor_6=A6;
const int ProxSensor_7=A7;
const int ProxSensor_8=A8;
const int ProxSensor_9=A9;
const int ProxSensor_10=A10;
const int ProxSensor_11=A11;

int inputVal[12] = {0,0,0,0,0,0,0,0,0,0,0,0};//stores line sensors values
int LFSensor[12]={0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
float direct;

//float distance1,distance2,distance3,distance4,distance5,distance6,distance7,fronterrorD1,fronterrorD2,backerrorD1,backerrorD2,errorD1,errorD2,derivative1,derivative2,integral1=0.0,integral2=0.0,previousErrorD1=0.0,previousErrorD2=0.0,outputD1,outputD2,MIN_SPEED=40;
//float Kp_d=0.9,Ki_d=0,Kd_d=0.9;
//int speedL,speedR,basespeed=110;



float wallSensorReading[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};
float wallSensors[8] = {0.0,0.0,0.0,0.0,0.0,0.0,0.0};

//sensor 
const int trigPin1 = 4;//distance1
const int echoPin1 = 5;
const int trigPin2 = 26;//distance2
const int echoPin2 = 27;
const int trigPin3 = 14;//distance3
const int echoPin3 = 15;
const int trigPin4 = 16;//distance4
const int echoPin4 = 17;
const int trigPin5 = 22;//distance5
const int echoPin5 = 23;
const int trigPin6 = 22;//distance6/////////////////////////////pins need to change
const int echoPin6 = 23;
const int trigPin7 = 22;//distance7/////////////////////////////pins need to change
const int echoPin7 = 23;

NewPing sonar1(trigPin1, echoPin1, MAX_DISTANCE);
NewPing sonar2(trigPin2, echoPin2, MAX_DISTANCE);
NewPing sonar3(trigPin3, echoPin3, MAX_DISTANCE);
NewPing sonar4(trigPin4, echoPin4, MAX_DISTANCE);
NewPing sonar5(trigPin5, echoPin5, MAX_DISTANCE);
NewPing sonar6(trigPin6, echoPin6, MAX_DISTANCE);
NewPing sonar7(trigPin7, echoPin7, MAX_DISTANCE);

// Create an instance of the HMC5883L sensor object
//Adafruit_HMC5883L mag = Adafruit_HMC5883L();


// Motor A connections
int enA = 3;
int in1 = A0;
int in2 = A1;
// Motor B connections
int enB = 10;
int in3 = A3;
int in4 = A2;

//Function for sorting an array
void sort(int a[], int len) {
  for (int i = 0; i < (len - 1); i++) {
    bool flag = true;
    for (int o = 0; o < (len - (i + 1)); o++) {
      if (a[o] > a[o + 1]) {
        int t = a[o];
        a[o] = a[o + 1];
        a[o + 1] = t;
        flag = false;
      }
    }
    if (flag)break;
  }
}

void motorStop(){
   //stop the wheels
  //analogWrite(enA, x);
  //analogWrite(enB, y);
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
  delay(700);

}

void go(int x, int y) {   //right-x left-y
  
  //go forward in a curved path
  analogWrite(enA, x);
  analogWrite(enB, y);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW) ;
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW) ;

  if(x>y){
    Serial.println("Left");
  }
  else if(x<y){
    Serial.println("Right");
  }
  else{
    Serial.println("Forward");
  }
 //delay(200);//take a decision to use or not delays
}

void takeLineSensors(){
  int tempInputValue_0[5] = {10,10,10,10,10};
  int tempInputValue_1[5] = {10,10,10,10,10};
  int tempInputValue_2[5] = {10,10,10,10,10};
  int tempInputValue_3[5] = {10,10,10,10,10};
  int tempInputValue_4[5] = {10,10,10,10,10};
  int tempInputValue_5[5] = {10,10,10,10,10};
  int tempInputValue_6[5] = {10,10,10,10,10};
  int tempInputValue_7[5] = {10,10,10,10,10};
  int tempInputValue_8[5] = {10,10,10,10,10};
  int tempInputValue_9[5] = {10,10,10,10,10};
  int tempInputValue_10[5] = {10,10,10,10,10};
  int tempInputValue_11[5] = {10,10,10,10,10};

  for (int i = 0 ; i < 5 ; i++) {
    tempInputValue_0[i] = digitalRead(ProxSensor_0);
    tempInputValue_1[i] = digitalRead(ProxSensor_1);
    tempInputValue_2[i] = digitalRead(ProxSensor_2);
    tempInputValue_3[i] = digitalRead(ProxSensor_3);
    tempInputValue_4[i] = digitalRead(ProxSensor_4);
    tempInputValue_5[i] = digitalRead(ProxSensor_5);
    tempInputValue_6[i] = digitalRead(ProxSensor_6);
    tempInputValue_7[i] = digitalRead(ProxSensor_7);
    tempInputValue_8[i] = digitalRead(ProxSensor_8);
    tempInputValue_9[i] = digitalRead(ProxSensor_9);
    tempInputValue_10[i] = digitalRead(ProxSensor_10);
    tempInputValue_11[i] = digitalRead(ProxSensor_11);
  }
  sort(tempInputValue_0,5);
  sort(tempInputValue_1,5);
  sort(tempInputValue_2,5);
  sort(tempInputValue_3,5);
  sort(tempInputValue_4,5);
  sort(tempInputValue_5,5);
  sort(tempInputValue_6,5);
  sort(tempInputValue_7,5);
  sort(tempInputValue_8,5);
  sort(tempInputValue_9,5);
  sort(tempInputValue_10,5);
  sort(tempInputValue_11,5);

  inputVal[0] = tempInputValue_0[2];
  inputVal[1] = tempInputValue_1[2];
  inputVal[2] = tempInputValue_2[2];
  inputVal[3] = tempInputValue_3[2];
  inputVal[4] = tempInputValue_4[2];
  inputVal[5] = tempInputValue_5[2];
  inputVal[6] = tempInputValue_6[2];
  inputVal[7] = tempInputValue_7[2];
  inputVal[8] = tempInputValue_8[2];
  inputVal[9] = tempInputValue_9[2];
  inputVal[10] = tempInputValue_10[2];
  inputVal[11] = tempInputValue_11[2];
  
}


void setup() 
{    
  Serial.begin(9600);           

  
  pinMode(ProxSensor_0,INPUT);//right line sensors
  pinMode(ProxSensor_1,INPUT);    
  pinMode(ProxSensor_2,INPUT);    
  pinMode(ProxSensor_3,INPUT);    
  pinMode(ProxSensor_4,INPUT);
  pinMode(ProxSensor_5,INPUT);
  pinMode(ProxSensor_6,INPUT);
  pinMode(ProxSensor_7,INPUT);
  pinMode(ProxSensor_8,INPUT);
  pinMode(ProxSensor_9,INPUT);
  pinMode(ProxSensor_10,INPUT);
  pinMode(ProxSensor_11,INPUT);
  

  // Set all the motor control pins to outputs
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  // color sensor
  pinMode(MZS0, OUTPUT);
  pinMode(MZS1, OUTPUT);
  pinMode(MZS2, OUTPUT);
  pinMode(MZS3, OUTPUT);
  pinMode(MZout, INPUT);
  
  digitalWrite(MZS0, HIGH);
  digitalWrite(MZS1, HIGH);
  //////////////////////////////////

  int count = 0;
  
}

void loop()
{
  takeLineSensors();
  settleLine();
  delay(10000);
  
}


void forward(int lSpeed,int rSpeed){
  analogWrite(enA, lSpeed);
  analogWrite(enB, rSpeed);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW) ;
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW) ;
}

void turnLeft(){
  analogWrite(enA, 130);
  analogWrite(enB, 130);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW) ;
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH) ;  
}

void turnRight(){
  analogWrite(enA, 130);
  analogWrite(enB, 130);
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH) ;
  digitalWrite(in3, HIGH);
  digitalWrite(in4, LOW) ;  
}
/////////////////////////////////////////////////////////////////////////////////////////////////
void settleLine(){
  int MazeError=1;
  while(!(MazeError==0)){
  takeLineSensors();
   if ((inputVal[6]==0)&&(inputVal[7]==0)){
       MazeError=0;
       break;
   }
  MazeError = -200*inputVal[0]-200*inputVal[1]-200*inputVal[2]-200*inputVal[3]-200*inputVal[4]+200*inputVal[7]+200*inputVal[8]+200*inputVal[9]+200*inputVal[10]+200*inputVal[11];
  
  // if eror is +, take as robot has turn left(rigt sensors on the line)
  // considerd when line goes toward the right the eror as positive (robot going out from the line to left)
  // for positive error robot need to turn right  
  // for negative error robot need to turn left
    
  if (!((inputVal[6]==0)&&(inputVal[7]==0))){
    //make adjusments to settle on the line
    if (MazeError>0){
      turnRight(); 
  }
    if (MazeError<0){
      turnLeft(); 
  }
}
}
}
////////////////////////////////////////////////////////////////////////////////////////////////////////

void MZupdateMode(){
  takeLineSensors();
  
  // line detected inputValue[i]==0.
  
  if((inputVal[0]==0)&&(inputVal[1]==0)&&(inputVal[2]==0)&&(inputVal[3]==0)&&(inputVal[4]==0)&&(inputVal[5]==0)&&(inputVal[6]==0)&&(inputVal[7]==0)&&(inputVal[8]==0)&&(inputVal[9]==0)&&(inputVal[10]==0)&&(inputVal[11]==0)){
    MZmode = "C";//cross line
  }
  else if((inputVal[0]==1)&&(inputVal[1]==1)&&(inputVal[2]==1)&&(inputVal[3]==1)&&(inputVal[4]==1)&&(inputVal[5]==1)&&(inputVal[6]==1)&&(inputVal[7]==1)&&(inputVal[8]==1)&&(inputVal[9]==1)&&(inputVal[10]==1)&&(inputVal[11]==1)){
    MZmode = "N";//No line
  }
  else if((inputVal[0]==0)&&(inputVal[1]==0)&&(inputVal[10]==1)&&(inputVal[11]==1)){
    MZmode = "R";//Right line Turn (w r t robot)
  }
  else if((inputVal[0]==1)&&(inputVal[1]==1)&&(inputVal[10]==0)&&(inputVal[11]==0)){
    MZmode = "L";//Left line Turn (w r t robot)
  }
  else{
    MZmode = "F";//following line
  }

}

void MZgoLeft(){
  //first turn left
  //go untill steps are finished
   MZturn("l");
   int i=0;
   while(i<MZsteps){//MZsteps should give
    //linefolloow();
    i++;
   }
   motorStop();
  
}

void MZgoRight(){
   MZturn("r");
   int i=0;
   while(i<MZsteps){//MZsteps should give
    //linefolloow();
    i++;
   }
   motorStop();
  
}
void MZgoUp(){
   MZturn("u");
   int i=0;
   while(i<MZsteps){//MZsteps should give
    //linefolloow();
    i++;
   }
   motorStop();
  
}
void MZgoDown(){
   MZturn("d");
   int i=0;
   while(i<MZsteps){//MZsteps should give
    //linefolloow();
    i++;
   }
   motorStop();
  
}

void MZturn(char MZdere){ //'l' for left, 'r' for right, 'u' for up, 'd' for down.
  // derections with respect to the magnatometer on the green squre
  if(!(MZdere == MZcurentDerection)){
    
  
  if (MZcurentDerection == "r"){
    if (MZdere == "u"){
      MZturnDegrees(270);
    }
    if (MZdere == "d"){
      MZturnDegrees(90);
    }
    if (MZdere == "l"){
      MZturnDegrees(180);
    }
  }

  if (MZcurentDerection == "l"){
    if (MZdere == "u"){
      MZturnDegrees(90);
    }
    if (MZdere == "d"){
      MZturnDegrees(270);
    }
    if (MZdere == "r"){
      MZturnDegrees(180);
    }
  }

  if (MZcurentDerection == "u"){
    if (MZdere == "l"){
      MZturnDegrees(270);
    }
    if (MZdere == "r"){
      MZturnDegrees(90);
    }
    if (MZdere == "d"){
      MZturnDegrees(180);
    }
  }

  if (MZcurentDerection == "d"){
    if (MZdere == "r"){
      MZturnDegrees(270);
    }
    if (MZdere == "l"){
      MZturnDegrees(90);
    }
    if (MZdere == "u"){
      MZturnDegrees(180);
    }
  }

     
  
}
else{
  delay(10);
}
}

void MZupdateDerection(int MZnewderection){
  if(!(MZcurentDerection == MZnewderection)){
    
  
  if (MZcurentDerection == "r"){
    if (MZnewderection == 90){
      MZcurentDerection = "d";
    }
    if (MZnewderection == 180){
      MZcurentDerection = "l";
    }
     if (MZnewderection == 270){
      MZcurentDerection = "u";
    }
    
  }

   if (MZcurentDerection == "l"){
    if (MZnewderection == 90){
      MZcurentDerection = "u";
    }
    if (MZnewderection == 180){
      MZcurentDerection = "r";
    }
     if (MZnewderection == 270){
      MZcurentDerection = "d";
    }
    
  }

    if (MZcurentDerection == "u"){
    if (MZnewderection == 90){
      MZcurentDerection = "r";
    }
    if (MZnewderection == 180){
      MZcurentDerection = "d";
    }
     if (MZnewderection == 270){
      MZcurentDerection = "l";
    }
    
  }

    if (MZcurentDerection == "d"){
    if (MZnewderection == 90){
      MZcurentDerection = "l";
    }
    if (MZnewderection == 180){
      MZcurentDerection = "u";
    }
     if (MZnewderection == 270){
      MZcurentDerection = "r";
    }
    
  }
  else{
    delay(10);
  }
}
}

void MZflorColor(){    // will give the color of the floor (as frequency)

  //use if conditios to detect color
  digitalWrite(MZS2, LOW);
  digitalWrite(MZS3, LOW);
  delay(50);
  
  MZredFrequency = pulseIn(MZout, LOW);
  
  digitalWrite(MZS2, HIGH);
  digitalWrite(MZS3, HIGH);
  delay(50);
  
  MZgreenFrequency = pulseIn(MZout, LOW);
  
  digitalWrite(MZS2, LOW);
  digitalWrite(MZS3, HIGH);
  delay(50);
  
  MZblueFrequency = pulseIn(MZout, LOW);
}

void MZturnDegrees(int MZangle){
  float MZmagreading = 0.0;
  float MZturningAngle = 0.0;
  bool MZturned = false;
  MZmagreading = readMagntoMeter();
  
  if (MZangle == 90){//right
    while(!(MZturned)){
    turnRight();
    MZturningAngle = readMagntoMeter();
    if((abs((MZturningAngle)-(MZmagreading)))>MZangle){
      MZturned = true;
      motorStop();
      settleLine();
      MZupdateDerection(90);
    }
    }
  }

    if (MZangle == 270){//left
    while(!(MZturned)){
    turnLeftt();
    MZturningAngle = readMagntoMeter();
    if((abs((MZturningAngle)-(MZmagreading)))>90){
      MZturned = true;
      motorStop();
      settleLine();
      MZupdateDerection(270);
    }
    }
  }

    if (MZangle == 180){//left can take right as well
    while(!(MZturned)){
    turnLeftt();
    MZturningAngle = readMagntoMeter();
    if((abs((MZturningAngle)-(MZmagreading)))>180){
      MZturned = true;
      motorStop();
      settleLine();
      MZupdateDerection(180);
    }
    }
  }
  
}

void checkLine(){
  // read line sensor values detect wether cross(P),T or just line
  
  int j=0;
  while(MZextraSteps>j){
    forward();
    j++;
  }
  char MZpreviousMode= MZmode;
  
  MZupdateMode();
  if((MZpreviousMode=="C")&&(MZmode=="F")){
    MZjunctionType="P";//cross junction
  }
  if((MZpreviousMode=="C")&&(MZmode=="N")){
    MZjunctionType="T";//T junction
  }
  if((MZpreviousMode=="R")&&(MZmode=="N")){
    MZjunctionType="D";//just passed right turn immideately take it///////////////////////////////////////////////////
  }
  if((MZpreviousMode=="L")&&(MZmode=="N")){
    MZjunctionType="W";//just passed left turn immideately take it////////////////////////////////////////////////////
  }
  
  
  
  
}
void readMagntoMeter(){
 
  
}
