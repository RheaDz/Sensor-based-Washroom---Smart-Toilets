//for flushing mechanism
#include <Servo.h>   
Servo myservo;     
int flush_trig = 7;    
int flush_echo = 8;   
int servo = 11;
long flush_duration, flush_distance;   
long a[3]; 

//for hand wash mechanism
const int handwash_trigPin = 9;
const int handwash_echoPin = 10;
const int handwash_relay = 5;
const int dirtywater_relay = 6;
long handwash_duration;
int handwash_distanceCm;

//for water level monitoring
int state = 0;
const int flush_tank_moisture_sensor = A2;
const int handwash_tank_moisture_sensor = A1;

void setup() {
//for flushing mechanism
myservo.attach(servo);  
pinMode(flush_trig, OUTPUT);  
pinMode(flush_echo, INPUT);  
myservo.write(0);   // servo position 0 degree      
delay(1000);
myservo.detach();

//for handwash mechanism
pinMode(handwash_trigPin, OUTPUT);
pinMode(handwash_echoPin, INPUT);
pinMode(handwash_relay, OUTPUT);
pinMode(dirtywater_relay, OUTPUT);
digitalWrite(handwash_relay, LOW);
digitalWrite(dirtywater_relay, LOW);
    
Serial.begin(9600); 
}

void measure() 
{  
digitalWrite(flush_trig, LOW);
delayMicroseconds(6);
digitalWrite(flush_trig, HIGH);
delayMicroseconds(15);
digitalWrite(flush_trig, LOW);
pinMode(flush_echo, INPUT);
flush_duration = pulseIn(flush_echo, HIGH);
flush_distance = (flush_duration/2) / 29.1;    
}


void SendSMS()
{
  Serial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  Serial.println("AT+CMGS=\"+919422667850\"\r"); //Change to destination phone number
  delay(1000);
  Serial.println("Low Water Level in Tank");//the content of the message
  delay(200);
  Serial.println((char)26); //the stopping character Ctrl+Z
  delay(10000);  
}

void loop() {
//the flushing mechanism  
for (int i=0;i<=2;i++) {   
    measure();               
   a[i]=flush_distance;            
    delay(50);              
  }
 flush_distance=(a[0]+a[1]+a[2])/3;
 Serial.print("Flush distance= ");
  Serial.println(flush_distance);   

if ( flush_distance<10 ) // change this value to increase or decrease detection range.
{
 myservo.attach(servo);
  delay(1);
 myservo.write(90);  //servo position 90 degree
 delay(2000);       
 myservo.write(0);    //servo position 0 defree
 delay(1000);
 myservo.detach();      
}

//the hand wash mechanism  
digitalWrite(handwash_trigPin, LOW);
delayMicroseconds(2);
digitalWrite(handwash_trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(handwash_trigPin, LOW);
handwash_duration = pulseIn(handwash_echoPin, HIGH);
handwash_distanceCm= handwash_duration*0.034/2;
delay(100);
//Serial.println(handwash_distanceCm);
if(handwash_distanceCm<=10){
  Serial.println("RELAY 1 has started");
  digitalWrite(handwash_relay, LOW);
  delay(1000);
  Serial.println("RELAY 2 has started");
  digitalWrite(dirtywater_relay, LOW);
  delay(100);
  }
else{
  digitalWrite(handwash_relay, HIGH);
  delay(1000); 
  digitalWrite(dirtywater_relay, HIGH);
  delay(100);

//for water level monitoring
 float moisture_percentage_1;
 int sensor_analog_1;
 sensor_analog_1 = analogRead(flush_tank_moisture_sensor);
 moisture_percentage_1 = ( 100 - ( (sensor_analog_1/1023.00) * 100 ) );
 Serial.print("Moisture Percentage in Tank A= ");
 Serial.print(moisture_percentage_1);
 Serial.println("%");
 delay(1000);
 if(moisture_percentage_1 < 10)
   SendSMS();

 float moisture_percentage_2;
 int sensor_analog_2;
 sensor_analog_2 = analogRead(handwash_tank_moisture_sensor);
 moisture_percentage_2 = ( 100 - ( (sensor_analog_2/1023.00) * 100 ) );
 Serial.print("Moisture Percentage in Tank B= ");
 Serial.print(moisture_percentage_2);
 Serial.println("%");
 delay(1000);
 if(moisture_percentage_2 < 10)
   SendSMS();
}
}
