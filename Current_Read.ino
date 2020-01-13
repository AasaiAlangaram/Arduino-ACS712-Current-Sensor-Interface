#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

//Measuring Current Using ACS712

const int Low_analogIn = 1; //Connect current sensor with A1 of Arduino
const int Drl_analogIn = 0; //Connect current sensor with A0 of Arduino
const int Turn_analogIn = 2; //Connect current sensor with A2 of Arduino
const int High_analogIn = 4; //Connect current sensor with A2 of Arduino
const int Sublow_analogIn = 5; //Connect current sensor with A2 of Arduino

int mVperAmp = 185; // use 100 for 20A Module and 66 for 30A Module
int ACSoffset = 2500; 

/*Low Beam Sensor Values*/
int LowRawValue= 0;
double LowVoltage = 0; //voltage measuring
double LowAmps = 0;// Current measuring

/*Drl Sensor Values*/
int DrlRawValue= 0;
double DrlVoltage = 0; //voltage measuring
double DrlAmps = 0;// Current measuring

/*Turn Sensor Values*/
int TurnRawValue= 0;
double TurnVoltage = 0; //voltage measuring
double TurnAmps = 0;// Current measuring

void setup() {
  // initialize the LCD
  lcd.init();

  // Turn on the blacklight and print a message.
  lcd.backlight();
  
  //baud rate
  Serial.begin(115200);//baud rate at which arduino communicates with Laptop/PC
  // set up the LCD's number of columns and rows:
  
  lcd.begin(20, 4);  //LCD order
  // Print a message to the LCD.

  /*Starter Code */
  lcd.setCursor(1,1);//Setting cursor on LCD
  lcd.print("Project JX1");//Prints on the LCD
  lcd.setCursor(4,2);
  lcd.print("Current Read");
  delay(2000);//time delay for 2 sec
  lcd.clear();//clearing the LCD display
  lcd.display();//Turning on the display again
  lcd.setCursor(1,0);//setting LCD cursor
  lcd.print("Reading Values from");//prints on LCD
  lcd.setCursor(1,1);
  lcd.print("DC Current Sensor");
  lcd.setCursor(5,2);
  lcd.print("ACS 712");
  delay(2000);//delay for 2 sec
}

void loop() //method to run the source code repeatedly
{ 
 int current_arr[] = {};

 /*Call Current Read Function To Display values in Serial Monitor and LCD*/
 Current_Read();

}

void Current_Read()
{
  /*Read Low Beam Current*/
  LowRawValue = analogRead(Low_analogIn);//reading the value from the analog pin
  LowVoltage = (LowRawValue / 1024.0) * 5000; // Gets you mV
  LowAmps = ((LowVoltage - ACSoffset) / mVperAmp);

  /*Read Drl Current*/
  DrlRawValue = analogRead(Drl_analogIn);//reading the value from the analog pin
  DrlVoltage = (DrlRawValue / 1024.0) * 5000; // Gets you mV
  DrlAmps = ((DrlVoltage - ACSoffset) / mVperAmp);

  /*Read Turn Current*/
  TurnRawValue = analogRead(Turn_analogIn);//reading the value from the analog pin
  TurnVoltage = (TurnRawValue / 1024.0) * 5000; // Gets you mV
  TurnAmps = ((TurnVoltage - ACSoffset) / mVperAmp);

  /*
  //Prints Digital Value on the serial port
  Serial.print("Low Beam Raw Value = " ); // prints on the serial monitor
  Serial.print(LowRawValue); //prints the results on the serial monitor
  */
  
  /* LCD Print Functions*/
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("LowCurrent:");
  lcd.setCursor(11,0);
  lcd.print(LowAmps);
  lcd.setCursor(15,0);
  lcd.print("a");

  /* LCD Print Functions*/
  lcd.setCursor(0,1);
  lcd.print("DrlCurrent:");
  lcd.setCursor(11,1);
  lcd.print(DrlAmps);
  lcd.setCursor(15,1);
  lcd.print("a");
 
  /* 
   lcd.clear();//clears the display of LCD
   delay(1000);//delay of 1 sec
   lcd.display();
   lcd.setCursor(1,0);
   lcd.print("Raw Value = ");
   lcd.setCursor(13,0);
   lcd.print(RawValue);
   */

   /*Display Drl Current Values in Serial Monitor*/
   //Serial.print("DrlAmps = "); // shows the Current measured 
   Serial.print(DrlAmps,3); // the '3' after Current allows you to display 3 digits after decimal point
   Serial.print(' '); 
   
   /*
   lcd.setCursor(1,1);
   lcd.print("Voltage = ");
   lcd.setCursor(11,1);
   lcd.print(Voltage,3);
   lcd.setCursor(17,1);
   lcd.print("mV");//Unit for the voltages to be measured
   */

   /*Display Low Current Values in Serial Monitor*/    
   //Serial.print("LowAmps = "); // shows the Current measured 
   Serial.println(LowAmps,3);// the '3' after Current allows you to display 3 digits after decimal point 
   
   /*
   lcd.setCursor(1,2);
   lcd.print("Current = ");
   lcd.setCursor(11,2);
   lcd.print(Amps,3);
   lcd.setCursor(16,2);
   lcd.print("A"); //unit for the current to be measured
   */

   /*Display Turn Current Values in Serial Monitor*/
   //Serial.print("TurnAmps = "); // shows the Current measured 
   Serial.println(TurnAmps,3);// the '3' after Current allows you to display 3 digits after decimal point

   delay(1000); //Read Every 1S
}

