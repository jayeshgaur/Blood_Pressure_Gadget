///*******************************************/
// This program takes the pressure values and returns the systolic and diastolic values
///*******************************************/
#include "mbed.h"
#include "stdlib.h"
#include "drivers/LCD_DISCO_F429ZI.h"
// Global Variables
#define MIN_OUTPUT_VALUE 419430.4
#define MAX_OUTPUT_VALUE 3774873.6

SPI spi(PE_6, PE_5, PE_2); // mosi, miso, sclk
DigitalOut slave_slct(PE_4);
LCD_DISCO_F429ZI lcd;

float pressure_values[1000] = {0};
float difference_values[1000] = {0};
float pressure = 0.0;
int i = 0;

// Message Display variables
uint8_t msg[50];
uint8_t msg_2[50];
void config_spi()
{
    spi.format(8, 0);
    spi.frequency(500000);
}
void init_LCD()
{ // helper function to initialize the lcd
    BSP_LCD_SetFont(&Font24);
    lcd.Clear(LCD_COLOR_LIGHTBLUE);
    lcd.SetBackColor(LCD_COLOR_LIGHTBLUE);
    lcd.SetTextColor(LCD_COLOR_BLACK);
}

void display_start_message() //
{

    int k = 0;
    while(k < 3)
    {
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    lcd.SetTextColor(LCD_COLOR_WHITE);
    BSP_LCD_DrawRect(20, LINE(7), 200, LINE(3));
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"PRESSURE_IS_ON", CENTER_MODE);
    wait_us(1000000);
    lcd.SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DrawRect(20, LINE(7), 200, LINE(3));
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"PRESSURE_IS_ON", CENTER_MODE);
    wait_us(1000000);
    k++;
    }
    lcd.SetTextColor(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_LIGHTBLUE);
    lcd.Clear(LCD_COLOR_LIGHTBLUE);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"PLEASE REMEMBER THE", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"FOLLOWING POINTS", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"BEFORE STARTING", CENTER_MODE);
    wait_us(5000000);
    lcd.Clear(LCD_COLOR_LIGHTBLUE);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"MAKE SURE THAT", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"CUFF IS TIED TIGHT", CENTER_MODE);
    wait_us(5000000);
    lcd.Clear(LCD_COLOR_LIGHTBLUE);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"THE BLOOD PRESSURE", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"IS DISPLAYED WITH", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"COLOR CODES, BASED", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"ON THE SEVERITY", CENTER_MODE);
    wait_us(5000000);
    lcd.Clear(LCD_COLOR_LIGHTBLUE);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"NOW YOU CAN SIT", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"BACK AND RELAX..", CENTER_MODE);
}

