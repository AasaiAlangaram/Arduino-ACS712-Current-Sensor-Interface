#include <mcp_can.h>
#include <mcp_can_dfs.h>

// demo: CAN-BUS Shield, send data
// loovee@seeed.cc

#include <mcp_can.h>
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
int Analogue[6] = { A0, A1, A2, A3, A4, A5 };
int SensorVal[6][7] ={0};

int lowRawValue = 0;
int mVperAmp = 185; //Use 185 for 5A Sensor
int ACoffset = 2500;

float lowVoltage = 0;
float lowAmps = 0;
float test = 0;
float test2 = 0;

/*CAN Message receice variable*/
unsigned char len = 0;
unsigned char buf[8];
unsigned char ilcu_RH_01_200ms_buf[8];

typedef union
{
 float number;
 uint8_t bytes[4];
} FLOATUNION_t;

FLOATUNION_t myFloat;

typedef union
{
 unsigned int number1;
 uint8_t bytes[2];
} INTUNION_t;

INTUNION_t myint;

/*
typedef union
{
 unsigned int lownumber;
 uint8_t lowbytes[2];
} LOWUNION_t;

LOWUNION_t myint;
*/
unsigned char stmp[8] = {0, 0, 0, 0, 0, 0, 0, 0};
unsigned char stmp1[8]= {0};
unsigned char arr[4] = {0,0,0,0};
unsigned char int_arr[2] = {0,0};
unsigned char low_arr[2] = {0,0};
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


float bytesToFloat(unsigned char b0, unsigned char b1, unsigned char b2, unsigned char b3)
{
    float output;

    *((unsigned char*)(&output) + 3) = b0;
    *((unsigned char*)(&output) + 2) = b1;
    *((unsigned char*)(&output) + 1) = b2;
    *((unsigned char*)(&output) + 0) = b3;

    return output;
}

void loop()
{
    static int ibuf=0;
    int i,i1;
    int AvgSnsrData[6]={0};
    int locBuf =0;

    /*Data Receive*/
    if(CAN_MSGAVAIL == CAN.checkReceive())            // check if data coming
    {
        CAN.readMsgBuf(&len, buf);    // read data,  len: data length, buf: data buf

        unsigned long canId = CAN.getCanId();
        
        //SERIAL.println("-----------------------------");
        //SERIAL.print("Get data from ID: 0x");
        SERIAL.print("Waiting");

        if(canId == 0x426)
        {
          //SERIAL.print("Received");
          //SERIAL.println(canId, HEX);
          for(int i = 0; i<len; i++)    // print the data
          {
            ilcu_RH_01_200ms_buf[i] = buf[i];
            SERIAL.print(buf[i], HEX);
            SERIAL.print("\t");
            //SERIAL.print(ilcu_RH_01_200ms_buf[i], BIN);
            //SERIAL.print("\t");
          }
          SERIAL.println();
        }
        else
        {
          
        }
  
    }
       
    
    for(i=0;i<6;i++){
      AvgSnsrData[i] = analogRead(Analogue[i]);
      delay(0.1);
    }

    /*
    stmp[0] = AvgSnsrData[0] & 0xFF;
    stmp[1] = (AvgSnsrData[0] & 0xFF00)>>8;
    stmp[2] = AvgSnsrData[1] & 0xFF;
    stmp[3] = (AvgSnsrData[1] & 0xFF00)>>8;
    stmp[4] = AvgSnsrData[2] & 0xFF;
    stmp[5] = (AvgSnsrData[2] & 0xFF00)>>8;
    stmp[6] = AvgSnsrData[3] & 0xFF;
    stmp[7] = (AvgSnsrData[3] & 0xFF00)>>8;

    stmp1[0] = AvgSnsrData[4] & 0xFF;
    stmp1[1] = (AvgSnsrData[4] & 0xFF00)>>8;
    stmp1[2] = AvgSnsrData[5] & 0xFF;
    stmp1[3] = (AvgSnsrData[5] & 0xFF00)>>8;
    */
    
    lowRawValue = AvgSnsrData[0];
    lowVoltage = (lowRawValue / 1024.0) * 5000; // Gets you mV
    lowAmps = ((lowVoltage - ACoffset) / mVperAmp);
    
    //Serial.print(lowAmps);Serial.print(" ");
    
    CAN.sendMsgBuf(0x100, 0, 8, stmp1);
    CAN.sendMsgBuf(0x101, 0, 8, stmp);

    myint.number1 = lowRawValue;
    myFloat.number = lowAmps; // Assign a number to the float
    for (int i=0; i<4; i++)
    {
     //Serial.print(myFloat.bytes[i], HEX); // Print the hex representation of the float
     //Serial.print(' ');
     arr[i] = myFloat.bytes[i];
    }
    //Serial.print(' ');
    
    for (int i=0; i<2; i++)
    {
     //Serial.print(myint.bytes[i], HEX); // Print the hex representation of the float
     //Serial.print(' ');
     int_arr[i] = myint.bytes[i];
    }
    //Serial.print(' ');
    //Serial.print(lowRawValue);
    //Serial.print(" ");
    output_var = ((myint.bytes[1]<<8)|(myint.bytes[0]<<0)) ;
    //Serial.print(output_var);
    
    
    //test = (myFloat.bytes[3]<<24|myFloat.bytes[2]<<16|myFloat.bytes[1]<<8|myFloat.bytes[0]<<0);
    //test2 = (((myFloat.bytes[3])<<24)|((myFloat.bytes[2])<<16)|((myFloat.bytes[1])<<8)|((myFloat.bytes[0])<<0));
    //test2 = float(((uint32_t(myFloat.bytes[3])<<24))|((uint32_t(myFloat.bytes[2])<<16))|((uint32_t(myFloat.bytes[1])<<8))|((uint32_t(myFloat.bytes[0])<<0)));
    
    /*Current Value in Float Data Type*/
    CAN.sendMsgBuf(0x200,0,4, arr);
    /*Current Value in Uint16 Data Type*/
    CAN.sendMsgBuf(0x300,0,2, int_arr);
    
    //Serial.print(bytesToFloat(myFloat.bytes[3],myFloat.bytes[2],myFloat.bytes[1],myFloat.bytes[0]));  
   
    Serial.print('\n');
   
    delay(1000);                       // send data per 100ms
}

// END FILE
