#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN 6

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, 13, NEO_GRB + NEO_KHZ800);
volatile bool shouldUpdate = false;
int pattern = 1;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

uint32_t Wheel(byte WheelPos);
uint32_t Wheel2(byte WheelPos);
uint32_t Wheel3(byte WheelPos);
void changePattern();

void setup() {
    // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
    // End of trinket special code
    randomSeed(analogRead(1));
    Serial.begin(9600);

    strip.begin();
    strip.setBrightness(128);
    strip.show(); // Initialize all pixels to 'off'
    
    pinMode(2, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(2), changePattern, RISING);
}

void loop() {
    if (shouldUpdate) {
        pattern++;

        if (pattern > 6) {
            pattern = 1;
        }
        
        shouldUpdate = false;
    }
    
    switch(pattern) {
        case 0:
            // Some example procedures showing how to display to the pixels:
            for (uint8_t i = 0; i < 1; i++) {
                colorWipeOnOff(strip.Color(200, 0, 0), 20);
            }
            break;
        case 1:
            flickerRed(50);
            break;
        case 2:
            flickerPurple(50);
            break;
        case 3:
            flickerBlue(50);
            break;
        case 4:
            flickerYellow(1);
            break;
        case 5:
            rainbowCycle(5);
            break;
        case 6:
            rainbowCycle2(5);
            break;
    }
    
    //colorWipe(strip.Color(0, 200, 0), 50); // Green
    //colorWipe(strip.Color(0, 0, 200), 50); // Blue
    //colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
    // Send a theater pixel chase in...
    //colorWipe(strip.Color(200, 0, 0), 200);
    //reverseColorWipe(strip.Color(200, 200, 200), 200);
    //colorWipe(strip.Color(0, 0, 200), 200);
    //flicker(100);
    
    
    
    
    //flicker2(300);
    //fadeInAndOut(255, 0, 0, 100); // Red
    //fadeInAndOut(0, 255, 0, 100); // Green
    //fadeInAndOut(0, 0, 255, 100); // Blue

    //theaterChase(strip.Color(240, 240, 240), 20); // White
    //theaterChase(strip.Color(127, 0, 0), 50); // Red
    //theaterChase(strip.Color(0, 0, 127), 50); // Blue

    //theaterChase2(strip.Color(240, 240, 240), 100); // White
    //theaterChase2(strip.Color(127, 0, 0), 100); // Red
    //theaterChase2(strip.Color(0, 0, 127), 100); // Blue

    //rainbow(5);
    
    //rainbowCycleJump(5);
    //rainbowCycleRandom(5);
    
    //theaterChaseRainbow(50);
}

