/*
  Autor: Equipe Embarcados
*/

// Incluir bibliloteca 
#include <ESP32Servo.h> 

// Declarar as váriaveis
String cmd; 
int angle;

//Declarar o objeto
Servo servo;
//Definir o pino do servo
const int servo_pin = 18;

void setup() {

  // Incializar a Serial e o baudrate
  Serial.begin(115200); 
  // Configura a GPIO e os limites mínimos e máximos em milisegundos
  servo.attach(servo_pin,500, 2400);
  // Move o eixo para o ângulo 0
  servo.write(0);
  // Aguarda 1 segundo
  delay(1000);
  // Imprime na serial 
  Serial.println("Iniciando Projeto com Motor Servo");
  Serial.println(" Insira o angulo: ");
}


void loop()
{
  // Caso receba algum dado na Serial, então ...
  if(Serial.available()>0)
  {
    // Ler todo o buffer da serial antes de chegar no \n( caracter fim de linha)
    cmd = Serial.readStringUntil('\n');
    // Realizar um cast de String para um valor inteiro
    angle = atoi(cmd.c_str());
    // Imprimir um caracter "">>"
    Serial.print(">> ");
    // Se o ângulo for maior que 0 e menor que 180, então ...
    if(angle >=0 && angle <=180)
    {
      // Imprime o valor do ângulo na tela
      Serial.println(angle);
      // Move o eixo do motor para o ângulo desejado
      servo.write(angle);
      
    }
    // Caso não seja um valor dentro dos limites então...
    else
    {
      // Imprimie na saída do monitor serial
      Serial.println ("Valor inválido");
    }
    
  }
}
