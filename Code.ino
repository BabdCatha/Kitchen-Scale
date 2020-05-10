// Copyright (C) 2020  Cyril THOMAS
//
// This program is free software; you can redistribute it and/or
// modify it under the terms of the GNU Library General Public
// License as published by the Free Software Foundation; either
// version 3 of the License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Library General Public License for more details.
//
// You should have received a copy of the GNU Library General Public
// License along with this library; if not, see <https://www.gnu.org/licenses/>.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789
#include <SPI.h>             // Arduino SPI library
#include <Fonts/FreeSerif24pt7b.h> // Font for the display
#include "HX711.h"
 
// ST7789 TFT module connections
#define TFT_CS     D8
#define TFT_RST    D2
#define TFT_DC     D1
#define DOUT       D4
#define CLK        D3
#define INP        D8
 
// Initialize Adafruit ST7789 TFT library
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);

HX711 scale;

float calibrationFactor = 176.64;
float weight = 0;
int displayedWeight = 42;
int hOffset = 0;

int i = 0;
 
void setup(void) {
  Serial.begin(9600);
  tft.init(240, 240, SPI_MODE2);
  tft.setRotation(3);
  tft.fillScreen(ST77XX_BLACK);
  tft.setFont(&FreeSerif24pt7b);
  tft.setTextSize(2);

  pinMode(D8, INPUT);

  scale.begin(DOUT, CLK);
  scale.set_scale(calibrationFactor);
  scale.tare();
}
 
void loop(){

  if(digitalRead(D8) == HIGH){
    scale.tare();
  }

  weight = scale.get_units(12); //Takes the average of 12 readings from the ADC

  if((int)weight != displayedWeight){
    displayedWeight = (int)weight;

    if(displayedWeight < -100){ //To set the horizontal offset for the screen
      hOffset = 0;
    }else if(displayedWeight < -10){
      hOffset = 30;
    }else if(displayedWeight < 0){
      hOffset = 60; 
    }else if(displayedWeight < 10){
      hOffset = 84;
    }else if(displayedWeight < 100){
      hOffset = 60;
    }else{
      hOffset = 30;
    }

    tft.fillRect(0,80,240,90,ST77XX_BLACK);
    tft.setCursor(hOffset,145);
    tft.print(displayedWeight);
    tft.print("g");
  }
}
 
