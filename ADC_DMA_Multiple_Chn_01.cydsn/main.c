#include "project.h"
#include <stdio.h>
#include <stdbool.h>

volatile bool data_ready = false;

enum {
    FORM_FEED_CMD = 0x0C,
};

struct adc_result {
    char    string[20];
    float32 volts[ADC_SEQUENCED_CHANNELS_NUM];
    int16_t counts[ADC_SEQUENCED_CHANNELS_NUM];
} adc_result;

CY_ISR_PROTO(DMA_Handler);

int main(void)
{
    
    struct adc_result ADC_Data;
    size_t string_size = sizeof(ADC_Data.string) / sizeof(char);

    DMA_ADC_SetInterruptCallback(&DMA_Handler);
    CyIntEnable(CYDMA_INTR_NUMBER);
    
    CyGlobalIntEnable;

    UART_Start();
    UART_PutString("Test ADC -> DMA -> memory\r\n");
    
    PWM_Start();
    ADC_Start();
    DMA_ADC_Start((void*) ADC_SAR_CHAN0_RESULT_PTR, &ADC_Data.counts);
    
    ADC_StartConvert();
    
    while (1) {
        
        if ( true == data_ready ) {
            
            UART_PutChar(FORM_FEED_CMD);
            
            for (uint8_t channel = 0; channel < ADC_SEQUENCED_CHANNELS_NUM; channel++) {
                ADC_Data.volts[channel] = ADC_CountsTo_Volts(channel, ADC_Data.counts[channel]);
                snprintf(ADC_Data.string, string_size, "CH %d: %.3f Volts\r\n", channel, ADC_Data.volts[channel]);
                UART_PutString(ADC_Data.string);
            }
            
            data_ready = false;
        }
        
    }
}

CY_ISR(DMA_Handler)
{
    data_ready = true;
    CyDmaClearInterruptSource(DMA_ADC_CHANNEL_MASK);
}

/* [] END OF FILE */
