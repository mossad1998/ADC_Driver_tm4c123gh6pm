#include "tm4c123gh6pm.h"
#include "GPIO_Driver.h"
#include "ADC_Driver.h"
#include <stdint.h>
#include "stdio.h"

//Example of using ADC_Driver

/* This driver uses the GPIO_Driver and ADC_Driver libraries which provide definitions for
   different functions that help to intialize Tiva c  peripherals.
   To use this driver you will also need to use tm4c123gh6pm to map 
   registers correctly.
   This example used to read adc value from different potentiometers
   logic of tiva c is 3.3 and it has 12bit adc so minimum volt that it can sense is .8mV
   Used pins
   ===========
   Pin GND as Ground
   Pin E3 as adc input
   Pin 3.3 as Source

   History
   =======
   2021/August/08  - First release (Mosad)
   mo.eldibani@gmail.com
*/

void PORTE_Initialization();                                                    //To intialize Pins in port E for adc
void ADC_Initialization();                                                      //To Intialize adc
void Delay(unsigned long counter);                                              //Most dummy delay
float voltage;                                                                  //variable to carry adc value

void main()
{
  PORTE_Initialization();                                                       //Pin E3 as ADC
  ADC_Initialization();                                                         //ADC is ready
	
    while(1)
    {
      ADC_START_CONVERSION(0,3,"Start");                                        //Enable SS3 conversion or start sampling data from AN0
      while(ADC_FLAG_READ(0,3)) ;                                               //Wait untill sample conversion completed*/
      voltage = ADC_READ(0,3,0.000805664);                                      //factor is 3.3/(2^12)
      voltage = (voltage*270/3.3-90);                                           //Driver offest -90
      ADC_FLAG_CLEAR(0,3);                                                      //Clear ADC Flag
      printf("adc value is :%f",voltage);                                       //print adc value
      Delay(2000);                                                              //Dummy Delay
    }
}

void PORTE_Initialization()
{
  CLK_Enable('E');                                                              //Enable Port E clock
  GPIO_PORT_UNLOCK('E');                                                        //Unlock for port
  PIN_COMMIT_UNLOCK('E',3);                                                     //Make Changes To Pin E3
  PIN_INPUT_OUTPUT('E',3,"Input");                                              //Pin E3 as Input
  PIN_FUNCTION_SELECT('E',3,"Alternative");                                     //Pin E3 Alternative
  PIN_CONFIGURE('E',3,"ADC");                                                   //Configure pin E3 as adc
  PIN_ANALOG_DIGITAL('E',3,"Analog");                                           //Set pin E3 as Analog
}

void ADC_Initialization()
{
  CLK_ADC_Enable(0);                                                            //Enable ADC module 0
  ACTIVE_SAMPLE_SEQUENCER_ENABLE(0,3,"Disable");                                //Disable Sequencer 3
  ADC_EMAX_SELECTION(0,3,"Processor");                                          //Processor mode for sequencer 3
  ADC_INPUT_CHANNEL(0,3,0);                                                     //Pick Channel 0
  ADC_SAMPLES_AVERAGE(0,64);                                                    //set 64 samples to take average
  ADC_CONTROL_MODE(0,3,"Sequence");                                             //Set Sequence mode for sequencer 3
  ADC_CONTROL_MODE(0,3,"Interrupt");                                            //Set Interrupt mode for sequencer 3
  ACTIVE_SAMPLE_SEQUENCER_ENABLE(0,3,"Enable");                                 //Enable Sequencer 3
}
void Delay(unsigned long counter)                                               //Dummy Delay
{
	unsigned long i = 0;
	
	for(i=0; i< counter*1000; i++);
}
