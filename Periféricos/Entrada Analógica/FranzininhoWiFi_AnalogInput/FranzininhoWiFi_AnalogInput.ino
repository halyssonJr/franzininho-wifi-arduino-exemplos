/*
  Autor: Equipe Embarcados

*/
#define POTENCIOMETRO 4
#define LED           2
#define LIMIAR 3000 // Leitura ADC

void setup() 
{
  /*Configura o LED com saída digital*/
  pinMode(LED, OUTPUT);

  /*Configura o canal ADC*/
  analogReadResolution(12);

}

void loop() 
{
  /* Se a leitura analogica ultrapassar o limiar ...
    aciona o LED */
  if(analogRead(POTENCIOMETRO)> LIMIAR)
  {
    digitalWrite(LED, HIGH);
  } 
  
  /*Caso contrário desliga o LED*/
  else
  {
    digitalWrite(LED, LOW);
  } 

}
