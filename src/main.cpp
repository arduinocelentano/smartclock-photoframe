#define FS_NO_GLOBALS
#include <FS.h>
#include <TFT_eSPI.h>
#include <TJpg_Decoder.h>

#define TIMEOUT 25000
#define IMAGES_COUNT 150

TFT_eSPI tft = TFT_eSPI();

bool tft_output(int16_t x, int16_t y, uint16_t w, uint16_t h, uint16_t* bitmap)
{
   // Stop further decoding as image is running off bottom of screen
  if ( y >= tft.height() ) return 0;

  // This function will clip the image block rendering automatically at the TFT boundaries
  tft.pushImage(x, y, w, h, bitmap);

  // Return 1 to decode next block
  return 1;
}

//disappearing effect with random pixels
void disappear_pixels()
{
  for (u_int32_t i=0; i<19999; i++){
    tft.fillRect(rand()%(tft.width()), rand()%(tft.height()), 5,5, TFT_BLACK);
  }
  tft.fillScreen(TFT_BLACK);
}

//disappearing effect with horizontal lines
void disappear_hlines()
{
  for (u_int8_t i=0; i<40; i++){
    for (u_int8_t j=i; j<240; j+=40)
      tft.drawLine(0, j, tft.width() ,j, TFT_BLACK);
    delay(10);
    }
  tft.fillScreen(TFT_BLACK);
}

//disappearing effect with zooming-out rectangle
void disappear_rectangle()
{
  for (u_int8_t i=0; i<120; i++){
    tft.drawRect(i, i, tft.width()-2*i, tft.height()-2*i, TFT_BLACK);
    delay(10);
    }
  tft.fillScreen(TFT_BLACK);
}

void setup() {
  randomSeed(analogRead(0));
  Serial.begin(115200);
  //Turning backlight on
  pinMode(5, OUTPUT);
  digitalWrite(5, LOW);
  tft.begin();
  tft.fillScreen(0);
 
  if (!SPIFFS.begin()) {
    Serial.println("SPIFFS initialization failed!");
    while (1) yield(); // Stay here twiddling thumbs waiting
  }
  Serial.println("\r\nSPIFS Initialization done.");

  tft.setSwapBytes(true); // We need to swap the colour bytes (endianess)

  // The jpeg image can be scaled by a factor of 1, 2, 4, or 8
  TJpgDec.setJpgScale(1);

  // The decoder must be given the exact name of the rendering function above
  TJpgDec.setCallback(tft_output);
}

void loop() {
  tft.fillScreen(TFT_BLACK);

  String filename = "/" + String(rand()%IMAGES_COUNT+1) + ".jpg";
  // Time recorded for test purposes
  //uint32_t t = millis();

  // Get the width and height in pixels of the jpeg if you wish
  //uint16_t w = 0, h = 0;
  //TJpgDec.getFsJpgSize(&w, &h, filename); // Note name preceded with "/"
  //Serial.print("Width = "); Serial.print(w); Serial.print(", height = "); Serial.println(h);

  // Draw the image, top left at 0,0
  TJpgDec.drawFsJpg(0, 0, filename);

  // How much time did rendering take
  //t = millis() - t;
  //Serial.print(t); Serial.println(" ms");

  // Wait before drawing again
  delay(TIMEOUT);

  //disappear
  switch (rand()%3){
    case 0:
      disappear_pixels();
      break;
    case 1:
      disappear_hlines();
      break;
    case 2:
      disappear_rectangle();
      break;
  }
}

