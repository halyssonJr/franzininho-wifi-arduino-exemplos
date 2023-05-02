/*
  Autor : Equipe Embarcados
*/

// Inclui a biblioteca do DHT
#include "DHT.h"

// Define o pino  
#define DHTPIN 4

// Declara o construtor e insere os pâmetros
DHT dht22(DHTPIN, DHT22);

// Estrutura de dados para receber as váriaveis 
// relacionadas ao DHT22
struct data_t
{ 
   float hum;
   float temp;
};

//Delcara a estrutura de dados
struct data_t  dht;




void setup() 
{
  // Inicializa comunicação Serial com baudrate de 115200 bps
  Serial.begin(115200);
  // Inicializa o driver One Wire para comunica-se com o sensor
  dht22.begin();
}

void loop() 
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
    
    Serial.print(F("#Temperatura: "));
    Serial.print(dht.temp);
    Serial.println(F(" °C "));
  } 
// Aguarda 2 segundos
  delay(2000);
}
