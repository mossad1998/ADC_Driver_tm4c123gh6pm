#include "ADC_Driver.h"


volatile uint32_t* ADCACTSS_R[]={&ADC0_ACTSS_R,&ADC1_ACTSS_R};                                    //Array of ACTSS Registers for each module

volatile uint32_t* ADCEMUX_R[]={&ADC0_EMUX_R,&ADC1_EMUX_R};                                       //Array of EMUX Registers for each module

volatile uint32_t* ADC_PC_R[]={&ADC0_PC_R,&ADC1_PC_R};                                            //Array of PC Registers for each module

volatile uint32_t* ADC_SAC_R[]={&ADC0_SAC_R,&ADC1_SAC_R};                                         //Array of SAC Registers for each module

volatile uint32_t* ADC_SSMUX[]={&ADC0_SSMUX0_R,&ADC0_SSMUX1_R,&ADC0_SSMUX2_R,&ADC0_SSMUX3_R,      //Array of SSMUX Registers for each module
                              &ADC1_SSMUX0_R,&ADC1_SSMUX1_R,&ADC1_SSMUX2_R,&ADC1_SSMUX3_R};

volatile uint32_t* ADC_CTL_R[]={&ADC0_SSCTL0_R,&ADC0_SSCTL1_R,&ADC0_SSCTL2_R,&ADC0_SSCTL3_R,      //Array of SSCTL Registers for each module
                            &ADC1_SSCTL0_R,&ADC1_SSCTL1_R,&ADC1_SSCTL2_R,&ADC1_SSCTL3_R};

volatile uint32_t* ADC_PSSI_R[]={&ADC0_PSSI_R,&ADC1_PSSI_R};                                      //Array of PSSI Registers for each module

volatile uint32_t* ADC_RIS[]={&ADC0_RIS_R,&ADC0_RIS_R};                                           //Array of RIS Registers for each module

volatile uint32_t* ADC_SSFIFO[]={&ADC0_SSFIFO0_R,&ADC0_SSFIFO1_R,&ADC0_SSFIFO2_R,&ADC0_SSFIFO3_R, //Array of SSFIFO Registers for each module
                             &ADC1_SSFIFO0_R,&ADC1_SSFIFO1_R,&ADC1_SSFIFO2_R,&ADC1_SSFIFO3_R};
volatile uint32_t* ADC_ISC[]={&ADC0_ISC_R,&ADC0_ISC_R};


void CLK_ADC_Enable(int ADC_module)                                                               //To Eable CLK for specific module
{
  SYSCTL_RCGCADC_R |= 1UL << ADC_module;
    volatile uint32_t dummy ;                                                                     //To wait untill enabling the CLK
    dummy = SYSCTL_RCGCADC_R ;
}

void ACTIVE_SAMPLE_SEQUENCER_ENABLE(int ADC_module,int sequencer_module,char type[10])            //Enable or Disable sequencer
{
  if(strcmp(type , "Enable")==0)
  {
    *ADCACTSS_R[ADC_module] |= 1UL << sequencer_module;                                           //To Enable Sequencer
  }
  else if (strcmp(type , "Disable")==0)
  {
    *ADCACTSS_R[ADC_module] &= ~(1UL << sequencer_module);                                        //To Disable sequencer
  }
}

void ADC_EMAX_SELECTION(int ADC_module,int sequencer_module,char type[10])                        //Select the trigger source for Sample Sequencer
{
  if(strcmp(type , "Cont_Samping")==0)
  {
    *ADCEMUX_R[ADC_module] |= 15 << 4*sequencer_module;                                           //Continuously sample
  }
  else if (strcmp(type , "Comparator0")==0)
  {
    *ADCEMUX_R[ADC_module] |= 1UL << 4*sequencer_module;                                          //Analog Comparator Control
  }
  else if (strcmp(type , "Processor")==0)
  {
    *ADCEMUX_R[ADC_module] &= ~(0xF << 4*sequencer_module);                                       //The trigger is initiated by setting the SSn bit in the ADCPSSI register
  }
}

void ADC_INPUT_CHANNEL(int ADC_module,int sequencer_module,int channel)                           //Select specific channel
{
  int index = sequencer_module +4*ADC_module;                                                     //Calculate the index
  *ADC_SSMUX[index] = channel;                                                                    //Set the channel
}

void ADC_CONTROL_MODE(int ADC_module,int sequencer_module,char type[10])                          //Select the Mode
{
 int index = sequencer_module +4*ADC_module;                                                      //Calculate the index
  if(strcmp(type,"Differential")==0)
  {
     *ADC_CTL_R[index] |= 1UL << 0;                                                               //Differential mode
  }
  else if (strcmp(type , "Sequence")==0)
  {
     *ADC_CTL_R[index] |= 1UL << 1;                                                               //Sequence mode
  }
  else if (strcmp(type , "Interrupt")==0)
  {
     *ADC_CTL_R[index] |= 1UL << 2;                                                               //Interrupt mode
  }
  else if (strcmp(type , "Temperature")==0)
  {
     *ADC_CTL_R[index] |= 1UL << 3;                                                               //Temperature mode
  }
}

void ADC_START_CONVERSION(int ADC_module,int sequencer_module,char type[10])                      //Start or stop Conversion
{
    if(strcmp(type , "Start")==0)
  {
    *ADC_PSSI_R[ADC_module] |= 1UL << sequencer_module;                                           //Start Conversion
  }
  else if (strcmp(type , "Stop")==0)
  {
    *ADC_PSSI_R[ADC_module] &= ~(1UL << sequencer_module);                                        //Stop Conversion
  }
}

bool ADC_FLAG_READ(int ADC_module,int sequencer_module)                                           //Flag when ADC finished
{
      return(!(*ADC_RIS[ADC_module] & (1UL << sequencer_module)));                                //Return adc flag
}

float ADC_READ(int ADC_module,int sequencer_module, float factor)                                 //To read ADC value and mapping it
{
    int index = sequencer_module +4*ADC_module;                                                   //Calculate the index
    return (*ADC_SSFIFO[index]*factor);                                                           //To return adc value
}

void ADC_FLAG_CLEAR(int ADC_module,int sequencer_module)                                          //Clear ADC flag
{
*ADC_ISC[ADC_module] |= 1UL << sequencer_module;
}

void ADC_SAMPLES_AVERAGE(int ADC_module,int number_samples)                                       //To make Averaging for number of samples
{
  int samples = (int)(log(number_samples)/log(2));
  *ADC_PC_R[ADC_module] |= 0x7;
  *ADC_SAC_R[ADC_module] = samples;                                                               //Wrting number of samples to Average
}
