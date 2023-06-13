#include "RMaker.h"
#include "WiFi.h"
#include "WiFiProv.h"
#include "AppInsights.h"

#define TIME_UPDATE_MS 5000

const char *service_name = "Franzininho WiFi - Prov";
const char *pop = "opensource";

//static uint8_t bs18b20_pin = 46;
static uint8_t relay_pin = 2;
bool relay_pin_state = false;

bool wifi_connected = 0;


//Declare devices
static TemperatureSensor temperature("Temperatura");
static Switch relay("Relay", &relay_pin);
float temp = 25.00;

void sysProvEvent(arduino_event_t *sys_event)
{
    switch (sys_event->event_id) {
        case ARDUINO_EVENT_PROV_START:
        
            Serial.printf("\nProvisioning Started with name \"%s\" and PoP \"%s\" on SoftAP\n", service_name, pop);
            printQR(service_name, pop, "softap");

            break;
        case ARDUINO_EVENT_PROV_INIT:
            wifi_prov_mgr_disable_auto_stop(10000);
            break;
        case ARDUINO_EVENT_PROV_CRED_SUCCESS:
            wifi_prov_mgr_stop_provisioning();
            break;
        default:;
    }
}

void write_callback(Device *device, Param *param, const param_val_t val, void *priv_data, write_ctx_t *ctx)
{
  //Busca pelo nome do dispositivo
  const char *device_name = device->getDeviceName();
  Serial.println(device_name);
  // Busca pelos parâmetros do dispositivo
  const char *param_name = param->getParamName();

  // Caso o nome do dispositivo seja um relé (Relay), então ...
  if (!strcmp(device_name, "Relay"))
  {
    //Caso o parametro do dispositivo for Power, então ..
    if (!strcmp(param_name, "Power"))
    {
      Serial.printf("Valor recebido = %s for %s - %s\n", val.val.b ? "true" : "false", device_name, param_name);
      // Receb o valor enviado pelo aplicativo (true ou false)
      relay_pin_state = val.val.b;
      
      // Se o valor foi falso liga e caso contrário liga o relé
      (relay_pin_state == false) ? digitalWrite(relay_pin, LOW) : digitalWrite(relay_pin, HIGH);
      
      // Atualiza o valor da variável
      param->updateAndReport(val);
    }
  }
}


void setup()
{
  Serial.begin(115200);
  delay(5000);
  pinMode(relay_pin, OUTPUT);
  digitalWrite(relay_pin,LOW);
  
  //Declara o nó
  Node my_node;
  my_node = RMaker.initNode("Franzininho WiFi RainMaker");

  // Registra a função de callback para o botão
  relay.addCb(write_callback);

  //Adicionando dispositivos (sensores ou atuadores) ao nó
  my_node.addDevice(temperature);
  my_node.addDevice(relay);


  
  // RMaker.setTimeZone("America/Sao_Paulo");
  // Habilita o fuso horario e deixa o smartphone se encarregar de selecionar
  RMaker.enableTZService();
  RMaker.enableSchedule();
  RMaker.enableScenes(); 

  initAppInsights();
  RMaker.enableSystemService(SYSTEM_SERV_FLAGS_ALL, 2, 2, 2);

  // Inicializa o Rain Maker
  RMaker.start();
  Serial.println(" Franzininho WiFi - RainMaker");
  
  //Registra a função de callback para receber os eventos do Provision
  WiFi.onEvent(sysProvEvent);
  Serial.println(" Franzininho WiFi - onEvent");
  
  // Inicializa o provisoring para ficar visível o nome da rede para acessar pelo aplicativo.
  WiFiProv.beginProvision(WIFI_PROV_SCHEME_SOFTAP, WIFI_PROV_SCHEME_HANDLER_NONE, WIFI_PROV_SECURITY_1, pop, service_name);
  Serial.println(" Franzininho WiFi - Provisoring");

}

uint64_t elipse_time = 0;

void loop()
{
  if ((millis() - elipse_time)> TIME_UPDATE_MS && wifi_connected) 
  {                    
    Serial.println("Enviando Temperatura Atual");
    temperature.updateAndReportParam("Temperatura", temp);
    elipse_time = millis();                       
  }

 
  delay(100);
}
