#include "project.h"
#include <stdio.h>
#include <stdbool.h>

volatile bool data_ready = false;

enum { FORM_FEED_CMD = 0x0C, };

struct adc_result {
    char    string[20];
    float32 volts[ADC_SEQUENCED_CHANNELS_NUM];
    //uint32_t counts[ADC_SEQUENCED_CHANNELS_NUM];
    int32_t counts[ADC_SEQUENCED_CHANNELS_NUM];
} adc_result;

CY_ISR(DMA_Handler)
{
    data_ready = true;
    CyDmaClearInterruptSource(DMA_ADC_CHANNEL_MASK);
}

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
                // Check if the result is saturated
                if ( ADC_Data.counts[channel] & ADC_SATURATE_INTR_MIR) {
                    UART_PutString("The result is saturated\r\n");
                }
                // Check if the result is in range
                if ( ADC_Data.counts[channel] & ADC_RANGE_INTR_MIR) {
                    UART_PutString("The result is in range\r\n");
                }
                // Check if the result is valid
                if ( ADC_Data.counts[channel] & ADC_CHAN_RESULT_VALID_MIR) {
                    UART_PutString("The result is valid\r\n");
                }
                // Converting from counts to Volts
                ADC_Data.volts[channel] = ADC_CountsTo_Volts(channel, ADC_Data.counts[channel]);
                snprintf(ADC_Data.string, string_size, "CH %d: %.3f Volts\r\n", channel, ADC_Data.volts[channel]);
                UART_PutString(ADC_Data.string);
            }
            
            data_ready = false;
        }
    }
}

/* [] END OF FILE */
