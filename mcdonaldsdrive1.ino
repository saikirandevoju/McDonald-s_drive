#include<Servo.h>
#include<IRremote.h>

int trigPin1=13;    //trigPin of us_1( for entering),greenWire
int echoPin1=12;    //echoPin of us_1,purpleWire
int trigPin2=10;    //trigPin of us_2( for leaving),greenWire
int echoPin2=11;    //echoPin of us_2,purpleWire
float pingTime1;    //pingTime for us_1
float pingTime2;    //pingTime for us_2
float distance1;    //measured distance by us_1
float distance2;    //measured distance by us_2
float speed=34300.0;    //speed of sound in cm/sec
int servoPin1=8;    //signalPin for servo_1(while entering),blueWire
int servoPin2=7;    //signalPin for servo_2(while leaving),blueWire
Servo servo_1;    //barricade while entering
Servo servo_2;    //barricade while leaving

int IRpin=5;    //setting IR controlling pin to 0
IRrecv myIR(IRpin);
decode_results result;    //storing input results in variable result

int redLED=6;
int blueLED=2;
int greenLED=3;
int yellowLED=4;

void setup()
{
  Serial.begin(9600);    //turn on serial monitor
  
  pinMode(trigPin1,OUTPUT);    //set trigPin1 as an output
  pinMode(echoPin1,INPUT);    //set echoPin1 as an input
  pinMode(trigPin2,OUTPUT);    //set trigPin2 as an output
  pinMode(echoPin2,INPUT);    //set echoPin2 as an input
  
  servo_1.attach(servoPin1);    //attach servoPin1 to servo_1
  servo_2.attach(servoPin2);    //attach servoPin2 to servo_2
  
  myIR.enableIRIn();    //enable IR sensor
  
  pinMode(redLED,OUTPUT);    //set redLEDpin as an output
  pinMode(blueLED,OUTPUT);    //set redLEDpin as an output
  pinMode(greenLED,OUTPUT);    //set redLEDpin as an output
  pinMode(yellowLED,OUTPUT);    //set redLEDpin as an output

}

void loop()
{
  servo_1.write(90);    //intially closing entering barricade
  servo_2.write(90);    //intially closing leaving barricade
  
  digitalWrite(trigPin1,LOW);    //sending low pulse(or sending nothing)
  delayMicroseconds(1000);    //wait for 1 millisec
  digitalWrite(trigPin1,HIGH);    //sending high pulse
  delay(10);    //wait for 10 millisec
  digitalWrite(trigPin1,LOW);    //again sending low pulse(or sending nothing)
  pingTime1=pulseIn(echoPin1,HIGH);    //echoPin recieves echo signal from object
  distance1=speed*(pingTime1/1000000.0);    //measuring travelling distance by pulse
  distance1=distance1/2.0;    //actual distance is half off measured distance
  
  if(distance1<100)    //if object found within 100cm open barricade
  {
    servo_1.write(0);    //open barricade
    delay(1000);    //open barricade for 1 sec
    servo_1.write(90);    //again close barricade
    
    Serial.println("Press the remote numbers as follows:");
    Serial.println("1 for A");
    Serial.println("2 for B");
    Serial.println("3 for C");
    Serial.println("4 for D");
    
    while(myIR.decode(&result)==0){}

    if(myIR.decode(&result))
    {

      if(result.value==0xFD08F7)
      {
        Serial.println("You choosed item A.");
        digitalWrite(redLED,HIGH);
        digitalWrite(blueLED,LOW);
        digitalWrite(greenLED,LOW);
        digitalWrite(yellowLED,LOW);
        delay(1000);    //make LED bright for 1 sec
        digitalWrite(redLED,LOW);
      }
      else if(result.value==0xFD8877)
      {
        Serial.println("You choosed item B.");
        digitalWrite(redLED,LOW);
        digitalWrite(blueLED,HIGH);
        digitalWrite(greenLED,LOW);
        digitalWrite(yellowLED,LOW);
        delay(1000);    //make LED bright for one sec
        digitalWrite(blueLED,LOW);
      }
      else if(result.value==0xFD48B7)
      {
        Serial.println("You choosed item C.");
        digitalWrite(redLED,LOW);
        digitalWrite(blueLED,LOW);
        digitalWrite(greenLED,HIGH);
        digitalWrite(yellowLED,LOW);
        delay(1000);    //make LED bright for one sec 
        digitalWrite(greenLED,LOW);
      }
      else if(result.value==0xFD28D7)
      {
        Serial.println("You choosed item D.");
        digitalWrite(redLED,LOW);
        digitalWrite(blueLED,LOW);
        digitalWrite(greenLED,LOW);
        digitalWrite(yellowLED,HIGH);
        delay(1000);    //make LED bright for one sec
        digitalWrite(yellowLED,LOW);
      }
      myIR.resume();
    }
    
    digitalWrite(trigPin2,LOW);    //sending low pulse(or sending nothing)
    delayMicroseconds(1000);    //wait for 1 millisec
    digitalWrite(trigPin2,HIGH);    //sending high pulse
    delay(10);    //wait for 10 millisec
    digitalWrite(trigPin2,LOW);    //again sending low pulse(or sending nothing)
    pingTime2=pulseIn(echoPin2,HIGH);    //echoPin recieves echo signal from object
    distance2=speed*(pingTime2/1000000.0);    //measuring travelling distance by pulse
    distance2=distance2/2.0;    //actual distance is half off measured distance
    if(distance2<100)    //if object found within 100cm open barricade
    {
      servo_2.write(0);    //open barricade
      delay(1000);    //open barricade for 2 sec
      servo_1.write(90);    //again close barricade
    }
  }
}