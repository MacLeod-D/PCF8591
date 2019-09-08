// PCF8591
// 1 channel 8 Bit DA
// 4 channel 8 Bit AD

/*
AIN3<-AOUT (both connected to Nan0o A0)  from about i==100(1.8V): Diode D2 will light proportional to value
AIN2<-Input2 = thermistor (connected with switch)
AIN1<-Input1 = photoresistor (connected with switch)
AIN0<-Input0 = potentiometer (connected with switch) LED2 wil
*/


/*
 What is it good for ?
 Arduino Nano/Uno has only PWM foranalog output, but sometimes you may want a real DA converter.
 For less than 1$ you can get a PCF8591 true 8 Bit DA converter
 And it has 4 channels 8 Bit AD converter.
 But Nano/Uno already have 7/5 channels 10 Bit AD converter.
 For that some manufactorers add sensors to the PCF8591 channels:
 
 1. Photoresistor
 You can set a switch  to get values from a photoresistor from Ain0.
 It makes it easy to switch a light if it gets dark

  2. Thermistor
  You may calibrate it to get information about temperature

  3. Potentiometer
  You may set the potentiometer to a concrete voltage - for Instance 1.5V
  With the switch set you can read the set voltage
  Then remove the switch and you may use this voltage at Inp0 for instance for a comparator or to set the base voltage of a
  transistor (to gain capavitive coupled AC voltage)


  But it is not so easy to calibrate this device !

  First Trick:
  I had problems to set an output voltage and read it back.
  When I read Ain3 connected to AOUT gets down to 0.
  You have to set Bit6 in the read command !!!
      Wire.write(4+64); // 64: sets the PCF8591 into a AD mode - without 64(bit6) the AD-value is reset to 0 !!!!

  Second Trick:
  My Nano shows 4.68V at the %V pin - where the PCF8591 is conected to to get power.
  If I put 255 to AOUT I only get 4.55V
  Even that in mind I only get linear voltage between output 0 to 215.
  Here is the sketch to set the voltage as estimated - see Image.

  The voltage set with PCF8591 DA is measured with Nano and PCF8591 AD (and with an instrument).
  The precision is +/- 10mV

  Connections:
  
                  Nano GND to GND
                  Nano 5V  to VCC
                  Nano A4  to SDA
                  Nano A5  to SCL
                  Nano A0  to AOUT
                  Nano A0  to AIN3
  
  
 */





#include "Wire.h"

#define PCF8591 0x48

byte A_0 =0;
byte A_1 =1;

int RawValue0 = 0;
int DACout = 0;
float DACoutVolt = 0.0;
float Voltage = 0.0;
float Offset = 0.0;

void setup()
{
  Wire.begin();
  Wire.setClock(100000); // I had no luck with 400000
  Serial.begin(921600);

 

}
void loop()
{
unsigned long start, end;
  byte val0, val1, val2, val3;
  float value;
  for (int i = 0; i < 215; i++)  // above 215 voltage gets clamped !
  {
    start=micros();
    Wire.beginTransmission(PCF8591);
    Wire.write(0x40); // sets the PCF8591 into a DA mode
    Wire.write(i); // sets the output voltage, from about i==100(1.8V): Diode D2 will light proportional to value
    Wire.endTransmission();
    //end=micros();
    //Serial.println(end-start); // 328 µs
   
    //start=micros();
    Wire.beginTransmission(PCF8591);
                      // 4:  autoincrement
    Wire.write(4+64); // 64: sets the PCF8591 into a AD mode - without 64(bit6) the AD-value is reset to 0 !!!!
    Wire.endTransmission();
    // end=micros();
    // Serial.println(end-start); // 224 µs
    Wire.requestFrom(PCF8591, 5);

    
    val0 = Wire.read(); // Ain0 
    val0 = Wire.read(); // Ain0 0 potentiometer 0-255
    val1 = Wire.read(); // Ain1 photoresistor 0-211 light-dark
    val2 = Wire.read(); // Ain2 thermistor NTC 125 +/- 10
    val3 = Wire.read(); // Ain3 = INP03
    // end=micros();
    // Serial.println(end-start); // 1170 µs

    value=(float)val0/255.0*4.55;
    value=(float)val3/200.0*3.66;  // 5/255*200=3.92
    
    
    DACout = i;
    RawValue0 = analogRead(A_0);
    //RawValue0 = analogRead(A_1);
    
    Voltage = (RawValue0 * 4.68 ) / 1023.0;     // 4.68 = Nano 5V
    DACoutVolt = ((DACout * 4.55) / 255.0);     // 4.55 = PCF8591 AOut 255
    
  
    if( DACoutVolt < 0.0){
      DACoutVolt = 0.0;
    }   


     //Serial.print(DACout); Serial.print(" ");
     Serial.print(DACoutVolt,2); Serial.print(" ");    // wished voltage
     Serial.print(Voltage, 2);  Serial.print(" ");     // Nano measured voltage
     Serial.println(value,3);                          // PCF8591 measure voltage
  }
 
}
