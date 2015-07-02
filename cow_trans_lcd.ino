#include <SPI.h>
#include <avr/pgmspace.h>

// temp correction in degrees celsius
//const double correction = -2.109;
/*


with LCD facing away from you and plugged into breadboard (pin 8 on lcd is to your left):
  GND
  11
  +5V
  7
  6
  5
  4
  3
  
cow id: 48053

This is a work in progress but hopefully it will help someone else by providing
a base to start and work from.
Please check out my Youtube videos here and consider a thumbs up if this helped you!
Youtube : http://www.youtube.com/user/Shadow5549
Website, Forum and store are at http://mkme.org

Pin 11 for PWM LCD backlight on the Nokia 5110
pin 7 - Serial clock out (SCLK)
pin 6 - Serial data out (DIN)
pin 5 - Data/Command select (D/C)
pin 4 - LCD chip select (CS)
pin 3 - LCD reset (RST)
*/



/*

Message format ideas:
  0x00 ...    0x00
  0x00 ...    0xFF
  0x00 0x00 ... 0x00
  0x00 0x00 ... 0xff
  0x00 ...    0x00 0xFF
  0xff
  0xAA
  0x55

Cow id: 48053
Chip is a 64bit shift register
0000 0000  0000 0000  0x00 0x00
0000 0100  0000 1000  0x04 0x08
0000 0000  0000 0101  0x00 0x05
0000 0011  1111 1111  0x03 0xff

*/


#include "Adafruit_GFX.h"
#include "Adafruit_PCD8544.h"
Adafruit_PCD8544 display = Adafruit_PCD8544(7, 6, 5, 4, 3);//4 and 3 are reversed on the cheap eBay models

