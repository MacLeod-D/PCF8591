# PCF8591
How to calibrate

1 channel 8 Bit DA
4 channel 8 Bit AD


AIN3<-AOUT (both connected to Nano A0)  from about i==100(1.8V): Diode D2 will light proportional to value
AIN2<-Input2 = thermistor (connected with switch)
AIN1<-Input1 = photoresistor (connected with switch)
AIN0<-Input0 = potentiometer (connected with switch) LED2 wil




 What is it good for ?
 
 Arduino Nano/Uno has only PWM for analog output, but sometimes you may want a real DA converter.
 For less than 1$ you can get a PCF8591 true 8 Bit DA converter
 And it has 4 channels 8 Bit AD converter.
 But Nano/Uno already have 7/5 channels 10 Bit AD converter.
 For that some manufactorers add sensors to the PCF8591 channels:
 
 1. Photoresistor Inp0
 You can set a switch  to get values from a photoresistor from Inp0.
 It makes it easy to switch a light if it gets dark

  2. Thermistor Inp1
  You may calibrate it to get information about temperature

  3. Potentiometer Inp2
  You may set the potentiometer to a stable voltage - for Instance 1.5V
  With the switch set you can read the set voltage
  Then remove the switch and you may use this voltage at Inp0 for instance for a comparator or to set the base voltage of a
  transistor (to gain capavitive coupled AC voltage)


  But it is not so easy to calibrate this device !

  First Trick:
  I had problems to set an output voltage and read it back.
  When I read Ain3 connected to AOUT it gets down to 0.
  You have to set Bit6 in the read command !!!
      Wire.write(4+64); // 64: sets the PCF8591 into a AD mode - without 64(bit6) the AD-value is reset to 0 !!!!

  Second Trick:
  My Nano shows 4.68V at the 5V pin - where the PCF8591 is conected to to get power.
  If I put 255 to AOUT I only get 4.55V
  Even that in mind I only get linear voltage between output 0 to 215.
  Here is the sketch to set the voltage as estimated - see Image.

  The voltage set with PCF8591 DA is measured with Nano and PCF8591 AD (and with an instrument).
  
  The resolution (1 bit) is about 0.018V
  The precision is +/- 2 bit
  
  Time needed for setting 1 voltage (AOut) and reading 1 channel: <1ms
  
  Conclusion:
  This device is not so fast and precise - but it is cheap and enough for a lot of measurements.
  And the LDR, Thermistor and Potentiometer may be useful AddOns.
  

  Connections:
  
                  Nano GND to GND
                  Nano 5V  to VCC
                  Nano A4  to SDA
                  Nano A5  to SCL
                  Nano A0  to AOUT
                  Nano A0  to AIN3
  
  
