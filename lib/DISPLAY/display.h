#include <Adafruit_ILI9341.h>

// #define TFT_DC 2
// #define TFT_CS 15
// #define TFT_MOSI 23
// #define TFT_CLK 18
// #define TFT_RST 5
// #define TFT_MISO 19
// #define UART_PMS7003_RX 12
// #define UART_PMS7003_TX 13

#define CO_x 20
#define CO_y 0
#define PM_x 20
#define PM_y 20
#define LEFT_BEGIN_X 0
#define LEFT_BEGIN_Y 20
#define LEFT_WIDTH 120
#define LEFT_HEIGHT 60
#define RIGHT_BEGIN_X 120
#define RIGHT_BEGIN_Y 20
#define RIGHT_WIDTH 120
#define RIGHT_HEIGHT 75
#define FIRST_LINE_X 0
#define FIRST_LINE_Y 60
#define TEXT_SIZE 2
#define LINE_WIDTH 22
const char title[9][15] = {
    "Temp: ",
    "Hum: ",
    "Lat: ",
    "Long: ",
    "Time: ",
    "PM 2.5: ",
    "PM 10: ",
    "CO: ",
    "Sound: "};
enum COMPONENT_DISPLAY
{
    TEMPERATURE = 0,
    HUMIDITY = 1,
    LATITUDE = 2,
    LONGITUDE = 3,
    TIME = 4,
    PM2_5 = 5,
    PM10 = 6,
    CACBON_MONOOXIT = 7,
    SOUND = 8
};

class display
{
    Adafruit_ILI9341 tft;

public:
    display(Adafruit_ILI9341 tft) : tft(tft)
    {
        this->tft.begin();
        this->tft.fillScreen(ILI9341_BLACK);

        // tft.setTextColor(ILI9341_GREEN);
        this->tft.setRotation(0);
        this->tft.setTextSize(TEXT_SIZE);
    };
    //Function draw background
    void background();
    //Funtion display CO concentration at fix position
    void cacborn_mono(float CO_ppm);
    //Function display PM1.0, PM2.5, PM10 concentration at fix position
    void PM(float PM2_5, float PM10);
    //Function display temperature, humidity at fix position
    void SHT(float temperature, float humidity);
    //Function display sound at fix position
    void SOUND(float sound);
    //Function display Lattitude, Longitude, Time at fix position
    void GPS(float lat, float lng, char *time);
    //Function delete last character
    void del(int num_char, uint16_t color);
    //Function get color based on threshold
    uint16_t color(float value, COMPONENT_DISPLAY component);
};