char ROMS[][8] = {
  {0xFF, 0xFF, 0x04, 0x08,  0x00, 0x05, 0x03, 0x00},
  {0xFF, 0xFF, 0x54, 0x58,  0x50, 0x55, 0x53, 0x00},
  {0xFF, 0xFF, 0xA4, 0xA8,  0xA0, 0xA5, 0xA3, 0x00},
  {0xFF, 0xFF, 0xF4, 0xF8,  0xF0, 0xF5, 0xF3, 0x00},
  {0xFF, 0xFF, 0x04, 0x18,  0x20, 0x35, 0x43, 0x00},
  {0xFF, 0xFF, 0x44, 0x38,  0x20, 0x15, 0x03, 0x00},

  {0xFF, 0xFF, 0x00, 0x04,  0x08, 0x00, 0x05, 0x03},
  {0xFF, 0xFF, 0x50, 0x54,  0x58, 0x50, 0x55, 0x53},
  {0xFF, 0xFF, 0xA0, 0xA4,  0xA8, 0xA0, 0xA5, 0xA3},
  {0xFF, 0xFF, 0xF0, 0xF4,  0xF8, 0xF0, 0xF5, 0xF3},
  {0xFF, 0xFF, 0x00, 0x14,  0x28, 0x30, 0x45, 0x53},
  {0xFF, 0xFF, 0x50, 0x44,  0x38, 0x20, 0x15, 0x03},

  
  {0x00, 0x00, 0x04, 0x08,  0x00, 0x05, 0x03, 0x00},
  {0x00, 0x00, 0x54, 0x58,  0x50, 0x55, 0x53, 0x00},
  {0x00, 0x00, 0xA4, 0xA8,  0xA0, 0xA5, 0xA3, 0x00},
  {0x00, 0x00, 0xF4, 0xF8,  0xF0, 0xF5, 0xF3, 0x00},
  {0x00, 0x00, 0x04, 0x18,  0x20, 0x35, 0x43, 0x00},
  {0x00, 0x00, 0x44, 0x38,  0x20, 0x15, 0x03, 0x00},

  {0x00, 0x00, 0x00, 0x04,  0x08, 0x00, 0x05, 0x03},
  {0x00, 0x00, 0x50, 0x54,  0x58, 0x50, 0x55, 0x53},
  {0x00, 0x00, 0xA0, 0xA4,  0xA8, 0xA0, 0xA5, 0xA3},
  {0x00, 0x00, 0xF0, 0xF4,  0xF8, 0xF0, 0xF5, 0xF3},
  {0x00, 0x00, 0x00, 0x14,  0x28, 0x30, 0x45, 0x53},
  {0x00, 0x00, 0x50, 0x44,  0x38, 0x20, 0x15, 0x03},

  
  {0xAA, 0xAA, 0x04, 0x08,  0x00, 0x05, 0x03, 0x00},
  {0xAA, 0xAA, 0x54, 0x58,  0x50, 0x55, 0x53, 0x00},
  {0xAA, 0xAA, 0xA4, 0xA8,  0xA0, 0xA5, 0xA3, 0x00},
  {0xAA, 0xAA, 0xF4, 0xF8,  0xF0, 0xF5, 0xF3, 0x00},
  {0xAA, 0xAA, 0x04, 0x18,  0x20, 0x35, 0x43, 0x00},
  {0xAA, 0xAA, 0x44, 0x38,  0x20, 0x15, 0x03, 0x00},

  {0xAA, 0xAA, 0x00, 0x04,  0x08, 0x00, 0x05, 0x03},
  {0xAA, 0xAA, 0x50, 0x54,  0x58, 0x50, 0x55, 0x53},
  {0xAA, 0xAA, 0xA0, 0xA4,  0xA8, 0xA0, 0xA5, 0xA3},
  {0xAA, 0xAA, 0xF0, 0xF4,  0xF8, 0xF0, 0xF5, 0xF3},
  {0xAA, 0xAA, 0x00, 0x14,  0x28, 0x30, 0x45, 0x53},
  {0xAA, 0xAA, 0x50, 0x44,  0x38, 0x20, 0x15, 0x03},

  
  {0x55, 0x55, 0x04, 0x08,  0x00, 0x05, 0x03, 0x00},
  {0x55, 0x55, 0x54, 0x58,  0x50, 0x55, 0x53, 0x00},
  {0x55, 0x55, 0xA4, 0xA8,  0xA0, 0xA5, 0xA3, 0x00},
  {0x55, 0x55, 0xF4, 0xF8,  0xF0, 0xF5, 0xF3, 0x00},
  {0x55, 0x55, 0x04, 0x18,  0x20, 0x35, 0x43, 0x00},
  {0x55, 0x55, 0x44, 0x38,  0x20, 0x15, 0x03, 0x00},

  {0x55, 0x55, 0x00, 0x04,  0x08, 0x00, 0x05, 0x03},
  {0x55, 0x55, 0x50, 0x54,  0x58, 0x50, 0x55, 0x53},
  {0x55, 0x55, 0xA0, 0xA4,  0xA8, 0xA0, 0xA5, 0xA3},
  {0x55, 0x55, 0xF0, 0xF4,  0xF8, 0xF0, 0xF5, 0xF3},
  {0x55, 0x55, 0x00, 0x14,  0x28, 0x30, 0x45, 0x53},
  {0x55, 0x55, 0x50, 0x44,  0x38, 0x20, 0x15, 0x03},
};

enum ButtonCommands {
  None = 0,
  BtnUp = 1,
  BtnDown = 2,
  BtnNext = 3,
  BtnBack = 4
};

const double correction = -2.109;

// PIN Definitions
const int Pin_Btn1        = A0;
const int Pin_Btn2        = A2;
const int Pin_Btn3        = A3;
const int Pin_Btn4        = A1;
const int Pin_LED         = 2;
const int Pin_Temp        = A7;
const int Pin_Trans_Clk   = 10;
const int Pin_Trans_Data  = 9;

int PrevButton = None;
unsigned long lastButtonPress = 0;


