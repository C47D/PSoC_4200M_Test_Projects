#include "project.h"

static const uint8_t array[5] = {0x77, 0xF4, 0x73, 0x49, 0xDC};

void DMA_Handler(void);

int main()
{
    // DMA internal interrupt
    DMA_SetInterruptCallback(&DMA_Handler);
    CyIntEnable(CYDMA_INTR_NUMBER);
    
    CyGlobalIntEnable;

    Timer_Start();
    DMA_Start((void *)array, (void *)CYREG_GPIO_PRT2_DR);

    while (1) {

    }
}

void DMA_Handler(void){
    /* Clear the interrupt flags */
    CyDmaClearInterruptSource(CyDmaGetInterruptSourceMasked());
    
    LED_Write(~LED_Read());
}

/* [] END OF FILE */
