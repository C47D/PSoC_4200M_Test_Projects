#include "project.h"
#include <stdio.h>
#include <stdbool.h>

volatile bool data_ready = false;

enum {
    ADC_CHANNEL_0 = 0,
    ADC_CHANNEL_1 = 1,
    ADC_CHANNEL_2 = 2,
};

enum {
    FORM_FEED_CMD = 0x0C,
};

struct adc_result {
    char    string[20];
    float32 volts;
    int16_t counts;
} adc_result;

CY_ISR_PROTO(DMA_Handler);

int main(void)
{
    
    struct adc_result ADC_Data[ADC_SEQUENCED_CHANNELS_NUM];
    size_t string_size = sizeof(ADC_Data[ADC_CHANNEL_0]) / sizeof(char);

    DMA_Chn0_SetInterruptCallback(&DMA_Handler);
    CyIntEnable(CYDMA_INTR_NUMBER);
    
    CyGlobalIntEnable;

    UART_Start();
    UART_PutString("Test ADC -> DMA -> memory\r\n");
    
    PWM_Start();
    ADC_Start();
    DMA_Chn0_Start((void*) ADC_SAR_CHAN0_RESULT_PTR, &ADC_Data[ADC_CHANNEL_0].counts);
    DMA_Chn1_Start((void*) ADC_SAR_CHAN1_RESULT_PTR, &ADC_Data[ADC_CHANNEL_1].counts);
    DMA_Chn2_Start((void*) ADC_SAR_CHAN2_RESULT_PTR, &ADC_Data[ADC_CHANNEL_2].counts);
    
    ADC_StartConvert();
    
    while (1) {
        
        if ( true == data_ready ) {
            
            UART_PutChar(FORM_FEED_CMD);
            
            for (uint8_t channel = 0; channel < ADC_SEQUENCED_CHANNELS_NUM; channel++) {
                ADC_Data[channel].volts = ADC_CountsTo_Volts(channel, ADC_Data[channel].counts);
                snprintf(ADC_Data[channel].string, string_size, "CH %d: %.3f Volts\r\n", channel, ADC_Data[channel].volts);
                UART_PutString(ADC_Data[channel].string);
            }
            
            data_ready = false;
        }
        
    }
}

CY_ISR(DMA_Handler)
{
    data_ready = true;
    CyDmaClearInterruptSource(DMA_Chn0_CHANNEL_MASK);
}

/* [] END OF FILE */
