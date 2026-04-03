#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <FluxGarage_RoboEyes.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_MOSI    23
#define OLED_CLK     18
#define OLED_DC      2
#define OLED_CS      5
#define OLED_RESET   4

// Pins for buttons
#define BTN_OUCH     21 
#define BTN_HAPPY    19

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);
RoboEyes<Adafruit_SSD1306> roboEyes(display);

// Timer variables
unsigned long effect
StartTime = 0;
const long effectDuration = 1500; 

enum RobotState { IDLE, DOING_OUCH, DOING_HAPPY };
RobotState currentState = IDLE;

void setup() {
  Serial.begin(9600);
  pinMode(BTN_OUCH, INPUT_PULLUP);
  pinMode(BTN_HAPPY, INPUT_PULLUP);

  delay(250); 
  display.begin(SSD1306_SWITCHCAPVCC); 
  
  roboEyes.begin(SCREEN_WIDTH, SCREEN_HEIGHT, 100);
  roboEyes.setAutoblinker(ON, 3, 2);
  roboEyes.setIdleMode(ON, 2, 2);
}

void loop() {
  // 1. Check for Button Presses
  if (digitalRead(BTN_OUCH) == LOW) {
    currentState = DOING_OUCH;
    effectStartTime = millis();
    roboEyes.setMood(ANGRY); 
    roboEyes.anim_confused(); 
    roboEyes.setPosition(S); // Look Down
  } 
  else if (digitalRead(BTN_HAPPY) == LOW) {
    currentState = DOING_HAPPY;
    effectStartTime = millis();
    roboEyes.setMood(HAPPY); 
    roboEyes.blink();        
    roboEyes.setPosition(S); // Changed to S to look down like the angry one
  }

  // 2. Handle Timer
  if (currentState != IDLE && (millis() - effectStartTime >= effectDuration)) {
    currentState = IDLE;
    roboEyes.setMood(DEFAULT);
    roboEyes.setPosition(DEFAULT);
  }

  // 3. Update Eyes
  roboEyes.update(); 

  // 4. Overlay Text
  // Both are set to y=0 (top) because eyes are now at the bottom (S)
  if (currentState == DOING_OUCH) {
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(30, 0);  
    display.print("Ouch!?");
    display.display(); 
  } 
  else if (currentState == DOING_HAPPY) {
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(35, 0); 
    display.print("Yay!!");
    display.display(); 
  }
}