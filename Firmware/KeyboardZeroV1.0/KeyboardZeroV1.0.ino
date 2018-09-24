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

String  clipboard [4] = {"", "", "", ""}; 

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

  if(Serial.available() > 0){
    String msg = Serial.readString();
    if(msg=="ACK")
      handleAcknowledge();
    else if(msg == "OK" && ackMode){
        ackMode = false;
        customProfile = true;
        display.clearDisplay();
        displayShowText(2,0,0, "Connected");
        display.display();
    } 
    
    if (msg.startsWith("CPB:0:")){
        String text = msg.substring(6);
        showClipboard(0, text);
    } else if (msg.startsWith("CPB:1:")){
        String text = msg.substring(6);
        showClipboard(1, text);
    }  else if (msg.startsWith("CPB:2:")){
        String text = msg.substring(6);
        showClipboard(2, text);
    }  else if (msg.startsWith("CPB:3:")){
        String text = msg.substring(6);
        showClipboard(3, text);
    } else if (msg.startsWith("CPBP:0:")){
        String text = msg.substring(7);
        pushClipboard( text);
    }
  }
  
}

void handleAcknowledge(){
  display.clearDisplay();
  displayShowText(2,0,0, "Connecting");
  display.display();

  ackMode = true;
  Serial.print(name+":"+width+":"+height);
}

// Taking care of some special events.
void keypadEvent(KeypadEvent key, KeyState kpadState ){
    switch (kpadState){
    case PRESSED:
        if (locked) {

          display.clearDisplay();
          displayShowText(2,10,0, "Locked");
          display.display();
          
          checkPin(key, kpadState);

          
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
      
      

      unlocked = true;
      lockPressCount = 0;

      display.clearDisplay();
      displayShowText(2,10,0, "Unlocked");
      display.display();
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

  if (customProfile){
    
      String pressed = "P:";
      pressed+=key;
      pressed+="@\0";
      Serial.print(pressed);

      if (key == '0'){ showClipboard(0, clipboard[0]);}
      else if (key == '1'){ showClipboard(1, clipboard[1]);}
      else if (key == '2'){ showClipboard(2, clipboard[2]);}
      else if (key == '3'){ showClipboard(3, clipboard[3]);}

      

      return;
  }
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

void showClipboard(int index, String text){
    clipboard[index] = text;

    display.clearDisplay();
    displayShowText(1,8,0, clipboard[0]);
    displayShowText(1,8,7, clipboard[1]);
    displayShowText(1,8,14, clipboard[2]);
    displayShowText(1,8,21, clipboard[3]);

    if (index == 0){
       drawArrow(0);
    } else if (index == 1){
       drawArrow(7);
    } else if (index == 2){
       drawArrow(14);
    } else if (index == 3){
       drawArrow(21);
    }

    display.display();
}

void drawArrow(int y){
  display.drawPixel(1, 1 + y, WHITE);
  display.drawPixel(1, 2 + y, WHITE);
  display.drawPixel(1, 3 + y, WHITE);
  display.drawPixel(1, 4 + y, WHITE);
  display.drawPixel(1, 5 + y, WHITE);

  display.drawPixel(2, 2 + y, WHITE);
  display.drawPixel(2, 3 + y, WHITE);
  display.drawPixel(2, 4 + y, WHITE);

  display.drawPixel(3, 3 + y, WHITE);


}

void pushClipboard(String text){
  clipboard[3] = clipboard[2];
  clipboard[2] = clipboard[1];
  clipboard[1] = clipboard[0];
  showClipboard(0, text);
}