void setup() {
  pinMode(Pin_Btn1, INPUT_PULLUP); // switch
  pinMode(Pin_Btn2, INPUT_PULLUP); // switch
  pinMode(Pin_Btn3, INPUT_PULLUP); // switch
  pinMode(Pin_Btn4, INPUT_PULLUP); // switch
  pinMode(Pin_Temp, INPUT);
  pinMode(Pin_LED, OUTPUT); // led
  pinMode(Pin_Trans_Clk, INPUT);
  pinMode(Pin_Trans_Data, INPUT); // initially in so it doesn't erase data
  
  digitalWrite(Pin_LED, HIGH); // turn on LED
  pinMode(8, OUTPUT);
  // provide power to LCD
  digitalWrite(8, HIGH);
  
  //Serial.begin(9600);
  //Serial.println("Running..."); 
  setupLcd();
}

double mapf(double x, double in_min, double in_max, double out_min, double out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

double getTemp()
{
  double in = 1.0 * analogRead(Pin_Temp);
  double v = mapf(in, 0, 1024, 0, 5);
  double temp_c = (((v * 1000) - 400) / 19.5) + correction;
  return temp_c;
}

int GetButtonInput()
{
  if(PrevButton == None)
    lastButtonPress = 0;
  
  ButtonCommands newButton = None;
  
  if(analogRead(Pin_Btn1) < 128)
    newButton = BtnDown;
  else if(analogRead(Pin_Btn2) < 128)
    newButton = BtnNext;
  else if(analogRead(Pin_Btn3) < 128)
    newButton = BtnUp;
  else if(analogRead(Pin_Btn4) < 128)
    newButton = BtnBack;
  else
  {
    PrevButton = None;
    lastButtonPress = 0;
  }
  
  if((newButton == PrevButton && (millis() - lastButtonPress > 500)) || (newButton != PrevButton))
  {
    lastButtonPress = millis();
    PrevButton = newButton;
    return newButton;
  }
  else
  {
    return None;
  }
  
}
void setupLcd()
{
  analogWrite(11,220);// PWM of LCD backlight but ebay unit is backwards- 
  //must go high + cycle to dim 
  //Very Dim=230
  display.begin();//Display code
  display.setContrast(50);    //Nokia 5110 works best around 50- change to suit your unit
  //display.setContrast(90);    //Nokia 5110 works best around 50- change to suit your unit
  display.setTextSize(1);     // set text size
  display.clearDisplay();     // clears the screen and buffer
}

int MainMenu()
{
  String entries[] = {
    //"Display memory",
    "Program       ",
    "Cur Temp      ",
    //"Custom program",
    "About...      "
  };
  display.clearDisplay();
  
  return Menu("Main Menu", entries, 3);
}

enum MainMenuCommands {
  Back = -1,
//  DisplayMemory = 0,
  AutoProgram = 0,
//  CustomProgram = 2,
  Temp = 1,
  About = 2
};

int Menu(String title, String entries[], int entry_count)
{
  // Reset the display
  display.clearDisplay();
  
  display.setCursor(0, 0);
  display.setTextColor(0, 255);
  
  // Draw the menu title
  display.drawFastHLine(0, 3, 84, 255);
  display.drawFastHLine(0, 4, 84, 255);
  printCentered(0, title);
  
  // set up user controls
  bool selected = false;
  int selected_entry = 0;
  
  while(!selected)
  {
    // define allowable options
    bool CanGoUp = false;
    bool CanGoDown = false;
    if(selected_entry < entry_count - 1)
      CanGoDown = true;
    if(selected_entry > 0)
      CanGoUp = true;
    
    display.setCursor(0, 8);
    // draw each menu item
    for(int i = 0; i < entry_count && i < 5; i++)
    {
      // only draw 5 items at a time.
      // if this is the selected item, 'highlight' it
      if(i == selected_entry)
        display.setTextColor(0, 255);
      else
        display.setTextColor(255, 0);
      display.print(entries[i]);
    }
    
    display.setTextColor(255, 0);
    printUpDownIcon(CanGoUp, CanGoDown);
    display.display();
    
    // wait for user input
    //while(!Serial.available());
    //byte i = Serial.read();
    int input = GetButtonInput();
    
    if(input == BtnUp)
    {
      // up
      if(CanGoUp) selected_entry--;
    }
    else if(input == BtnDown)
    {
      // down
      if(CanGoDown) selected_entry++;
    }
    else if(input == BtnNext)
    {
      // Select
      selected = true;
    }
    else if(input == BtnBack)
    {

      return -1; // back
    }
  }
  
  display.clearDisplay();
  display.display();
  
  return selected_entry;
}

void splash()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Kevin's cow ID");
  printCentered(8, "automated");
  printCentered(16, "programmer");
  printCentered(27, "ver 0.1");
  printCentered(35, "05/30/2015");
  display.display();
  for(int i = 0; i < 84; i++)
  {
    display.drawPixel(i, 46, 255);
    display.drawPixel(i, 47, 255);
    display.display();
    delay(20);
  }
}

