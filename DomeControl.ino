
///Automatische Domebewegung
///
///Funktion über Schalter geht nur wenn RC aux nicht über 800
///Überschalter ist RC Mode Langsam ?



long randNumber;
long zeit;
long moving;

///Motor L298 Anschluss
int links = 6; //Pin 6
int rechts = 5; //Pin 5
int tempo = 255;
int durchlauf = 0;
int sensorValue = 1500;
int sensorCenter = 16;
int sensorMode = 10;
int sensorRC = 2;
int sensorRCaux = 14;
int Mode = 0;// 0=RandMove // 1=RCMove

const int ledPin1 =  21;  
const int ledPin2 =  20;  

int ledState = LOW; 


int randomMove() {
 
   // print a random number from 0 to 299
  zeit = random(2000, 6000);
  // print a random number from 10 to 19
  randNumber = random(10, 40);
  //Speed
  tempo = random(100,160);
  //Moving länge
  moving = random(500,1500);

 
 
  if (randNumber < 20){
    Serial.println("Links");
     Serial.println(randNumber);
     // set the LED with the ledState of the variable:
     digitalWrite(ledPin2, HIGH);

    //Drehung Links
     analogWrite(links, tempo);  
     analogWrite(rechts, 0); 
     delay(moving);
     digitalWrite(ledPin2, LOW);
    
    } else if (randNumber > 20 && randNumber <= 30) {
      Serial.println("rechts");
      Serial.println(randNumber);
     // set the LED with the ledState of the variable:
      digitalWrite(ledPin1, HIGH); 
      analogWrite(links, 0);  
      analogWrite(rechts, tempo); 
      delay(moving);      
      digitalWrite(ledPin1, LOW); 
    }  else {
      analogWrite(links, 0);  
      analogWrite(rechts, 0); 
      
      delay (zeit);
      
    }
  
  Serial.print("Zeit ");
  Serial.println(zeit);
  Serial.print("Druchlauf ");
  Serial.println(durchlauf);

 
  
}



int rcMove() {

    int sensorValue = pulseIn(sensorRC,HIGH);
    if (sensorValue < 1400){
    Serial.println("Links");
          // set the LED with the ledState of the variable:
     digitalWrite(ledPin2, HIGH);
     tempo = sensorValue /4;
     tempo = tempo *-1; 
     tempo = tempo +500;

    //Drehung Links
     analogWrite(links, tempo);  
     analogWrite(rechts, 0); 
     
    
    } else if (sensorValue > 1500) {
      Serial.println("rechts");
     
     // set the LED with the ledState of the variable:
      digitalWrite(ledPin1, HIGH); 
      tempo = sensorValue /4;
      //tempo = tempo /5;
      
      analogWrite(links, 0);  
      analogWrite(rechts, tempo); 
     
    }  else {
      analogWrite(links, 0);  
      digitalWrite(ledPin1, LOW); 
      analogWrite(rechts, 0); 
      digitalWrite(ledPin2, LOW);
      
      //delay (zeit);
      
    }

  Serial.print("Value ");
  Serial.println(tempo);
  
  
}

int center() {
   Serial.print("CenterMode ");

    analogWrite(rechts, 0); 
    analogWrite(links, 0); 
  
   
   while ( digitalRead(sensorCenter) == 1){
    analogWrite(rechts, 200); 
    Serial.println("try to get center");
    Serial.print(sensorCenter);
    digitalWrite(ledPin1, HIGH); 
    
    
   }
   digitalWrite(ledPin1, LOW); 
   analogWrite(rechts, 0);  
   
   delay(3000);
   durchlauf = 0;
   Mode = 0;
   
   
}

void setup(){
  Serial.begin(9600);
  delay(3000);
  Serial.print("DomeControl.ino  11.11.2018");
   
  delay(3000);
  // set the digital pin as output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);

  
  
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generateB
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));

  
}

void loop() {

  int RCaux = pulseIn(sensorRCaux,HIGH);
  if (RCaux >=800){
    Serial.print("RC AUX ");
    Serial.println(RCaux);
      if (RCaux >= 1200){
      Mode = 0; 
      }
      if (RCaux <= 1200){
      Mode = 1; 
      }
  }

  if (Mode == 0){
    randomMove();
    durchlauf = durchlauf+1;
  }

  if (Mode == 1){
  rcMove();
  }
  
  Mode = digitalRead(sensorMode);
      
  Serial.print("Mode= ");
  Serial.println(Mode);
  

  if (durchlauf == 10) {
    center();
  }
  
  
  delay(1);
  
}





