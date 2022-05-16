///*******************************************/
// This program takes the pressure values and returns the systolic and diastolic values
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

float pressure_values[472] = {0.0};

float difference_values[472] = {0.0}

float pressure = 0.0;
int i = 0;

// Message Display variables
uint8_t msg[50];
uint8_t msg_2[50];

//Setup SPI configurations.
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
        wait_us(2000000);
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

    lcd.SetTextColor(LCD_COLOR_WHITE);
    lcd.SetBackColor(LCD_COLOR_BLACK);
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"STARTING UP.", CENTER_MODE);
    wait_us(1000000);
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)" STARTING UP..", CENTER_MODE);
    wait_us(1000000);
    lcd.Clear(LCD_COLOR_BLACK);
    lcd.DisplayStringAt(0, LINE(9), (uint8_t *)"  STARTING UP...", CENTER_MODE);
    wait_us(1000000);

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
void display_inflate(float value)
{
    lcd.SetTextColor(LCD_COLOR_BLACK);
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

//***********************************************************************************
// This function selects the slave (our Honeywell Sensor), gets one pressure reading
// and then unselects the slave again
//***********************************************************************************
float get_pressure()
{
    //unselect and select the slave sensor, just a precaution
    slave_slct = 1;
    slave_slct = 0;
    // Write to the MOSI
    spi.write(0xAA);
    spi.write(0x00);
    spi.write(0x00);
    wait_us(5000);

    //Unselect and select the slave again after clearing the MISO line
    slave_slct = 1;
    // To read the MISO, select the slave again by asserting the slave select line
    slave_slct = 0;

    //To get the pressure readings, we need to send 0xF0, 0x00, 0x00 and 0x00 commands. This will return 4 bytes
    // byte 1 = status byte
    // byte 2 through 4 = 24 bit pressure reading
    spi.write(0xF0);
    uint32_t data_byte_1 = spi.write(0x00);
    uint32_t data_byte_2 = spi.write(0x00);
    uint32_t data_byte_3 = spi.write(0x00);
    slave_slct = 1;

    //We saved the 3 bytes of pressure readings into 3 different variable.
    //Now we adjust the bits to get a single integer value to get the "Output value, as per the datasheet"
    u_int32_t output1 = data_byte_1 << 16 | data_byte_2 << 8 | data_byte_3;

    // Transfer Function B is used in this case for calculation of the actual pressure value in mmHg.
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
                wait_us(200000);

                difference = current_pressure - previous_pressure;
                printf("\n Difference from previous pressure is: %0.2lf. Please maintain 1.5mm/Hg reduction per second.", difference);
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

float calculate_systolic() // Calculates the systolic
{
    int i = 0;
    int positive_count = 0;
    int negative_count = 0;
    float systolic_pressure = 0.0;
    while (i<1000)
    {
        if (difference_values[i] > 0)
        {
            positive_count = 0;
            negative_count = 0;
            for (int j = i; j < i + 20; j++)
            {
                if (difference_values[j] > 0)
                {
                    positive_count++;
                }
                else
                {
                    negative_count++;
                }
            }
            //
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

int calculate_MAP(){
    int i = 0;
    float sum = 0.0;
    int MAP_index = 0;
    int index = 0;
    float max_ripple = 0.0;
    int j = 0;

    for(i=1; pressure_values[i] > 130; i++){
        index=i;
    }
    printf("MAP break index: %d, value: %0.2lf",i,pressure_values[i]);
    for(i=index; pressure_values[i] > 80; i++){
        sum = 0;
        if(difference_values[i] > 0){
           for(j=i+1; difference_values[j] >= 0;j++){
               sum = sum + difference_values[j];
           } 
           if(sum > max_ripple){
                max_ripple = sum;
                MAP_index = j;
           }
        }
     }
    return MAP_index;
}

float calculate_diastolic(float systolic, float MAP_value){
    return (MAP_value - (systolic - MAP_value));
}

int calculate_heartbeat(int MAP_index){
    int heartbeat_count = 0;
    for(int i = MAP_index - 37; i <= MAP_index + 37; i++){
        if(difference_values[i] > 0 && difference_values[i+1]<0){
            heartbeat_count++;
        }
    }
    return heartbeat_count * 4;
}

int main()
{
    int chk_flag = 0;
    int i = 0;
    // float curt_pressure = 0.0;
    //display_start_message();
    config_spi();
   // chk_flag = deflation_rate_check();

    float systolic = calculate_systolic();

    printf("Systolic: %0.2lf", systolic);
    int MAP_index = calculate_MAP();

    float MAP_value = pressure_values[MAP_index];

    printf("\nMAP: : %0.2lf", MAP_value);
    
    float diastolic = calculate_diastolic(systolic, MAP_value);

    printf("\nDiastolic: %0.2lf", diastolic);

    int heartbeat = calculate_heartbeat(MAP_index);

    printf("\nHeartbeat: %d", heartbeat);
    display_final_message(systolic, diastolic);
    printf("\n Done");
    printf("\n Done");
}