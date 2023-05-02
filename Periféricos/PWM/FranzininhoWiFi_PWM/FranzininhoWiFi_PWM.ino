/*
  Autor : Equipe Embarcados
*/

#define MAX_ADC_VALUE 4096
#define MIN_ADC_VALUE 0

#define MAX_DUTY_CYCLE 255
#define MIN_DUTY_CYCLE 0

#define MAX_RESOLUTION 13

int adc_signal;
int duty_cycle;

const int potentiometer =  4;
const int led_blue      =  33;

void setup()
{
  /*Configura a resolução do sinal PWM */
  analogWriteResolution(13);
  pinMode(led_blue, OUTPUT);
}

void loop() 
{
  /* Pega a leitura do canal ADC*/
  adc_signal = analogRead(potentiometer);
  /*Mapea as leituras ADC para valores entre MIN_DUTY_CYCLE - MAX_DUTY_CYCLE */
  duty_cycle = map(adc_signal,MIN_ADC_VALUE,MAX_ADC_VALUE,MIN_DUTY_CYCLE,MAX_DUTY_CYCLE);
  /*Gera o sinal PWM com o duty_cycle*/
  analogWrite(led_blue, duty_cycle);
}
