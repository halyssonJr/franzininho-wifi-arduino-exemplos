/*
 Autor: Equipe Embarcados
*/

//Incluir a biblioteca para ESP_NOW
#include <esp_now.h>
#include <esp_wifi.h>
#include <WiFi.h>

#define CHANNEL 1 // Canais disponíveis de 1-15

uint64_t last_time = 0; 

void on_recv_data (const uint8_t * mac_addr, const uint8_t *data, int data_len)
{
  char mac_str[20];
  char msg[] = {"Oi Franzininho, estou bem, obrigado !"};
  
  snprintf(mac_str, 
           sizeof(mac_str), 
           "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0],mac_addr[1],
           mac_addr[2],mac_addr[3], 
           mac_addr[4],mac_addr[5]
           );
  // Mostra o endereço MAC de quem recebeu a mensagem          
  Serial.print("MAC: "); 
  Serial.println(mac_str);
  
  // Mostra a mensagem enviada o tamannho dela
  Serial.printf("Dado : %s | len : %d \n",(char *)data,data_len);
  
  // Mostra o último dado recebido desde do monto que ligou
  Serial.print("Última mensagem enviada: ");
  Serial.print((millis() - last_time)/1000);
  Serial.println(" segundos");

  esp_now_send(mac_addr, (uint8_t*)msg, sizeof(msg));
  last_time = millis();
}


void setup() 
{
  Serial.begin (115200);
  Serial.println("Configurando o ESP NOW");
  
  WiFi.disconnect();
  
  //Configurando em modo Station
  WiFi.mode(WIFI_STA);
  esp_wifi_set_channel(CHANNEL, WIFI_SECOND_CHAN_NONE);
  Serial.println("Inicializa ESP NOW");
  
  // Caso não seja inicializado corretamente, então ...
  if( esp_now_init() != ESP_OK)
  {
    Serial.println("Erro ao inicializar o ESP NOW ");
    Serial.println("Reiniciando Franzininho em 3 segundos ...");
    delay(3000);
    ESP.restart();
  }

  esp_now_register_recv_cb (on_recv_data);
}

void loop() 
{
 
}
