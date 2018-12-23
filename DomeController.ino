
///Automatische Domebewegung Steuerung
///Doc Snyder Tool Droid DomeController
///Funktion über Schalter deaktiviert
///für Arduino pro mini

#include <SoftwareSerial.h>        // Durch diesen Include können wir die Funktionen 
                                   // der SoftwareSerial Bibliothek nutzen.
SoftwareSerial MainInput(14, 15); // Pin D10 ist RX, Pin D11 ist TX.
                                   // Die Funktion softSerial() kann nun wie Serial() genutzt werden.
                                   

String data; //Consohlen Input
String VERSION = "20-12-18";

byte debug = false;
long randNumber;
long zeit;
long moving;

///Motor L298 Anschluss
int links = 6; //Pin 6
int rechts = 5; //Pin 5
int tempo = 200;
int Rpos ;  //Zeit von Center to Zielposition in Millisec
int durchlauf = 0;
int sensorValue = 1500;
int sensorCenter = 16;
int sensorRC = 2;
int Mode = 0;// 0=RandMove // 1=RCMove  //2=Service

const int ledPin1 =  21;  
const int ledPin2 =  20;  

int ledState = LOW; 

void setup(){
  Serial.begin(9600);
  MainInput.begin(9600);
   // set the digital pin as output:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
 
  // if analog input pin 0 is unconnected, random analog
  // noise will cause the call to randomSeed() to generateB
  // different seed numbers each time the sketch runs.
  // randomSeed() will then shuffle the random function.
  randomSeed(analogRead(0));
  Serial.println("DomeController_ Doc Tooldroide");
  delay(200);

  Serial.println("DomeController_ Doc Tooldroide");
  Serial.println("...ready for Command_");
  startseq();
}


void loop() {
  
  
  if (Mode == 0){
     randomMove();
     durchlauf = durchlauf+1;
  }

  if (Mode == 1  ){
     rcMove();
  }

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
  ////Prüfung 2///
  MainInput.listen();
  
   if(MainInput.available() > 0)
    {
        data = MainInput.readStringUntil('\r');
        
        Serial.println(MainInput);
        Serial.println(data);
        ProzessComando();
    }
  ///////////////////////
    
}


void ProzessComando() {

    if (data == "test" )
      {   
        Serial.println("test gefunden");
      }

    if (data == "try dome"){
        Serial.println("Testprogramm");
     
        for (int zaehler=1; zaehler<10; zaehler = zaehler+1){
                digitalWrite(ledPin1, HIGH);
                digitalWrite(ledPin2, HIGH);
                delay(200);
                digitalWrite(ledPin1, LOW);
                digitalWrite(ledPin2, LOW);
                delay(200);
            }
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
        Serial.println(VERSION);
        Serial.println("### Komandoreferenz ###");
        Serial.println("debug on");
        Serial.println("debug off");
        Serial.println("test");
        Serial.println("vers");
        Serial.println("try dome");
        Serial.println("mode1");
        Serial.println("mode0");
        Serial.println("usb");
        Serial.println("tool1");
        Serial.println("tool2");
        Serial.println("tool3");
        Serial.println("nono");
        Serial.println("yea");
        Serial.println("dance");
        Serial.println("center");
      }

    if (data == "mode1")
      {
        Serial.println("Mode = 1 RC");
        Mode = 1;
      }


    if (data == "mode0")
      {
        Serial.println("Mode 0 Random");
        Mode = 0;
      }

    if (data == "usb")
      {
        Serial.println("Rotation zu Position USB");
        
        rotateR(600);    
      }

    if (data == "tool1")
    {
      Serial.println("Rotation zu Position tool1");
        
        rotateR(1000); 
    }
    
    if (data == "tool2")
    {
      Serial.println("Rotation zu Position tool2");
       
        rotateR(1400); 
    }

    if (data == "tool3")
    {
      Serial.println("Rotation zu Position tool2");
       
        rotateR(4000); 
    }  

    
    
    if (data == "nono")
    {
      Serial.println("Rotation nono");
       
        nono();
        
    }
    
    if (data == "yea")
    {
      Serial.println("Rotation yea");
       
        rotateR(2000); 
        rotateL(100);
        
    }

    if (data =="dance") {
      
    }

    if (data =="center") {
      center();
    }
    
   
      
    //delay(2000);
    data = "";
    Serial.flush();
    
 
}

void rotateR( int Rpos) {
     
     if (debug){Serial.println(Rpos);}
     digitalWrite(ledPin1, HIGH);
     analogWrite(rechts, 160);  
     delay(Rpos);
     analogWrite(rechts, 0);  
     digitalWrite(ledPin1, LOW);     
     delay(100);
     
}

