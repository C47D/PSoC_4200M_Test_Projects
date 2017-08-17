#include "project.h"

static uint8_t memoria[20];

int main(void)
{
    CyGlobalIntEnable;

    DMA_Sts_Mem_Start((void*) Status_Reg_Status_PTR, (void*)memoria);

    while (1) {
        
    }
}

/* [] END OF FILE */
