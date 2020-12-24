


#include <SoftwareSerial.h>
SoftwareSerial espSerial =  SoftwareSerial(3,2);     
String apiKey = "RT9RQ311J6HLVRU5";     // replace with your channel's thingspeak WRITE API key
String ssid="rahul";    // Wifi network SSID
String password ="rahul999";  // Wifi network password
boolean DEBUG=true;

int led = 11;
int led1 = 10;

int ldr = A0;

 int ir = A1;
 int ir1 = A2;


//======================================================================== showResponce
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (espSerial.available()){
        c=espSerial.read();
        if (DEBUG) Serial.print(c);
      }
    }
                   
}


//========================================================================
boolean thingSpeakWrite(float value1, float value2){
  delay(15000);
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";

  espSerial.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(espSerial.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
  
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKey;
  
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  espSerial.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(espSerial.find(">")){
    espSerial.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
   espSerial.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  else{
    espSerial.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}
//================================================================================ setup


void setup() {                
  DEBUG=true;           // enable debug serial
  Serial.begin(9600); 
   
  pinMode (led,OUTPUT);
  pinMode (led1,OUTPUT);

  pinMode (ldr,INPUT);

  pinMode (ir,INPUT);
  pinMode (ir1,INPUT);


  
  espSerial.begin(115200);  // enable software serial
                          // Your esp8266 module's speed is probably at 115200. 
                          // For this reason the first time set the speed to 115200 or to your esp8266 configured speed 
                          // and upload. Then change to 9600 and upload again
  
  //espSerial.println("AT+RST");         // Enable this line to reset the module;
  //showResponse(1000);

  //espSerial.println("AT+UART_CUR=9600,8,1,0,0");    // Enable this line to set esp8266 serial speed to 9600 bps
  //showResponse(1000);
  
  

  espSerial.println("AT+CWMODE=1");   // set esp8266 as client
  showResponse(1000);

  espSerial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password
  showResponse(5000);

   if (DEBUG)  Serial.println("Setup completed");
}


// ====================================================================== loop
void loop() {

  // Read sensor values
   float t = 0;
   float h = 0;
   
   t = analogRead(A0);
     h = analogRead(A2) + analogRead(A1);
   h = h/2;
   Serial.print(analogRead(A0));
   Serial.println("\n");
  int ldrStatus = analogRead (ldr);
  if (ldrStatus <=300)
  {
    digitalWrite(led,HIGH);
    analogWrite(led,255/5);

    digitalWrite(led1,HIGH);
    analogWrite(led1,255/5);
Serial.println("a1");
               Serial.print(analogRead(A1));
               Serial.println("\n");
               Serial.println("a2");
               Serial.print(analogRead(A2));
               Serial.println("\n");


    if(analogRead(A1)<500){
                digitalWrite(led,HIGH);
                
            
    }
    else{
                digitalWrite(led,HIGH);
                analogWrite(led,255/5);
               
      
    }

    if(analogRead(A2)<500){
                digitalWrite(led1,HIGH);
                
    }
    else{
                digitalWrite(led1,HIGH);
                analogWrite(led1,255/5);
               
      
    }


 
   }
 
  else{
    digitalWrite(led,LOW);
    digitalWrite(led1,LOW);
  
  }
    thingSpeakWrite(t,h);
    delay(20000);
  // thingspeak needs 15 sec delay between updates,     
  
}




