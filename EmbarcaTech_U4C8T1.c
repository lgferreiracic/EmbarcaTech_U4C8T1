/*
   Exemplo 1 do EBook pelo prof. Pedro Henrique Almeida Miranda
*/


#include <stdio.h>            
#include "pico/stdlib.h"      
#include "hardware/adc.h"     

int main() {
    stdio_init_all();
    adc_init();
    adc_gpio_init(26);   // Inicializa o GPIO 26 para o ADC
    adc_select_input(0); // Seleciona o canal 0, GPIO26 do ADC

    gpio_init(11);
    gpio_set_dir(11, GPIO_OUT);
    gpio_put(11, 1);

    while (true) {
        uint16_t adc_value = adc_read();
        printf("Valor do ADC: %u\n", adc_value);
        sleep_ms(500);
    }
    return 0;
}