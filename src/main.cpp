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
// Minor Changes
// #define output_min 419430.4
// #define output_max 3774873.6
#include <mbed.h>
#include <stdlib.h>

SPI spi(PA_7, PA_6, PA_5); // mosi, miso, sclk
DigitalOut slave_slct(PA_4);

#define output_min 419430.4
#define output_max 3774873.6

float pressure_values[1000] = {0};
float differences[1000] = {0};

float pressure = 0.0;
int i = 0;

void config_spi()
{
  spi.format(8, 0);
  spi.frequency(500000);
}

float get_pressure()
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

  // Transfer Function B
  return (((output1 - output_min) * (300)) / (output_max - output_min));

  // printf("\n Difference: %0.2lf", pressure - previous_pressure);

  // if((previous_pressure - pressure) < -1.5){
  //   printf("You are releasing the pressure too fast, please slow down");
  // }
  // else if((previous_pressure - pressure) > -0.5){
  //   printf("You are releasing the pressure too slow, please release the pressure faster");
  // }
  // else{
  //   printf("Deflation rate is perfect!");
  // }
  // printf("\nOutput: : %0.2lf", pressure);
  // pressure_values[i] = pressure;
  // i = i + 1;
  // printf("RD2: %d", byte2);
  // printf("RD3: %d", byte3);
  // wait_us(200000);
}

int main()
{
  float temp = 0;
  float current_pressure = 0.0;
  float previous_pressure = 0.0;
  float difference = 0.0;
  config_spi();
  while (1)
  {
    // Increasing pressure
    while (current_pressure < 200)
    {
      printf("\nCurrent pressure is: %f \n. Please get the pressure to 200mm/Hg to start the processing.", current_pressure);
      current_pressure = get_pressure();
      wait_us(500000);
    }

    //Maximum pressure reached, now start deflation
    i = 0;
    previous_pressure = current_pressure;

    // Decreasing pressure
    while (1)
    {
      current_pressure = get_pressure();
      
      pressure_values[i] = current_pressure;
      printf("\nCurrent pressure is: %0.2lf \n. Please maintain 1.5mm/Hg reduction per second.", current_pressure);
      wait_us(333333);

      difference = current_pressure - previous_pressure;
      printf("\nDifference from previous pressure is: %0.2lf. Please maintain 1.5mm/Hg reduction per second.", difference);
      if(current_pressure < 30){
        break;
      }
      differences[i] = difference;
      printf("\nAA %0.2lf. AA", differences[i]);
      if ((difference) < -1)
      {
        printf("\nYou are releasing the pressure too fast, please slow down");
      }
      else if ((difference) > -0.5)
      {
        printf("\nYou are releasing the pressure too slow, please release the pressure faster");
      }
      else
      {
        printf("\nDeflation rate is perfect!");
      }
      previous_pressure = current_pressure;
      i++;
    }

    printf("START OF CALCULATIONS");
    //Calculations
    //int length_of_pressure_array = sizeof(pressure_values)/sizeof(pressure_values[0]);
    //int length_of_differences = sizeof(differences)/sizeof(differences[0]);

    i=0;
    temp = differences[i];
    while(temp < 0){
      i++;
      temp = differences[i];
    }
    printf("Systolic Pressure: %0.2lf", differences[i]);
    return 0;
  }
}