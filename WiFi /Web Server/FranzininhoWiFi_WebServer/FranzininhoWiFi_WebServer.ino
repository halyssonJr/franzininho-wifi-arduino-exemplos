#include "WiFi.h"
#include <WebServer.h>

#define LDR_PIN 3
#define MIN_ANALOG 0
#define MAX_ANALOG 4096
#define MIN_PERCENT 0
#define MAX_PERCENT 100

const char *ssid = "brisa-2845162" ;
const char *password = "6c30f3zi";


// Declara o objeto 
WebServer server(80);


int light_percent = 0;

String html_page(int percent)
{
 String index = "<!DOCTYPE html><html>\n";
 index +="<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
 index +="<title>Franzininho WiFi Web Server</title>\n";
 index +="<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
 index +="body{margin-top: 50px;} h1 {color: #37973f;margin: 50px auto 30px;}\n";
 index +="p {font-size: 24px;color: #000000;margin-bottom: 10px;}\n";
 index +="div { background-color: lightgrey;width: 400px;border: 20px solid green;padding: 30px;margin: 20px;}\n";
 index +="</style>\n";
 index +="</head>\n";
 index +="<body>\n";
 index +="<div id=\"franzininho_webpage\">\n";
 index +="<h1>Franzininho WiFi Web Server</h1>\n";
 index +="<p>Intensidade luminosa: ";
 index +=(int)percent;
 index +=" %</p>";
 index +="</div>\n";
 index +="</body>\n";
 index +="</html>\n";
 return index;
}


void handle_on_connect()
{
 server.send(200, "text/html",html_page(light_percent));
}


void handle_not_found()
{
 server.send(404, "text/plain", "Página não encontrada");
}
void setup()
{
 Serial.begin(115200);
 // Configura o driver WiFi para o modo Station
 WiFi.mode(WIFI_STA);
 //Inicializa o driver e conecta-se a rede local
 WiFi.begin(ssid,password);
  delay(5000);

 //Enquanto não conecta-se a rede local imprimir na serial
 while (WiFi.status() !=WL_CONNECTED)
 {
   Serial.println("Aguarde conectar-se a rede ...");
   delay(500);
 }
 // Imprimir o endereço de IP
 Serial.println("Conectado com sucesso !");
 Serial.print("IP: ");
 Serial.println(WiFi.localIP());
 // adiciona a função "handle_on_connect"
 //quando o server estiver online
 server.on("/",handle_on_connect);
  // adiciona a função "handle_not_found"
 // quando o server estiver offline
 server.onNotFound(handle_not_found);
  //Inicia o server
 server.begin();
 // Informa que o Web Server foi inicializado
 Serial.println("Web Server Ativo");
}
void loop()
{
 //Trata as requisições HTTP e sinaliza os eventos
 server.handleClient();
 // Realiza uma conversão do valor de tensão para valor em percentual
 light_percent = map(analogRead(LDR_PIN), MAX_ANALOG,MIN_ANALOG,MIN_PERCENT,MAX_PERCENT) ;
 //Aguarda 5 milisegundos
 delay(5);
}
