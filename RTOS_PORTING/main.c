#include "FreeRTOS.h"
#include "task.h"

/**
 * main.c
 */
//---GPIO-F REGISTER---//
#define PORTF_DATA  (*((volatile unsigned long *)0x40025038))   //offset of DATA register for PF1, PF2, PF3 is 0x38 [PF7:PF0::9:2]
#define PORTF_DIR   (*((volatile unsigned long *)0x40025400))   //offset of DIR register is 0x400
#define PORTF_DEN   (*((volatile unsigned long *)0x4002551C))   //offset of DEN register is 0x51C
//---PORT-F I/O---//
#define PF1 0x02
#define PF2 0x04
#define PF3 0x08
//---SYSTEM CONTROL REGISTERS---//
#define SYS_CTRL_RCGC2  (*((volatile unsigned long *)0x400FE108))   //offset of RCGC2 register is 0x108
#define CLK_GPIOF   0x20

/* Task to be init the micro */
void Init_task( void * pvParameters )
{
    /* The parameter value is expected to be 1 as 1 is passed in the
    pvParameters value in the call to xTaskCreate() below.  */
    configASSERT( ( ( uint32_t ) pvParameters ) == 1 );
    SYS_CTRL_RCGC2 |= CLK_GPIOF | 0x1F;
    PORTF_DIR |= 0x0000000E;    //set PF1, PF2, PF3 as output
    PORTF_DEN |= 0x0000000E;    //enable PF1, PF2, PF3
    PORTF_DATA = 0;
    uint8_t count = 0;
    for( ;; )
    {
        if(count % 8 == 0){
            PORTF_DATA = 0;
            PORTF_DATA |= (PF1);
            count++;
        }
        else if(count % 8 == 1){
            PORTF_DATA = 0;
            PORTF_DATA |= ( PF2);
            count++;
        }
        else if(count % 8 == 2){
            PORTF_DATA = 0;
            PORTF_DATA |= ( PF3);
            count++;
        }
        else if(count % 8 == 3){
            PORTF_DATA = 0;
            PORTF_DATA |= ( PF1 | PF2);
            count++;
        }
        else if(count % 8 == 4){
            PORTF_DATA = 0;
            PORTF_DATA |= ( PF2 | PF3);
            count++;
        }
        else if(count % 8 == 5){
            PORTF_DATA = 0;
            PORTF_DATA |= ( PF1 | PF3);
            count++;
        }
        else if(count % 8 == 6){
            PORTF_DATA = 0;
            PORTF_DATA |= ( PF1 | PF2 | PF3);
            count++;
        }
        else if(count % 8 == 7){
            PORTF_DATA = 0;

            count++;
        }
vTaskDelay(500);
    }
}

int main(void)
{
xTaskCreate(Init_task, "a", 100, NULL, 1, NULL);
	vTaskStartScheduler();
}

void vApplicationIdleHook (void)
{

}

void vApplicationStackOverflowHook (void)
{

}
