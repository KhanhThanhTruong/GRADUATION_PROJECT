#include "display.h"

void display::background()
{
    // tft.drawLine(N1_x0,N1_y0,N1_x0+N1_LENGTH,N1_y0+N1_WIDTH,N1_COLOR);
    // for (int i = 0; i < 5; i++)
    // {
    //     tft.drawRect(LEFT_BEGIN_X, LEFT_BEGIN_Y + i * LEFT_HEIGHT, LEFT_WIDTH, LEFT_HEIGHT, ILI9341_WHITE);
    // }

    // for (int i = 0; i < 5; i++)
    // {
    //     tft.drawRect(RIGHT_BEGIN_X, RIGHT_BEGIN_Y + i * RIGHT_HEIGHT, RIGHT_WIDTH, RIGHT_HEIGHT, ILI9341_WHITE);
    // }

    // tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y);
    // tft.println("Temperature : ");
    // tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y+LINE_WIDTH);
    // tft.println("Humidity : ");
    // tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y+LINE_WIDTH*2);
    // tft.println("Latitude : ");
    // tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y+LINE_WIDTH*3);
    // tft.println("Longitude : ");
    // tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y+LINE_WIDTH*4);
    // tft.println("Time : ");
    for (int i = 0; i < 9; i++)
    {
        tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
        tft.println(title[i]);
    }
}

void display::SHT(float temp, float humi)
{
    
    // Display Temperature
    int i = COMPONENT_DISPLAY::TEMPERATURE;
    // Set up color base on threshold
    tft.setTextColor(color(temp, COMPONENT_DISPLAY::TEMPERATURE));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title[i]);
    del(12, ILI9341_BLACK);
    tft.printf("%4.2f oC   ", temp);

    // Display Humidity
    i = COMPONENT_DISPLAY::HUMIDITY;
    // Set up color base on threshold
    tft.setTextColor(color(humi, COMPONENT_DISPLAY::HUMIDITY));
    tft.print(title[i]);
    del(7, ILI9341_BLACK);
    tft.printf("%4.2f ", humi);
    tft.print("%");
}

void display::PM(float PM2_5, float PM10)
{
    // Display PM2.5
    int i = COMPONENT_DISPLAY::PM2_5;
    // Set up color base on threshold
    tft.setTextColor(color(PM2_5, COMPONENT_DISPLAY::PM2_5));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title[i]);
    del(14, ILI9341_BLACK);
    tft.printf("%4.2f ug/m3", PM2_5);

    // Display PM10
    i = COMPONENT_DISPLAY::PM10;
    // Set up color base on threshold
    tft.setTextColor(color(PM10, COMPONENT_DISPLAY::PM10));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title[i]);
    del(14, ILI9341_BLACK);
    tft.printf("%4.2f ug/m3  ", PM10);
}

void display::cacborn_mono(float CO)
{
    // Display CO
    int i = COMPONENT_DISPLAY::CACBON_MONOOXIT;
    // Set up color base on threshold
    tft.setTextColor(color(CO, COMPONENT_DISPLAY::CACBON_MONOOXIT));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title[i]);
    del(13, ILI9341_BLACK);
    tft.printf("%4.2f ppm   ", CO);
}

uint16_t display::color(float value, COMPONENT_DISPLAY component)
{
    switch (component)
    {
    case TEMPERATURE:
        if (value > 25)
        {
            return ILI9341_YELLOW;
        }

        if (value > 30)
        {
            return ILI9341_RED;
        }

        return ILI9341_GREEN;
        break;
    case HUMIDITY:
        if (value < 20)
        {
            return ILI9341_RED;
        }

        if (value < 80)
        {
            return ILI9341_YELLOW;
        }

        return ILI9341_GREEN;
        break;
    case PM2_5:
    case PM10:
    case CACBON_MONOOXIT:
        if (value < 50)
        {
            return ILI9341_GREEN;
        }
        if (value < 100)
        {
            return ILI9341_YELLOW;
        }
        return ILI9341_RED;
        break;
    case COMPONENT_DISPLAY::SOUND:
        if (value > 55)
        {
            return ILI9341_YELLOW;
        }
        return ILI9341_GREEN;
    default:
        return ILI9341_WHITE;
        break;
    }
}
void display::GPS(float latitude, float longitude, char *time)
{
    // Display Latitude
    int i = COMPONENT_DISPLAY::LATITUDE;
    tft.setTextColor(color(latitude, COMPONENT_DISPLAY::LATITUDE));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title[i]);
    del(12, ILI9341_BLACK);
    tft.printf("%4.2f    ", latitude);
    // Display Longitude
    i = COMPONENT_DISPLAY::LONGITUDE;
    tft.setTextColor(color(longitude, COMPONENT_DISPLAY::LONGITUDE));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title[i]);
    del(12, ILI9341_BLACK);
    tft.printf("%4.2f   ", longitude);
    // Display Time
    i = COMPONENT_DISPLAY::TIME;
    tft.setTextColor(color(0, COMPONENT_DISPLAY::TIME));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title[i]);
    del(12, ILI9341_BLACK);
    tft.print(time);
}

void display::SOUND(float sound)
{
    // Display Sound
    int i = COMPONENT_DISPLAY::SOUND;
    tft.setTextColor(color(sound, COMPONENT_DISPLAY::SOUND));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title[i]);
    del(12, ILI9341_BLACK);
    tft.printf("%4.2f dBA  ", sound);
}

void display::del(int num_char, uint16_t color)
{
    tft.fillRect(tft.getCursorX(), tft.getCursorY(), 12*num_char, 16, color);
}