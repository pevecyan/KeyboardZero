#include <Keypad.h>
#include <Keyboard.h>

const String name = "KeyboardZero v1.0";
const byte width = 6;
const byte height = 4;

bool ackMode = false;
bool customProfile = false;

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
  // put your setup code here, to run once:
  Serial.begin(9600);
  Keyboard.begin();
  
}

void loop() {
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