void rotateL( int Rpos) {
     
     if (debug){Serial.println(Rpos);}
     digitalWrite(ledPin2, HIGH);
     analogWrite(links, 160);  
     delay(Rpos);
     analogWrite(links, 0);  
     digitalWrite(ledPin2, LOW);  
     delay(100);   
}


void nono() {
      center(); 
     if (debug){Serial.println(Rpos);}
     digitalWrite(ledPin2, HIGH);
     analogWrite(links, 150);  
     analogWrite(links, 0); 
     delay(500); 
     digitalWrite(ledPin2, LOW);  
     digitalWrite(ledPin1, HIGH);
     analogWrite(rechts, 150);  
     
     analogWrite(rechts, 0); 
     delay(500); 
     digitalWrite(ledPin1, LOW);  
     digitalWrite(ledPin2, HIGH);
     analogWrite(links, 150);  
     
     analogWrite(links, 0);  
     delay(500);
     digitalWrite(ledPin2, LOW);  
 
     delay(100);  

  
}




int randomMove() {
 
   // print a random number from 0 to 299
  zeit = random(2000, 6000);
  // print a random number from 10 to 19
  randNumber = random(10, 40);
  //Speed
  tempo = random(80,120);
  //Moving länge
  moving = random(500,1500);
 
  if (randNumber < 20){     ///Drehung Links
    if (debug){
    Serial.println("Links");
     Serial.println(randNumber);
    }
     // set the LED with the ledState of the variable:
     digitalWrite(ledPin2, HIGH);
   
     analogWrite(links, tempo);  
     delay(moving);
     analogWrite(links, 0);  
     digitalWrite(ledPin2, LOW);
     delay(500);
    
    } else if (randNumber > 20 && randNumber <= 30) {  ///Rechts Drehung

     if (debug) {
      Serial.println("rechts");
      Serial.println(randNumber);
     }
     // set the LED with the ledState of the variable:
      digitalWrite(ledPin1, HIGH); 
      
      analogWrite(rechts, tempo); 
      delay(moving);      
      digitalWrite(ledPin1, LOW); 
      analogWrite(rechts, 0); 
      delay(500);
      
    }  else {
      
      analogWrite(links, 0);  
      analogWrite(rechts, 0); 
      digitalWrite(ledPin1, LOW); 
      digitalWrite(ledPin2, LOW); 
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
    
    if (sensorValue >=800){ ///Check if Sensor is Connectet an RC on

    
    if (sensorValue < 1400){
      if (debug){ 
        Serial.println("Links");
      }
          // set the LED with the ledState of the variable:
     digitalWrite(ledPin2, HIGH);
     tempo = sensorValue /4;
     tempo = tempo *-1; 
     tempo = tempo +500;

    //Drehung Rechts
      
     digitalWrite(ledPin1, HIGH); 
     analogWrite(links, 0); 
     analogWrite(rechts, tempo); 
     
    
    } else if (sensorValue > 1500) {
      if (debug) {
      Serial.println("rechts");
      }     
     // set the LED with the ledState of the variable:
      digitalWrite(ledPin1, HIGH); 
      tempo = sensorValue /4;
      //tempo = tempo /5;
      
      analogWrite(rechts, 0);  
      analogWrite(links, tempo); 
     
    }  else {
      analogWrite(links, 0);  
      digitalWrite(ledPin1, LOW); 
      analogWrite(rechts, 0); 
      digitalWrite(ledPin2, LOW);
      
      //delay (zeit);
      
    }

    }///End Sensor Check
    
  if (debug) { Serial.print("Value ");Serial.println(tempo);}
  
}

int center() {
    /// Fuert den Dome in die Ausgangsposition //
   if (debug) { Serial.print("CenterMode ");}

    analogWrite(rechts, 0); 
    analogWrite(links, 0); 
 
   while ( digitalRead(sensorCenter) == 1){
    analogWrite(links, 130); 
    if (debug){Serial.println("try to get center");Serial.print(sensorCenter);}
    
    digitalWrite(ledPin1, HIGH); 
    
    
   }
   digitalWrite(ledPin1, LOW); 
   analogWrite(links, 0);  
   
   delay(200);
   durchlauf = 0;
   //Mode = 0;
   
   
}

void startseq() {

    
    analogWrite(rechts, 100);
    analogWrite(rechts, 0);
    delay(500);
    analogWrite(links, 100);  
    analogWrite(links, 0);
     
    center();


}

