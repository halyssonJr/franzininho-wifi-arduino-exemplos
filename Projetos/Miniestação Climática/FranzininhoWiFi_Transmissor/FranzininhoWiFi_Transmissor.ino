/*
  Autor : Equipe Embarcados
*/

// Inclui a biblioteca do DHT
#include "DHT.h"
//Incluir a biblioteca para ESP_NOW
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

//Define o canal do ESP NOW (1-15)
#define CHANNEL 3
//Define o nome do dispositivo
#define SSID_ESP_NOW "Franz_Sensor"
// Define o pino  
#define DHTPIN 4
//Define o intervalos de envio das leitura
#define READ_INTERVAL_MS 5000
// Declara o construtor e insere os parâmetros
DHT dht22(DHTPIN, DHT22);

// Estrutura de dados para receber as variáveis 
// relacionadas ao DHT22
struct data_t
{ 
   float hum;
   float temp;
};

//Delcara a estrutura de dados
struct data_t  dht;
long last_time = 0;

//Váriavel que contém o endereço em hexadecimal do MAC
//Substitua de acordo com o seu dispositivo
uint8_t franzininho_recv[] = {0x7C,0xDF,0xA1,0x17,0x65,0xE6}; 

int32_t get_WiFi_Channel(const char *ssid) 
{
  if (int32_t n = WiFi.scanNetworks()) 
  {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) 
          {
              return WiFi.channel(i);
          }
      }
  }
  return 0;
}

void setup() 
{
  uint32_t ch;
  
  // Inicializa comunicação Serial com baudrate de 115200 bps
  Serial.begin(115200);
  // Inicializa o driver One Wire para comunica-se com o sensor
  dht22.begin();
  ch = get_WiFi_Channel(SSID_ESP_NOW);
  
  //Configura o dispositivo como Station e configura o canal 
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(ch, WIFI_SECOND_CHAN_NONE);
   
  //Inicializa o ESP NOW
  if(esp_now_init() != ESP_OK)
  {
    //Caso não seja inicializado corretamente, reseta o dispositivo
    Serial.println("Erro ao iniciar o protocolo ESP NOW");
    Serial.println("Reiniciando Franzininho em 3 segundos");
    delay(3000);
    ESP.restart();
  }
  
  Serial.println("Protocolo ESP NOW iniciou com sucesso");

  //Configuramos um novo dispositivo 
  esp_now_peer_info_t peerInfo;
  
  memcpy(peerInfo.peer_addr, franzininho_recv, 6);
  peerInfo.encrypt = false;
  
  //Adicionamos o novo dispositivo        
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Falha ao adicionar um novo endereço");
    return;
  }
}

void loop() 
{
  // A cada intervalo determinado, vamos enviar os dados de leitura ao receptor
  if(millis() - last_time > READ_INTERVAL_MS)
  {
    // Recebe o valor de umidade
    dht.hum  = dht22.readHumidity();
    // Recebe o valor de temperatura
    dht.temp = dht22.readTemperature();
   
    // Caso as leituras não recebam valores válido, então 
    // imprime a informação ("Falha na leitura") 
    if (isnan(dht.hum) || isnan(dht.temp)) 
    {
      Serial.println(F("Falha na leitura!"));
    }
    // Caso contrário então mostra as informações de temperarura e umidade
    else 
    {
      
      Serial.println(" Dados climáticos ");
      Serial.print(F("# Umidade: "));
      Serial.print(dht.hum);
      Serial.println("% ");
      
      Serial.print(F("# Temperatura: "));
      Serial.print(dht.temp);
      Serial.println(F(" °C "));
      
      Serial.print("Enviando Dados ... ");

      //Enviamos os dados para o endereço adiconado anteriormenre 
      esp_err_t result = esp_now_send(franzininho_recv, (uint8_t*)&dht, sizeof(dht));
      if(result != ESP_OK)
      {
        Serial.println("Falha ao enviar a mensagens");
      }
      else
      {
        //Em caso de falha mostramos essa mensagem no monitor serial
        Serial.println("Mensagem enviada com sucesso");
      }
    }
     
    //Atualizado a ultima leitura
    last_time = millis();
  }
}
