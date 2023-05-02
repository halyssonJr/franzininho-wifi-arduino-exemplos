
#include <OneWire.h>
#include <DallasTemperature.h>

// Definir qual pino está conectado o sensor DS18B20
#define DS_18B20 3

//Declara construtor para responsável pela comunicação 
OneWire one_wire(DS_18B20);

//Declara construtor para responsável pela leitura do sensor DS18B20
DallasTemperature sensor(&one_wire);

//Declara as variáveis

int num_sensor;

uint8_t addr[8];

float temp;

void setup() 
{
  // Inicializa a comunicação Serial em 115200 bps
  Serial.begin(115200);
  delay(2000);
  Serial.println("Iniciando a demo do DS18B20...");
  
  // Inicia a configura a GPIO 
  sensor.begin();
  
  
  Serial.println("Procurando sensores ...");
  // Verifica a quantidade de sensores conectados ao pino
  num_sensor = sensor.getDeviceCount();
  //Caso tenha encontrado algum sensor
  if(num_sensor >0)
  {
    Serial.print("Encontrou  ");
    Serial.print(num_sensor);
    Serial.println(" sensores");
  }
  // Caso não tenha encontrado nenhum sensor
  else
  {
    Serial.println(" Não encontrou o(s) sensor(es)");
    return;
  }
  
  // Identifica o endereço do sensor
  if(one_wire.search(addr))
  {
    Serial.print("Endereço : ");
    //Procura apenas o primeiro endereço dos sensores
    for(int i =0; i<8; i++)
    {
      Serial.print("0x");
      if(addr[i]<0x10) Serial.print("0");
      Serial.print(addr[i],HEX);
      if(i < 7) Serial.print(":");
    }
    Serial.println();
  }
}

void loop() {
 
  //Envia comando para realizar a leitura de temperatura
  sensor.requestTemperatures();

  // Realiza as leturas de temperatura
  temp = sensor.getTempCByIndex(0);

  //Caso receba o valor valido de temperatura
  if(temp != DEVICE_DISCONNECTED_C)
  {
    Serial.print("Temperatura - ");
    Serial.print(temp);
    Serial.println("C");
  }
  //Caso valor de temperatura não válido
  else
  {
    Serial.println("[Erro] : Não foi possível realizar a leitura do sensor");
  }
  delay(1000);
}
