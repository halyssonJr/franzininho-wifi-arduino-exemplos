/*
 Autor: Equipe Embarcados
*/

// Inclusão a biblioteca USB
#include "USB.h"

USBCDC USBSerial; // Declara o objeto da class USBCDC

String TAG = "[CDC FRANZININHO]";

// Função de callback para tratar os eventos sinalizado durante o uso da USB
static void usb_event_callback(void* arg, esp_event_base_t event_base, int32_t event_id, void* event_data)
{
  
  if(event_base == ARDUINO_USB_CDC_EVENTS)
  {
    arduino_usb_cdc_event_data_t * data = (arduino_usb_cdc_event_data_t*)event_data;
    switch (event_id)
    {
      case ARDUINO_USB_CDC_CONNECTED_EVENT:
        Serial.println(TAG +" Conectado");
        break;
      case ARDUINO_USB_CDC_DISCONNECTED_EVENT:
        Serial.println(TAG + " Disconectada");
        break;
      case ARDUINO_USB_CDC_LINE_STATE_EVENT:
          ;
        break;
      case ARDUINO_USB_CDC_LINE_CODING_EVENT:
        Serial.print(TAG);
        Serial.printf(" baund rate: %u, data bits: %u, stop bits : %u, pariade: %u\n", data->line_coding.bit_rate, data->line_coding.data_bits, data->line_coding.stop_bits, data->line_coding.parity);
        break;
      case ARDUINO_USB_CDC_RX_EVENT:
      
        Serial.print(TAG);
        Serial.printf(" RX[%u] - ", data->rx.len);
        {
          uint8_t buf[data->rx.len];
          size_t len = USBSerial.read(buf, data->rx.len);
          Serial.write(buf, len);
        }        
        Serial.println();
        
        break;
       case ARDUINO_USB_CDC_RX_OVERFLOW_EVENT:
        Serial.print(TAG);
        Serial.printf(" Overflow de %d bytes\n", data->rx_overflow.dropped_bytes);
        break;
     
      default:
      
        break;
    }
  }
}

void setup() 
{
  // Configura a Serial para velocidade de 115200 bps
  Serial.begin(115200);
  // Habilita o debug dos eventos da Serial
  Serial.setDebugOutput(true);
  
  // Registra a função de callback para detectar os eventos da USB
  USB.onEvent(usb_event_callback); 
  USBSerial.onEvent(usb_event_callback); 
  
  // Inicializa o driver da USB 
  USBSerial.begin();
  USB.begin();
}

void loop() 
{
  while(Serial.available()) // Enquando houve bytes trafegando pela serial, então ...
  {
    size_t len = Serial.available(); // Armazena o número de bytes que recebeu pela USB
    uint8_t usb_buffer[len]; // Declara a variável para receber os bytes da Serial
    len = Serial.read(usb_buffer, len); // Armazena no buffer os bytes recebidos
    USBSerial.write(usb_buffer, len);  //  Método para receber 
  }
}
