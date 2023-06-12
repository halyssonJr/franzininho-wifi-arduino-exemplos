/*
  Autor:Equipe Embarcado
*/

//Inclusão de biblioteca
#include "USB.h"
#include "USBMSC.h"
#include "FirmwareMSC.h"

//Caso esteja no modo CDC, então ..
#if ARDUINO_USB_CDC_ON_BOOT
  #define HWSerial Serial0
  #define USBSerial Serial

//Caso contrario, então ...
#else
  #define HWSerial Serial
  USBCDC USBSerial;
#endif

//Define a velocidade da comunicação Serial em 115200 bps
#define BAUD_RATE 115200

FirmwareMSC MSC;

String TAG = "[FRANZININHO MSC] ";

void ubs_event_cb(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
 if(event_base == ARDUINO_FIRMWARE_MSC_EVENTS)
 {    
    arduino_firmware_msc_event_data_t * data = (arduino_firmware_msc_event_data_t*)event_data;
    
    switch (event_id)
    {
      case ARDUINO_FIRMWARE_MSC_START_EVENT:
        HWSerial.println(TAG + "Inicializando atualização ");
        break;
        
      case ARDUINO_FIRMWARE_MSC_WRITE_EVENT:
        HWSerial.println(TAG + "Escrevendo FW ...");
        break;
        
      case ARDUINO_FIRMWARE_MSC_END_EVENT:
        HWSerial.print(TAG);
        HWSerial.printf("Atulização finalizada: %u bytes\n", data->end.size);
        break;
        
      case ARDUINO_FIRMWARE_MSC_ERROR_EVENT:
        HWSerial.printf("Erro na atualização! Progresso: %u bytes\n", data->error.size);
        break;
         
      default:
        break;
    }
  }
}

void setup() 
{
  //Inicializa a comunicação Serial
  HWSerial.begin(BAUD_RATE);
  //Habilita o debug 
  HWSerial.setDebugOutput(true);
  
  //Registrando a função de callback
  USB.onEvent(ubs_event_cb);
 
  // Inicializar o periférico da USB para configuração MSC
  MSC.begin();
  USBSerial.begin();
  USB.begin();
  
}

void loop() 
{
  
}
