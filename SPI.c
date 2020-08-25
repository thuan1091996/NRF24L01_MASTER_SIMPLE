/*
 * SPI.c
 *
 *  Created on: Oct 26, 2018
 *      Author: Itachi
 */

#include "include.h"
#include "SPI.h"

/* SSI (SPI) Initialization
 * Function: Initialization SSI0 with 2MHz, Motorola Mode0, Master, Band width 8 bits
 * Input: No
 * Output: No
 * Change this function:
       + Change SSI Port by changing GPIO port, SSI base (SysCtlEn & SysCtlReady,SSI Enable)
         then change appropriate pins (PinConfigure,PinTypeSSI)
       + Changing Other SSI characteristic (SSIConfigSetExpClk)
 * Associate with
       + PA2,PA3,PA4,PA5
       + No variable
*/
void SSI0_Init(void) {
    //Enable peripheral and wait until ready
    SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);         //Enable the SSI0 peripheral
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);        //PORTA for SSI0 pins
    while(!SysCtlPeripheralReady(SYSCTL_PERIPH_SSI0));  //Wait for the SSI0 module to be ready
    //PORT pin configure for SSI
    GPIOPinConfigure(GPIO_PA2_SSI0CLK);                 //PA2 CLK
    GPIOPinConfigure(GPIO_PA4_SSI0RX);                  //PA5 TX
    GPIOPinConfigure(GPIO_PA5_SSI0TX);                  //PA4 RX
    GPIOPinTypeSSI(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_2);

    //Configure for SSI -> Clock=2 000 000, 8bit data, master
    SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0,SSI_MODE_MASTER, 2000000, 8);
    SSIEnable(SSI0_BASE);
}

unsigned char SSI_COM(unsigned char byte){
    uint32_t data_read;
    SSIDataPut(SSI0_BASE, byte);
    while(SSIBusy(SSI0_BASE));
    SSIDataGet(SSI0_BASE, &data_read);
    return data_read;
}
