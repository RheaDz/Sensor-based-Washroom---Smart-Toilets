//for door mechanism
#include <Servo.h>
Servo myservo;  // create servo object to control a servo

boolean toDisable1 = false;// to enable the outside IR sensor (irPin1) in the start
boolean toDisable2 = true;//to disable the inside IR sensor (irPin2) in the start
int irPin1=2;
int irPin2=4;

int count=0;//to count the number of people using the toilet
unsigned long startTime = 0;
unsigned long perUserTime = 0;
unsigned long totalUsageTime = 0;

//for garbage can
const int trigPin = 6;
const int echoPin = 7;
long duration;
int distanceCm;

void setup() {
Serial.begin(9600);

//for door mechanism
myservo.attach(5);  // attaches the servo on pin 9 to the servo object
myservo.write(0);// send servo to the middle at 90 degrees

pinMode(irPin1, INPUT);// assign pin 2 as input for first IR sensor
pinMode(irPin2, INPUT);// assign pin 4 as input for second IR sensor

pinMode(9, OUTPUT);// defind pin 10 as output
digitalWrite(9, HIGH);// turn the pin 10 LOW i.e. LED outside the toilet for VACANT

pinMode(10, OUTPUT);// defind pin 10 as output
digitalWrite(10, LOW);// turn the pin 10 LOW i.e. LED outside the toilet for OCCUPIED

pinMode(11, OUTPUT);// defind pin 10 as output
digitalWrite(11, LOW);// turn the pin 10 LOW i.e. LED inside the toilet foe indicating light bulb


//for garbage can
pinMode(trigPin, OUTPUT);
pinMode(echoPin, INPUT);

}

void loop() {

  //door mechanism 
  while(digitalRead(irPin1) == LOW && toDisable1 == false){  // to open and close the door for coming inside the toilet
    Serial.println("Entering the toilet");

    startTime = millis();

    digitalWrite(11, HIGH);// turn the pin 11 HIGH (give it 5v)i.e. bulb LED
    
    myservo.write(90);  //servo position 90 degree
    delay(4000);       
    myservo.write(0);    //servo position 0 defree
    delay(1000);

    digitalWrite(9, LOW);// turn the pin 9 LOW (give it 0v)i.e. vacant LED
    digitalWrite(10, HIGH);// turn the pin 10 HIGH (give it 5v)i.e. occupied LED
    
    
    toDisable1 = true;// to disable the outside IR sensor
    delay(5000);//keeping a five second delay before enabling the inside IR sensor

    toDisable2 = false;//to enable the inside IR sensor
  }

  while(digitalRead(irPin2) == LOW && toDisable2 == false){  // to open and close the door for coming out of thr toilet 
    count= count+1;
    
    Serial.println("Exiting the toilet");

    myservo.write(90);  //servo position 90 degree
    delay(4000);       
    myservo.write(0);    //servo position 0 defree
    delay(1000);     

    digitalWrite(9, HIGH);// turn the pin 9 HIGH (give it 5v)i.e. vacant LED
    digitalWrite(10, LOW);// turn th pin 10 Low (give it 0v)i.e. occupied LED
    digitalWrite(11, LOW);// turn th pin 11 Low (give it 0v)i.e. bulb LED

    delay(5000);//keeping a five second delay before enalbing the outside IR sensor
    toDisable1 = false;// to again enable the IR sensor outside the washroom

    toDisable2 = true;//to again disable the IR sensor inside the washroom
    
    Serial.print("Number of people that have used the toilet:");
    Serial.println(count);

    perUserTime = (millis() - startTime)/1000;
    Serial.print("User ");
    Serial.print(count);
    Serial.print(" usage time in seconds: ");
    Serial.println(perUserTime);
    
    totalUsageTime = totalUsageTime + perUserTime;
    Serial.print("Total Usage Time in seconds: ");
    Serial.println(totalUsageTime);
  }


  //for garbage can
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  distanceCm= duration*0.034/2;
  delay(100);
  if(distanceCm<=4){
  Serial.println("Garbage can is full");
  SendSMS();
  } 

}


void SendSMS()
{
  Serial.println("AT+CMGF=1");    //To send SMS in Text Mode
  delay(1000);
  Serial.println("AT+CMGS=\"+917972576561\"\r"); //Change to destination phone number
  delay(1000);
  Serial.println("Garbage can is full");//the content of the message
  delay(200);
  Serial.println((char)26); //the stopping character Ctrl+Z
  delay(10000);  
}