void printUpDownIcon(boolean up, boolean down)
{
  char disp[3] = {up ? (char)24 : ' ', down ? (char)25 : ' ', (char)'\0'};
  
  display.setCursor(72, 40);
  // 24 up
  // 25 down
  // 30 up
  // 31 down
  display.print(disp);
}

void printCentered(int down, String text)
{
  int length = text.length();
  //while(text[length] != '\0') length++;
  
  // width of display is 84 px / 2 = 42 px
  display.setCursor(42 - (3 * length), down);
  display.print(text);
}

void waitFor(bool val)
{
  bool keepGoing = true;
  while(keepGoing)
  {
    if(digitalRead(Pin_Trans_Clk) == val)
      keepGoing = false;
  }
}

void program(int offset)
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(255, 0);
  //display.print("Prog test");
  printCentered(0, "Programming...");
  
//  display.drawFastHLine(0, 3, 84, 255);
  display.drawFastHLine(9, 40, 65, 255);
  display.drawFastHLine(9, 43, 65, 255);
  display.drawFastVLine(9, 40, 4, 255);
  display.drawFastVLine(74, 40, 4, 255);
  display.display();
  //delay(750);
  waitFor(HIGH);
  pinMode(Pin_Trans_Data, OUTPUT);
  pinMode(Pin_Trans_Clk, OUTPUT);
//  for(int i = 0; i < 64; i++)
//  {
    //waitFor(LOW);
    //waitFor(HIGH);
//    digitalWrite(Pin_Trans_Data, i % 2 == 0);
//    if(i == 63) i = 0;
//    int progress = 10 + i; // map(i, 0, 64, 0, 84);
//    display.drawPixel(progress, 41, 255);
//    display.drawPixel(progress, 42, 255);
//    display.display();

//    delay(10);
//  }

  pinMode(Pin_Trans_Clk, OUTPUT);
  for(int i = 0; i < 8; i++)
  {
    shiftOut(Pin_Trans_Data, Pin_Trans_Clk, MSBFIRST, ROMS[offset][i]);
  }

  pinMode(Pin_Trans_Clk, INPUT);
  pinMode(Pin_Trans_Data, INPUT);
/* 
  display.clearDisplay();
  display.println(shiftIn(Pin_Trans_Data, Pin_Trans_Clk, MSBFIRST));
  display.println(shiftIn(Pin_Trans_Data, Pin_Trans_Clk, MSBFIRST));
  display.println(shiftIn(Pin_Trans_Data, Pin_Trans_Clk, MSBFIRST));
  display.println(shiftIn(Pin_Trans_Data, Pin_Trans_Clk, MSBFIRST));
  display.display();
  delay(4000);*/
  delay(50);
}

void PrintHex(char a, bool includeHexPrefix = false)
{
  if(includeHexPrefix == true)
    display.print("0x");

  int i = 0;

  for(int i = 1; i >= 0; i--)
  {
    int val = (bitRead(a, i * 4 + 0) ? 1 : 0) +
              (bitRead(a, i * 4 + 1) ? 1 : 0) * 2 +
              (bitRead(a, i * 4 + 2) ? 1 : 0) * 4 +
              (bitRead(a, i * 4 + 3) ? 1 : 0) * 8;
  
    char hexMap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
    display.print(hexMap[val]);
  }
}

void PrintHex(bool arr[], int offset, bool includeHexPrefix = false)
{
  if(includeHexPrefix == true)
    display.print("0x");

  int val = (arr[offset + 3] ? 1 : 0) +
            (arr[offset + 2] ? 1 : 0) * 2 +
            (arr[offset + 1] ? 1 : 0) * 4 +
            (arr[offset    ] ? 1 : 0) * 8;
  
  char hexMap[] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};
  display.print(hexMap[val]);
}

