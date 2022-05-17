///*******************************************/
// ECE-6483 Embedded Challenge (Spring 2022)
// Team Members: Jayesh Jayprakash Gaur and Divya Prakash Mannivanan
// Respective NET IDs: jjg9777 and dm5309
//
// This program uses SPI Communication to communicate with the HoneyWell Sensor.
// Pin Configuration: {MOSI, MISO, SCLK, SlaveSelect} = {PE_6, PE_5, PE_2, PE4}
//
// This program reads the pressure values and returns the systolic and diastolic values
//
// NOTE: Please run this program with the same directory structure as provided while submission.
// NOTE: The src folder should also contain the drivers folder as attached with this submission
//          so as to implement the LCD screen functionalities along with this main file.
///*******************************************/

#include "mbed.h"
#include "stdlib.h"
#include "drivers/LCD_DISCO_F429ZI.h"
// Global Variables
#define MIN_OUTPUT_VALUE 419430.4
#define MAX_OUTPUT_VALUE 3774873.6
// PIN Configurations are as MOSI, MISO, SCLK
SPI spi(PE_6, PE_5, PE_2);
DigitalOut slave_slct(PE_4);
LCD_DISCO_F429ZI lcd;

// Arrays to store the Pressure Values and the Pressure difference values
float pressure_values[2000] = {0.0};
float difference_values[2000] = {0.0};
float pressure = 0.0;

// Message Display variables
uint8_t msg[50];
uint8_t msg_2[50];
uint8_t msg_3[50];

//***********************************************************************************
// Function Name: config_spi
// This function sets the mode, byte transfer rate and frequency for our SPI Communication.
//***********************************************************************************
void config_spi()
{
    spi.format(8, 0);
    spi.frequency(500000);
}

//***********************************************************************************
// Function Name: display_heart
// This function enables to heart on the LCD screen when required.
//***********************************************************************************

void display_heart(int flg)
{
    if (flg == 1)
    {
        BSP_LCD_FillCircle(15, LINE(9.8), 4);
        BSP_LCD_FillCircle(20, LINE(9.8), 4);
        BSP_LCD_FillTriangle(11.1, 23.4, 18, LINE(9.7), LINE(9.7), LINE(10.3));
        BSP_LCD_FillCircle(220, LINE(9.8), 4);
        BSP_LCD_FillCircle(215, LINE(9.8), 4);
        BSP_LCD_FillTriangle(223.9, 211.6, 217, LINE(9.7), LINE(9.7), LINE(10.3));
    }
    else
    {
        int j = 0;
        while (j < 3)
        {
            BSP_LCD_FillCircle(109, LINE(9.8), 8);
            BSP_LCD_FillCircle(124, LINE(9.8), 8);
            BSP_LCD_FillTriangle(102.4, 130.7, 116, LINE(10.1), LINE(10.1), LINE(10.9));
            j++;
            wait_us(714285);
            lcd.Clear(LCD_COLOR_BLACK);
            wait_us(714285);
        }
    }
}

