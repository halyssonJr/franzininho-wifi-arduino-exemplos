/*
  Autor: Equipe Embarcados
*/
#include <WiFi.h>

const char *ssid = "nome da rede";
const char *password = "senha da rede";

// Comente essa definição caso deseje configurar manualmente
#define CONFIG_MANUAL


#ifdef CONFIG_MANUAL
// Configurar o IP (Internet Protocol)
IPAddress static_ip(192,168,1,22);
// Configurar o gateway
IPAddress gateway(192,168,1,9);
// Configurar o subnet 
IPAddress subnet(255,255,255,0);
#endif

void setup() 
{
  Serial.begin(115200);
  Serial.println("Iniciando Franzininho Station");

  // Inicializar a configurações de redes manualmente
  #ifdef CONFIG_MANUAL
   WiFi.config(static_ip,gateway,subnet);
  #endif

  WiFi.begin(ssid,password);
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.println( "Aguarde conectar ...");
    delay(500);
  }
  
  Serial.println();
  Serial.println(" *** Informações da Rede ***");

  Serial.print(" - Endereço IP: ");
  Serial.println(WiFi.localIP());
  Serial.print(" - Gateway : ");
  Serial.println(WiFi.gatewayIP().toString().c_str());
  Serial.print(" - Subnet: ");
  Serial.println(WiFi.subnetMask().toString().c_str());
}

void loop() 
{
 
}
