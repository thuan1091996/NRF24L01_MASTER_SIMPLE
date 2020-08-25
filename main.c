/* --------0.Project information--------------------
 * NRF24L01 Master
 * Protocol communication: SPI (SSI0)
 * Debug through UART0 (PC)
 * Send data to slave, band width 1 byte
 * Author : TRAN MINH THUAN
 * -------------------------------------------------*/

/* --------1.System requirement--------------------
 * 1.Create a simple SSI communication system
 * 2.Create a simple UART0 communication to debug
 * 3.Create a protocol to send data through a Master NRF24L01 to NRF24L01 slave
 * -------------------------------------------------*/

/* --------2.Pre-processor Directives Section-------------------*/
#include "include.h"

/*-------------------------Define value------------------------*/
#define BUTTON          GPIO_PIN_4
#define BUTTON_BASE     GPIO_PORTF_BASE
#define LED             GPIO_PIN_1
#define LED_BASE        GPIO_PORTF_BASE

void Monitor(void);
void GPIOF_Init(void);
void GPIOA_Init();
void UART0_Init(void);
unsigned char Data_NRF[50]={0};
int32_t  ui32Button=0;
unsigned char ui8Data=0;

void main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_2_5| SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ); //80MHz
    GPIOF_Init();                               //PF1-Monitor LED, PF4-Button
    GPIOA_Init();                               //PA3-CSN,PA6-CE,PA7-ISR
    SSI0_Init();                                //Master,8bit,2Mhz/Mode0
    Systick_Init();
    nrf24l01_initialize_debug(false, 1, false); //TX,1 Byte, No Enhanced ShockBurst
    while(true)
    {
        nrf24l01_get_all_registers(&Data_NRF);
        ui32Button=GPIOPinRead(BUTTON_BASE, BUTTON); //Read button
        if(ui32Button==0)                            //If presseed
        {
            ui8Data=0x01;
            nrf24l01_write_tx_payload(&ui8Data, 1, true);
            delay_us(200000);           //Debounce
            nrf24l01_get_all_registers(&Data_NRF);
            nrf24l01_irq_clear_all();
        }
    }
}
/* GPIO PORTF Initialization
 * Function: Define PF4 as input pin, 4MA, Pull-up resistor (For Testing SPI communication)
 * Input: No
 * Output: No
 * Changing this function:
       + Changing GPIO Port (SysCtlEn & SysCtlReady)
       + Changing GPIO Pin function (GPIOPinType)
       + Changing Other Input pin characteristic (GPIOPadConfig)
 * Associate with:
       + PF4
       + No variable
 * Testing this function: Click Button and LED on PF1 will bright until release
*/
void GPIOF_Init(void) {
    //Enable peripheral and wait until ready
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);        //Enable clock for PortF
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF)); //Wait for PortF ready
    //Configure for pins
    GPIOPinTypeGPIOInput(BUTTON_BASE, BUTTON);          //Define Pin4 as input pin
    GPIOPadConfigSet(BUTTON_BASE, BUTTON, GPIO_STRENGTH_4MA, GPIO_PIN_TYPE_STD_WPU);
    //For testing run this function in infinite loop
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1); //Configure red LED for testing
//    if(GPIOPinRead(BUTTON_BASE, BUTTON)==1)                //Don't press
//        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x00);
//    else                                                   //Press
//        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1, 0x02);
}
void GPIOA_Init(){
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
    GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6|GPIO_PIN_3);
}
/* UART Initialization
 * Function: Initialization UART0: Use System Clock, 115200, 8-bit data, 1 stop, no parity bit
 * Input: No
 * Output: No
 * Change this function:
       + Change UART Port by changing GPIO port, UART base (SysCtlEn & SysCtlReady)
         then change appropriate pins (GPIOPinConfigure,GPIOPinTypeUART)
       + Changing Other UART characteristic (UARTConfigSetExpClk)
 * Associate with
       + PA0,PA1
       + No variable
*/
void UART0_Init(void) {
    //Enable peripheral and wait until ready
    SysCtlPeripheralEnable(SYSCTL_PERIPH_UART0);             //Enable Clock for UART0
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);             //Clock for PortA
    //PORT Pin configure for UART
    GPIOPinConfigure(GPIO_PA0_U0RX);                         //PIN mux for UART0
    GPIOPinConfigure(GPIO_PA1_U0TX);                         //PIN mux for UART0
    GPIOPinTypeUART(GPIO_PORTA_BASE, GPIO_PIN_0 |GPIO_PIN_1);//PIN mux for UART0
    //Configure for UART
    UARTConfigSetExpClk(UART0_BASE, SysCtlClockGet(), 115200,(UART_CONFIG_WLEN_8 | UART_CONFIG_STOP_ONE | UART_CONFIG_PAR_NONE));
}
//Flashing RED LED 10Hz to show that the system is running