//***********************************************************************************
// Function Name: display_start_message
// This function is used to implement the LCD display , when the device is turned on.
// This function welcomes the user to our program via a series of texts.
//***********************************************************************************
void display_start_message() //
{

    int k = 0;
    while (k < 2)
    {
        lcd.Clear(LCD_COLOR_BLACK);
        lcd.SetBackColor(LCD_COLOR_BLACK);
        lcd.SetTextColor(LCD_COLOR_WHITE);
        BSP_LCD_FillRect(15, LINE(7.5), 210, LINE(4.12));
        lcd.SetTextColor(LCD_COLOR_BLACK);
        BSP_LCD_FillRect(20, LINE(8), 200, LINE(3));
        lcd.SetTextColor(LCD_COLOR_WHITE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)" PRESSURE_IS_ON", CENTER_MODE);
        wait_us(1000000);
        if (k == 1)
        {
            lcd.SetBackColor(LCD_COLOR_BLACK);
            lcd.SetTextColor(LCD_COLOR_RED);
            BSP_LCD_FillRect(15, LINE(7.5), 210, LINE(4.12));
            lcd.SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_FillRect(20, LINE(8), 200, LINE(3));
            lcd.SetTextColor(LCD_COLOR_RED);
            lcd.DisplayStringAt(0, LINE(9), (uint8_t *)" |PRESSURE_IS_ON|", CENTER_MODE);
            wait_us(1000000);
        }
        else
        {
            lcd.SetBackColor(LCD_COLOR_BLACK);
            lcd.SetTextColor(LCD_COLOR_BLUE);
            BSP_LCD_FillRect(15, LINE(7.5), 210, LINE(4.12));
            lcd.SetTextColor(LCD_COLOR_BLACK);
            BSP_LCD_FillRect(20, LINE(8), 200, LINE(3));
            lcd.SetTextColor(LCD_COLOR_BLUE);
            lcd.DisplayStringAt(0, LINE(9), (uint8_t *)" |PRESSURE_IS_ON|", CENTER_MODE);
            wait_us(1000000);
        }
        k++;
    }

    lcd.SetBackColor(LCD_COLOR_BLACK);
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetTextColor(LCD_COLOR_RED);
    display_heart(0);

    lcd.SetTextColor(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_LIGHTCYAN);
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)" PLEASE REMEMBER THE", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"FOLLOWING POINTS", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"BEFORE STARTING", CENTER_MODE);
    BSP_LCD_DrawRect(8, LINE(7), 224, LINE(5));
    wait_us(5000000);
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    BSP_LCD_DrawHLine(0, LINE(2), 240);
    lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"POINTS TO REMEMBER", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"MAKE SURE THAT", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"CUFF IS TIED TIGHT", CENTER_MODE);
    BSP_LCD_DrawRect(8, LINE(7), 224, LINE(4));
    wait_us(5000000);
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    BSP_LCD_DrawHLine(0, LINE(2), 240);
    lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"POINTS TO REMEMBER", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"THE FINAL BLOOD", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)" PRESSURE VALUE WILL", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"BE DISPLAYED WITH", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"COLOR CODES BASED", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"ON THE SEVERITY", CENTER_MODE);
    BSP_LCD_DrawRect(8, LINE(7), 224, LINE(7));
    wait_us(5000000);
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    BSP_LCD_DrawHLine(0, LINE(2), 240);
    lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"POINTS TO REMEMBER", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)"THE REALTIME BLOOD", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)" PRESSURE VALUES CAN", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"ALSO BE TRACKED", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"ON THE SCREEN", CENTER_MODE);
    BSP_LCD_DrawRect(8, LINE(7), 224, LINE(6));
    wait_us(5000000);
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)" LAST BUT NOT LEAST", CENTER_MODE);
    BSP_LCD_DrawRect(10, LINE(7), 220, LINE(3));
    wait_us(3000000);
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    lcd.DisplayStringAt(0, LINE(8), (uint8_t *)" SIT BACK AND RELAX", CENTER_MODE);
    BSP_LCD_DrawRect(10, LINE(7), 220, LINE(3));
    wait_us(5000000);
}

//***********************************************************************************
// Function Name: display_inflate
// This function is used to implement the LCD display.
// The screen shows the real time values of pressures on the screen.
//***********************************************************************************
void display_inflate(float value)
{
    lcd.SetTextColor(LCD_COLOR_BLACK);
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    lcd.SetBackColor(LCD_COLOR_LIGHTCYAN);
    BSP_LCD_DrawHLine(0, LINE(4), 240);
    lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"START INCREASING", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"THE PRESSURE VALUE", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"TILL IT IS 200mmHg", CENTER_MODE);
    sprintf((char *)msg, "%.2f mmHg", value);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)&msg, CENTER_MODE);
    BSP_LCD_DrawRect(40, LINE(8), 160, LINE(5));
}

//***********************************************************************************
// Function Name: display_deflate_Message
// This function is used to implement the LCD display.
// The screen displays an alert message on the screen , when the threshold pressure value is reached.
//***********************************************************************************
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

