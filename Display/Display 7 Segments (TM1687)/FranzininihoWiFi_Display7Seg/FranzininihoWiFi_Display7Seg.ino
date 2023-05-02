
#include <TM1637Display.h>

#define SCL 9
#define SDA 8

// Declara o objeto 
TM1637Display display ( SCL, SDA, DEFAULT_BIT_DELAY) ;

const uint8_t ret[] =
{
  SEG_A|SEG_F|SEG_E|SEG_D,
  SEG_A|SEG_D,
  SEG_A|SEG_D,
  SEG_A|SEG_B|SEG_C|SEG_D
};

const uint8_t one_seg[]
{
  SEG_G
};
const uint8_t turn_off_one_seg[]
{
  0x00
};

void animation()
{
  int i;
  
   for(i = 0; i<4;i++)
  {
     display.setSegments(one_seg,1,i); 
     delay(500);
     display.setSegments(turn_off_one_seg,1,i);
  }
  for(i =3 ;i>=0;i--)
  {
     display.setSegments(one_seg,1,i); 
     delay(500);
     display.setSegments(turn_off_one_seg,1,i);
  }
}
void setup() 
{
  // Incializa a comunicação serial na velocidade de 115200 bps
  Serial.begin(115200);
 
  // 0 -7 a intensidade luminosa do display
  display. setBrightness (5);
  // Limpa tela do display
  display.clear(); 

  // Formato para mostrar hora e minuto
  int time_in_display = (14 * 100)+ 15;
  display.showNumberDecEx(time_in_display, 0b11100000,true);
  delay(1500);
  display.clear();
  
  // Criar um retangulo 
  display.setSegments(ret,4,0);
  delay(1500);
  display.clear();

  
  display.showNumberDec(-10);  
  delay(1500);
  display.clear();
  
  display.showNumberDec(15, true); 
  delay(1500);
  display.clear();
  
  display.showNumberDec(20, false, 2, 1);  // Mostra na tela _10_
  delay(2000);
  display.clear();
}

void loop() 
{
 
   animation();
   
}
