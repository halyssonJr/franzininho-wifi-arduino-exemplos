

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// Inclui o arquivo de fontes
#include <Fonts/FreeSansBoldOblique9pt7b.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


void setup() 
{
 
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  delay(2000);

  display.clearDisplay();
  display.setFont(&FreeSansBoldOblique9pt7b);
  Serial.println("Clear display");  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(12,14);
  display.print("Franzininho");
  display.setCursor(35,30);
  display.print("WiFi");
  display.display();
  
 
}

void loop() 
{
 display.invertDisplay(true);
 delay(500); 
 display.invertDisplay(false);
 delay(500); 
}