//***********************************************************************************
// Function Name: display_deflate_reading
// This function is used to implement the LCD display.
// The screen shows the real time values of pressures on the screen.
// This also gives the status of defaltion rate which can be used to adjust the pressure valve accordingly.
//***********************************************************************************
void display_deflate_reading(float value, float diff)
{ // change the background color of lcd to green and print message
    lcd.Clear(LCD_COLOR_LIGHTCYAN);
    lcd.SetBackColor(LCD_COLOR_LIGHTCYAN);
    lcd.SetTextColor(LCD_COLOR_BLACK);
    if ((diff) < -0.8)
    {
        lcd.Clear(LCD_COLOR_LIGHTCYAN);
        BSP_LCD_DrawHLine(0, LINE(3), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"STATUS OF THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"DEFLATION RATE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"THE PRESSURE VALUE", CENTER_MODE);
        sprintf((char *)msg, "%.2f mmHg", value);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"THE DEFLATION RATE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)" IS HIGH ", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"PLEASE RELEASE THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"PRESSURE STEADILY", CENTER_MODE);
        BSP_LCD_DrawRect(10, LINE(8), 220, LINE(6));
    }
    else if (((diff) > -0.2) && (diff < 0))
    {
        lcd.Clear(LCD_COLOR_LIGHTCYAN);
        BSP_LCD_DrawHLine(0, LINE(3), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"STATUS OF THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(2), (uint8_t *)"DEFLATION RATE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(5), (uint8_t *)"THE PRESSURE VALUE", CENTER_MODE);
        sprintf((char *)msg, "%.2f mmHg", value);
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
        sprintf((char *)msg, "%.2f mmHg", value);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"THE DEFLATION RATE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)" IS STEADY ", CENTER_MODE);
        BSP_LCD_DrawRect(10, LINE(8), 220, LINE(4));
    }
}

