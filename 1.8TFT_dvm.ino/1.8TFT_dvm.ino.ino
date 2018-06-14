#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>

//how to wire from chinese H340 based USB nano.
//vcc to 5v
//gnd to gnd by 6 pin header
//LED+ to 3.3 V
//LED- to GND by 6 pin header


// For the breakout, you can use any 2 or 3 pins
// These pins will also work for the 1.8" TFT shield
#define cs    4  //don't use cs by the LED+, wrong cs port, but you knew that
#define sclk  5  // sck on board, you knew that
#define mosi  6  // sda connect on board, but you knew that
                      
#define dc    7 // ao connect on board, you knew that as well
#define rst   8

int sensorPin = A1;
float sensorValue = 0;
float multiplier = 1.1;
float displayValue = 0;


Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, mosi, sclk, rst);

void setup(void) {


  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab



tft.fillScreen(0x0000);


tft.setCursor(2,7);
tft.setTextSize(1);
tft.setTextColor(0xFFF,0x0000);
tft.setTextWrap(true);
tft.print ("value read");
tft.setTextSize(2);
}



void loop() 

{
sensorValue = analogRead(sensorPin);
displayValue = (sensorValue * multiplier);
tft.setCursor(30,30);

tft.print(displayValue);
delay(1);
}

