// #include <mbed.h>
// #include <stdlib.h>
// #include "drivers/LCD_DISCO_F429ZI.h"
// #define BACKGROUND 1
// #define FOREGROUND 0
// #define GRAPH_PADDING 5
// /*******************************************/
// // DEFINES
// #define STATUS_REG 0x27
// #define WHO_AM_I 0x0F
// #define Pi 3.142
// /*******************************************/
// // PIN DECLARATIONS
// /*******************************************/
// // SPI spi(PF_9, PF_8, PF_7);
// SPI spi(PA_7, PA_6, PA_5); // mosi, miso, sclk
// DigitalOut slave_slct(PA_4);
// LCD_DISCO_F429ZI lcd;
// /*******************************************/
// // GLOBAL VARIABLES
// /*******************************************/
// #define SPI_READ 0xF0
// #define SPI_WRITE 0xAA
// #define SPI_DUMMY 0x00
// /**************************/

// #define output_min 419430.4
// #define output_max 3774873.6
#include <mbed.h>
#include <stdlib.h>

SPI spi(PA_7, PA_6, PA_5); // mosi, miso, sclk
DigitalOut slave_slct(PA_4);

#define output_min 419430.4
#define output_max 3774873.6

float raw_pressure[1000] = {0};
float previous_pressure = 0.0;
float pressure = 0.0;
int i = 0;

void config_spi()
{
  spi.format(8, 0);
  spi.frequency(500000);
}

void get_pressure()
{
  slave_slct = 1;

  slave_slct = 0;
  // spi.write(SPI_WRITE);
  // spi.write(0x00);
  spi.write(0xAA);
  spi.write(0x00);
  spi.write(0x00);
  wait_us(5000);
  slave_slct = 1;

  // spi.write();
  // spi.write(0x00);
  // spi.write(0x00);
  slave_slct = 0;
  spi.write(0xF0);
  uint32_t byte1 = spi.write(0x00);
  uint32_t byte2 = spi.write(0x00);
  uint32_t byte3 = spi.write(0x00);
  slave_slct = 1;
  // printf("\n The status value is %d", variable);
  // printf("\n The output value is %d", final);

  u_int32_t output1 = byte1 << 16 | byte2 << 8 | byte3;

  previous_pressure = pressure;

  // Transfer Function B
  pressure = (((output1 - output_min) * (300)) / (output_max - output_min));

  if((previous_pressure - pressure) > 1.5){
    printf("You are releasing the pressure too fast, please slow down");
  }
  else if((previous_pressure - pressure) < 0.5){
    printf("You are releasing the pressure too slow, please release the pressure faster");
  }
  else{
    printf("Deflation rate is perfect!");
  }
  printf("\nOutput: : %0.2lf", pressure);
  raw_pressure[i] = pressure;
  i = i + 1;
  // printf("RD2: %d", byte2);
  // printf("RD3: %d", byte3);
  wait_us(500000);
}

int main()
{
  config_spi();
  while(i<500){
    get_pressure();
  }
  return 0;
}