void changePattern() {
   shouldUpdate = true;
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
    for(uint16_t i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void colorWipeOnOff(uint32_t c, uint8_t wait) {
    uint16_t i = 0;
    for (uint16_t j = 0; j < 256; j=j+4) {
        strip.setPixelColor(i, Wheel((i+j) & 255));
        strip.show();
        delay(wait);
        strip.setPixelColor(i, strip.Color(0, 0, 0));
        i++;

        if (i == strip.numPixels()) {
          i = 0;
        }
      }
}

// Fill the dots one after the other with a color
void reverseColorWipe(uint32_t c, uint8_t wait) {
    for(int16_t i = strip.numPixels() - 1; i >= 0; i--) {
        strip.setPixelColor(i, c);
        strip.show();
        delay(wait);
    }
}

void flicker(uint8_t cycles) {
    for (uint8_t i = 0; i < cycles; i++) {
        for (uint8_t pixel = 0; pixel < strip.numPixels(); pixel++) {
            strip.setPixelColor(random(0, strip.numPixels()), strip.Color(random(70, 100), 0, 0));
        }

        strip.show();

        if ((i % random(5, 10)) == 0) {
            uint16_t redPixel = random(0, strip.numPixels());
            strip.setPixelColor(redPixel, strip.Color(random(150, 200), 0, 0));
            strip.show();
            delay(15);
            strip.setPixelColor(redPixel, strip.Color(random(50, 100), 0, 0));
            strip.show();
        }

        if ((i % random(3, 10)) == 0) {
            uint16_t whitePixel = random(0, strip.numPixels());
            strip.setPixelColor(whitePixel, strip.Color(128, 128, 128));
            strip.show();
            delay(10);
            strip.setPixelColor(whitePixel, strip.Color(random(50, 100), 0, 0));
            strip.show();
            delay(5);
        }

        delay(random(80, 160));
    }
}

void flickerRed(uint8_t cycles) {
    for (uint8_t i = 0; i < cycles; i++) {
        for (uint8_t pixel = 0; pixel < strip.numPixels(); pixel++) {
            strip.setPixelColor(random(0, strip.numPixels()), strip.Color(random(70, 100), 0, 0));
        }

        strip.show();

        if ((i % random(3, 5)) == 0) {
            uint16_t redPixel = random(0, strip.numPixels());
            strip.setPixelColor(redPixel, strip.Color(200, 0, 0));
            strip.show();
            delay(25);
            strip.setPixelColor(redPixel, strip.Color(random(70, 100), 0, 0));
            strip.show();
            delay(1);
        }

        if ((i % random(10, 30)) == 0) {
            uint16_t whitePixel = random(0, strip.numPixels());
            strip.setPixelColor(whitePixel, strip.Color(128, 128, 128));
            strip.show();
            delay(20);
            strip.setPixelColor(whitePixel, strip.Color(random(70, 100), 0, 0));
            strip.show();
            delay(1);
        }

        if ((i % random(20, 50)) == 0) {
            uint16_t whitePixel = random(0, strip.numPixels());
            strip.setPixelColor(whitePixel, strip.Color(128, 128, 128));
            strip.show();
            delay(30);


            for(uint16_t b=100; b > 0; b=b/1.04) {
                strip.setPixelColor(whitePixel, strip.Color(random(70, 100), b, b));
                strip.show();
                delay(1);


                strip.setPixelColor(random(0, strip.numPixels()), strip.Color(random(70, 100), 0, 0));
                strip.show();
                delay(2);
            }
        }

        delay(random(60, 180));
    }
}

void flickerPurple(uint8_t cycles) {
    for (uint8_t i = 0; i < cycles; i++) {
        for (uint8_t pixel = 0; pixel < strip.numPixels(); pixel++) {
            strip.setPixelColor(random(0, strip.numPixels()), strip.Color(random(70, 100), 0, random(70, 100)));
        }

        strip.show();

        if ((i % random(3, 5)) == 0) {
            uint16_t redPixel = random(0, strip.numPixels());
            strip.setPixelColor(redPixel, strip.Color(200, 0, 0));
            strip.show();
            delay(20);
            strip.setPixelColor(redPixel, strip.Color(random(50, 100), 0, random(50, 100)));
            strip.show();
            delay(1);
        }

        if ((i % random(3, 5)) == 0) {
            uint16_t bluePixel = random(0, strip.numPixels());
            strip.setPixelColor(bluePixel, strip.Color(0, 0, 200));
            strip.show();
            delay(20);
            strip.setPixelColor(bluePixel, strip.Color(random(50, 100), 0, random(50, 100)));
            strip.show();
            delay(1);
        }

     /*if ((i % random(10, 30)) == 0) {
      uint16_t whitePixel = random(0, strip.numPixels());
      strip.setPixelColor(whitePixel, strip.Color(128, 128, 128));
      strip.show();
      delay(7);
      strip.setPixelColor(whitePixel, strip.Color(random(50, 100), 0, 0));
      strip.show();
      delay(1);
    }
    */

        if ((i % random(10, 40)) == 0) {
            uint16_t greenPixel = random(0, strip.numPixels());
            strip.setPixelColor(greenPixel, strip.Color(0, 128, 0));
            strip.show();
            delay(30);

            for(uint16_t b=100; b > 0; b=b/1.04) {
                strip.setPixelColor(greenPixel, strip.Color(random(30, 60), b, random(30, 60)));
                strip.show();
                delay(1);

                strip.setPixelColor(random(0, strip.numPixels()), strip.Color(random(50, 100), 0, random(50, 100)));
                strip.show();
                delay(2);
            }
        }

        delay(random(80, 160));
    }
}

void flickerBlue(uint8_t cycles) {
    for (uint8_t i = 0; i < cycles; i++) {
        for (uint8_t pixel = 0; pixel < strip.numPixels(); pixel++) {
            strip.setPixelColor(random(0, strip.numPixels()), strip.Color(0, 0, random(70, 100)));
        }

        strip.show();

        if ((i % random(3, 5)) == 0) {
            uint16_t bluePixel = random(0, strip.numPixels());
            strip.setPixelColor(bluePixel, strip.Color(0, 0, 200));
            strip.show();
            delay(20);
            strip.setPixelColor(bluePixel, strip.Color(0, 0, random(50, 100)));
            strip.show();
            delay(1);
        }

        if ((i % random(10, 30)) == 0) {uint16_t whitePixel = random(0, strip.numPixels());
            strip.setPixelColor(whitePixel, strip.Color(128, 128, 100));
            strip.show();
            delay(7);
            strip.setPixelColor(whitePixel, strip.Color(0, 0, random(50, 100)));
            strip.show();
            delay(1);
        }

        if ((i % random(20, 50)) == 0) {
            uint16_t whitePixel = random(0, strip.numPixels());
            strip.setPixelColor(whitePixel, strip.Color(128, 128, 100));
            strip.show();
            delay(20);

            for(uint16_t b=100; b > 0; b=b/1.04) {
                strip.setPixelColor(whitePixel, strip.Color(b, b, random(50, 100)));
                strip.show();
                delay(1);

                strip.setPixelColor(random(0, strip.numPixels()), strip.Color(0, 0, random(70, 100)));
                strip.show();
                delay(2);
            }
        }

        delay(random(80, 160));
    }
}

void flickerYellow(uint8_t cycles) {
    for (uint8_t i = 0; i < cycles; i++) {
        for (uint8_t pixel = 0; pixel < strip.numPixels(); pixel++) {

            strip.setPixelColor(random(0, strip.numPixels()), strip.Color(random(70, 100), random(70, 100), 0));
        }

        strip.show();

        if ((i % random(3, 5)) == 0) {
            uint16_t yellowPixel = random(0, strip.numPixels());
            strip.setPixelColor(yellowPixel, strip.Color(170, 170, 0));
            strip.show();
            delay(20);
            strip.setPixelColor(yellowPixel, strip.Color(random(70, 100), random(70, 100), 0));
            strip.show();
            delay(1);
        }

        if ((i % random(5, 20)) == 0) {
            uint16_t whitePixel = random(0, strip.numPixels());
            strip.setPixelColor(whitePixel, strip.Color(0, 0, 160));
            strip.show();
            delay(30);
            strip.setPixelColor(whitePixel, strip.Color(random(70, 100), random(70, 100), 0));
            strip.show();
            delay(1);
        }

        if ((i % random(10, 15)) == 0) {
            uint16_t bluePixel = random(0, strip.numPixels());
            strip.setPixelColor(bluePixel, strip.Color(0, 0, 160));
            strip.show();
            delay(30);

            for (uint16_t b=0; b < 80; b++) {
                uint16_t c=160; b > 0; c--;
                strip.setPixelColor(bluePixel, strip.Color(b, b, b/c));
                strip.show();
                delay(7);

                strip.setPixelColor(random(0, strip.numPixels()), strip.Color(random(70, 100), random(70, 100), 0));
                strip.show();
                delay(1);
            }
        }

        delay(random(70, 200));
    }
}



void flicker2(uint8_t cycles) {
    for (uint8_t i = 0; i < cycles; i++) {
        int16_t whitePixel = -1;
        uint8_t whiteIntensity = 127;

        for (uint8_t pixel = 0; pixel < strip.numPixels(); pixel++) {
            strip.setPixelColor(random(0, strip.numPixels()), strip.Color(random(70, 100), 0, 0));
        }

        strip.show();

        if ((i % random(3, 7)) == 0) {
            uint16_t redPixel = random(0, strip.numPixels());
            strip.setPixelColor(redPixel, strip.Color(random(150, 200), 0, 0));
            strip.show();
            delay(15);
            strip.setPixelColor(redPixel, strip.Color(random(50, 100), 0, 0));
            strip.show();
        }

        if (whitePixel > -1) {
            strip.setPixelColor(whitePixel, strip.Color(whiteIntensity--, whiteIntensity--, whiteIntensity--));
            strip.show();

            if (whiteIntensity == -1) {
                whiteIntensity = 127;
                whitePixel = -1;
            }
        }

        if ((i % random(3, 10)) == 0 && whitePixel < 0) {
            whitePixel = random(0, strip.numPixels());
            strip.setPixelColor(whitePixel, strip.Color(128, 128, 128));
            strip.show();
            delay(10);
        }

        delay(random(80, 160));
    }
}

void fadeInAndOut(uint8_t red, uint8_t green, uint8_t blue, uint8_t wait) {
    for(uint8_t b=0; b <255; b++) {
        for(uint8_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, red*b/255, green*b/255, blue*b/255);
        }

        strip.show();
        delay(wait);
    }

    for(uint8_t b=255; b > 0; b--) {
        for(uint8_t i=0; i < strip.numPixels(); i++) {
            strip.setPixelColor(i, red*b/255, green*b/255, blue*b/255);
        }

        strip.show();
        delay(wait);
    }
}

void rainbow(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256; j++) {
        for(i=0; i<strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i+j) & 255));
        }

        strip.show();
        delay(wait);
    }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
        }

        strip.show();
        delay(wait);
    }
}

