#include <stdio.h>

#include "project.h"
#include "cyapicallbacks.h"

#define ADC_CH_0    0

volatile uint8_t data_ready = 0;

int main(void)
{
    int16_t adc_result;
    float32 adc_volts;
    char volts_str[30];
    size_t volts_str_size = sizeof(volts_str) / sizeof(char);
    
    CyGlobalIntEnable;

    PWM_Start();
    UART_Start();
    UART_PutString("ADC Polling test\r\n");
    
    ADC_Start();
    // El ADC contiene una interrupcion en blanco
    // en ADC_INT.c. Se puede colocar codigo en
    // las areas designadas para realizar las funciones
    // necesarias.
    // Se pueden utilizar los cyapicallbacks para 
    // implementar esas funciones.
    ADC_IRQ_Enable();
    
    ADC_StartConvert();
    
    for(;;)
    {

        if ( data_ready != 0 ){
            adc_result = ADC_GetResult16(ADC_CH_0);
            adc_volts = ADC_CountsTo_Volts(ADC_CH_0, adc_result);
            snprintf(volts_str, volts_str_size, "CH 0: %.3f Volts", adc_volts);
            UART_PutString(volts_str);
            UART_PutCRLF();
            
            data_ready = 0;
        }
        
    }
}

// ADC IRQ Handler
void ADC_ISR_InterruptCallback(){
    data_ready = 1;
}

/* [] END OF FILE */
