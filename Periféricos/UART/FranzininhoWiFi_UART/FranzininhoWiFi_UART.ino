/*
  Autor: Equipe Embarcados
*/
const byte led_blue =  33;
int data_serial;

void setup() 
{
  
  Serial.begin(115200);
  Serial.println("Iniciando Franzininho WiFi ...");

  pinMode(led_blue, OUTPUT);  
}

void loop() 
{
  if(Serial.available()>0)
  {
    data_serial = Serial.read();
    
    if(data_serial == 'H')
    {
      Serial.println("Comando Help :");
      Serial.print("1 - Liga o LED");
      Serial.print("0 - Desliga o LED");

    }
    else if (data_serial == '1')
    {
      digitalWrite(led_blue, HIGH);
    }
    else if (data_serial == '0')
    {
      digitalWrite(led_blue, LOW);
    }
    else 
    {
      Serial.println("Comando Invalido, tente novamente");
    }
  }

}