void display_inflate(float value)
{ // change the background color of lcd to green and print message
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    lcd.SetBackColor(LCD_COLOR_LIGHTCYAN);
    BSP_LCD_DrawHLine(0, LINE(4), 240);
    lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"START INCREASING", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"THE PRESSURE VALUE", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"TILL IT IS 200mm/Hg", CENTER_MODE);
    sprintf((char *)msg, "%.2f mm/Hg", value);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)&msg, CENTER_MODE);
    BSP_LCD_DrawRect(40, LINE(8), 160, LINE(5));
}
void display_deflate_Message()
{
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    lcd.SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DrawHLine(0, LINE(2), 240);
    lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"!ALERT MESSAGE!", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"THESHOLD VALUE", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"IS REACHED", CENTER_MODE);
    BSP_LCD_DrawRect(20, LINE(8), 200, LINE(4));
    wait_us(3000000);
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    BSP_LCD_DrawHLine(0, LINE(2), 240);
    lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"!ALERT MESSAGE!", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"START DECREASING", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"THE PRESSURE", CENTER_MODE);
    BSP_LCD_DrawRect(20, LINE(8), 200, LINE(4));
    wait_us(3000000);
}
void display_deflate_reading(float value, float diff)
{ // change the background color of lcd to green and print message
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    lcd.SetBackColor(LCD_COLOR_LIGHTCYAN);
    lcd.SetTextColor(LCD_COLOR_BLACK);
    if ((diff) < -1)
    {
        lcd.Clear(LCD_COLOR_LIGHTCYAN);
        BSP_LCD_DrawHLine(0, LINE(3), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"STATUS OF THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"DEFLATION RATE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"THE PRESSURE VALUE", CENTER_MODE);
        sprintf((char *)msg, "%.2f mm/Hg", value);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"THE DEFLATION RATE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)" IS HIGH ", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"PLEASE RELEASE THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"PRESSURE STEADILY", CENTER_MODE);
        BSP_LCD_DrawRect(10, LINE(8), 220, LINE(6));
    }
    else if ((diff) > -0.5)
    {
        lcd.Clear(LCD_COLOR_LIGHTCYAN);
        BSP_LCD_DrawHLine(0, LINE(3), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"STATUS OF THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"DEFLATION RATE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"THE PRESSURE VALUE", CENTER_MODE);
        sprintf((char *)msg, "%.2f mm/Hg", value);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"THE DEFLATION RATE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)" IS LOW ", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"PLEASE RELEASE THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"PRESSURE STEADILY", CENTER_MODE);
        BSP_LCD_DrawRect(10, LINE(8), 220, LINE(6));
    }
    else
    {
        lcd.Clear(LCD_COLOR_LIGHTCYAN);
        BSP_LCD_DrawHLine(0, LINE(3), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"STATUS OF THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"DEFLATION RATE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"THE PRESSURE VALUE", CENTER_MODE);
        sprintf((char *)msg, "%.2f mm/Hg", value);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"THE DEFLATION RATE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)" IS STEADY ", CENTER_MODE);
        BSP_LCD_DrawRect(10, LINE(8), 220, LINE(4));
    }
}
void display_final_message(float sys_val, float dias_val)
{
    lcd.SetTextColor(LCD_COLOR_BLACK);

    if ((sys_val < 120) && (dias_val < 80))
    {
        lcd.Clear(LCD_COLOR_GREEN);
        lcd.SetBackColor(LCD_COLOR_GREEN);
        BSP_LCD_DrawHLine(0, LINE(2), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"FINAL RESULT", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"SYSTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mm/Hg", sys_val);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"DIASTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg_2, "VALUE IS %.1f mm/Hg", dias_val);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)&msg_2, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"YOUR BLOOD", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"PRESSURE IS IN THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"NORMAL RANGE", CENTER_MODE);
        BSP_LCD_DrawRect(10, LINE(9), 220, LINE(5));
    }
    else if (((sys_val >= 120) && (sys_val < 130)) && ((dias_val < 80)))
    {
        lcd.Clear(LCD_COLOR_LIGHTYELLOW);
        lcd.SetBackColor(LCD_COLOR_LIGHTYELLOW);
        BSP_LCD_DrawHLine(0, LINE(2), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"FINAL RESULT", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"SYSTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mm/Hg", sys_val);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"DIASTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg_2, "VALUE IS %.1f mm/Hg", dias_val);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)&msg_2, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"YOUR BLOOD", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"PRESSURE IS IN THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"ELEVATED RANGE", CENTER_MODE);
        BSP_LCD_DrawRect(10, LINE(9), 220, LINE(5));
    }
    else if (((sys_val >= 130) && (sys_val < 140)) || ((dias_val >= 80) && (dias_val < 90)))
    {
        lcd.Clear(LCD_COLOR_YELLOW);
        lcd.SetBackColor(LCD_COLOR_YELLOW);
        BSP_LCD_DrawHLine(0, LINE(2), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"FINAL RESULT", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"SYSTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mm/Hg", sys_val);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"DIASTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg_2, "VALUE IS %.1f mm/Hg", dias_val);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)&msg_2, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"YOUR BLOOD", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"PRESSURE IS HIGH AND", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"IN THE RANGE OF", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(13), (uint8_t *)"HYPERTENSION", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(14), (uint8_t *)"STAGE-1", CENTER_MODE);
        BSP_LCD_DrawRect(4, LINE(9), 232, LINE(7));
    }
    else if (((sys_val >= 140) && (sys_val < 179)) || ((dias_val >= 90) && (dias_val < 120)))
    {
        lcd.Clear(LCD_COLOR_LIGHTRED);
        lcd.SetBackColor(LCD_COLOR_LIGHTRED);
        BSP_LCD_DrawHLine(0, LINE(2), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"FINAL RESULT", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"SYSTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mm/Hg", sys_val);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"DIASTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg_2, "VALUE IS %.1f mm/Hg", dias_val);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)&msg_2, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"YOUR BLOOD", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"PRESSURE IS HIGH AND", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"IN THE RANGE OF", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(13), (uint8_t *)"HYPERTENSION", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(14), (uint8_t *)"STAGE-2", CENTER_MODE);
        BSP_LCD_DrawRect(4, LINE(9), 232, LINE(7));
    }
    else if ((sys_val >= 180) || (dias_val >= 120))
    {
        lcd.Clear(LCD_COLOR_DARKRED);
        lcd.SetBackColor(LCD_COLOR_DARKRED);
        BSP_LCD_DrawHLine(0, LINE(2), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"FINAL RESULT", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"SYSTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mm/Hg", sys_val);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"DIASTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mm/Hg", dias_val);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"YOUR BLOOD", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"PRESSURE IS HIGH AND", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"IN THE RANGE OF", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(13), (uint8_t *)"HYPERTENSION", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(14), (uint8_t *)"STAGE-3", CENTER_MODE);
        BSP_LCD_DrawRect(4, LINE(9), 232, LINE(7));
    }
}

