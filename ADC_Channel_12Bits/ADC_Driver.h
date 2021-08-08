#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"
#include <string.h>
#include <math.h>



#ifndef __ADC_Driver_H__
#define __ADC_Driver_H__


extern void CLK_ADC_Enable(int ADC_module);                                                              //Clk for ADC module
extern void ACTIVE_SAMPLE_SEQUENCER_ENABLE(int ADC_module,int sequencer_module,char type[10]);           //Enable or Disable sequencer
extern void ADC_EMAX_SELECTION(int ADC_module,int sequencer_module,char type[10]);                       //Select the trigger source for Sample Sequencer
extern void ADC_INPUT_CHANNEL(int ADC_module,int sequencer_module,int channel);                          //Select specific channel
extern void ADC_CONTROL_MODE(int ADC_module,int sequencer_module,char type[10]);                         //Select the Mode
extern void ADC_START_CONVERSION(int ADC_module,int sequencer_module,char type[10]);                     //Start or stop Conversion
extern void ADC_SAMPLES_AVERAGE(int ADC_module,int number_samples);                                      //To make Averaging for number of samples
extern float ADC_READ(int ADC_module,int sequencer_module, float factor);                                //To read ADC value and mapping it
extern bool ADC_FLAG_READ(int ADC_module,int sequencer_module);                                          //Flag when ADC finished
extern void ADC_FLAG_CLEAR(int ADC_module,int sequencer_module);                                         //Clear ADC flag

#endif
