// leds
#include <FastLED.h>
#include <EEPROM.h>
#define NUM_LEDS 169
#define DATA_PIN 9
CRGB leds[NUM_LEDS];

// rotary
#define CLK_PIN 4 
#define DT_PIN 3 
#define SW_PIN 2 
int brightness = EEPROM.read(0);
int CLK_state;
int prev_CLK_state;
int SW_state;
int prev_SW_state;

// refresh leds
short prev_sec = 0;
short cur_sec = 0;

void setup() {
  //Serial.begin(9600);

  // rotary
  pinMode(CLK_PIN, INPUT);
  pinMode(DT_PIN, INPUT);
  pinMode(SW_PIN, INPUT);
  CLK_state = digitalRead(CLK_PIN);
  prev_CLK_state = CLK_state;
  SW_state = digitalRead(SW_PIN);
  prev_SW_state = SW_state;

  //leds
  FastLED.addLeds<WS2815, DATA_PIN, GRB>(leds, NUM_LEDS);
  FastLED.setTemperature(Candle); //best white correction is Tungsten40W or use Candle for a more fluorescent (warmer) look.
  FastLED.setBrightness(brightness); //read from eeprom previously
  for (int dot = 0; dot < 43; dot++) { //for whatever reason this strip is 1 indexed
    leds[dot] = CRGB::Orange;
  }
  for (int dot = 0; dot < 13; dot++) {
    leds[dot+5] = CRGB::Blue;
    leds[dot+26] = CRGB::Blue;
  }
  for (int dot = 43; dot <= NUM_LEDS; dot++) {
    leds[dot] = CRGB::White;
  }
  FastLED.show();
  FastLED.show();
}

void loop() {
  SW_state = digitalRead(SW_PIN);
  CLK_state = digitalRead(CLK_PIN);

  cur_sec = (millis() / 1000) % 10;
  if (cur_sec != prev_sec) {
    FastLED.show();
    prev_sec = cur_sec;
  }

  if (CLK_state != prev_CLK_state && CLK_state == HIGH) {
    if (digitalRead(DT_PIN) == HIGH) {
        brightness = constrain(brightness+5, 0, 255);
        //Serial.print("CW");
      } else {
        brightness = constrain(brightness-5, 0, 255);
        //Serial.print("CCW");
      }

      FastLED.setBrightness(brightness);
      delay(2); // prevents glitches
      FastLED.show();
  }

  if (SW_state != prev_SW_state && SW_state == LOW) {
    EEPROM.write(0, brightness); 
  }

  prev_CLK_state = CLK_state;
  prev_SW_state = SW_state;
}
