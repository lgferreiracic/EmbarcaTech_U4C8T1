#include <stdio.h>              
#include "pico/stdlib.h"      
#include "hardware/adc.h"   
#include "hardware/gpio.h"
#include "hardware/pwm.h"
#include "inc/ssd1306.h"  

// Definição dos pinos correspondentes aos botões, LEDs e Joystick
#define BUTTON_A_PIN 5
#define JOYSTICK_X_PIN 26
#define JOYSTICK_Y_PIN 27
#define JOYSTICK_BUTTON_PIN 22
#define LED_GREEN_PIN 11
#define LED_BLUE_PIN 12
#define LED_RED_PIN 13

// Definição dos parâmetros do display OLED
#define SSD1306_WIDTH 128
#define SSD1306_HEIGHT 64
#define SSD1306_ADDR 0x3C

// Definição dos parâmetros do I2C
#define I2C_PORT i2c1
#define SDA_PIN 14
#define SCL_PIN 15

// Definição do erro de margem para o joystick
#define MARGIN_OF_ERROR 200

volatile uint32_t button_a_time = 0; // Variável para debounce do botão A
volatile uint32_t joystick_button_time = 0; // Variável para debounce do botão do joystick
volatile bool border_style = true; // Variável para controle do estilo da borda
volatile bool pwm_state = true; // Variável para controle do estado do PWM

// Função para debounce dos botões
bool debounce(volatile uint32_t *last_time){
    uint32_t current_time = to_us_since_boot(get_absolute_time());
    if (current_time - *last_time > 200000){ 
        *last_time = current_time;
        return true;
    }
    return false;
}

// Função de callback para os botões
void gpio_irq_handler(uint gpio, uint32_t events){
    if (gpio == JOYSTICK_BUTTON_PIN){
        if (debounce(&joystick_button_time)){
            gpio_put(LED_GREEN_PIN, !gpio_get(LED_GREEN_PIN)); // Inverte o estado do LED verde
            border_style = !border_style; // Inverte o estilo da borda
        }
    }
    else if (gpio == BUTTON_A_PIN){
        bool current_state = gpio_get(BUTTON_A_PIN);

        if (!current_state && debounce(&button_a_time)){ 
            pwm_state = !pwm_state; // Inverte o estado do PWM

            if (!pwm_state){ // Garante que os LEDs estejam desligados quando o PWM estiver desativado
                pwm_set_chan_level(pwm_gpio_to_slice_num(LED_BLUE_PIN), PWM_CHAN_A, 0);
                pwm_set_chan_level(pwm_gpio_to_slice_num(LED_RED_PIN), PWM_CHAN_B, 0);
            }
        }
    }
}

// Função para inicialização dos LEDs
void init_leds(){
    // Inicialização do LED verde
    gpio_init(LED_GREEN_PIN);
    gpio_set_dir(LED_GREEN_PIN, GPIO_OUT);
    gpio_put(LED_GREEN_PIN, false);

    // Inicialização do LED azul
    gpio_set_function(LED_BLUE_PIN, GPIO_FUNC_PWM);
    uint slice_num_blue = pwm_gpio_to_slice_num(LED_BLUE_PIN);
    pwm_set_wrap(slice_num_blue, 4095);
    pwm_set_chan_level(slice_num_blue, PWM_CHAN_A, 0);
    pwm_set_enabled(slice_num_blue, true);

    // Inicialização do LED vermelho
    gpio_set_function(LED_RED_PIN, GPIO_FUNC_PWM);
    uint slice_num_red = pwm_gpio_to_slice_num(LED_RED_PIN);
    pwm_set_wrap(slice_num_red, 4095);                
    pwm_set_chan_level(slice_num_red, PWM_CHAN_B, 0); 
    pwm_set_enabled(slice_num_red, true);             
}

