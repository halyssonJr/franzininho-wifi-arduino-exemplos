/*
Autor: Equipe Embarcados
*/

//Incluindo biblioteca WiFi Manager
#include <WiFiManager.h> 

//Declara o objeto da classe
WiFiManager wm;

// Adicionando campo de texto utilizando HTML
const char* custom_str = "<br/><label for='opt1'>Campo Customizado: </label><br>"
                           "<input type='text' name='opt1'><br>";
                           
//Declara o objeto da classe 
WiFiManagerParameter field (custom_str);

bool ret;

//Função para tratar o texto inserido no campo customizado
void save_param_network()
{
  String value;
  if(wm.server->hasArg("opt1"))
  {
    value = wm.server->arg("opt1");
  }
  Serial.println( "[Campo Customizado] função de callback");
  Serial.print("Recebeu o valor: ");
  Serial.println(value);
  
}
void setup() 
{
  Serial.begin(115200);
  
  // Serial.setDebugOutput(true); // Habilita o debug para vizualizar as acões do WiFi  
  delay(1000);
  Serial.println("Iniciando demo com WiFi Manager");
  
  WiFi.mode(WIFI_STA);

  wm.addParameter(&field);
  wm.setSaveParamsCallback(save_param_network);

  //wm.setClass(""); // Tema Claro
  wm.setClass("invert"); // Tema Escuro
  
  //Seleciona a qualidade do sinal desejado durante o scanner
  //wm.setMinimumSignalQuality(10);

  //ret = wm.autoConnect("Franzininho WiFi",""); // Ap sem senha
  //ret = wm.autoConnect();// AP sem SSID e sem senha
  ret = wm.autoConnect("Franzininho WiFi","franz1234"); // AP com SSID customizado e senha
  
  if(!ret)
  {
    Serial.println("Falha ao entrar na rede WiFi ");
  }
  else 
  {
    Serial.println("Conectado a rede");
  }
}

void loop() {

}