//***********************************************************************************
// Function Name: display_final_message
// This function is used to implement the LCD display.
// The screen displays the final result according to the observed values.
// The final results are color coded with the below codes:
//  GREEN --> Blood Pressure is in normal range
//  LIGHT_YELLOW --> Blood Pressure is in elevated range
//  YELLOW --> Blood Pressure is hypertension Stage-1
//  LIGHT_RED --> Blood Pressure is hypertension Stage-2
//  DARK_RED --> Blood Pressure is hypertension Stage-3
//***********************************************************************************
void display_final_message(float sys_val, float dias_val, int bpm)
{
    lcd.SetTextColor(LCD_COLOR_BLACK);

    if ((sys_val < 120) && (dias_val < 80))
    {
        lcd.Clear(LCD_COLOR_GREEN);
        lcd.SetBackColor(LCD_COLOR_GREEN);
        BSP_LCD_DrawHLine(0, LINE(2), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"FINAL RESULT", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"SYSTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mmHg", sys_val);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"DIASTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg_2, "VALUE IS %.1f mmHg", dias_val);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)&msg_2, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"HEART BEAT", CENTER_MODE);
        sprintf((char *)msg_3, "VALUE IS %d BPM", bpm);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)&msg_3, CENTER_MODE);
        lcd.SetTextColor(LCD_COLOR_RED);
        display_heart(1);
        lcd.SetTextColor(LCD_COLOR_BLACK);
        lcd.DisplayStringAt(0, LINE(13), (uint8_t *)"YOUR BLOOD", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(14), (uint8_t *)"PRESSURE IS IN THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(15), (uint8_t *)"NORMAL RANGE", CENTER_MODE);
        BSP_LCD_DrawRect(10, LINE(12), 220, LINE(5));
    }
    else if (((sys_val >= 120) && (sys_val < 130)) && ((dias_val < 80)))
    {
        lcd.Clear(LCD_COLOR_LIGHTYELLOW);
        lcd.SetBackColor(LCD_COLOR_LIGHTYELLOW);
        BSP_LCD_DrawHLine(0, LINE(2), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"FINAL RESULT", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"SYSTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mmHg", sys_val);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"DIASTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg_2, "VALUE IS %.1f mmHg", dias_val);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)&msg_2, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"HEART BEAT", CENTER_MODE);
        sprintf((char *)msg_3, "VALUE IS %d BPM", bpm);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)&msg_3, CENTER_MODE);
        lcd.SetTextColor(LCD_COLOR_RED);
        display_heart(1);
        lcd.SetTextColor(LCD_COLOR_BLACK);
        lcd.DisplayStringAt(0, LINE(13), (uint8_t *)"YOUR BLOOD", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(14), (uint8_t *)"PRESSURE IS IN THE", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(15), (uint8_t *)"ELEVATED RANGE", CENTER_MODE);
        BSP_LCD_DrawRect(10, LINE(12), 220, LINE(5));
    }
    else if (((sys_val >= 130) && (sys_val < 140)) || ((dias_val >= 80) && (dias_val < 90)))
    {
        lcd.Clear(LCD_COLOR_YELLOW);
        lcd.SetBackColor(LCD_COLOR_YELLOW);
        BSP_LCD_DrawHLine(0, LINE(2), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"FINAL RESULT", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"SYSTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mmHg", sys_val);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"DIASTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg_2, "VALUE IS %.1f mmHg", dias_val);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)&msg_2, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"HEART BEAT", CENTER_MODE);
        sprintf((char *)msg_3, "VALUE IS %d BPM", bpm);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)&msg_3, CENTER_MODE);
        lcd.SetTextColor(LCD_COLOR_RED);
        display_heart(1);
        lcd.SetTextColor(LCD_COLOR_BLACK);
        lcd.DisplayStringAt(0, LINE(13), (uint8_t *)"YOUR BLOOD", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(14), (uint8_t *)"PRESSURE IS HIGH AND", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(15), (uint8_t *)"IN THE RANGE OF", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(16), (uint8_t *)"HYPERTENSION", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(17), (uint8_t *)"STAGE-1", CENTER_MODE);
        BSP_LCD_DrawRect(4, LINE(12), 232, LINE(7));
    }
    else if (((sys_val >= 140) && (sys_val < 179)) || ((dias_val >= 90) && (dias_val < 120)))
    {
        lcd.Clear(LCD_COLOR_LIGHTRED);
        lcd.SetBackColor(LCD_COLOR_LIGHTRED);
        BSP_LCD_DrawHLine(0, LINE(2), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"FINAL RESULT", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"SYSTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mmHg", sys_val);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"DIASTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg_2, "VALUE IS %.1f mmHg", dias_val);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)&msg_2, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"HEART BEAT", CENTER_MODE);
        sprintf((char *)msg_3, "VALUE IS %d BPM", bpm);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)&msg_3, CENTER_MODE);
        lcd.SetTextColor(LCD_COLOR_DARKGRAY);
        display_heart(1);
        lcd.SetTextColor(LCD_COLOR_BLACK);
        lcd.DisplayStringAt(0, LINE(13), (uint8_t *)"YOUR BLOOD", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(14), (uint8_t *)"PRESSURE IS HIGH AND", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(15), (uint8_t *)"IN THE RANGE OF", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(16), (uint8_t *)"HYPERTENSION", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(17), (uint8_t *)"STAGE-2", CENTER_MODE);
        BSP_LCD_DrawRect(4, LINE(12), 232, LINE(7));
    }
    else if ((sys_val >= 180) || (dias_val >= 120))
    {
        lcd.Clear(LCD_COLOR_DARKRED);
        lcd.SetBackColor(LCD_COLOR_DARKRED);
        BSP_LCD_DrawHLine(0, LINE(2), 240);
        lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"FINAL RESULT", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(3), (uint8_t *)"SYSTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mmHg", sys_val);
        lcd.DisplayStringAt(0, LINE(4), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(6), (uint8_t *)"DIASTOLIC PRESSURE", CENTER_MODE);
        sprintf((char *)msg, "VALUE IS %.1f mmHg", dias_val);
        lcd.DisplayStringAt(0, LINE(7), (uint8_t *)&msg, CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"HEART BEAT", CENTER_MODE);
        sprintf((char *)msg_3, "VALUE IS %d BPM", bpm);
        lcd.DisplayStringAt(0, LINE(10), (uint8_t *)&msg_3, CENTER_MODE);
        lcd.SetTextColor(LCD_COLOR_DARKGRAY);
        display_heart(1);
        lcd.SetTextColor(LCD_COLOR_BLACK);
        lcd.DisplayStringAt(0, LINE(13), (uint8_t *)"YOUR BLOOD", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(14), (uint8_t *)"PRESSURE IS HIGH AND", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(15), (uint8_t *)"IN THE RANGE OF", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(16), (uint8_t *)"HYPERTENSION", CENTER_MODE);
        lcd.DisplayStringAt(0, LINE(17), (uint8_t *)"STAGE-3", CENTER_MODE);
        BSP_LCD_DrawRect(4, LINE(12), 232, LINE(7));
    }
}
//***********************************************************************************
// Function Name: display_error
// This function is used to implement the LCD display.
// The screen will show an error message if the deflation was too high such that ripple
// could not be analysed.
//***********************************************************************************
void display_error()
{
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    lcd.SetTextColor(LCD_COLOR_RED);
    BSP_LCD_DrawHLine(0, LINE(2), 240);
    lcd.DisplayStringAt(0, LINE(1), (uint8_t *)"!ALERT MESSAGE!", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)" DEFLATION RATE WAS", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(10), (uint8_t *)"NOT STEADY", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(11), (uint8_t *)"PLEASE PRESS THE", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(12), (uint8_t *)"RESET BUTTON TO", CENTER_MODE);
    lcd.DisplayStringAt(0, LINE(13), (uint8_t *)"START AGAIN", CENTER_MODE);
    BSP_LCD_DrawRect(14, LINE(8), 212, LINE(7));
}