// Função para inicialização dos botões
void init_buttons(){
    // Inicialização do ADC
    adc_init();

    // Inicialização do botão A
    adc_gpio_init(JOYSTICK_X_PIN);
    gpio_init(BUTTON_A_PIN);
    gpio_set_dir(BUTTON_A_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_A_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_A_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);

    // Inicialização do botão do joystick
    adc_gpio_init(JOYSTICK_Y_PIN);
    gpio_init(JOYSTICK_BUTTON_PIN);
    gpio_set_dir(JOYSTICK_BUTTON_PIN, GPIO_IN);
    gpio_pull_up(JOYSTICK_BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(JOYSTICK_BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_irq_handler);
}

// Função para inicialização do display OLED
void init_display(ssd1306_t *ssd){
    i2c_init(I2C_PORT, 400000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);
    ssd1306_init(ssd, SSD1306_WIDTH, SSD1306_HEIGHT, false, SSD1306_ADDR, I2C_PORT);
    ssd1306_config(ssd);
    ssd1306_fill(ssd, false);
    ssd1306_send_data(ssd);
}

// Função para controle do LED azul com PWM
void control_blue_led_with_pwm(uint16_t y_value){
    if (!pwm_state) // Verifica se o PWM está ativado
        return; 

    uint16_t pwm_value = 0;
    if (y_value < (2048 - MARGIN_OF_ERROR) || y_value > (2048 + MARGIN_OF_ERROR)) 
        pwm_value = (y_value > 2048) ? (y_value - 2048) * 2 : (2048 - y_value) * 2;
    // Define o valor do PWM de acordo com a posição do joystick
    pwm_set_chan_level(pwm_gpio_to_slice_num(LED_BLUE_PIN), PWM_CHAN_A, pwm_value);
}

// Função para controle do LED vermelho com PWM
void control_red_led_with_pwm(uint16_t x_value){
    if (!pwm_state) // Verifica se o PWM está ativado
        return; 

    uint16_t pwm_value = 0;
    if (x_value < (2048 - MARGIN_OF_ERROR) || x_value > (2048 + MARGIN_OF_ERROR))
        pwm_value = (x_value > 2048) ? (x_value - 2048) * 2 : (2048 - x_value) * 2;
    // Define o valor do PWM de acordo com a posição do joystick
    pwm_set_chan_level(pwm_gpio_to_slice_num(LED_RED_PIN), PWM_CHAN_B, pwm_value);
}

// Função para movimentação do quadrado no display OLED
void joystic_movimentation(ssd1306_t *ssd, uint16_t x_value, uint16_t y_value){
    static uint8_t square_x;
    static uint8_t square_y;

    square_x = (x_value * SSD1306_WIDTH) / 4095; // Calcula a posição do quadrado no eixo x
    square_y = SSD1306_HEIGHT - ((y_value * SSD1306_HEIGHT) / 4095); // Calcula a posição do quadrado no eixo y

    square_x = (square_x < 0) ? 0 : (square_x > SSD1306_WIDTH - 8) ? SSD1306_WIDTH - 8 : square_x; // Limita a posição do quadrado no eixo x
    square_y = (square_y < 0) ? 0 : (square_y > SSD1306_HEIGHT - 8) ? SSD1306_HEIGHT - 8 : square_y; // Limita a posição do quadrado no eixo y

    ssd1306_fill(ssd, !border_style);
    ssd1306_rect(ssd, 3, 3, 122, 58, border_style, !border_style); // Desenha a borda do display
    ssd1306_rect(ssd, square_y, square_x, 8, 8, true, true); // Desenha o quadrado

    ssd1306_send_data(ssd);
}

// Função principal
int main(){
    stdio_init_all(); // Inicialização da comunicação serial
    ssd1306_t ssd; // Declaração da estrutura do display OLED
    init_display(&ssd); // Inicialização do display OLED
    init_leds(); // Inicialização dos LEDs
    init_buttons(); // Inicialização dos botões e do joystick

    while (true){
        adc_select_input(1); // Seleciona o pino do ADC para leitura do eixo x do joystick
        uint16_t x_value = adc_read(); // Lê o valor do eixo x do joystick
        adc_select_input(0); // Seleciona o pino do ADC para leitura do eixo y do joystick
        uint16_t y_value = adc_read(); // Lê o valor do eixo y do joystick

        control_red_led_with_pwm(x_value); // Controla o LED vermelho com PWM utilizando o valor do eixo x do joystick
        control_blue_led_with_pwm(y_value); // Controla o LED azul com PWM utilizando o valor do eixo y do joystick
        joystic_movimentation(&ssd, x_value, y_value); // Movimenta o quadrado no display OLED

        sleep_ms(10); // Delay para evitar a leitura do ADC em alta frequência
    }
    return 0;
}