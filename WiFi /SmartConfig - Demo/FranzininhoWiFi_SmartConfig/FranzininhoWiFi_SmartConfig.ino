/*
Autor : Equipe Embarcados
*/

// Incluindo biblioteca para acessa o driver do WiFi
#include "WiFi.h"

// Definido GPIO para indicação do LED
#define LED_WIFI 33

bool i = true;

void setup() 
{
  Serial.begin(115200);

  pinMode(LED_WIFI, OUTPUT);
  // Seleciona o modo do WiFi para Access Point e Station
  WiFi.mode(WIFI_AP_STA);

  Serial.println("Iniciando SmartConfig ...");
  WiFi.beginSmartConfig();

  //Aguarda os pacote chegarem, caso contrário aguarda 
  while(WiFi.smartConfigDone())
  {
    Serial.print('.');
    // Alterna os níveis lógicos do LED para funcionar como pisca-pisca
    // indicando que está aguardando o pacotes chegarem 
    digitalWrite(LED_WIFI,i);
    i = i^1;
    delay(500);
  }
  
  // Após receber os pacotes e conectar na rede, permanece o LED ligado
  digitalWrite(LED_WIFI, HIGH);
  
  Serial.println("Conectado a rede");
  Serial.print(" IP: ");
  //Informa o IP da rede
  Serial.println(WiFi.localIP());
  
}

void loop() 
{
  

}