void rainbowCycleJump(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel3(((i * 256 / strip.numPixels()) + j) & 255));
        }

        strip.show();
        delay(wait);
    }
}

void rainbowCycleRandom(uint8_t wait) {
    uint16_t i, j;

    for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
        for(i=0; i< strip.numPixels(); i++) {
            strip.setPixelColor(i, Wheel((i * 256 / strip.numPixels() + j) & 255));
        }

        strip.show();
        delay(wait);
    }
}

void rainbowCycle2(uint8_t wait) {
    uint16_t i, j, k, l;

    for(j=0; j<256*1; j++) { // 5 cycles of all colors on wheel
        //for(l=256*5; l>0; l--)
        for(k=strip.numPixels(); k > 3; k--) {
            for(i=0; i < strip.numPixels()-4; i++) {
                strip.setPixelColor(i, Wheel(((i * 200 / 8) + j) & 255));
                strip.setPixelColor(k, Wheel2(((k * 140 / 6) - j) & 255));
            }

            strip.show();
            delay(wait);
        }
    }

  /*for(k=0; k<256*10; k++) {
    for(l=5; l < 8; l++) {
     strip.setPixelColor(l, Wheel(((l * 256 / strip.numPixels()) - k) & 255));
    }
     strip.show();
    delay(wait);

    }*/
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
    for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        for (int q=0; q < 8; q++) {
            for (uint16_t i=0; i < strip.numPixels(); i=i+4) {
                strip.setPixelColor(i+q, c);    //turn every third pixel on
            }

            strip.show();

            delay(wait);

            for (uint16_t i=0; i < strip.numPixels(); i=i+8) {
                strip.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

void theaterChase2(uint32_t c, uint8_t wait) {
    for (int j=0; j<10; j++) {  //do 10 cycles of chasing
        for (int q=0; q < 8; q++) {
            for (uint16_t i=0; i < strip.numPixels(); i=i+4) {
                strip.setPixelColor(i+q, c);    //turn every third pixel on
            }

            strip.show();

            delay(wait/2);

            for (uint16_t i=0; i < strip.numPixels(); i=i+8) {
                strip.setPixelColor(i-q, 0);        //turn every third pixel off
            }
        }
    }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
    for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
        for (int q=0; q < 3; q++) {
            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
            }

            strip.show();

            delay(wait);

            for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
                strip.setPixelColor(i+q, 0);        //turn every third pixel off
            }
        }
    }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
    WheelPos = 255 - WheelPos;

    if(WheelPos < 85) {
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }

    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }

    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}


uint32_t Wheel2(byte WheelPos) {
    WheelPos = 255 - WheelPos;

    if(WheelPos < 85) {
        return strip.Color(WheelPos * 3, 0, 255 - WheelPos * 3);
    }

    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, WheelPos * 3, 0);
    }

    WheelPos -= 170;
    return strip.Color(0, 255 - WheelPos * 3, WheelPos * 3);
}

uint32_t Wheel3(byte WheelPos) {
    WheelPos = 255 - WheelPos;

    if(WheelPos < 85) {
        return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }

    if(WheelPos < 170) {
        WheelPos -= 85;
        return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }

    WheelPos -= 170;
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
