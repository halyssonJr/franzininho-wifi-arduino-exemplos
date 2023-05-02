/*
  Autor: Equipe Embarcados
  
*/
#define ENA 37 
#define MS1 36 
#define MS2 36 
#define MS3 34 
#define RST 33 
#define SLP 29 
#define STP 21 
#define DIR 18 
 
 // Passo suportados pelo driver  
#define FULL      0
#define HALF      1
#define QUARTER   2
#define EIGHTH    3
#define SIXTENNTH 4

//Declaração da quantidade de passos
int step;

// Função responsável por configurar os passos do motor 
void steppers (uint8_t stp)
{
 

  switch (stp)
  {
    case FULL:
      digitalWrite(MS1, LOW);  
      digitalWrite(MS2, LOW);
      digitalWrite(MS3, LOW);
      step = 200;
    break;

    case HALF:
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, LOW);
      digitalWrite(MS3, LOW);
      step = 400;
    break;

    case QUARTER:
      digitalWrite(MS1, LOW);
      digitalWrite(MS2, HIGH);
      digitalWrite(MS3, LOW);
      step = 800;
    break;

    case EIGHTH:
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, HIGH);
      digitalWrite(MS3, LOW);
      step = 1600;
    break;

    case SIXTENNTH:
      digitalWrite(MS1, HIGH);
      digitalWrite(MS2, HIGH);
      digitalWrite(MS3, HIGH);
      step = 3200;
    break;
    default:
      step = -1;
    break;
  }

  Serial.println("### Motor de passo configurado ###");
  Serial.print(step);
  Serial.println(" passo por revolução");
}

void setup() 
{
  // Inilizando comunicação serial
  Serial.begin(115200);
  delay(1000);
  Serial.println("Iniciando demo Motor de Passo com a Franzininho WiFi");

  // Configura as GPIOs como saída digital
  pinMode(ENA,OUTPUT);
  pinMode(MS1,OUTPUT);
  pinMode(MS2,OUTPUT);
  pinMode(MS3,OUTPUT);
  pinMode(RST,OUTPUT);
  pinMode(SLP,OUTPUT);
  pinMode(STP,OUTPUT);
  pinMode(DIR,OUTPUT);
  
  // Configura o driver
  digitalWrite(SLP, LOW);   // Sleep desabilitado
  digitalWrite(RST, HIGH);  // RST habilitado 

  steppers(FULL);

  // Caso não seja suportado pelo driver
  if(step <0)
  {
    
    Serial.println("Opção de passo incorreto");
    return;
  }
}

void loop() 
{

  int i;

  digitalWrite(DIR, HIGH);  // Sentido Horário 
  
  for(i =0; i< step; step++)
  {
    digitalWrite(STP, HIGH);
    delayMicroseconds(500);
    digitalWrite(STP, LOW);
    delayMicroseconds(500);
  }
  delay(1000);

  digitalWrite(DIR, LOW);  // Sentido antihorário 
  
  for(i =0; i< step; step++)
  {
    digitalWrite(STP, HIGH);
    delayMicroseconds(500);
    digitalWrite(STP, LOW);
    delayMicroseconds(500);
  }

  delay(1000);
}
