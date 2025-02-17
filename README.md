# EmbarcaTech_U4C8T1.
<p align="center">
  <img src="Group 658.png" alt="EmbarcaTech" width="300">
</p>

## Atividade: Controle de LEDs e Representação Gráfica com Joystick e ADC no RP2040

![C](https://img.shields.io/badge/c-%2300599C.svg?style=for-the-badge&logo=c&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![Raspberry Pi](https://img.shields.io/badge/-Raspberry_Pi-C51A4A?style=for-the-badge&logo=Raspberry-Pi)
![GitHub](https://img.shields.io/badge/github-%23121011.svg?style=for-the-badge&logo=github&logoColor=white)
![Windows 11](https://img.shields.io/badge/Windows%2011-%230079d5.svg?style=for-the-badge&logo=Windows%2011&logoColor=white)

## Descrição do Projeto

Este projeto tem como objetivo consolidar a compreensão dos conceitos sobre o uso de conversores analógico-digitais (ADC) no microcontrolador RP2040 e explorar as funcionalidades da placa de desenvolvimento BitDogLab. O sistema desenvolvido utiliza um joystick para controlar LEDs RGB via PWM e para representar a posição no display SSD1306 através de um quadrado móvel.

## Componentes Utilizados

- **Joystick (ADC nos eixos X e Y)**: Captura dos valores analógicos de movimentação.
- **Microcontrolador Raspberry Pi Pico W (RP2040)**: Responsável pelo controle dos pinos GPIO.
- **LED RGB**: Com os pinos conectados às GPIOs 11, 12 e 13.
- **Botão A**: Conectado à GPIO 5.
- **Botão do Joystick**: Conectado à GPIO 22.
- **Display SSD1306**: Conectado via I2C nas GPIOs 14 e 15.

## Ambiente de Desenvolvimento

- **VS Code**: Ambiente de desenvolvimento utilizado para escrever e debugar o código.
- **Linguagem C**: Linguagem de programação utilizada no desenvolvimento do projeto.
- **Pico SDK**: Kit de Desenvolvimento de Software utilizado para programar a placa Raspberry Pi Pico W.
- **Simulador Wokwi**: Ferramenta de simulação utilizada para testar o projeto.

## Guia de Instalação

1. Clone o repositório:
2. Importe o projeto utilizando a extensão da Raspberry Pi.
3. Compile o código utilizando a extensão da Raspberry Pi.
4. Caso queira executar na placa BitDogLab, insira o UF2 na placa em modo bootsel.
5. Para a simulação, basta executar pela extensão no ambiente integrado do VSCode.

## Guia de Uso

- **Movendo o joystick nos eixos X e Y**:
  1. Controla a intensidade dos LEDs RGB por PWM.
  2. Move um quadrado no display SSD1306 proporcionalmente aos valores lidos.

- **Pressionando o botão do joystick**:
  1. Alterna o estado do LED Verde.
  2. Modifica a borda do display.

- **Pressionando o botão A**:
  1. Ativa ou desativa os LEDs RGB PWM.

## Testes

Testes básicos foram implementados para garantir que cada componente está funcionando corretamente. 

## Desenvolvedor

[Lucas Gabriel Ferreira](https://github.com/usuario-lider)

## Vídeo da Solução

Clique na imagem abaixo para assistir ao vídeo que demonstra a solução trabalhada e os resultados obtidos nos experimentos:

<p align="center">
  <a href="https://youtu.be/vAGab-Dv7XY">
    <img src="Video.jpg" alt="Vídeo demonstrativo" width="900">
  </a>
</p>


