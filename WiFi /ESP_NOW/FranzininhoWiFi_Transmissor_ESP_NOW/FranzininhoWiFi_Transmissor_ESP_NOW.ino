/*
 Autor: Equipe Embarcados
*/

//Incluir a biblioteca para ESP_NOW
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

#define CHANNEL 1 // Canais disponíveis de 1-15

//Substitua de acordo com o seu ESP 32
uint8_t franzininho_recv[] = {0xFF,0xFF,0xFF,0xFF,0xFF,0xFF}; 
esp_now_peer_info_t peerInfo;

void on_sent_data (const uint8_t *mac_addr, esp_now_send_status_t status)
{
  char mac_str[20];
  snprintf(mac_str, 
           sizeof(mac_str), 
           "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0],mac_addr[1],
           mac_addr[2],mac_addr[3], 
           mac_addr[4],mac_addr[5]
           );
  Serial.print("MAC Destino: "); 
  Serial.println(mac_str);
}
void setup() 
{
  Serial.begin (115200);
  Serial.println("Configurando o ESP NOW");
  delay(1000);  

  // Configurando o WiFi no modo Station
  WiFi.mode(WIFI_STA);

  // Iniciando ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar o ESP-NOW");
    return;
  }

  esp_now_register_send_cb(on_sent_data);
  
  // Registrando o novo endereço
  memcpy(peerInfo.peer_addr, franzininho_recv, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Adicionando novo dispositivo na lista de endereços do ESP-NOW       
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Falha ao adicionar");
    return;
  }
}

void loop() 
{
  char msg[] = "Olá Franzininho, tudo bem?";
  //Enviamos os dados para o endereço adiconado anteriormenre 
  esp_err_t result = esp_now_send(franzininho_recv, (uint8_t*)msg, sizeof(msg));
  
  if(result != ESP_OK)
  {
    Serial.println("Falha ao enviar a mensagens");
  }
  else
  {
    
    Serial.println("Mensagem enviada com sucesso");
  }
  delay(5000);
}
