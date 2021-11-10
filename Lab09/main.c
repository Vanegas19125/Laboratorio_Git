#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"

//Funciones para el antirebote y el semaforo
void antirrebote(void);
void semaforo(void);

bool push = 0;
bool push_active = 0;

int main(void)
{
    SysCtlClockSet(SYSCTL_SYSDIV_5 | SYSCTL_USE_PLL | SYSCTL_XTAL_16MHZ | SYSCTL_OSC_MAIN);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
    GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
    GPIOPinTypeGPIOInput(GPIO_PORTF_BASE, GPIO_PIN_4);
    GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
    while (1){
        if(!GPIOPinRead(GPIO_PORTF_BASE, GPIO_PIN_4)) push = 1;

        if(push == 1){
            push = 0;
            semaforo();
        }
    }

}

// -----------Se creo una funcion semaforo, que funciona con los leds de la Tiva  ------------

void semaforo(void){
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_3, 0x08);
    SysCtlDelay(16000000);
    int i = 0;
    for (i; i<3; i++) {
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_3, 0X00);
        SysCtlDelay(8000000);
        GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_3, 0x08);
        SysCtlDelay(8000000);
    }
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_3, 0X0A);
    SysCtlDelay(16000000);
    GPIOPinWrite(GPIO_PORTF_BASE, GPIO_PIN_1 | GPIO_PIN_3, 0x02);
    SysCtlDelay(16000000);
}
