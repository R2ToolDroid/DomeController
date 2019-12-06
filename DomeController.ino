///Automatische Domebewegung Steuerung
///Doc Snyder Tool Droid DomeController
///Funktion über Schalter deaktiviert
///für Arduino pro mini
///

#include <SoftwareSerial.h>        // Durch diesen Include können wir die Funktionen 
                                   // der SoftwareSerial Bibliothek nutzen.
SoftwareSerial MainInput(14, 15); // Pin D10 ist RX, Pin D11 ist TX.
                                   // Die Funktion softSerial() kann nun wie Serial() genutzt werden.                             

String data; //Consohlen Input
String VERSION = "06-12-19-RPI";

byte debug = false;
long randNumber;
long zeit;
long moving;
int rotime;
unsigned long zeit1, zeit2;

///Motor L298 Anschluss
int links = 6; //Pin 6
int rechts = 5; //Pin 5
int tempo = 250;
unsigned long Rpos ;  //Zeit von Center to Zielposition in Millisec
int durchlauf = 0;
int sensorValue = 1500;
int sensorCenter = 16;
int sensorRC = 2;
int Mode = 2;// 0=RandMove // 1=RCMove  //2=Service

int centerState = 0;

int ledPin1 =  21;  
int ledPin2 =  20;  

int ledState = LOW; 

int center(String dir) {
    /// Fuert den Dome in die Ausgangsposition //

    centerState = digitalRead(sensorCenter);
    
   if (debug) { 
    
    Serial.print("CenterMode ");
    Serial.println(centerState);
   
   }

    analogWrite(rechts, 0); 
    analogWrite(links, 0); 

      
      while ( digitalRead(sensorCenter) == 1){
                
                if (dir == "L" ) {
                  analogWrite(links, 200); 
                   
                } 
                
                if (dir == "R") {
                  analogWrite(rechts, 200); 
                }
                
                if (debug){Serial.println("try to get center");Serial.print(sensorCenter);}
    
            digitalWrite(ledPin1, HIGH); 
 
      }
  

   
   digitalWrite(ledPin1, LOW); 
   analogWrite(links, 0);  
    analogWrite(rechts, 0);  
   
   delay(200);
   durchlauf = 0;
   //Mode = 0;
   
   
}

void rotateR( int Rpos) {
     
     if (debug){Serial.println(Rpos);}
     digitalWrite(ledPin1, HIGH);
     analogWrite(rechts, 200);  
     delay(Rpos);
     analogWrite(rechts, 0);  
     digitalWrite(ledPin1, LOW);     
     delay(100);
     
}

void rotateL( int Rpos) {
     
     if (debug){Serial.println(Rpos);}
     digitalWrite(ledPin2, HIGH);
     analogWrite(links, 200);  
     delay(Rpos);
     analogWrite(links, 0);  
     digitalWrite(ledPin2, LOW);  
     delay(100);   
}


void nono() {
      center("L"); 
     center("R");
     center("L");
 
     delay(100);  

  
}




int randomMove() {
 
   // print a random number from 0 to 299
  zeit = random(2000, 6000);
  // print a random number from 10 to 19
  randNumber = random(10, 40);
  //Speed
  tempo = random(160,200);
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

int randomMove2() {
 
   // print a random number from 0 to 299
  zeit = random(5000, 8000);
  // print a random number from 10 to 19
  randNumber = random(10, 40);
  //Speed
  tempo = random(100,150);
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

    
    if (sensorValue < 1250){
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
     
    
    } else if (sensorValue > 1650) {
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
    
  if (debug) { 
    
    Serial.print("Tempo ");Serial.println(tempo);
    Serial.print("Value ");Serial.println(sensorValue);
    }
  
}




void FindRoTime(){

    //int rotime;
    center("L");
    Serial.println("Dome ist Center");
    delay(1000);
    zeit1 = millis();
    center("L");
    zeit2 = millis();
    
    Serial.print("Zeit1_");
    Serial.println(zeit1);
    Serial.print("Zeit2_"); 
    Serial.println(zeit2); 

    rotime = zeit2-zeit1;

    Serial.print("rotime_"); 
    Serial.println(rotime);

    Rpos = rotime/360; ///Rpos ist dann Winkel in Zeit Variable  90° also SUM*90;
    center("R");
    
     Serial.print("Rpos_"); 
    Serial.println(Rpos);
}

////
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
  delay(2000);

  Serial.println("DomeController_ Doc Tooldroide");
  Serial.println("...ready for Command_");
  //startseq();
  FindRoTime();
  
}







void ProzessComando() {

    if (data == "test" )
      {   
        Serial.println("test gefunden");
        FindRoTime();

        
      }

    if (data == "try dome"){
        Serial.println("Testprogramm");
     
        rotateR(Rpos*180); 
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

     if (data == "mode3")
      {
        Serial.println("Mode 0 Random");
        Mode = 3;
      }
 

    if (data == "usb")
      {
        Serial.println("Rotation zu Position USB");
       
        rotateR(Rpos*80);    
      }

    if (data == "tool1")
    {
      Serial.println("Rotation zu Position tool1");
        
        rotateR(Rpos*110); 
    }
    
    if (data == "tool2")
    {
      Serial.println("Rotation zu Position tool2");
        
        rotateR(Rpos*140); 
    }

    if (data == "tool3")
    {
      Serial.println("Rotation zu Position tool3");
       
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
      
      center("L");
    }

    
      
    //delay(2000);
    data = "";
    Serial.flush();
    
 
}



void startseq() {

    delay(4000);

    analogWrite(rechts, 100);
    analogWrite(rechts, 0);
     
    center("L");

    delay(2000);

}

void loop() {
  
  
  if (Mode == 0){
     randomMove();
     durchlauf = durchlauf+1;
  }

  if (Mode == 3){
     randomMove2();
     //durchlauf = durchlauf+1;
  }


  if (Mode == 1  ){
     rcMove();
  }

  if (debug) {
      Serial.print("Mode= ");
      Serial.println(Mode);
  }

  if (durchlauf == 10 ) {
    center("L");
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
