/*

Autor : Equipe Embarcados

*/

#include "USB.h"
#include "USBHIDKeyboard.h"
// Declara o objeto 
USBHIDKeyboard Keyboard;

//Define qual pino do botão
#define BUTTON_PIN 3

//Define qual sistema operacional está utilizando no momento
#define LINUX_OS 
//#define WIN_OS

//Declarando Variáveis globais
int previousButtonState = HIGH;   
int buttonState;

void setup() 
{
  // Configa pino saída digital e habilita o resistor de pull up interno
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  
  //Inicializa a interface HID - Teclado
  Keyboard.begin();
  USB.begin();
}

void loop() {

  //Realiza a leitura do botão
  buttonState = digitalRead(BUTTON_PIN);
    
  //Caso o estado do botão for diferente do estado anterior e a leitura do botão
  // for um nível lógico baixo, então ...
  if ((buttonState != previousButtonState) && (buttonState == LOW)) 
  {
   #ifdef LINUX_OS
    Keyboard.press(KEY_LEFT_CTRL); // Pressiona o botão CTRL
    Keyboard.press(KEY_LEFT_ALT);  // Pressiona o botão ALT
    Keyboard.press('t');          //  Pressiona o botão t
    Keyboard.releaseAll();        // Para de pressionar os botões
    delay(500);                 // Aguarda até que o terminal seja aberto
    Keyboard.print("sudo apt -y update && sudo apt upgrade"); // Escreve o comando no terminal
    Keyboard.write(KEY_RETURN); // Pressiona Enter 

    delay(500); // Aguarda para habilitar o campo de senha, caso tenha
    Keyboard.print("sua senha"); // Insere sua senha
    Keyboard.write(KEY_RETURN);// Escreve Enter
    #endif
    
    #ifdef WIN_OS
    Keyboard.press(KEY_LEFT_GUI); // Pressiona botão WIN
    Keyboard.press('r'); // Pressiona a letra r
    Keyboard.releaseAll(); // Para de pressionar essa 
    delay(500); // Aguarda o campo de texto abrir
    Keyboard.print("cmd"); // Escreve cmd
    Keyboard.write(KEY_RETURN);// Pressiona Enter
    delay(500);// Aguarda o terminal abrir
    Keyboard.print("winget upgrade --all"); // Escreve o comando de atualização 
    Keyboard.write(KEY_RETURN);//Pressioan Enter
    #endif
  }
  // Salva estado atual do botão
  previousButtonState = buttonState;
}
