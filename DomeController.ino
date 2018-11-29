
///Automatische Domebewegung Steuerung
///Doc Snyder Tool Droid DomeController
///Funktion über Schalter geht nur wenn RC aux nicht über 800
///Über Schalter ist noch im Konflikt mit RC Auswahl

String data; //Consohlen Input
String VERSION = "28-11-18";

long randNumber;
long zeit;
long moving;

///Motor L298 Anschluss
// Mini  6  5
// Pro Micro 8 9
int links = 8; //Pin 6
int rechts = 9; //Pin 5
int tempo = 255;
int durchlauf = 0;
int sensorValue = 1500;
int sensorCenter = 16;  // Sensor für Center Position
int sensorMode = 10;    // Schalter am Board für Modus
int sensorRC = 2;       // RC Steuerung
int sensorRCaux = 14;   // Anschluss zum Schalten der Modi
int RCaux = 0;
boolean debug = false;
int Mode = 0;// 0=RandMove // 1=RCMove

const int ledPin1 =  19;  
const int ledPin2 =  18;  

int ledState = LOW; 

void setup(){
  Serial.begin(9600);
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
  
  RCaux = pulseIn(sensorRCaux,HIGH);
  
  if (RCaux >=800){
    if (debug) {
    Serial.print("RC AUX ");
    Serial.println(RCaux);
    }
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

  if (Mode == 1  ){
     rcMove();
  }
  
  //Mode = digitalRead(sensorMode);    ///Schalteingang ist abgeschaltet
  
  if (debug) {
      Serial.print("Mode= ");
      Serial.println(Mode);
  }

  if (durchlauf == 10) {
    center();
  }

  ///Comando Pruefung////
   if(Serial.available() > 0)
    {
        data = Serial.readStringUntil('\n');
        ProzessComando();
    }
  ///////////////////////
    
}


void ProzessComando() {

    if (data == "test" )
      {   
        Serial.println("test gefunden");
      }
    
    if (data == "debug on")
      {
        Serial.println("Debug Mode ON");
        debug = true;      
      }

    if (data == "debug off")
      {
        Serial.println("Debug Mode off");
        debug = false;
      }
    if (data == "vers")   
      {
        Serial.println("----Versionnsnummer-----");
        Serial.print(VERSION);
        Serial.println("Komandoreferenz");
        Serial.println("debug on");
        Serial.println("debug off");
        Serial.println("vers");
        
      }
    delay(2000);
    data = "";
    Serial.flush();
    
 
}



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
    if (debug){
    Serial.println("Links");
     Serial.println(randNumber);
    }
     // set the LED with the ledState of the variable:
     digitalWrite(ledPin2, HIGH);

    //Drehung Links
     analogWrite(links, tempo);  
     analogWrite(rechts, 0); 
     delay(moving);
     digitalWrite(ledPin2, LOW);
     delay(500);
    
    } else if (randNumber > 20 && randNumber <= 30) {

     if (debug) {
      Serial.println("rechts");
      Serial.println(randNumber);
     }
     // set the LED with the ledState of the variable:
      digitalWrite(ledPin1, HIGH); 
      analogWrite(links, 0);  
      analogWrite(rechts, tempo); 
      delay(moving);      
      digitalWrite(ledPin1, LOW); 
      delay(500);
      
    }  else {
      
      analogWrite(links, 0);  
      analogWrite(rechts, 0); 
      delay (zeit);
      
    }

  if (debug) {
      Serial.print("Zeit ");
      Serial.println(zeit);
      Serial.print("Druchlauf ");
      Serial.println(durchlauf);
  }
}



int rcMove() {

    int sensorValue = pulseIn(sensorRC,HIGH);
    if (sensorValue < 1400){
      if (debug){
          Serial.println("Links");
      }
          // set the LED with the ledState of the variable:
     digitalWrite(ledPin2, HIGH);
     tempo = sensorValue /4;
     tempo = tempo *-1; 
     tempo = tempo +500;

    //Drehung Links
     analogWrite(links, tempo);  
     analogWrite(rechts, 0); 
     
    
    } else if (sensorValue > 1500) {
      if (debug) {
      Serial.println("rechts");
      }     
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
  if (debug) { Serial.print("Value ");Serial.println(tempo);}
  
}

int center() {
    /// Fuert den Dome in die Ausgangsposition //
   if (debug) { Serial.print("CenterMode ");}

    analogWrite(rechts, 0); 
    analogWrite(links, 0); 
 
   while ( digitalRead(sensorCenter) == 1){
    analogWrite(rechts, 200); 
    if (debug){Serial.println("try to get center");Serial.print(sensorCenter);}
    
    digitalWrite(ledPin1, HIGH); 
    
    
   }
   digitalWrite(ledPin1, LOW); 
   analogWrite(rechts, 0);  
   
   delay(1000);
   durchlauf = 0;
   Mode = 0;
   
   
}


