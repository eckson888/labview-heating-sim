#include <dht11.h>
#define tempPin 7

// String command="";
// String string;
dht11 DHT11;

void setup() {
  Serial.begin(9600);
  pinMode(12,OUTPUT);    
  pinMode(11,OUTPUT);  
  pinMode(10,OUTPUT);
}

void loop() {    
  int chk = DHT11.read(tempPin);  

  Serial.println((float)DHT11.temperature, 2);
  
  while(Serial.available())
  {
    char inChar=(char)Serial.read();

    if(inChar=='0')
    {
      digitalWrite(12,LOW);
      digitalWrite(11,LOW);
      digitalWrite(10,LOW);
      
    }
    else if(inChar=='1')
    {
      digitalWrite(12,HIGH);
      digitalWrite(11,LOW);
      digitalWrite(10,LOW);
      
    }
    else if(inChar=='2')
    {
      digitalWrite(12,HIGH);
      digitalWrite(11,HIGH);
      digitalWrite(10,LOW);
    }
    else if(inChar=='3')
    {
      digitalWrite(12,HIGH);
      digitalWrite(11,HIGH);
      digitalWrite(10,HIGH);
    }
    else
    {
      digitalWrite(12,HIGH);
      digitalWrite(11,HIGH);
      digitalWrite(10,HIGH);
      delay(100);
      digitalWrite(12,LOW);
      digitalWrite(11,LOW);
      digitalWrite(10,LOW);
    }
  } 
  delay(500);  
}
