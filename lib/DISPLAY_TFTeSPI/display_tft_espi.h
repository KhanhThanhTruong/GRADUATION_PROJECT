#ifndef DISPLAY_TFTeSPI_H
#define DISPLAY_TFTeSPI_H
#include <TFT_eSPI.h>
#include <SD.h>

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
#define FIRST_LINE_Y 2
#define TEXT_SIZE 2
#define LINE_WIDTH 22
#define BUTTON_OFFSET_X 238
#define BUTTON_COMPONENT_WIDTH 25
#define BUTTON_COMPONENT_HEIGHT 17
#define ROTATION_TFT 1
#define TIME_TOUCH_DEBOUNCE 300
#define NUM_FUNCTION_ICON 4
#define FIRST_ICON_X 295
#define FIRST_ICON_Y 40
#define ICON_RADIUS 20
#define ICON_BORDER_COLOR TFT_WHITE
#define ICON_BACKGROUND_COLOR_DISABLE TFT_DARKGREY
#define ICON_BACKGROUND_COLOR_ENABLE TFT_BLUE
#define ICON_BORDER_WIDTH 2

const char title_display_tft[10][15] = {
    "Temp: ",
    "Hum: ",
    "Lat: ",
    "Long: ",
    "Time: ",
    "PM1.0: ",
    "PM2.5: ",
    "PM10: ",
    "CO: ",
    "Sound: "};
enum COMPONENT_DISPLAY
{
    TEMPERATURE = 0,
    HUMIDITY = 1,
    LATITUDE = 2,
    LONGITUDE = 3,
    TIME_GPS = 4,
    PM1_0 = 5,
    PM2_5 = 6,
    PM10 = 7,
    CACBON_MONOOXIT = 8,
    SOUND = 9,

};
enum FUNCTION_ICON
{
    WIFI = 0,
    LTE = 1,
    SMS = 2,
    CALL = 3
};
class display_tft_espi
{
    bool state_component[sizeof(title_display_tft) / sizeof(title_display_tft[0])];
    bool state_function_icon[NUM_FUNCTION_ICON];
    TFT_eSPI tft;

public:
    TFT_eSPI *get_tft();
    void init();
    void end();
    void begin();
    // Function draw background
    void background();
    // Funtion display CO concentration at fix position
    void cacborn_mono(float CO_ppm);
    // Function display PM1.0, PM2.5, PM10 concentration at fix position
    void PM(float PM1_0, float PM2_5, float PM10);
    // Function display temperature, humidity at fix position
    void SHT(float temperature, float humidity);
    // Function display sound at fix position
    void SOUND(float sound);
    // Function display Lattitude, Longitude, Time at fix position
    void GPS(float lat, float lng, const char *time);
    // Function delete last character
    void del(int num_char, uint16_t color);
    // Function get color based on threshold
    uint16_t color(float value, COMPONENT_DISPLAY component);
    // Function draw button
    void draw_button(size_t x, size_t y, size_t width, size_t height, uint16_t color);
    // Function process touch
    void process_touch();
    // Function drar wifi icon
    void draw_wifi_icon(uint16_t x, uint16_t y, uint16_t radius, bool isActive);
    // Function draw 4g icon
    void draw_4g_icon(uint16_t x, uint16_t y, uint16_t radius, bool isActive);
    // Function draw arc
    void drawArc(int x, int y, int radius1, int radius2, int start_angle, int end_angle, uint16_t color);
    // Function draw SMS icon
    void draw_sms_icon(int x, int y, int radius, bool isActive);
    // Function draw call icon
    void draw_call_icon(int x, int y, int radius, bool isActive);
    // Function draw battery icon
    void draw_battery_icon(int x, int y, int w, int h, float percent);
    // Function draw SIM signal level
    void draw_sim_signal_icon(int x, int y, float percent);
    // Function get state function icon
    bool get_state_function_icon(FUNCTION_ICON icon);
    // Function display notification
    void display_notification(String noti);
    // Function set state function icon
    void set_state_function_icon(FUNCTION_ICON icon, bool state);
};

void display_home_page();

void display_setting_page();

void display_typing_page(int num_title, String *title);

#endif