void DispRom(int i)
{
  for(int y = 0; y < 4; y++)
  {
    if(y > 0)
      display.println("");
    
    int bitStart = y * 16;
    
    if(bitStart < 10)
      display.print(" ");
    
    display.print(bitStart);
    display.print(": ");

    for(int x = 0; x < 2; x++)
    {
      if(x > 0)
        display.print(" ");

      PrintHex(ROMS[i][y * 2 + x    ], true);
    }
  }
}

void DispMem()
{
  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextColor(255, 0);
  printCentered(0, "Memory:");
  display.println("");
  display.display();

  bool mem[64] = {0};

  waitFor(LOW);
  for(int i = 0; i < 64; i++)
  {
    waitFor(HIGH);
    waitFor(LOW);
    bool bit = digitalRead(Pin_Trans_Data);
    mem[i] = bit;
  }
/*  pinMode(Pin_Trans_Clk, OUTPUT);
  for(int i = 0; i < 8; i++)
  {
    byte b = shiftIn(Pin_Trans_Data, Pin_Trans_Clk, MSBFIRST);
    for(int x = 0; x < 8; x++)
      mem[i * 8 + x] = bitRead(b, x);
  }
  pinMode(Pin_Trans_Clk, INPUT);
*/
  for(int y = 0; y < 4; y++)
  {
    if(y > 0)
      display.println("");
    
    int bitStart = y * 16;
    
    if(bitStart < 10)
      display.print(" ");
    
    display.print(bitStart);
    display.print(": ");

    for(int x = 0; x < 4; x += 2)
    {
      if(x > 0)
        display.print(" ");
      PrintHex(mem, y * 16 + x, true);
      PrintHex(mem, y * 16 + x + 1, false);
    }
  }
  display.display();

  // what for user input
  while(GetButtonInput() == None);
}

void ProgramLoop()
{
  int max = 48; // number of potential ROMs to program
  int cur = 0;

  int selection;

  do {

    program(cur);

    display.clearDisplay();
    printCentered(0, "Cur Mem:");
    display.setCursor(0, 40);
    display.print("Combo ");
    display.print(cur + 1);
    display.print(" / ");
    display.print(max);
    
    display.setCursor(0, 8);
    DispRom(cur);

    display.display();
    do {
      selection = GetButtonInput();
    } while(selection == None);

    if(selection == BtnUp)
    {
      cur++;
    }
    else if(selection == BtnDown)
    {
      cur--;
    }
    else if(selection == BtnNext)
    {
      cur++;
    }

    if(cur >= max)
      cur = 0;
    if(cur < 0)
      cur = max - 1;

  } while(selection != BtnBack);
}

void loop() {
  MainMenuCommands entry = (MainMenuCommands)MainMenu();

  if(entry == Back)
  {
    // cancel
    display.clearDisplay();
  }
//  if(entry == DisplayMemory)
//  {
    //Serial.println("You selected display memory");
//    DispMem();
//  }
  if(entry == AutoProgram)
  {
    //program();
    ProgramLoop();
    //Serial.println("You selected auto program");
  }
  else if(entry == Temp)
  {
    //Serial.println("You selected custom program");
    do{
      double temp = getTemp();
      display.clearDisplay();
      display.println("Cur Temp:");
      display.print(temp);
      display.println(" deg C");

      display.print(temp * (9.0 / 5.0) + 32);
      display.println(" deg F");
      display.display();
      //delay(100);

    } while(GetButtonInput() == None);
  }
  else if(entry == About)
  {
    display.clearDisplay();
    display.print("Kevin Schaefer");
    display.print("Firmware: 1.0 ");
    display.print("Build: 7/1/'15");
    display.println("");
    display.print("kevin.l.schaef");
    display.print("er@gmail.com  ");
    display.display();
    while(GetButtonInput() == None);
    //Serial.println("You selected the about option");
  }
}
