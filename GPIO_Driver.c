#include "GPIO_Driver.h"

int CLK_PORTS[]={CLK_PORTA,CLK_PORTB,CLK_PORTC,CLK_PORTD,CLK_PORTE,CLK_PORTF};                  //Array of CLK values for each port

volatile uint32_t* AMSEL_R[]={&GPIO_PORTA_AMSEL_R,&GPIO_PORTB_AMSEL_R,&GPIO_PORTC_AMSEL_R,      //Array of AMSEL Registers for each port
                              &GPIO_PORTD_AMSEL_R,&GPIO_PORTE_AMSEL_R,&GPIO_PORTF_AMSEL_R};

volatile uint32_t* DEN_R[]={&GPIO_PORTA_DEN_R,&GPIO_PORTB_DEN_R,&GPIO_PORTC_DEN_R,              //Array of DEN Registers for each port
                              &GPIO_PORTD_DEN_R,&GPIO_PORTE_DEN_R,&GPIO_PORTF_DEN_R};

volatile uint32_t* AFSEL_R[]={&GPIO_PORTA_AFSEL_R,&GPIO_PORTB_AFSEL_R,&GPIO_PORTC_AFSEL_R,      //Array of AFSEL Registers for each port
                              &GPIO_PORTD_AFSEL_R,&GPIO_PORTE_AFSEL_R,&GPIO_PORTF_AFSEL_R};

volatile uint32_t* DIR_R[]={&GPIO_PORTA_DIR_R,&GPIO_PORTB_DIR_R,&GPIO_PORTC_DIR_R,              //Array of DIR Registers for each port
                            &GPIO_PORTD_DIR_R,&GPIO_PORTE_DIR_R,&GPIO_PORTF_DIR_R};

volatile uint32_t* PUR_R[]={&GPIO_PORTA_PUR_R,&GPIO_PORTB_PUR_R,&GPIO_PORTC_PUR_R,              //Array of PUR Registers for each port
                            &GPIO_PORTD_PUR_R,&GPIO_PORTE_PUR_R,&GPIO_PORTF_PUR_R};

volatile uint32_t* DATA_R[]={&GPIO_PORTA_DATA_R,&GPIO_PORTB_DATA_R,&GPIO_PORTC_DATA_R,          //Array of DATA Registers for each port
                             &GPIO_PORTD_DATA_R,&GPIO_PORTE_DATA_R,&GPIO_PORTF_DATA_R};

volatile uint32_t* PCTL_R[]={&GPIO_PORTA_PCTL_R,&GPIO_PORTB_PCTL_R,&GPIO_PORTC_PCTL_R,          //Array of PCTL Registers for each port
                             &GPIO_PORTD_PCTL_R,&GPIO_PORTE_PCTL_R,&GPIO_PORTF_PCTL_R};

volatile uint32_t* CR_R[]={&GPIO_PORTA_CR_R,&GPIO_PORTB_CR_R,&GPIO_PORTC_CR_R,                  //Array of CR Registers for each port
                             &GPIO_PORTD_CR_R,&GPIO_PORTE_CR_R,&GPIO_PORTF_CR_R};

void CLK_Enable(char port_name)                                                                 //To Eable CLK for specific port
{
  SYSCTL_RCGCGPIO_R |= CLK_PORTS[port_name-65];
    volatile uint32_t dummy ;                                                                   //To wait untill enabling the CLK
    dummy = SYSCTL_RCGCGPIO_R ;
}

void GPIO_PORT_UNLOCK(char port_name)                                                           //To Unlock port F, D, or E
{
    if(port_name == 'F')
    {
    GPIO_PORTF_LOCK_R = 0x4C4F434B ;                                                            //To Activate CLK for port F
    }
    else if(port_name == 'D')
    {
    GPIO_PORTD_LOCK_R = 0x4C4F434B ;                                                            //To Activate CLK for port D
    }
    else if(port_name == 'E')
    {
    GPIO_PORTE_LOCK_R = 0x4C4F434B ;                                                            //To Activate CLK for port E
    }
}

void PIN_COMMIT_UNLOCK(char port_name,int pin)                                                 //To Commit specific Pin in a port
{
    if(port_name == 'F')
    {
    *CR_R[port_name-65] |= 1UL << pin;                                                         //Commit Pins in Port F
    }
    else if(port_name == 'D')
    {
    *CR_R[port_name-65] |= 1UL << pin;                                                         //Commit Pins in Port D
    }
    else if(port_name == 'E')
    {
    *CR_R[port_name-65] |= 1UL << pin;                                                         //Commit Pins in Port E
    }
}

void PIN_ANALOG_DIGITAL(char port_name,int pin,char type[10])                                   //To choose digital or analog
{
    if(strcmp(type , "Digital")==0)
  {
    *AMSEL_R[port_name-65] &= ~(1UL << pin);                                                    //To Disable AMSEL
    *DEN_R[port_name-65] |= 1UL << pin;                                                         //To Enable DEN
  }
  else if (strcmp(type , "Analog")==0)
  {
    *AMSEL_R[port_name-65] |= 1UL << pin;                                                       //To Enable AMSEL
    *DEN_R[port_name-65] &= ~(1UL << pin);                                                      //To Disable DEN
  }
}

void PIN_FUNCTION_SELECT(char port_name,int pin,char type[15])                                  //To choose input/output or alternative
{
if(strcmp(type , "General I/O")==0)
  {
    *AFSEL_R[port_name-65] &= ~(1UL << pin);                                                    //To Disable AFSEL
  }
  else if (strcmp(type , "Alternative")==0)
  {
    *AFSEL_R[port_name-65] |= 1UL << pin;                                                       //To Enable AFSEL
  }
}

void PIN_INPUT_OUTPUT(char port_name,int pin,char type[10])                                     //To choose Input or output
{
  if(strcmp(type , "Input")==0)
  {
    *DIR_R[port_name-65] &= ~(1UL << pin);                                                      //To specify input
  }
  else if (strcmp(type , "Output")==0)                                                          //To specify output
  {
    *DIR_R[port_name-65] |= 1UL << pin;
  }
}

void PIN_PULLUP_RESISTOR(char port_name,int pin,char type[10])                                  //To choose enable Pullup
{
if(strcmp(type,"Pullup")==0)
  {
     *PUR_R[port_name-65] |= 1UL << pin;                                                        //To Enable pullup
  }
  else if (strcmp(type , "Pulldown")==0)
 {
     *PUR_R[port_name-65] &= ~(1UL << pin);                                                     //To Disable pullup
  }
}

void PIN_CONFIGURE(char port_name,int pin,char type[10])                                       //To Configure Pins For Different modules
{
  if(strcmp(type,"UART")==0)
    *PCTL_R[port_name-65]|= 1UL << 4*pin;                                                      //Configure as UART
  else if(strcmp(type,"ADC")==0)
    *PCTL_R[port_name-65]&= ~(1UL << 4*pin);                                                   //Configure as ADC
  else if(strcmp(type,"PWM")==0)
    *PCTL_R[port_name-65]|= 0x4 << 4*pin;                                                      //Configure as PWM
}

void PIN_WRITE_DIGITAL(char port_name,int pin,char data[3])                                     //To Write data on pins
{
if(strcmp(data,"HIGH")==0)
 *DATA_R[port_name-65] |= 1UL << pin;                                                           //To give HIGH
else
 *DATA_R[port_name-65] &= ~(1UL << pin);                                                        //To give LOW
}

bool PIN_READ_DIGITAL(char port_name,int pin)                                                   //To Read data on pins
{
  return (*DATA_R[port_name-65] & (1UL << pin));                                                //To return pin value
}
