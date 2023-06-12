/*
Autor: Equipe Embarcados
*/

#include <WiFi.h>
#include <ESPmDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

const char* ssid = "nome da sua rede";
const char* password = "senha da sua rede";

//Versão do Firmware
#define VERSION_FW "0.0.2"

//Definir os pinos on board
#define PIN_LED_YELLOW 33
#define PIN_LED_BLUE 21

//Definir o número máximo de contagens
#define MAX_COUNTER 5
//Definir o tempo de incrementos das contagens
#define TICKS_BLINK_MS 1000

// Declarando variáveis globais
uint64_t elipse_time = 0;
bool i = true;
int counter = 0;

void setup() 
{
  // Incializa comunicação serial em 115200 bps
  Serial.begin(115200);
  delay(1000);
 // Configura o WiFi como station
  WiFi.mode(WIFI_STA);

 //Inicia o drive WiFi para conectar na rede local
  WiFi.begin(ssid, password);
  // Enquanto não confirmar conexão com a rede, reinicia ...
  while (WiFi.waitForConnectResult() != WL_CONNECTED)
  {
    Serial.println("Falha ao conectar! Reiniciando...");
    delay(5000);
    ESP.restart();
  }
  
  ArduinoOTA.onStart([]() 
  {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else 
        type = "filesystem";

      Serial.println("Iniciando a atualização " + type);
  })
    .onEnd([]() { // Quando finalizar o envio do firmware
      Serial.println("\nFim OTA");
      Serial.println("Pacote recebido com sucesso");
    })
    .onProgress([](unsigned int progress, unsigned int total) { 
      // Método para informar o progresso do envio dos pacotes 
      Serial.printf("Progresso: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      // Método para informar qual tipo de erro ocorreu
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR)
      {
        Serial.println("Falha na Autenticação");
      }
      else if (error == OTA_BEGIN_ERROR) 
      {
        Serial.println("Falha ao Inicializar");
      }
      else if (error == OTA_CONNECT_ERROR)
      {
        Serial.println("Falha na conexão");
      }
      else if (error == OTA_RECEIVE_ERROR)
      {
        Serial.println("Falha na recepção");
      }
      else if (error == OTA_END_ERROR) 
      {
        Serial.println("Falha no fim do pacote");
      }
    });
  //
  ArduinoOTA.begin();
  
  // Imprime no monitor serial  a versão do Firmware
  Serial.print ("Contador - Versão : ");
  Serial.println (VERSION_FW);
  // Imprime no monitor serial
  // o Endereço IP
  Serial.print("Endereço IP: ");
  Serial.println(WiFi.localIP());
  
  
  // Configura as GPIOs como saída digital
  pinMode(PIN_LED_YELLOW,OUTPUT);
  pinMode(PIN_LED_BLUE, OUTPUT);
}


void loop() 
{
  
  ArduinoOTA.handle();
  
  // Caso tenha passado o tempo definido (TICKS_BLINK_MS), então ...
  if(millis() - elipse_time > TICKS_BLINK_MS)
  {
      //Se o contador ultrapasar a contagem máxima, então...
      if ( counter <= MAX_COUNTER)
      {  
        counter++; // Incremento a variável
        digitalWrite(PIN_LED_YELLOW,i); 
        i = i^1; //Operador lógico XOR, para alternar os estados do LED
      } 
      // Caso o contador ultrapsse esse valor máxima, então...
      else if (counter > MAX_COUNTER)
      {
        //Liga o LED azul
        digitalWrite(PIN_LED_BLUE, HIGH);
        // Aguarda 500 milisegundo
        delay(500);
        //Desliga o LED azul
        digitalWrite(PIN_LED_BLUE, LOW);
        //Reinicia o contador
        counter = 0;
      }
      // Recebe o tempo em milisegundos atual.
      elipse_time = millis();
  }

}
