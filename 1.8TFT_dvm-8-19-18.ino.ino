#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library
#include <SPI.h>



//how to wire no name clone TFT from clone H340 based USB nano.
//vcc to 5v
//gnd to gnd by 6 pin header
//LED+ to 3.3 V
//LED- to GND by 6 pin header

#define cs    4  //on TFT board, don't use cs by the LED+, use the other CS one, but you knew that
#define sclk  5  // connect to sck pin on TFT display board, you knew that
#define mosi  6  // connect to sda pin on TFT board, but you knew that
                      
#define dc    7 // connect to ao pin on clone board, you knew that as well
#define rst   8

//int sensorPin = A1;// test VU with this pin, voltages 0V to about 4.08V
int posPin = A4; // read positive voltages from the real world

int negPin = A2; // read negative voltages from the real world

int polarityPin = A6; // sense if incoming signal is positive or  negative DC


float posValue = 0;
float negValue = 0;
int   polarityValue = 0;
float sensorValue = 0;
float multiplier = .012;
//float multiplier = .004;
float displayValue = 0;
float displayValue2 = 0;
byte meterx = 0;
byte lastState = 0;
byte yValueStart = 64;
byte yValue = 64;
byte lasty = 0;
int Center = 512; //reads 0 on VU

//array for yTravel

byte yOffset[ ] = {15,12,10,8,7,5,4,1,0};

Adafruit_ST7735 tft = Adafruit_ST7735(cs, dc, mosi, sclk, rst);

void setup(void) {

Serial.begin(9600);

  //built in LED
pinMode(LED_BUILTIN, OUTPUT);


//We are using adafruit 4040 4.096 precision ref
analogReference(EXTERNAL);

  // Use this initializer if you're using a 1.8" TFT
  tft.initR(INITR_BLACKTAB);   // initialize a ST7735S chip, black tab



tft.fillScreen(0x0000);


tft.setCursor(2,7);
tft.setTextSize(1);
tft.setTextColor(0xFFF,0x0000);
tft.setTextWrap(true);
tft.print ("Charhead Synth DVM");
tft.setTextSize(2);

//top of vu
tft.drawLine(0,50,128,50,0x07E0);
tft.drawLine(0,51,128,51,0x07E0);
tft.drawLine(0,52,128,52,0x07E0);


//scale on the VU
tft.drawChar(13,65,'-',0x07E0,0x0000,1);
tft.drawChar(19,65,'9',0x07E0,0x0000,1);
tft.drawChar(25,65,'V',0x07E0,0x0000,1);
tft.drawChar(62,54,'0',0x07E0,0x0000,1);
tft.drawChar(102,68,'+',0x07E0,0x0000,1);
tft.drawChar(108,68,'9',0x07E0,0x0000,1);
tft.drawChar(114,68,'V',0x07E0,0x0000,1);


//bottom of VU needle
tft.drawCircle(64,127,2,0xFFF);

//top of vu--green line
tft.drawLine(0,50,128,50,0x07E0);
tft.drawLine(0,51,128,51,0x07E0);
tft.drawLine(0,52,128,52,0x07E0);

//bottom of VU -- green line
tft.drawLine(20,140,108,140,0x07E0);
tft.drawLine(20,141,108,141,0x07E0);
tft.drawLine(20,142,108,142,0x07E0);

}



void loop() 

