/*
 Autor: Equipe Embarcados
*/

//Incluir a biblioteca para ESP_NOW
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

// Incluir a biblioteca da Adafruit IO 
#include "AdafruitIO_WiFi.h"

#define IO_USERNAME "Seu usuário"
#define IO_KEY      "Sua chave"

#define WIFI_SSID "Nome da rede local"
#define WIFI_PASS "Senha da rede" 

//Define o nome do dispositivo
#define SSID_ESP_NOW "Franz_Sensor"

// Estrutura de dados para receber as variáveis
// relacionadas ao DHT22
struct data_t
{ 
   float hum;
   float temp;
};
struct data_t  dht;
bool is_recv = false;

//Declarando o construtor da classe
AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);

//Configurando os feeds de acordo com o nome criado
AdafruitIO_Feed *temp_adafruit = io.feed("temperatura");
AdafruitIO_Feed *hum_adafruit = io.feed("umidade");

void on_recv_data(const uint8_t *mac_addr, const uint8_t *data, int data_len) 
{
  char macStr[18];
  //Formatando o valor do MAC para string
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], 
           mac_addr[1], 
           mac_addr[2], 
           mac_addr[3], 
           mac_addr[4], 
           mac_addr[5]
           );
           
  Serial.print("MAC: "); 
  Serial.println(macStr);
  //Armazenando os dados na estrutura criada das leituras do DHT
  memcpy(&dht,data,sizeof(dht));
  //Sinalizando que recebeu os dados via ESP NOW
  is_recv = true;
}
// Busca o canal da rede local por meio do SSID
int32_t get_WiFi_Channel(const char *ssid) {
  if (int32_t n = WiFi.scanNetworks()) {
      for (uint8_t i=0; i<n; i++) {
          if (!strcmp(ssid, WiFi.SSID(i).c_str())) {
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
  //Enquanto o Monitor Serial não for aberto, fique aguardando
  while(!Serial);
  
  delay(1000);
  
  Serial.print("Conectando ao Adafruit IO");
  io.connect();
  
  WiFi.disconnect();
  //Configura o dispositivo como Station e configura o canal 
  WiFi.mode(WIFI_AP_STA);
  ch = get_WiFi_Channel(WIFI_SSID);
  // Configura o SSID do ponto de acesso e o canal
  WiFi.softAP(SSID_ESP_NOW, "", ch, 0);
  
  //Seleciona a rede e a senha 
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  //Enquando estiver conectado, então ...
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(1000);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("Conectado a rede local");

  
  esp_wifi_set_channel(ch,WIFI_SECOND_CHAN_NONE);
  Serial.println("Selecionando canal ESP NOW");
  
  //Inicializa o ESP NOW
  Serial.println("Inicializando ESP NOW");
  if(esp_now_init() != ESP_OK)
  {
    //Caso não seja inicializado corretamente, reseta o dispositivo
    Serial.println("Erro ao iniciar o protocolo ESP NOW");
    Serial.println("Reiniciando Franzininho em 3 segundos");
    delay(3000);
    ESP.restart();
  }
  //Registrando a função de callback para receber os dados
  esp_now_register_recv_cb(on_recv_data);
  
}

void loop() 
{
  io.run();
  //Caso recebe os dados do transmissor, então publicamos os dados na dashboard
  if(is_recv)
  {
    Serial.printf("Temperatura %.2f \n",dht.temp);
    Serial.printf("Umidade %.1f \n",dht.hum);
    // Realizar o request via MQTT para dashboard da Adafruit
    temp_adafruit->save(dht.temp);
    hum_adafruit->save(dht.hum);
    
    is_recv = false;
  }
  delay(50);
}
