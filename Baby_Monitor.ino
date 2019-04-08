#include<SoftwareSerial.h>
#define DEBUG true
#include <Wire.h>

#define CTRL_REG1 0x20
#define CTRL_REG2 0x21
#define CTRL_REG3 0x22
#define CTRL_REG4 0x23
#define CTRL_REG5 0x24

int L3G4200D_Address = 107; //I2C address of the L3G4200D
int x;
int y;
int z;
int counter_one = 0;
int counter_two = 0;
int ptB;
int id;

SoftwareSerial esp(2,3);//rx = 3,tx = 2

void setup()
{
  //pinMode(A0,INPUT);
  Serial.begin(57600);  
  esp.begin(57600);
  transmit("AT+RST\r\n",2000,DEBUG); // reset module
  transmit("AT+CWMODE=3\r\n",1000,DEBUG); // configure as access point
  transmit("AT+CIFSR\r\n",1000,DEBUG); // get ip address
  transmit("AT+CIPMUX=1\r\n",1000,DEBUG); // configure for multiple connections
  transmit("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 
  //delay(1000);
  Wire.begin();
  Serial.println(F("starting up L3GD20H"));
  setupL3G4200D(2000); // Configure L3G4200 - 250, 500 or 2000 deg/sec
  delay(1500); //wait for the sensor to be ready 
}

void loop()
{ 
  delay(3000);
  
  //int mvalue = random(300,600);
  //int sensorValue = random(200,700);
  int sensorValue = 200;
  //int sensorValue = analogRead(A0);
  Serial.println(sensorValue);

  getGyroValues(); // This will update x, y, and z with new values
  
  Serial.print(F("X: "));
  Serial.print(x); //Here you can do some operations befor you use that value
                   //For example set it on a surface and substract or add numbers to get 0,0,0 if you want that position to be your reference
 
  Serial.print(F(" Y: "));
  Serial.print (y);

  Serial.print(F(" Z: "));
  Serial.println(z);

  delay(2000); //Just here to slow down the serial to make it more readable
  
  count();
  awake();
  still_sleeping();

  delay(1000);
 
  if(esp.find("+IPD,"))
    {
     
     delay(1000);
 
     id = esp.read()-48; // subtract 48 because the read() function returns 
                                           // the ASCII decimal value and 0 (the first decimal number) starts at 48
   if(sensorValue < 700 && sensorValue > 600)
     {
      //ptA=0;
     String Data ="<!DOCTYPE html><html><head><meta http-equiv='refresh' content='10'><style>h1 {font-size:250%;text-align: center;}dt {font-size:200%;text-align: center;}</style>";
     String cipSend = "AT+CIPSEND="; 
     cipSend += id;
     cipSend += ",";
     cipSend +=Data.length();
     cipSend +="\r\n";
     
     Serial.println(cipSend);
     
     transmit(cipSend,1000,DEBUG);
     transmit(Data,1000,DEBUG);
     
     Data ="<h1>DIAPER STATUS</h1><dl><dt style=\"color:Tomato;\"><b>LEVEL 1</b></dt><dt>LEVEL 2</dt><dt>LEVEL 3</dt><dt>CHANGE DIAPER!</dt></dl><hr><hr>";

     cipSend = "AT+CIPSEND=";
     cipSend += id;
     cipSend += ",";
     cipSend +=Data.length();
     cipSend +="\r\n";
     
     Serial.println(cipSend);
     
     transmit(cipSend,1000,DEBUG);
     transmit(Data,1000,DEBUG);
     }
     
    if(sensorValue < 600 && sensorValue > 400)
     {  
      //ptA=1;
      String Data ="<!DOCTYPE html><html><head><meta http-equiv='refresh' content='10'><style>h1 {font-size:250%;text-align: center;}dt {font-size:200%;text-align: center;}</style>";
     String cipSend = "AT+CIPSEND=";
     cipSend += id;
     cipSend += ",";
     cipSend +=Data.length();
     cipSend +="\r\n";
     
     Serial.println(cipSend);
     
     transmit(cipSend,1000,DEBUG);
     transmit(Data,1000,DEBUG);
     Data ="<h1>DIAPER STATUS</h1><dl><dt>LEVEL 1</dt><dt style=\"color:Tomato;\"><b>LEVEL 2</b></dt><dt>LEVEL 3</dt><dt>CHANGE DIAPER!</dt></dl><hr><hr>";

     cipSend = "AT+CIPSEND=";
     cipSend += id;
     cipSend += ",";
     cipSend +=Data.length();
     cipSend +="\r\n";
     
     Serial.println(cipSend);
     
     transmit(cipSend,1000,DEBUG);
     transmit(Data,1000,DEBUG);

     }
     if(sensorValue < 400 && sensorValue > 300)
     {
//      ptA=2;
     String Data ="<!DOCTYPE html><html><head><meta http-equiv='refresh' content='10'><style>h1 {font-size:250%;text-align: center;}dt {font-size:200%;text-align: center;}</style>";
     String cipSend = "AT+CIPSEND=";
     cipSend += id;
     cipSend += ",";
     cipSend +=Data.length();
     cipSend +="\r\n";
     
     Serial.println(cipSend);
     
     transmit(cipSend,1000,DEBUG);
     transmit(Data,1000,DEBUG);
     Data ="<h1>DIAPER STATUS</h1><dl><dt>LEVEL 1</dt><dt>LEVEL 2</dt><dt style=\"color:Tomato;\"><b>LEVEL 3</b></dt><dt>CHANGE DIAPER!</dt></dl><hr><hr>";

     cipSend = "AT+CIPSEND=";
     cipSend += id;
     cipSend += ",";
     cipSend +=Data.length();
     cipSend +="\r\n";
     
     Serial.println(cipSend);
     
     transmit(cipSend,1000,DEBUG);
     transmit(Data,1000,DEBUG);

    } 
    if(sensorValue < 300)
    {
      //ptA=3;
      String Data ="<!DOCTYPE html><html><head><meta http-equiv='refresh' content='30'><style>h1 {font-size:250%;text-align: center;}dt {font-size:200%;text-align: center;}</style>";
     String cipSend = "AT+CIPSEND=";
     cipSend += id;
     cipSend += ",";
     cipSend +=Data.length();
     cipSend +="\r\n";
     
     Serial.println(cipSend);
     
     transmit(cipSend,1000,DEBUG);
     transmit(Data,1000,DEBUG);
     
     Data ="<h1>DIAPER STATUS</h1><dl><dt>LEVEL 1</dt><dt>LEVEL 2</dt><dt>LEVEL 3</dt><dt style=\"color:Tomato;\"><b>CHANGE DIAPER!</b></dt></dl><hr><hr>";

     cipSend = "AT+CIPSEND=";
     cipSend += id;
     cipSend += ",";
     cipSend +=Data.length();
     cipSend +="\r\n";
     
     Serial.println(cipSend);
     
     transmit(cipSend,1000,DEBUG);
     transmit(Data,1000,DEBUG);
     
    }
     // delay(2000);
     // print_wb();
      if(ptB==0)
    {

     String Data ="<style>dt {font-size:200%;text-align: center;}</style><h1>SLEEP STATUS</h1><dl><dt style = \"color:Orange;\">BABY IS SLEEPING!!</dt></dl></head></html></html>";
     String cipSend = "AT+CIPSEND=";
     cipSend += id;
     cipSend += ",";
     cipSend +=Data.length();
     cipSend +="\r\n";
     
     Serial.println(cipSend);
     
     transmit(cipSend,1000,DEBUG);
     transmit(Data,1000,DEBUG);
     
     delay(2000);
 
     String cipclose = "AT+CIPCLOSE=";
     cipclose+=id;                  //append connection id
     cipclose+="\r\n";
     Serial.println(cipclose);
    
     transmit(cipclose,3000,DEBUG);
    delay(3000);
  }     
      if(ptB==1)
 {         
         String Data ="<style>dt {font-size:200%;text-align: center;}</style><h1>SLEEP STATUS</h1><dl><dt style = \"color:Blue;\">BABY IS AWAKE!!</dt></dl></head></html></html>";

     String cipSend = "AT+CIPSEND=";
     cipSend += id;
     cipSend += ",";
     cipSend +=Data.length();
     cipSend +="\r\n";
     
     Serial.println(cipSend);
     
     transmit(cipSend,1000,DEBUG);
     transmit(Data,1000,DEBUG);
      
      delay(2000); 
      
     String cipclose = "AT+CIPCLOSE=";
     cipclose+=id;                  //append connection id
     cipclose+="\r\n";
     Serial.println(cipclose);
    
     transmit(cipclose,3000,DEBUG);
     delay(3000);
    }

  }
}

String transmit(String esp_command, const int timeout, boolean debug)
{
    String response = "";
    
    esp.print(esp_command); // send the read character to the esp
    
    long int time = millis();

    while( (time+timeout) > millis())
    {
      while(esp.available())
      {
        
        // The esp has data so display its output to the serial window 
        char c = esp.read(); // read the next character.
        response+=c;
      }  
    }
    
    if(debug)
    {
      Serial.print(response);
    }
    
    return response;
}

void getGyroValues(){

byte xMSB = readRegister(L3G4200D_Address, 0x29);
 byte xLSB = readRegister(L3G4200D_Address, 0x28);
 x = ((xMSB << 8) | xLSB);

byte yMSB = readRegister(L3G4200D_Address, 0x2B);
 byte yLSB = readRegister(L3G4200D_Address, 0x2A);
 y = ((yMSB << 8) | yLSB);

byte zMSB = readRegister(L3G4200D_Address, 0x2D);
byte zLSB = readRegister(L3G4200D_Address, 0x2C);
 z = ((zMSB << 8) | zLSB);
}

int setupL3G4200D(int scale){

 // Enable x, y, z and turn off power down:
 writeRegister(L3G4200D_Address, CTRL_REG1, 0b00001111);

 // If you'd like to adjust/use the HPF, you can edit the line below to configure CTRL_REG2:
 writeRegister(L3G4200D_Address, CTRL_REG2, 0b00000000);

 // Configure CTRL_REG3 to generate data ready interrupt on INT2
 // No interrupts used on INT1, if you'd like to configure INT1
 // or INT2 otherwise, consult the datasheet:
 writeRegister(L3G4200D_Address, CTRL_REG3, 0b00001000);

 // CTRL_REG4 controls the full-scale range, among other things:

 if(scale == 250){
 writeRegister(L3G4200D_Address, CTRL_REG4, 0b00000000);
 }else if(scale == 500){
 writeRegister(L3G4200D_Address, CTRL_REG4, 0b00010000);
 }else{
 writeRegister(L3G4200D_Address, CTRL_REG4, 0b00110000);
 }

 // CTRL_REG5 controls high-pass filtering of outputs, use it
 // if you'd like:
 writeRegister(L3G4200D_Address, CTRL_REG5, 0b00000000);
 }

void writeRegister(int deviceAddress, byte address, byte val) {
 Wire.beginTransmission(deviceAddress); // start transmission to device 
 Wire.write(address); // send register address
 Wire.write(val); // send value to write
 Wire.endTransmission(); // end transmission
}

int readRegister(int deviceAddress, byte address){
  int v;
  Wire.beginTransmission(deviceAddress);
  Wire.write(address); // register to read
  Wire.endTransmission();

  Wire.requestFrom(deviceAddress, 1); // read a byte

  while(!Wire.available()) {
  // waiting
 }
  
    v = Wire.read();
  return v;
}

void count(){
  if((x<-30 || x>-25) && (y<47 || y>51) && (z<-13 || z>-9)){
    counter_one++;
    
    Serial.print(F(" COUNT:"));
 Serial.println(counter_one);
 }  
 else{
      counter_two++;
    } 
}

void awake(){
  if(counter_one==5){
    Serial.println(F("BABY WOKE UP"));
    ptB = 1;
    counter_one = 0;
  }
}

void still_sleeping(){
  if(counter_two==5){
    counter_one = 0;
    counter_two = 0;
    Serial.println(F("BABY IS HAVING SEXY DREAMS"));
    ptB = 0;
  }
}