//***********************************************************************************
// Function Name: get_pressure
// This function selects the slave (the Honeywell Sensor), gets one pressure reading
// and then unselects the slave again
//***********************************************************************************
float get_pressure()
{
    // unselect and select the slave sensor, just a precaution
    slave_slct = 1;
    slave_slct = 0;
    // Write to the MOSI
    spi.write(0xAA);
    spi.write(0x00);
    spi.write(0x00);
    wait_us(5000);

    // Unselect and select the slave again after clearing the MISO line
    slave_slct = 1;
    // To read the MISO, select the slave again by asserting the slave select line
    slave_slct = 0;

    // To get the pressure readings, we need to send 0xF0, 0x00, 0x00 and 0x00 commands. This will return 4 bytes
    //  byte 1 = status byte
    //  byte 2 through 4 = 24 bit pressure reading

    //We ignore the status register because it's optional. The datasheet specifies that we can simply wai for 5ms and continue.
    spi.write(0xF0);
    uint32_t data_byte_1 = spi.write(0x00);
    uint32_t data_byte_2 = spi.write(0x00);
    uint32_t data_byte_3 = spi.write(0x00);
    slave_slct = 1;

    // We saved the 3 bytes of pressure readings into 3 different variable.
    // Now we adjust the bits to get a single integer value to get the "Output value", as per the datasheet"
    u_int32_t output1 = data_byte_1 << 16 | data_byte_2 << 8 | data_byte_3;

    // Transfer Function B is used in this case for calculation of the actual pressure value in mmHg.
    return (((output1 - MIN_OUTPUT_VALUE) * (300)) / (MAX_OUTPUT_VALUE - MIN_OUTPUT_VALUE));
}

