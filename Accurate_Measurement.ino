
#include <mcp_can.h>
#include <mcp_can_dfs.h>
#include <SPI.h>

/*SAMD core*/
#ifdef ARDUINO_SAMD_VARIANT_COMPLIANCE
  #define SERIAL SerialUSB
#else
  #define SERIAL Serial
#endif

// the cs pin of the version after v1.1 is default to D9
// v0.9b and v1.0 is default D10
const int SPI_CS_PIN = 10;

const int Low_analogIn = 1; //Connect current sensor with A1 of Arduino
const int Drl_analogIn = 0; //Connect current sensor with A0 of Arduino
const int Turn_analogIn = 2; //Connect current sensor with A2 of Arduino
const int High_analogIn = 3; //Connect current sensor with A2 of Arduino
const int Sublow_analogIn = 4; //Connect current sensor with A2 of Arduino

int Analogue[5] = { Drl_analogIn, Low_analogIn, Turn_analogIn, High_analogIn, Sublow_analogIn };

unsigned int RawValue[5] = {0,0,0,0,0};
float Voltage[5] = {0,0,0,0,0};
float Amps[5] = {0,0,0,0,0};

float mVperAmp = 0.185; //Use 185 for 5A Sensor
float ACoffset = 2.5;
float involtage = 5.0;
float ADC10bit_resolrange = 1024.0;

/*Maximum Payload 8Bytes*/
typedef union
{
 unsigned int in_vol[5];
 uint8_t bytes[10];
} INTUNION_t;

INTUNION_t myint;

/*Drl Low Turn High SubLow Pstn */ 
unsigned char invol_arr[10] = {0,0,0,0,0,0,0,0,0,0};
unsigned char invol_arr1[2] = {0,0};
unsigned int output_var;

MCP_CAN CAN(SPI_CS_PIN);                                    // Set CS pin


void setup()
{
  SERIAL.begin(115200);

    while (CAN_OK != CAN.begin(CAN_500KBPS))              // init can bus : baudrate = 500k
    {
        SERIAL.println("CAN BUS Shield init fail");
        SERIAL.println(" Init CAN BUS Shield again");
        delay(100);
    }
    SERIAL.println("CAN BUS Shield init ok!");
}

void loop() 
{
  unsigned int x=0;
  unsigned int i = 0;
  float AcsValue[5] = {0.0,0.0,0.0,0.0,0.0},Samples[5]= {0.0,0.0,0.0,0.0,0.0},AvgAcs[5]= {0.0,0.0,0.0,0.0,0.0},AcsValueF[5]= {0.0,0.0,0.0,0.0,0.0};
  
  /*Read 5 Sensor Values*/
  for(i=0;i<5;i++)
  {
    /*Read 150 samples*/
    for (int x = 0; x < 150; x++)
    { 
      AcsValue[i] = analogRead(Analogue[i]);     //Read ADC values   
      Samples[i] = Samples[i] + AcsValue[i];  //Add samples together
      delay (3); // let ADC settle before next sample 3ms
    }
    
    AvgAcs[i]=Samples[i]/150.0;//Taking Average of Samples
    
    myint.in_vol[i] = AvgAcs[i];  //Assign input voltage digital value
    /* Read byte values for input voltage*/
    for(int j = i;j < i+1 ;j++)
    {
       invol_arr[j*2] = myint.bytes[j*2];
       invol_arr[(j*2)+1] = myint.bytes[(j*2)+1];
    }

    /*
     * Current Calculation
     * -------------------
     * ((AvgAcs * (5.0 / 1024.0)) is converitng the read voltage in 0-5 volts
     * 2.5 is offset(I assumed that arduino is working on 5v so the viout at no current comes
     * out to be 2.5 which is out offset. If your arduino is working on different voltage than 
     * you must change the offset according to the input voltage)
     * 0.185v(185mV) is rise in output voltage when 1A current flows at input
    */
    AcsValueF[i] = ((AvgAcs[i] * (involtage / ADC10bit_resolrange)) - ACoffset )/mVperAmp;
  
  }
    invol_arr1[0] = invol_arr[9];
    invol_arr1[1] = invol_arr[10];

    /*
     * Send Data through CAN BUS
     * Messsage Id:0x300 - send first 8Bytes
     * Messsage Id:0x301 - send last 2Bytes
    */
    CAN.sendMsgBuf(0x300,0,8, invol_arr);
    CAN.sendMsgBuf(0x301,0,2, invol_arr1);
    
    /*Print First Three Current Value for reference
     * Drl
     * Low
     * Turn 
    */
    Serial.print(AcsValueF[0]);//Print the read current on Serial monitor
    Serial.print(' ');
    Serial.print(AcsValueF[1]);//Print the read current on Serial monitor
    Serial.print(' ');
    Serial.print(AcsValueF[2]);//Print the read current on Serial monitor
    Serial.print('\n');
    
    delay(50);
}


