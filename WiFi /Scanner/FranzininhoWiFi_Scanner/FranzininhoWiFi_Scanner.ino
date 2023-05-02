// Inclui biblioteca
#include <WiFi.h>

int num_network;

void setup() 
{
  //Inicializa e configura a comunicação Serial
  Serial.begin(115200);
  //Inicializa o drive do WiFi
  WiFi.begin();
  // Configura o drive WiFi para modo Station
  WiFi.mode(WIFI_STA);
  // Aguarda 1 segundo
  delay(1000);
}

void loop() 
{

  Serial.println("Iniciando scanner ...");
  // Inicia o scanner da rede
  WiFi.scanNetworks();
  // Verifica a quantidade de redes disponíveis
  num_network =WiFi.scanComplete();

  //Caso tenha encontrado alguma rede, então...
  if(num_network >0)
  {
    
    Serial.println("*** Rede(s) Wi-Fi *** ");
    Serial.println();

    // Entra no laço de repetição, para imprimir as informações
    // de cada rede encontrada 
    for(int j = 0; j < num_network; j++)
    {
      Serial.print(j+1);
      Serial.print(") ");
      Serial.print(" SSID: " + WiFi.SSID(j));
      Serial.print(" | Canal: ");
      Serial.print(WiFi.channel(j));
      Serial.print(" | RSSI: ");
      Serial.print(WiFi.RSSI(j));
      Serial.print(" (dBm) ");
      Serial.print(" | BSSID: "+ WiFi.BSSIDstr(j));
      Serial.print(" | Wi-Fi Aberto: ");
      Serial.print(WiFi.encryptionType(j) == WIFI_AUTH_OPEN ? "sim": "não");
      Serial.println();
      delay(10);
    }
    Serial.println();
  }
  // Caso não encontre a rede Wi-Fi, então ... 
  else
  {
    Serial.println("Sem rede Wi-Fi");
  }
  
  //Aguarde 3 segundos
  delay(3000);  

}