{

//what are we sensing--pos or negative

polarityValue = analogRead(polarityPin);
  
if (polarityValue > 0)
{ 
  digitalWrite(LED_BUILTIN, HIGH);
  pinMode(12,INPUT); // hiz for this pin
  pinMode(3,OUTPUT);
  digitalWrite(3,LOW); // send to ground; clobber inverting in
Serial.println("you are polarity true"); 
  Serial.println(polarityValue); 
posValue = analogRead(posPin);
displayValue = (posValue * multiplier);
   
}
else
{
  pinMode(2,OUTPUT);
}

if (polarityValue == 0)
{
digitalWrite(LED_BUILTIN, LOW);

 pinMode(3, INPUT);  //highz for this pin
 pinMode(12,OUTPUT);
 digitalWrite(12,LOW); //clobber positive input
  Serial.println("you are polarity false"); 
   Serial.println(polarityValue); 

 negValue = analogRead(negPin);
 displayValue2 = (negValue * multiplier);

}  
else
{
  pinMode(3,OUTPUT);
}





//write voltage

tft.setCursor(30,30);

//print the V symbol
tft.drawChar(90,30,'V',0xFFF,0x000,2);

 tft.drawChar(18,30,'-',0x000,0x000,2);

//write postive reading to screen

if ((posValue > 0) && (negValue < posValue))
{
//print volt to the screen
tft.print(displayValue + .06);


}
else
{
}


//write negative reading to screen

if ((negValue > 0) && (posValue < negValue))  

// draw negative voltage symbol when appropriate


  {



 tft.drawChar(18,30,'-',0xFFF,0x000,2);
     tft.setCursor(30,30);
     tft.print(displayValue2 + .06);

   } 
else
{
}

if ((displayValue2) = 0 && (displayValue = 0))
{
 tft.drawChar(18,30,'-',0x000,0x000,2);
     tft.setCursor(30,30);
     tft.print('0');

   } 


//get vu meter going

//are we reading a value at positive or negative?

if ((negValue > 0) && (negValue > posValue))
{
sensorValue = (Center - (negValue/2));
}
else

{
  sensorValue = (Center + (posValue/2));
}


meterx = (sensorValue * .1) + 13;
if (meterx != lastState) 


{


  //compute the yvalue arc.  Otherwise meter needle doesn't look "analog" in its travel, you want the top of the needle to make an arc shape.
 if ((sensorValue >= 0 && sensorValue <= 50)  || (sensorValue >= 951 && sensorValue < 1025))

  {
  yValue = yValueStart + yOffset[0];
  } 

  if ((sensorValue >= 51 && sensorValue <=100) || (sensorValue >= 901 && sensorValue <= 950))

   {
   yValue = yValueStart + yOffset[1];
   } 

   if ((sensorValue >= 101 && sensorValue <=150) || (sensorValue >= 851 && sensorValue <= 900))

   {
   yValue = yValueStart + yOffset[2];
   } 

 if ((sensorValue >= 151 && sensorValue <= 200) || (sensorValue >= 801 && sensorValue <= 850))

  {
  yValue = yValueStart + yOffset[3];
   } 


 if ((sensorValue >= 201 && sensorValue <= 250) || (sensorValue >= 751 && sensorValue <= 800))

  {
  yValue = yValueStart + yOffset[4];
   } 

 if ((sensorValue >= 251 && sensorValue <= 300) || (sensorValue >= 701 && sensorValue <= 750))

  {
  yValue = yValueStart + yOffset[5];
   } 

 if ((sensorValue >= 301 && sensorValue <= 350) || (sensorValue >= 651 && sensorValue <= 700))

  {
  yValue = yValueStart + yOffset[6];
   } 

 if ((sensorValue >= 351 && sensorValue <= 400) || (sensorValue >= 601 && sensorValue <= 650))

  {
  yValue = yValueStart + yOffset[7];
   } 

 if ((sensorValue >= 401 && sensorValue <= 450) || (sensorValue >= 551 && sensorValue <= 600))

  {
  yValue = yValueStart + yOffset[8];
   } 

  if (sensorValue >= 451  && sensorValue <= 550)

  {
  yValue = yValueStart + yOffset[9];
   } 

  


 //values: topx, topy, bottom x (constant) bottom y (constant) 
tft.drawLine(lastState,lasty,64, 125,0x000);


tft.drawLine(meterx,yValue, 64, 125,0xFFF);
}

lastState = meterx;
lasty = yValue;

delay(100);

}

