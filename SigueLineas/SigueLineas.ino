// Este codigo lee el control remoto IR y setea modos.
// Presionando 8, activo o desactivo el modo "Line tracking". 
// Si el modo "LT" esta prendido, sigue una linea negra en un fondo blanco. En realidad, utiliza solo dos lectores,
// por lo que si toca una linea negra, gira hacia el lado opuesto para mantenerlo siguiendo esa linea.
// Con el modo desactivado, se queda quieto.
// Este el modo o no, con:
// 1: gira a la izquierda
// 2: avanza
// 3: gira a la deracha
// 5: retrocede


//    The direction of the car's movement
//  ENA   ENB   IN1   IN2   IN3   IN4   Description  
//  HIGH  HIGH  HIGH  LOW   LOW   HIGH  Car is runing forward
//  HIGH  HIGH  LOW   HIGH  HIGH  LOW   Car is runing back
//  HIGH  HIGH  LOW   HIGH  LOW   HIGH  Car is turning left
//  HIGH  HIGH  HIGH  LOW   HIGH  LOW   Car is turning right
//  HIGH  HIGH  LOW   LOW   LOW   LOW   Car is stoped
//  HIGH  HIGH  HIGH  HIGH  HIGH  HIGH  Car is stoped
//  LOW   LOW   N/A   N/A   N/A   N/A   Car is stoped
 
// Control remoto infrarrojo
#include <IRremote.h>
 
#define RECV_PIN  12        //Infrared signal receiving pin
#define L         16724175 // Boton 1
#define F         16718055 // Boton 2
#define R         16743045 // Boton 3
#define B         4294967295 // Boton 5
#define UNKNOWN_B 16726215 // Otro valor del botón 5
// 16730805 // Boton 8
#define UNKNOWN_L 1386468383

#define BOTON_PLAY 16761405
#define BOTON_PLAY_2 2380149631
#define BOTON_PLAY_3 4294967295


unsigned long val; 
IRrecv irrecv(RECV_PIN);   //initialization
decode_results results;   //Define structure type
unsigned int modoSeguimientoLinea;

//Line Tracking 
#define LT_R !digitalRead(10)

#define LT_L !digitalRead(11)


// Control de distancia
#define trigPin 6
#define echoPin 7

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


 
//define L298n module IO Pin
#define ENA 0
#define ENB 5
#define IN1 13
#define IN2 2
#define IN3 3
#define IN4 4


// Motor A hacia adelante es 1L2H (ruedas derechas)
// Motor B hacia adelante es 3H4L (ruedas izquierdas)

 
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
 
//before execute loop() function, 
//setup() function will execute first and only execute once
void setup() {
  Serial.begin(9600);//open serial and set the baudrate
  pinMode(IN1,OUTPUT);//before useing io pin, pin mode must be set first 
  pinMode(IN2,OUTPUT);
  pinMode(IN3,OUTPUT);
  pinMode(IN4,OUTPUT);
  pinMode(ENA,OUTPUT);
  pinMode(ENB,OUTPUT);
   // Pins del control de distancia
   pinMode(trigPin, OUTPUT);
   pinMode(echoPin, INPUT);

  //Seteo los pines del motor en LOW
  digitalWrite(ENA,LOW); //set IN1 hight level
  digitalWrite(IN1,LOW); //set IN1 hight level
  digitalWrite(IN2,LOW); //set IN1 hight level
  digitalWrite(IN3,LOW); //set IN1 hight level
  digitalWrite(IN4,LOW); //set IN1 hight level
  digitalWrite(ENB,LOW); //set IN1 hight level

  irrecv.enableIRIn();  // Start receiving control IR

  pinMode(LT_R,INPUT);
  pinMode(LT_L,INPUT);
  modoSeguimientoLinea=LOW;
}
 
//Repeat execution
void a() {

  int distance_0;
  distance_0 = distance();
  if (distance_0 < 20) {
    stop();
  }
  else {
    forward();
  }
  delay(200);
}

void loop() {
  if (irrecv.decode(&results)) { 
    // Si recibo señal del control remoto, activo.
    Serial.println(results.value);//: The actual IR code (0 if type is UNKNOWN)


// Imprimo los valores de los lectores de linea
    Serial.println("L");
    Serial.println( LT_L);
    Serial.println("R");
    Serial.println( LT_R);

    val = results.value;
    Serial.println("VAL");
    Serial.println(val);

    if (val == L) left();
    if (val == R) right();
    if (val == F) forward();
    if (val == B || val == UNKNOWN_B) back();

    if (val == BOTON_PLAY || val == BOTON_PLAY_2 || val == BOTON_PLAY_3 )
       modoSeguimientoLinea = !modoSeguimientoLinea;
    else
      delay(200);
    stop();
    irrecv.resume();      // Receive the next value
  }
  if (modoSeguimientoLinea) {
    if (LT_L && LT_R) {
      forward();
    }
    else {
      if (! (LT_L || LT_R) ) {
        stop();
      }
      else 
        if (LT_L)
          right();
        else
          left();
    }
     delay(20);
  }
}

void prueboElAuto() {
  forward();
  delay(200);//delay 1000 ms
  back();
  delay(200);//delay 1000 ms
right();
delay(200);
left();
delay(200);
stop();
delay(1000);
}


void p() {
  digitalWrite(ENA,LOW); //set IN1 hight level
  digitalWrite(IN1,LOW); //set IN1 hight level
  digitalWrite(IN2,LOW); //set IN1 hight level
  digitalWrite(IN3,LOW); //set IN1 hight level
  digitalWrite(IN4,LOW); //set IN1 hight level
  digitalWrite(ENB,LOW); //set IN1 hight level
delay(500);
 digitalWrite(ENA,LOW); //enable L298n A channel
  digitalWrite(ENB,LOW); //enable L298n B channel
  digitalWrite(IN1,HIGH); //set IN1 hight level
  digitalWrite(IN2,HIGH);  //set IN2 low level
  digitalWrite(IN3,HIGH);  //set IN3 low level
  digitalWrite(IN4,HIGH); //set IN4 hight level
  
  }
