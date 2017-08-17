#include <stdio.h>

#include "project.h"

#define ADC_CH_0    0

int main(void)
{
    int16 adc_result;
    float32 adc_volts;
    char volts_str[30];
    size_t volts_str_size = sizeof(volts_str) / sizeof(char);
    
    CyGlobalIntEnable;

    PWM_Start();
    UART_Start();
    UART_PutString("ADC Polling test\r\n");
    
    ADC_Start();
    ADC_StartConvert();
    
    for(;;)
    {
        // Exit the while loop until the result of the convertion is done
        while(0 == ADC_IsEndConversion(ADC_WAIT_FOR_RESULT));
        
        adc_result = ADC_GetResult16(ADC_CH_0);
        adc_volts = ADC_CountsTo_Volts(ADC_CH_0, adc_result);
        snprintf(volts_str, volts_str_size, "CH 0: %.3f Volts", adc_volts);
        
        UART_PutString(volts_str);
        UART_PutCRLF();
        
    }
}

/* [] END OF FILE */
