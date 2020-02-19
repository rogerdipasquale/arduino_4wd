#include <Wire.h>

#include <Servo.h>
Servo myservo;
int enableA = 1;
int pinA1 = 3;
int pinA2 = 2;
int obstaculo = 10;

int servposnum = 0;
int servpos = 0;

int enableB = 6;
int pinB1 = 5;
int pinB2 = 4;
#define trigPin 6
#define echoPin 7



//define L298n module IO Pin
#define ENA 0
#define ENB 5
#define IN1 13
#define IN2 2
#define IN3 3
#define IN4 4

 
void forward(){ 
  digitalWrite(ENA,HIGH); //enable L298n A channel
  digitalWrite(ENB,HIGH); //enable L298n B channel
  digitalWrite(IN1,LOW); //set IN1 high level - no pasa nada
  digitalWrite(IN2,HIGH);  //set IN2 low level -- Trasera derecha y delantera izquierda en low - camina
  digitalWrite(IN3,HIGH);  //set IN3 low level
  digitalWrite(IN4,LOW); //set IN4 high level
  Serial.println("Forward");//send message to serial monitor
}
 
void back(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH);
  Serial.println("Back");
}
 
void left(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1,LOW);
  digitalWrite(IN2,HIGH);
  digitalWrite(IN3,LOW);
  digitalWrite(IN4,HIGH); 
  Serial.println("Left");
}
 
void right(){
  digitalWrite(ENA,HIGH);
  digitalWrite(ENB,HIGH);
  digitalWrite(IN1,HIGH);
  digitalWrite(IN2,LOW);
  digitalWrite(IN3,HIGH);
  digitalWrite(IN4,LOW);
  Serial.println("Right");
}

void stop() {
   digitalWrite(ENA,HIGH);
   digitalWrite(ENB,HIGH);
   digitalWrite(IN1,LOW);
   digitalWrite(IN2,LOW);
   digitalWrite(IN3,LOW);
   digitalWrite(IN4,LOW);
  Serial.println("Stop!");
  
}

void setup() {
  // put your setup code here, to run once:
  //configure pin modes for the drive motors
 /* lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("NORF V 0.1" );
  lcd.setCursor(0,1);
  lcd.print("Autopilot mode" );*/
    Serial.begin(9600);//open serial and set the baudrate

   pinMode (enableA, OUTPUT);
   pinMode (pinA1, OUTPUT);
   pinMode (pinA2, OUTPUT);

   pinMode (enableB, OUTPUT);
   pinMode (pinB1, OUTPUT);
   pinMode (pinB2, OUTPUT); 

   //configure pin modes for the ultrasonci se3nsor
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);

   pinMode(obstaculo, INPUT);

   //Servo pins
   myservo.attach(9);
}

void loop() {
  // put your main code here, to run repeatedly:

 /*    car(); */
/*     avoid();
  */ 
  Serial.println("chequeo distancia");
  int distance_0;
  distance_0 = distance();

  Serial.println( distance_0); 
  
 /* myservo.write(distance_0);*/                  // sets the servo position according to the scaled value
  /*myservo.write(0);
  delay(1000);  
  myservo.write(90);
  delay(1000);  
*/
  if (distance_0 < 20) {
     stop();
     delay(500);
     right();
     delay(500);
    }
   else forward();


}

//Defining functions so it's more easy
//motor functions
void motorAforward() {
 digitalWrite (pinA1, HIGH);
 digitalWrite (pinA2, LOW);
}
void motorBforward() {
 digitalWrite (pinB1, LOW);
 digitalWrite (pinB2, HIGH);
}
void motorAbackward() {
 digitalWrite (pinA1, LOW);
 digitalWrite (pinA2, HIGH);
}
void motorBbackward() {
 digitalWrite (pinB1, HIGH);
 digitalWrite (pinB2, LOW);
}
void motorAstop() {
 digitalWrite (pinA1, HIGH);
 digitalWrite (pinA2, HIGH);
}
void motorBstop() {
 digitalWrite (pinB1, HIGH);
 digitalWrite (pinB2, HIGH);
}
void motorAcoast() {
 digitalWrite (pinA1, LOW);
 digitalWrite (pinA2, LOW);
}
void motorBcoast() {
 digitalWrite (pinB1, LOW);
 digitalWrite (pinB2, LOW);
}
void motorAon() {
 digitalWrite (enableA, HIGH);
}
void motorBon() {
 digitalWrite (enableB, HIGH);
}
void motorAoff() {
 digitalWrite (enableA, LOW);
}
void motorBoff() {
 digitalWrite (enableB, LOW);
}
// Movement functions
void forward (int duration) {
 motorAforward();
 motorBforward();
 delay (duration);
}
void backward (int duration) {
 motorAbackward();
 motorBbackward();
 delay (duration);
}
void right (int duration) {
 motorAbackward();
 motorBforward();
 delay (duration);
}
void left (int duration) {
 motorAforward();
 motorBbackward();
 delay (duration);
}
void coast (int duration) {
 motorAcoast();
 motorBcoast();
 delay (duration);
}
void breakRobot (int duration) {
 motorAstop();
 motorBstop();
 delay (duration);
}
void disableMotors() {
 motorAoff();
 motorBoff();
}
void enableMotors() {
 motorAon();
 motorBon();
}

int distance() {
  int duration, distance;
  String dato, distancia;
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(500);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;
  distancia=String(distance);
  if (distance > 999) dato = "999";
  if (distance < 100) dato = "0" + distancia;
  if (distance < 10) dato = "00" + distancia;
 /* lcd.setCursor(11,0);
  lcd.print("D=" );
  lcd.setCursor(13,0);
  lcd.print(dato); */
  return distance;
}


void car() {
int distance_0;
distance_0 = distance();
  while(distance_0 > 30)
  {
     
     if(servposnum == 0)
     {
      myservo.writeMicroseconds (1000);
      servposnum = 1;
      delay(110);
      distance_0 = distance();
////      obstaculator();     
     }
     else if(servposnum == 1)
     {
      myservo.writeMicroseconds (1500);
      distance_0 = distance();
      servposnum = 2;
     delay(110);
     }
     else if(servposnum  == 2)
     {
      myservo.writeMicroseconds (2000);
      distance_0 = distance();
      servposnum = 3;
      delay(110);
     }
     else if(servposnum == 3)
     {
      myservo.writeMicroseconds (1500);
      distance_0 = distance();
      servposnum = 0 ;
      delay(110);
     }
     motorAon();
     motorBon();
     forward(1);    
     distance_0 = distance();
  }
  breakRobot(150);

}
void avoid()
{
    backward(300);
    right(360);
}


void obstaculator()
{
int distance_0;
distance_0 = distance();

int obs;
String msg;
  while(distance_0 > 1)
  {
/* lcd.setCursor(11,1); */
obs = digitalRead(obstaculo);
  if (obs == HIGH)
   {
  Serial.println("BK=SI" );
  }
  else
   {
      Serial.println("BK=NO");

 /* lcd.print("BK=NO" ); */
    }
  delay(100);
  }
return;
  }
