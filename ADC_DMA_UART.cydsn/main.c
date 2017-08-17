#include <stdio.h>

#include "project.h"

#define ADC_CH_0    0

volatile int16_t adc_result = 0;
volatile uint8_t data_ready = 0;

CY_ISR_PROTO(DMA_Handler);

int main(void)
{
    float32 adc_volts;
    char volts_str[30];
    size_t volts_str_size = sizeof(volts_str) / sizeof(char);

    DMA_SetInterruptCallback(&DMA_Handler);
    CyIntEnable(CYDMA_INTR_NUMBER);
    
    CyGlobalIntEnable;

    UART_Start();
    UART_PutString("Test ADC -> DMA -> ADC\r\n");
    
    PWM_Start();
    ADC_Start();
    // Source: Result from Channel 0
    // Destination: result variable
    DMA_Start((void*) ADC_SAR_CHAN0_RESULT_PTR, (void*)&adc_result);
    
    ADC_StartConvert();
    
    while (1) {
        if ( data_ready != 0 ){
            adc_volts = ADC_CountsTo_Volts(ADC_CH_0, adc_result);
            snprintf(volts_str, volts_str_size, "CH 0: %.3f Volts", adc_volts);
            UART_PutString(volts_str);
            UART_PutCRLF();
            
            data_ready = 0;
        }
    }
}

CY_ISR(DMA_Handler){
    //Pin_1_Write(~Pin_1_Read());
    data_ready = 1;
    CyDmaClearInterruptSource(DMA_CHANNEL_MASK);
}

/* [] END OF FILE */
