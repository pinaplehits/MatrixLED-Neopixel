#include <Adafruit_NeoPixel.h>
#include <Bounce2.h>

#define PIN 9
#define NUMPIXELS 64

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

Bounce debouncerPin10 = Bounce();
Bounce debouncerPin11 = Bounce();

const int RGBColorLeds[][3] = {
  {0, 0, 0}, //turn cells off
  {255, 0, 0}, //red
  {0, 255, 0}, //green
  {0, 0, 255}, //blue
  {255, 0, 255}, //purple
  {0, 255, 255}, //green_blue
  {255, 255, 0}, //yellow
  {255, 255, 255}, //white
  {255, 100, 100}, //pink
  {255, 85, 0} //orange
};

const int colorPin[] = {5, 6, 7, 8};
const int brightnessPin[] = {10, 11};
const int brightnessIntensity[] = {10, 25, 45, 60, 75, 80, 110, 150, 200, 255};
const int flashColor[] = {1, 2, 3, 6, 7, 9};
const int sizeColorPin = sizeof(colorPin) / sizeof(colorPin[0]);
const int rowsRGBColorLeds = sizeof(RGBColorLeds) / sizeof(RGBColorLeds[0]);
const int sizeBrightnessIntensity = sizeof(brightnessIntensity) / sizeof(brightnessIntensity[0]);
const int delayBounce = 5;
const int delayBrightness = 150;
const int delayFlash = 400;

int i;
int currentColor;
int currentBrightness;

void setup()
{
  delay(100);
  Serial.begin(9600);
  Serial.println("Matrix LED Adafruit_NeoPixel Version 1.0");
  Serial.println("El pin 9 es el pin de datos");
  Serial.println("Selector de colores por pines: 5, 6, 7, 8");
  Serial.println("Selector de brillo por pines: 10 y 11");
  Serial.println("9 colores disponibles rojo [PIN 5], verde [PIN 6], azul [PIN 5+6], morado [PIN 7], verde-azul [PIN 5+7], amarillo [PIN 6+7], blanco[PIN 5+6+7], rosa [PIN 8], naranja [PIN 5+8]");
  Serial.println("6 colores con flash disponibles rojo [PIN 5], verde [PIN 6], azul [PIN 5+6], amarillo [PIN 6+7], blanco[PIN 5+6+7], naranja [PIN 5+8]");

  pixels.begin();
  pixels.setBrightness(128);

  for (i = 0; i < NUMPIXELS; i++)
  {
    pixels.setPixelColor(i, 255, 255, 255);
    pixels.show();
    delay(20);
  }

  pixels.clear();
  pixels.show();

  // Pin 10 and Pin 11 prevents electrical pulse and multiple inputs
  pinMode(brightnessPin[0], INPUT_PULLUP);
  debouncerPin10.attach(brightnessPin[0]);
  debouncerPin10.interval(delayBounce);

  pinMode(brightnessPin[1], INPUT_PULLUP);
  debouncerPin11.attach(brightnessPin[1]);
  debouncerPin11.interval(delayBounce);

  for (i = 0; i < sizeColorPin; i++)
  {
    pinMode(colorPin[i], INPUT);
  }

  pixels.setBrightness(brightnessIntensity[0]);
}

void loop() {
  debouncerPin10.update();
  debouncerPin11.update();

  int debouncerState10 = debouncerPin10.read();
  int debouncerState11 = debouncerPin11.read();
  
  if (digitalRead(brightnessPin[0]) == LOW)
  {
    BrightnessDown();
  }

  if (digitalRead(brightnessPin[1]) == LOW)
  {
    BrightnessUp();
  }

  SelectPixelColor();

  if (currentColor < rowsRGBColorLeds)
  {
    TurnOnPixel();
  }
  else
  {
    FlashPixel();
  }

  currentColor = 0;
}

void SelectPixelColor()
{
  for (i = 0; i < sizeColorPin; i++)
  {
    if (digitalRead(colorPin[i]) == LOW)
    {
      currentColor += bit(i);
    }
  }
}

void TurnOnPixel()
{
  for (i = 0; i < NUMPIXELS; i++)
    {
      pixels.setPixelColor(i, RGBColorLeds[currentColor][0], RGBColorLeds[currentColor][1], RGBColorLeds[currentColor][2]);
    }

    pixels.show();
}

void FlashPixel()
{
  currentColor -= rowsRGBColorLeds;
  currentColor = flashColor[currentColor];

  TurnOnPixel();

  delay(delayFlash);

  pixels.clear();
  pixels.show();

  delay(delayFlash);
}

void BrightnessUp()
{
  currentBrightness = currentBrightness + 1 < sizeBrightnessIntensity ? currentBrightness + 1 : 0;

  SetBrightness();
}

void BrightnessDown()
{
  currentBrightness = currentBrightness <= 0 ? (sizeBrightnessIntensity - 1) : currentBrightness - 1;

  SetBrightness();
}

void SetBrightness()
{
  pixels.setBrightness(brightnessIntensity[currentBrightness]);
  delay(delayBrightness);
}