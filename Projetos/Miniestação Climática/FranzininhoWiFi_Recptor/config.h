#include "AdafruitIO_WiFi.h"

/************************ Adafruit IO Config *******************************/
#define IO_USERNAME "Halysson"
#define IO_KEY "aio_baza04MRGNJ4cxoMQzaPvjyo2zHF"

/******************************* WIFI **************************************/

#define WIFI_SSID "TESLA"
#define WIFI_PASS "passwd39" 

AdafruitIO_WiFi io(IO_USERNAME, IO_KEY, WIFI_SSID, WIFI_PASS);