float get_pressure()
{
    slave_slct = 1;
    // Write to the MOSI
    slave_slct = 0;
    spi.write(0xAA);
    spi.write(0x00);
    spi.write(0x00);
    wait_us(5000);
    slave_slct = 1;

    // To read the MISO
    slave_slct = 0;
    spi.write(0xF0);
    uint32_t data_byte_1 = spi.write(0x00);
    uint32_t data_byte_2 = spi.write(0x00);
    uint32_t data_byte_3 = spi.write(0x00);
    slave_slct = 1;
    // printf("\n The status value is %d", variable);
    // printf("\n The output value is %d", final);
    u_int32_t output1 = data_byte_1 << 16 | data_byte_2 << 8 | data_byte_3;

    // Transfer Function B is used in this case for calculation of the final result
    return (((output1 - MIN_OUTPUT_VALUE) * (300)) / (MAX_OUTPUT_VALUE - MIN_OUTPUT_VALUE));
}
int deflation_rate_check()
{
    float current_pressure = 0.0;
    float previous_pressure = 0.0;
    float difference = 0.0;
    int i = 0;

    while (1)
    {
        // Increasing pressure
        while (current_pressure < 200)
        {
            printf("\nCurrent pressure is: %f \n. Please get the pressure to 200mm/Hg to start the processing.", current_pressure);
            current_pressure = get_pressure();
            display_inflate(current_pressure);
            wait_us(500000);
        }
        printf("\n The threshold has reached , Please start decreasing the pressure");
        display_deflate_Message();
        while (1)
        {
            previous_pressure = current_pressure;
            current_pressure = get_pressure();
            if ((current_pressure - previous_pressure) < 0)
            {
                break;
            }
            else
            {
                continue;
            }
        }
        // Maximum pressure reached, now start deflation
        previous_pressure = current_pressure;

        // Decreasing pressure
        while (1)
        {
            if (current_pressure > 30)
            {
                current_pressure = get_pressure();

                pressure_values[i] = current_pressure;
                printf("\n Current pressure is: %0.2lf \n. Please maintain 1.5mm/Hg reduction per second.", current_pressure);
                wait_us(333333);

                difference = current_pressure - previous_pressure;
                printf("\n Difference from previous pressure is: %0.2lf. Please maintain 1.5mm/Hg reduction per second.", difference);
                difference_values[i] = difference;
                printf("\n AA %0.2lf. AA", difference_values[i]);
                if ((difference) < -1) // based on 4mm concept
                {
                    printf("\n You are releasing the pressure too fast, please slow down");
                    display_deflate_reading(current_pressure, difference);
                }
                else if ((difference) > -0.5)
                {
                    printf("\n You are releasing the pressure too slow, please release the pressure faster");
                    display_deflate_reading(current_pressure, difference);
                }
                else
                {
                    printf("\n Deflation rate is Ideal!");
                    display_deflate_reading(current_pressure, difference);
                }
                previous_pressure = current_pressure;
            }
            else
            {
                printf("===Coming Out");
                break;
            }
            i++;
        }
        printf("\n ===Came Out");
        break;
    }
    printf("\n ===Came Out other");
    return 1;
}

void pressure_calculate() // Calculates the systolic and Diastolic Pressures
{
    int pressure_array_size = sizeof(pressure_values) / sizeof(pressure_values[0]);
    int difference_array_size = sizeof(difference_values) / sizeof(difference_values[0]);
    for (i = 0; i < difference_array_size; i++)
    {
        if (difference_values[i] < 0)
        {
            printf("\n =====i %d,value %f", i, difference_values[i]);
            break;
        }
    }
}
int main()
{
    int chk_flag = 0;
    int i = 0;
    // float curt_pressure = 0.0;
    display_start_message();
    config_spi();
    chk_flag = deflation_rate_check();
    int pressure_array_size = sizeof(pressure_values) / sizeof(pressure_values[0]);
    for (i = 0; i < pressure_array_size; i++)
    {
            printf("\n , %f",pressure_values[i]);
    }
    pressure_calculate();
    display_final_message(200.1, 121.2);
    printf("\n Done");
    printf("\n Done");
}