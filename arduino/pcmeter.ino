/**
 * Copyright (c) 2018 Inversebit
 * 
 * This code is licensed under the MIT License.
 * Full license text: https://opensource.org/licenses/MIT
 * 
 */
#include <FastLED.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define NUMPIXELS 16
#define PIXELS_PER_METER 6

#define LED_PIN 11
#define CPU_PIN 6
#define RAM_PIN 5

//max_value_for_analogwrite/max_value_coming_from_pc --> 190/100
const float MULTIPLIER = 1.9f;

const unsigned short CPU_LEDS[PIXELS_PER_METER] = {1, 2, 3, 4, 5, 6};
const unsigned short RAM_LEDS[PIXELS_PER_METER] = {9, 10, 11, 12, 13, 14};

CRGB pixels[NUMPIXELS];

void setup() {
  //INIT LEDS
  FastLED.addLeds<NEOPIXEL, LED_PIN>(pixels, NUMPIXELS);

  for(int i = 0; i < PIXELS_PER_METER; i++)
  {
    pixels[CPU_LEDS[i]].setHSV(0, 0, 30);
    pixels[RAM_LEDS[i]].setHSV(0, 0, 30);
  }
  FastLED.show();
  
  //INIT METERS
  pinMode(CPU_PIN, OUTPUT);
  pinMode(RAM_PIN, OUTPUT);

  analogWrite(CPU_PIN, 100*MULTIPLIER);
  analogWrite(RAM_PIN, 100*MULTIPLIER);

  delay(1000);
  
  analogWrite(CPU_PIN, 0);
  analogWrite(RAM_PIN, 0);

  //FINISH SETUP
  Serial.begin(9600);
}

CHSV GetColorFromVal(int val)
{  
  CHSV res;
  if(val > 60)
  {
    res.saturation = 255;
    res.hue = 92-(0.9f*val);
    res.value = 75;
  }
  else
  {
    res.saturation = 0;
    res.hue = 0;
    res.value = 30;
  }
  

  
  return res;
}

// the loop routine runs over and over again forever:
void loop() {  
  //READ INCOMING
  String readString = "";
  while (Serial.available()) 
  {
    char c = Serial.read();  //gets one byte from serial buffer
    readString += c; //makes the string readString
    delay(2);  //slow looping to allow buffer to fill with next character
  }

  //APPLY VALUES
  if (readString.length() > 0) 
  {    
    //INTERPRETING INPUT
    int i = 0;
    while(!isSpace(readString[i]))
    {
      i++;
    }

    String cpu = readString.substring(0, i);
    String ram = readString.substring(i+1, readString.length());

    int cpui = (min(cpu.toInt(), 100)) ;
    int rami = (min(ram.toInt(), 100)) ;

    //APPLYING INPUT
    analogWrite(CPU_PIN, cpui * MULTIPLIER);
    analogWrite(RAM_PIN, rami * MULTIPLIER);

    CHSV cpucolor = GetColorFromVal(cpui);
    CHSV ramcolor = GetColorFromVal(rami);

    for(int i = 0; i < PIXELS_PER_METER; i++)
    {
      pixels[CPU_LEDS[i]].setHSV(cpucolor.hue , cpucolor.sat, cpucolor.val);
      pixels[RAM_LEDS[i]].setHSV(ramcolor.hue, ramcolor.sat, ramcolor.val);
    }
    
    FastLED.show();
  }
}

