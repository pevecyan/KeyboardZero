#include <Keypad.h>
#include <Keyboard.h>

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);

const String name = "KeyboardZero v1.0";
const byte width = 6;
const byte height = 4;

bool ackMode = false;
bool customProfile = false;

bool locked = true;
bool previousPinCorrect = false;
char currentPinChar = "";

int lockPressCount = 0;

int previousTime = 0;
int isDisplayOnTimer = 0;

char hexaKeys[height][width] = {
  {'0','1','2','3','4','5'},
  {'6','7','8','9','a','b'},
  {'c','d','e','f','g','h'},
  {'i','j','k','l','m','n'}
};

byte rowPins[height] = {15,14,16,10};
byte colPins[width] = {9,8,7,6,5,4};

Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, height, width); 

void setup() {
   customKeypad.addStatedEventListener(keypadEvent); 
  // put your setup code here, to run once:
  Serial.begin(9600);
  Keyboard.begin();

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setRotation(2);
  display.clearDisplay();
  // Clear the buffer.

  display.clearDisplay();
  displayShowText(1,15,0, "Keyboard Zero");
  displayShowText(2, 30, 15, "V1.5");
  display.display();

  delay(2000);
  display.clearDisplay();
  
}

void loop() {
  int currentTime = millis();
  isDisplayOnTimer+= currentTime - previousTime;
  previousTime = currentTime;
  
  if (isDisplayOnTimer > 30000) {
    display.clearDisplay();
    display.display();
  } 

  customKeypad.getKeys();
  return;
  char customKey = customKeypad.getKey();
  
  if(Serial.available() > 0){
    String msg = Serial.readString();
    if(msg=="ACK")
      handleAcknowledge();
    if(msg == "OK" && ackMode){
        ackMode = false;
        customProfile = true;
    }
  }

  if(customProfile && customKey){
    Serial.print(customKey);
  }
  else if(customKey){
    switch(customKey){
      case '0':
        Keyboard.press(KEY_ESC);
        break;
      case 'i':
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('1');
        break;
      case 'j':
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('2');
        break;
      case 'k':
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('3');
        break;
      case 'l':
        Keyboard.press(KEY_LEFT_CTRL);
        Keyboard.press(KEY_LEFT_ALT);
        Keyboard.press('4');
        break;
    }
    Keyboard.releaseAll(); 
  }

  
}

void handleAcknowledge(){
  ackMode = true;
  Serial.print(name+":"+width+":"+height);
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key, KeyState kpadState ){
    switch (kpadState){
    case PRESSED:
        if (locked) {
          checkPin(key, kpadState);

          display.clearDisplay();
          displayShowText(2,10,0, "Locked");
          display.display();
        }
        else keyPressed(key);
        break;

    case RELEASED:
        
        break;

    case HOLD:
        
        break;
    }
}

void checkPin(KeypadEvent key, KeyState kpadState ){
  bool unlocked = false;
    //PIN 3245 - lkmn
    if (!previousPinCorrect){
      if (key == 'l'){
        previousPinCorrect = true;
        currentPinChar = 'l';
      } 
    }
    else if (currentPinChar == 'l' && key == 'k'){
      currentPinChar = 'k';
    }
    else if (currentPinChar == 'k' && key == 'm'){
      currentPinChar = 'm';
    }
    else if (currentPinChar == 'm' && key == 'n'){
      currentPinChar = 'n';
    }
    else if (currentPinChar == 'n' && key == 'c'){
      currentPinChar = "";
      previousPinCorrect = false;
      locked = false;
      
      display.clearDisplay();
      displayShowText(2,10,0, "Unlocked");
      display.display();

      unlocked = true;
      lockPressCount = 0;
    }
    else {
      currentPinChar = "";
      previousPinCorrect = false;
      
    }
    if (!previousPinCorrect && key == 'c' && !unlocked){
      ("Kezboard locked, please enter pin and press SKULL!");
    }
    
}

void keyPressed(KeypadEvent key){
  //First Row
  if (key == '0'){

  }
  else if (key == 'c'){
    lockPressCount++;
    if (lockPressCount == 4){
      locked = true;
      printText("Kezboard LOCKED\n");
    }
  }
  else if (key == 'g'){
    Keyboard.print(" ");
    delay(500);
    Keyboard.print("\n");
  }
  else if (key == 'd'){
    Keyboard.print("\n");
  }
  else if (key == 'e'){
    Keyboard.print("deploz\n");
  }
  //4. row
  else if (key == 'i'){
    moveToDesktop('1');

    display.clearDisplay();
    displayShowText(2,10,0, "Display 1\0");
    display.display();
  }
  else if (key == 'j'){
    moveToDesktop('2');
    display.clearDisplay();
    displayShowText(2,10,0, "Display 2\0");
    display.display();
  }
  else if (key == 'k'){
    moveToDesktop('3');
    display.clearDisplay();
    displayShowText(2,10,0, "Display 3\0");
    display.display();
  }
  else if (key == 'l'){
    moveToDesktop('4');
    display.clearDisplay();
    displayShowText(2,10,0, "Display 4\0");
    display.display();
  }
  else if (key == 'm'){
    moveToDesktop('5');
    display.clearDisplay();
    displayShowText(2,10,0, "Display 5\0");
    display.display();
  }
  else if (key == 'n'){
    moveToDesktop('6');
    display.clearDisplay();
    displayShowText(2,10,0, "Display 6\0");
    display.display();
  }
  
}

void moveToDesktop(char desktop){
    Keyboard.press(KEY_LEFT_CTRL);
    Keyboard.press(KEY_LEFT_ALT);
    Keyboard.press(desktop);

    Keyboard.release(KEY_LEFT_CTRL);
    Keyboard.release(KEY_LEFT_ALT);
    Keyboard.release(desktop);

   
}

void backSpace(int count, int delayTime){
  delay(delayTime);
  for(int i = 0; i < count; i++){
    Keyboard.press(8);
    Keyboard.release(8);

  }
}


void printText(char* text){
  Keyboard.print(text);
  backSpace(strlen(text), 1000);
}

void displayShowText(int size, int x, int y, String text){
    display.setTextSize(size);
    display.setTextColor(WHITE);
    display.setCursor(x,y);
    display.println(text);
    isDisplayOnTimer = 0;
}