//***********************************************************************************************************************
// Function Name: deflation_rate_check
// This function is called when the user is done inflating the pressure cuff and now is in a position to start deflating.
// It will store all further pressue values in the pressure array which will be used for our calculations.
// This function also guides the user with deflation rates.
// Appropriate warnings/messages are displayed if the user is deflating too slow or too fast.
// The function exits when the pressure value reaches 30, which is well below the lower limit of human blood pressure.
//***********************************************************************************************************************
int deflation_rate_check()
{
    float current_pressure = 0.0;
    float previous_pressure = 0.0;
    float difference = 0.0;
    int i = 0;

    while (1)
    {
        // Increasing pressure till 200mm/Hg
        while (current_pressure < 200)
        {
            printf("\nCurrent pressure is: %f \n. Please get the pressure to 200mmHg to start the processing.", current_pressure);
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
                printf("\n Current pressure is: %0.2lf \n. Please maintain 1.5mmHg reduction per second.", current_pressure);
                wait_us(200000);

                difference = current_pressure - previous_pressure;
                printf("\n Difference from previous pressure is: %0.2lf. Please maintain 1.5mmHg reduction per second.", difference);
                difference_values[i] = difference;
                printf("\n AA %0.2lf. AA", difference_values[i]);
                if ((difference) < -0.8) // based on 4mm concept
                {
                    printf("\n You are releasing the pressure too fast, please slow down");
                    display_deflate_reading(current_pressure, difference);
                }
                else if (((difference) > -0.2) && (difference < 0))
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

//***********************************************************************************************************************
// Function Name: calculate_systolic
// This function calculates the Systolic Blood Pressure and returns it.
//
// Theory: Systolic Blood Pressure is the point of the first "Ripple" in the blood pressure graph.
//
// Approach:
// 1. A ripple is a positive slope. When the blood starts flowing again as we go below the systolic blood pressure, we see
//      ripples in our graph.
// 2. From the point of systolic blood pressure, we would see ripples continuously, thus the graph would alternate between
//      positive and negative slope.
// 3. The first positive slope in this pattern of positive and negative slopes is our systolic pressure.
// 4. Keeping this approach in mind, it is also possible to eliminate "noise", which prevents our device from detecting an
//      outlier positive slope.
// 5. The logic used for avoiding outlier is:
//      a. Our sampling rate is 0.2ms. Hence, we get 5 readings in 1 second.
//      b. When we read a positive slope, we check for consistency in the next 20 readings.
//      c. Assuming heartbeats to be at an average of 70-100,  We would get about 1 to 2 heartbeats every second.
//      d. As each heartbeat would cause a positive slope; In 20 readings, which is 4 seconds, we should at least see
//          4 positive slopes.
//      e. Hence, a positive slope will indicate a systolic value when the next 20 pressure readings have at least 4
//          positive slopes.
//***********************************************************************************************************************
float calculate_systolic() // Calculates the systolic
{
    int i = 0;
    int positive_count = 0;
    int negative_count = 0;
    float systolic_pressure = 0.0;

    // Keep trying to find a systolic value until we've exhausted all pressure values
    while (i < 1000)
    {
        // When you encounter a positive slope, check the next 20 slope values
        if (difference_values[i] > 0)
        {
            // Initialize count to zero
            positive_count = 0;
            negative_count = 0;

            // For the next 20 readings
            for (int j = i; j < i + 20; j++)
            {
                // If slope is positive, increment the positive slope counter
                if (difference_values[j] > 0)
                {
                    positive_count++;
                }
                // If slope is negative, increment the negative slope counter
                else
                {
                    negative_count++;
                }
            }

            // If positive slope count is greater than 4, we are detecting heart beats. So the i-th index has our Systolic Pressure Value.
            if (positive_count > 4)
            {
                systolic_pressure = pressure_values[i];
                return systolic_pressure;
            }
        }
        i++;
    }
    return systolic_pressure;
}

//***********************************************************************************************************************
// Function Name: calculate_MAP
// This function calculates the Mean Arterial Pressure and returns the index where it lies in the pressure readings array.
//
// Theory: The MAP is used to calculate the diastolic pressure. MAP is usually the midpoint between systolic and diastolic
//          pressure values.
//
// Approach:
// 1. Mean Arterial Pressure is at the point where we see the longest/highest ripple when our flood is flowing while
//      deflating.
// 2. We implement the algorithm "Longest Increasing Subsequence" to find this pressure value.
// 3. Our goal is to find the longest positive slope. Hence, we first look for a positive slope reading.
// 4. Then we check the next reading, and if that slope is also positive, it means the ripple is higher than just the previous
//      difference.
// 5. Thus, we calculate magnitude of all positive slopes and retrieve the highest.
//
// NOTE: Here, by slope, we mean a slant in the graph and not geometric slope.
//***********************************************************************************************************************
int calculate_MAP()
{
    int i = 0;
    float sum = 0.0;
    int MAP_index = 0;
    int index = 0;
    float max_ripple = 0.0;
    int j = 0;

    // By theory, MAP is usually in the range of 130 to 80. To save some time, we evaluate our readings in between these pressure values only
    for (i = 1; pressure_values[i] > 130; i++)
    {
        index = i;
    }

    for (i = index; pressure_values[i] > 80; i++)
    {
        sum = 0;
        // When you detect a positive slope, take sum of the all further slopes/differences until you see a negative slope/difference.
        if (difference_values[i] > 0)
        {
            for (j = i + 1; difference_values[j] >= 0; j++)
            {
                sum = sum + difference_values[j];
            }

            // If current slope length is greater than max, update max and map_index
            if (sum > max_ripple)
            {
                max_ripple = sum;
                MAP_index = j;
            }
        }
    }
    // Return index of the MAP value.
    return MAP_index;
}

//***********************************************************************************************************************
// Function Name: calculate_diastolic
// This function calculates the Diastolic Blood Pressure
//
// Theory: Since MAP is roughly the midpoint of Systolic and Diastolic Blood Pressure, we can calculate diastolic
//          conveniently, once we have the other two values.
//
// Approach: Assuming MAP as midpoint, evaluate diastolic from systolic and MAP values
//
//***********************************************************************************************************************
float calculate_diastolic(float systolic, float MAP_value)
{
    return (MAP_value - (systolic - MAP_value));
}

//***********************************************************************************************************************
// Function Name: calculate_heartbeat
// This function calculates the HeartBeats per Minute
//
// Theory: The number of peaks or ripples in our readings in a minute time will give heartbeats per minute for a person
//
// Approach:
// 1. From our raw readings, we sample 75 readings.
// 2. 75 readings take 15 seconds, which should be sufficient for calculating heartbeats.
// 3. We use the readings around our MAP value/index, considering that's the mean point.
// 4. A peak can be identified by checking the change in slope. Every time our consecutive slope reading goes from
//      positive to negative, we can infer that we just crossed a peak.
// 5. We count all peaks in these 75 readings, which give us the number of heartbeats in 15 seconds.
// 6. Multiplying that by 4, we get heartbeats per minute.
//***********************************************************************************************************************
int calculate_heartbeat(int MAP_index)
{
    int heartbeat_count = 0;
    // Analyze 75 readings with MAP being the midpoint
    for (int i = MAP_index - 37; i <= MAP_index + 37; i++)
    {
        if (difference_values[i] > 0 && difference_values[i + 1] < 0)
        {
            heartbeat_count++;
        }
    }
    // Return heartbeats per minute
    return heartbeat_count * 4;
}

// Main Function
int main()
{
    int chk_flag = 0;
    int i = 0;
    float MAP_value = 0;
    // float curt_pressure = 0.0;

    // Set SPI frequency, mode and byte transfer size.
    config_spi();
    display_start_message();
    chk_flag = deflation_rate_check();

    float systolic = calculate_systolic();
    printf("\n Systolic: %0.2lf", systolic);

    int MAP_index = calculate_MAP();
    MAP_value = pressure_values[MAP_index];
    printf("\n MAP: : %0.2lf", MAP_value);
    // This condition checks for the Gradual decrease in the pressure.
    if (MAP_value > 130 || MAP_value < 80)
    {
        printf("\n You did not deflate pressure correctly. Please try again!");
        display_error();
    }
    else
    {
        
        printf("\n MAP: : %0.2lf", MAP_value);

        float diastolic = calculate_diastolic(systolic, MAP_value);
        printf("\n Diastolic: %0.2lf", diastolic);

        int heartbeat = calculate_heartbeat(MAP_index);
        printf("\n Heartbeat: %d", heartbeat);
        
        display_final_message(systolic, diastolic, heartbeat);
    }
    printf("\n Done");
    printf("\n Done");
}