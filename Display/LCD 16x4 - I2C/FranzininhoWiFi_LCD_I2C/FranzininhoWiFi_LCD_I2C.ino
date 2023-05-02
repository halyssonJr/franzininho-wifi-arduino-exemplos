/*
  Autor: Equipe Embarcados
*/

//Incluir biblioteca
#include <LiquidCrystal_I2C.h>


#define COL 16 // Número de coluna 
#define ROW 2 //Números de linhas
#define ADDR_I2C 0x27 // Endereço I2C

// Declarando construtor
LiquidCrystal_I2C lcd(ADDR_I2C,COL,ROW); 

 // Declarando váriaveis
const char sentence_cursor_on[] = "Cursor On";
const char sentence_cursor_off[] = "Cursor Off";

int i ;
size_t sz;

void setup() 
{
  //Inicia a comunicação I2C
  lcd.init();
  //Limpa a tela
  lcd.clear();
  //Habilita o backlight         
  lcd.backlight();      
  
  //Posiciona na coluna 2 e linha 0 o cursor na tela
  lcd.setCursor(2,0);   
  //Imprime na tela a frase "Franzininho"
  lcd.print("Franzininho");

  //Posiciona na coluna 5 e linha 1 cursor na tela
  lcd.setCursor(5,1); 
  //Imprime na tela a frase "WiFi"
  lcd.print("WiFi");
  delay(5000);
 
  //Limpa a tela
  lcd.clear();
  //Habilita o curso para piscar na tela
  lcd.blink();
  
  //Recebe a quantidade de caracteres da frase
  sz = strlen(sentence_cursor_on);
  //Posiciona o cursor para a linha zero e coluna zero
  lcd.home();
  //Escreve caracter por caracter 
  for(i = 0; i<sz; i++)
  {
    lcd.print(sentence_cursor_on[i]);
    delay(500);
  }
  delay(2000);
  
  //Limpa a tela
  lcd.clear();
  //Desabilitar o cursor
  lcd.noBlink();
  
  //Recebe a quantidade de caracteres da frase
  sz = strlen(sentence_cursor_off);
  //Posiciona o cursor para a linha zero e coluna zero
  lcd.home();
  //Escreve caracter por caracter 
  for(i = 0; i<sz; i++)
  {
    lcd.print(sentence_cursor_off[i]);
    delay(500);
  }
  delay(2000);

  //Desabilita o backlight
  lcd.noBacklight();
  delay(1000);
  //Habilita o backlight         
  lcd.backlight();
  
  delay(2000);
  //Limpa a tela
  lcd.clear();
  //Posiciona o cursor na tela
  lcd.setCursor(1,0);   
  //Imprime na tela a frase "Fim Demo"
  lcd.print("Fim Demo ;)");   
}
void loop() {
  
}
