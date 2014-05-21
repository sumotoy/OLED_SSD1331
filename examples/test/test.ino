/*
OLED_SSD1331 Test
 
 */


#include <Adafruit_GFX.h>
#include <OLED_SSD1331.h>
#include <SPI.h>

/*
Note for Teensy 3.x !!!!!!!!!!!!!!!!!!!
 You should choose from those pins for CS and RS:
 2,6,9,10,15,20,23
 */
#define cs   10
#define rst  14
#define dc   9


// Color definitions
#define	BLACK           0x0000
#define	BLUE            0x001F
#define	RED             0xF800
#define	GREEN           0x07E0
#define CYAN            0x07FF
#define MAGENTA         0xF81F
#define YELLOW          0xFFE0  
#define WHITE           0xFFFF
#define TRANSPARENT     -1


OLED_SSD1331 display = OLED_SSD1331(cs, dc, rst);

float p = 3.1415926;

void setup(void) {
  display.begin();
  display.setBitrate(24000000);

  uint16_t time = millis();
  time = millis() - time;

  lcdTestPattern();
  delay(1000);

  display.clearScreen();
  display.setCursor(0,0);
  display.print("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa");
  delay(1000);

  tftPrintTest();
  delay(2000);

  //a single pixel
  display.drawPixel(96/2, 64/2, GREEN);
  delay(500);

  // line draw test
  testlines(YELLOW);
  delay(500);    

  // optimized lines
  testfastlines(RED, BLUE);
  delay(500);    

  testdrawrects(GREEN);
  delay(1000);

  testfillrects(BLUE, YELLOW);
  delay(1000);

  randomRect(0);
  delay(100);
  randomCircles(0);
  delay(100);
  randomLines();
  delay(100);
  randomPoints();
  delay(500);
  
  display.clearScreen();
  testfillcircles(10, BLUE);
  testdrawcircles(10, WHITE);
  delay(1000);

  testroundrects();
  delay(500);

  testtriangles();
  delay(500);

}

void loop() {
  testlines(random(0x0010,0xFFFF));
  randomLines();
  //randomCircles(1);
  randomCircles(0);
  randomRect(1);
  randomRect(1);
  randomRect(1);
  randomRect(1);
  randomRect(1);
  randomRect(0);
  randomRect(0);
  randomRect(0);
  randomRect(0);
  randomRect(0);
  randomRect(0);
  randomPoints();
}

void testlines(uint16_t color) {
  display.clearScreen();
  for (int16_t x=0; x < 96-1; x+=6) {
    display.drawLine(0, 0, x, 64-1, color);
  }
  for (int16_t y=0; y < 64-1; y+=6) {
    display.drawLine(0, 0, 96-1, y, color);
  }
  display.clearScreen();
  for (int16_t x=0; x < 96-1; x+=6) {
    display.drawLine(96-1, 0, x, 64-1, color);
  }
  for (int16_t y=0; y < 64-1; y+=6) {
    display.drawLine(96-1, 0, 0, y, color);
  }

  display.clearScreen();
  for (int16_t x=0; x < 96-1; x+=6) {
    display.drawLine(0, 64-1, x, 0, color);
  }
  for (int16_t y=0; y < 64-1; y+=6) {
    display.drawLine(0, 64-1, 96-1, y, color);
  }
  display.clearScreen();
  for (int16_t x=0; x < 96-1; x+=6) {
    display.drawLine(96-1, 64-1, x, 0, color);
  }
  for (int16_t y=0; y < 64-1; y+=6) {
    display.drawLine(96-1, 64-1, 0, y, color);
  }
  delay(500);
}


void testdrawtext(char *text, uint16_t color) {
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  for (uint8_t i=0; i < 168; i++) {
    if (i == '\n') continue;
    display.write(i);
    if ((i > 0) && (i % 21 == 0))
      display.println();
  }    
}

void testfastlines(uint16_t color1, uint16_t color2) {
  display.clearScreen();
  for (int16_t y=0; y < 64-1; y+=5) {
    display.drawFastHLine(0, y, 96-1, color1);
  }
  for (int16_t x=0; x < 96-1; x+=5) {
    display.drawFastVLine(x, 0, 64-1, color2);
  }
}

void testdrawrects(uint16_t color) {
  display.clearScreen();
  for (int16_t x=0; x < 64-1; x+=6) {
    display.drawRect((96-1)/2 -x/2, (64-1)/2 -x/2 , x, x, color);
  }
}

void testfillrects(uint16_t color1, uint16_t color2) {
  display.clearScreen();
  for (int16_t x=64-1; x > 6; x-=6) {
    display.fillRect((96-1)/2 -x/2, (64-1)/2 -x/2 , x, x, color1);
    display.drawRect((96-1)/2 -x/2, (64-1)/2 -x/2 , x, x, color2);
  }
}

void testfillcircles(uint8_t radius, uint16_t color) {
  for (uint8_t x=radius; x < 96-1; x+=radius*2) {
    for (uint8_t y=radius; y < 64-1; y+=radius*2) {
      display.fillCircle(x, y, radius, color);
    }
  }  
}

void testdrawcircles(uint8_t radius, uint16_t color) {
  for (int16_t x=0; x < 96-1+radius; x+=radius*2) {
    for (int16_t y=0; y < 64-1+radius; y+=radius*2) {
      display.drawCircle(x, y, radius, color);
    }
  }  
}

