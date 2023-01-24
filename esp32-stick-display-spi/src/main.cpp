#include "Wire.h"
#include "SPI.h"
#include <ETH.h>
#include "Adafruit_GFX.h"
#include "Adafruit_ILI9341.h"
#include <DFRobot_MLX90614.h>
#include <SdFat.h>                // SD card & FAT filesystem library
#include <Adafruit_SPIFlash.h>    // SPI / QSPI flash library
#include <Adafruit_ImageReader.h> // Image-reading functions

static bool eth_connected = false;

#define TFT_CS    15
#define TFT_DC    4
#define SD_CS     5
#define TFT_RESET 2
Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC,TFT_RESET);
#define SPI_SCLK  14
#define SPI_MOSI  13
#define SPI_MISO  12

SdFat                SD;         // SD card filesystem
Adafruit_ImageReader reader(SD); // Image-reader object, pass in SD filesys
Adafruit_Image         img;        // An image loaded into RAM
int32_t                width  = 0, // BMP image dimensions
                       height = 0;
ImageReturnCode stat2; // Status from image-reading functions

void setup() {
  SPI.begin(SPI_SCLK,SPI_MISO,SPI_MOSI,TFT_CS);

  Serial.begin(115200);
  Serial.printf("Start\n\r");
  tft.begin();

  uint8_t x = tft.readcommand8(ILI9341_RDMODE);
  Serial.print("Display Power Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDMADCTL);
  Serial.print("MADCTL Mode: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDPIXFMT);
  Serial.print("Pixel Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDIMGFMT);
  Serial.print("Image Format: 0x"); Serial.println(x, HEX);
  x = tft.readcommand8(ILI9341_RDSELFDIAG);
  Serial.print("Self Diagnostic: 0x"); Serial.println(x, HEX); 
  
  Serial.println(F("Benchmark                Time (microseconds)"));
  if(!SD.begin(SdSpiConfig(SD_CS, SHARED_SPI,SD_SCK_MHZ(10),&SPI))) { // ESP32 requires 25 MHz limit
    Serial.println(F("SD begin() failed"));
    for(;;); // Fatal error, do not continue
  }

  tft.fillScreen(ILI9341_BLUE);
  Serial.print(F("Loading esp32_stick_miniimage_7.bmp to screen..."));
  tft.setRotation(3);
  stat2 = reader.drawBMP("/esp32_stick_miniimage_7.bmp", tft, 0, 0);
  reader.printStatus(stat2);   // How'd we do?
}


void loop(void) {
}