void testtriangles() {
  display.clearScreen();
  int color = 0xF800;
  int t;
  int w = 96/2;
  int x = 64;
  int y = 0;
  int z = 96;
  for(t = 0 ; t <= 15; t+=1) {
    display.drawTriangle(w, y, y, x, z, x, color);
    x-=4;
    y+=4;
    z-=4;
    color+=100;
  }
}

void testroundrects() {
  display.clearScreen();
  int color = 100;
  int i;
  int t;
  for(t = 0 ; t <= 4; t+=1) {
    int x = 0;
    int y = 0;
    int w = 96;
    int h = 64;
    for(i = 0 ; i <= 24; i+=1) {
      display.drawRoundRect(x, y, w, h, 5, color);
      x+=2;
      y+=3;
      w-=4;
      h-=6;
      color+=1100;
    }
    color+=100;
  }
}

void tftPrintTest() {
  display.clearScreen();
  display.setCursor(0, 5);
  display.setTextColor(RED);  
  display.setTextSize(1);
  display.println("Hello World!");
  display.setTextColor(YELLOW, GREEN);
  display.setTextSize(2);
  display.print("Hello Wo");
  display.setTextColor(BLUE);
  display.setTextSize(3);
  display.print(12.57);
  delay(1500);
  display.setCursor(0, 5);
  display.clearScreen();
  display.setTextColor(WHITE);
  display.setTextSize(0);
  display.println("Hello World!");
  display.setTextSize(1);
  display.setTextColor(GREEN);
  display.print(p, 5);
  display.println(" Want pi?");
  display.print(8675309, HEX); // print 8,675,309 out in HEX!
  display.print(" Print HEX");
  display.setTextColor(WHITE);
  display.println("Sketch has been");
  display.println("running for: ");
  display.setTextColor(MAGENTA);
  display.print(millis() / 1000);
  display.setTextColor(WHITE);
  display.print(" seconds.");
}


/**************************************************************************/
/*! 
 @brief  Renders a simple test pattern on the LCD
 */
/**************************************************************************/
void lcdTestPattern(void)
{
  uint32_t i,j;
  display.goTo(0, 0);

  for(i=0;i<64;i++)
  {
    for(j=0;j<96;j++)
    {
      if(i>55){
        display.writeData(WHITE>>8);
        display.writeData(WHITE);
      }
      else if(i>47){
        display.writeData(BLUE>>8);
        display.writeData(BLUE);
      }
      else if(i>39){
        display.writeData(GREEN>>8);
        display.writeData(GREEN);
      }
      else if(i>31){
        display.writeData(CYAN>>8);
        display.writeData(CYAN);
      }
      else if(i>23){
        display.writeData(RED>>8);
        display.writeData(RED);
      }
      else if(i>15){
        display.writeData(MAGENTA>>8);
        display.writeData(MAGENTA);
      }
      else if(i>7){
        display.writeData(YELLOW>>8);
        display.writeData(YELLOW);
      }
      else {
        display.writeData(BLACK>>8);
        display.writeData(BLACK);
      }
    }
  }
}


void randomRect(bool fill){
  display.clearScreen();
  uint8_t k,c;
  for (k = 0; k < 16; k++) {
    for (c = 0; c < 32; c++) {
      uint8_t cx, cy, x, y, w, h;
      //  center
      cx = random(0,96);//cx = rand() % 96;
      cy = random(0,64);//cy = rand() % 64;
      //  size
      w = random(0,30 + 6);//w = rand() % 30 + 6;
      h = random(0,20 + 4);//h = rand() % 20 + 4;
      //  upper-left
      x = cx - w / 2;
      y = cy - h / 2;
      if (x < 0) x = 0;
      if (y < 0) y = 0;
      //  adjust size
      if (x + w > 96) w = 96 - x;
      if (y + h > 64) h = 64 - y;
      if (fill){
        display.fillRect(x, y, w, h,random(0x0010,0xFFFF));
      } 
      else {
        display.drawRect(x, y, w, h,random(0x0010,0xFFFF));
      }

    }
    display.clearScreen();
  }
}

void randomCircles(bool fill){
  display.clearScreen();
  uint8_t k,c;
  for (k = 0; k < 64; k++) {
    for (c = 0; c < 32; c++) {
      //  coordinates
      uint8_t x = random(0,90 + 3), y = random(0,60 + 2), r = random(0,20 + 1);
      if (x - r <  0) r = x;
      if (x + r > 95) r = 95 - x;
      if (y - r <  0) r = y;
      if (y + r > 63) r = 63 - y;
      if (fill){
        display.fillCircle(x, y, r,random(0x0010,0xFFFF));
      } 
      else {
        display.drawCircle(x, y, r,random(0x0010,0xFFFF));
      }
    }
    if (!fill)display.clearScreen();
  }
}


void randomLines(){
  display.clearScreen();
  uint8_t k,c;
  for (k = 0; k < 64; k++) {
    for (c = 0; c < 32; c++) {
      uint8_t x1 = random(0,96), y1 = random(0,64), x2 = random(0,96), y2 = random(0,64);
      display.drawLine(x1, y1, x2, y2,random(0x0010,0xFFFF));
    }
    display.clearScreen();
  }
}


void randomPoints(){
  display.clearScreen();
  int k,c;
  for (k = 0; k < 128; k++) {
    for (c = 0; c < 1000; c++) {
      uint8_t x = random(0,96), y = random(0,64);
      display.drawPixel(x, y,random(0x0010,0xFFFF));
    }
    display.clearScreen();
  }
}
