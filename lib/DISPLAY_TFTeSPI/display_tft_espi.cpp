#include "display_tft_espi.h"

bool display_tft_espi::get_state_function_icon(FUNCTION_ICON icon)
{
    return state_function_icon[icon];
}
void display_tft_espi::background()
{

    for (int i = 0; i < sizeof(title_display_tft) / sizeof(title_display_tft[0]); i++)
    {
        tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
        tft.println(title_display_tft[i]);
        // tft.setCursor(FIRST_LINE_X+12, FIRST_LINE_Y + i * LINE_WIDTH + LINE_WIDTH);
        draw_button(FIRST_LINE_X + BUTTON_OFFSET_X, FIRST_LINE_Y + i * LINE_WIDTH, BUTTON_COMPONENT_WIDTH, BUTTON_COMPONENT_HEIGHT, TFT_GREEN);
        draw_wifi_icon(FIRST_ICON_X, FIRST_ICON_Y, ICON_RADIUS, state_function_icon[FUNCTION_ICON::WIFI]);
        draw_4g_icon(FIRST_ICON_X, FIRST_ICON_Y + 50, ICON_RADIUS, state_function_icon[FUNCTION_ICON::LTE]);
        draw_sms_icon(FIRST_ICON_X, FIRST_ICON_Y + 100, ICON_RADIUS, state_function_icon[FUNCTION_ICON::SMS]);
        draw_call_icon(FIRST_ICON_X, FIRST_ICON_Y + 150, ICON_RADIUS, state_function_icon[FUNCTION_ICON::CALL]);
    }
    // draw_battery_icon(275, 225, 20, 10, 0.5);
    // draw_sim_signal_icon(296, 225, 0.5);
}
void display_tft_espi::display_notification(String noti)
{
    tft.setTextColor(TFT_WHITE);
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + (sizeof(title_display_tft) / sizeof(title_display_tft[0])) * LINE_WIDTH);
    del(25, TFT_BLACK);
    tft.print(noti);
    tft.getWriteError();
}
void display_tft_espi::SHT(float temp, float humi)
{

    // Display Temperature
    int i = COMPONENT_DISPLAY::TEMPERATURE;
    // Set up color base on threshold
    tft.setTextColor(color(temp, COMPONENT_DISPLAY::TEMPERATURE));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title_display_tft[i]);
    del(9, TFT_BLACK);
    tft.printf("%4.2f oC", temp);

    // Display Humidity
    i = COMPONENT_DISPLAY::HUMIDITY;
    // Set up color base on threshold
    tft.setTextColor(color(humi, COMPONENT_DISPLAY::HUMIDITY));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title_display_tft[i]);
    del(8, TFT_BLACK);
    tft.printf("%4.2f ", humi);
    tft.print("%");
}

void display_tft_espi::process_touch()
{
    static uint32_t lastTouchTime = 0; // Biến lưu thời gian chạm cuối cùng
    uint16_t x = 0, y = 0;
    uint32_t currentTouchTime = millis(); // Lấy thời gian hiện tại

    if (tft.getTouch(&x, &y) && (currentTouchTime - lastTouchTime > TIME_TOUCH_DEBOUNCE)) // Kiểm tra chạm và thời gian debounce (200ms)
    {
        // Serial.printf("Touch detected at (%d, %d)\n", x, y);
        // Kiểm tra từng nút
        for (int i = 0; i < sizeof(title_display_tft) / sizeof(title_display_tft[0]); i++)
        {
            // uint16_t buttonWidth = tft.textWidth(title[i]) + BUTTON_PADDING * 2;
            uint16_t buttonX = FIRST_LINE_X + BUTTON_OFFSET_X; // Đặt nút bên phải của tiêu đề
            uint16_t buttonY = FIRST_LINE_Y + i * LINE_WIDTH;

            if (x > buttonX && x < (buttonX + BUTTON_COMPONENT_WIDTH) && y > buttonY && y < (buttonY + BUTTON_COMPONENT_HEIGHT))
            {
                state_component[i] = !state_component[i];
                lastTouchTime = currentTouchTime; // Cập nhật thời gian chạm cuối cùng
                // Chạm vào nút, thực hiện hành động tương ứng
                // Đổi màu nút để minh họa
                draw_button(buttonX, buttonY, BUTTON_COMPONENT_WIDTH, BUTTON_COMPONENT_HEIGHT, state_component[i] ? TFT_GREEN : TFT_RED);
            }
        }

        // Kiểm tra chạm vào biểu tượng WiFi
        // uint16_t wifiX =, FIRST_ICON_Y = 60, wifiRadius = 30;
        if (x > FIRST_ICON_X - ICON_RADIUS && x < FIRST_ICON_X + ICON_RADIUS && y > FIRST_ICON_Y - ICON_RADIUS && y < FIRST_ICON_Y + ICON_RADIUS)
        {
            lastTouchTime = currentTouchTime; // Cập nhật thời gian chạm cuối cùng

            state_function_icon[FUNCTION_ICON::WIFI] = !state_function_icon[FUNCTION_ICON::WIFI];              // Đổi trạng thái WiFi
            draw_wifi_icon(FIRST_ICON_X, FIRST_ICON_Y, ICON_RADIUS, state_function_icon[FUNCTION_ICON::WIFI]); // Vẽ lại biểu tượng WiFi
        }

        // Kiểm tra chạm vào biểu tượng 4G
        uint16_t g4X = FIRST_ICON_X, g4Y = FIRST_ICON_Y + 50, g4Radius = ICON_RADIUS;
        if (x > g4X - g4Radius && x < g4X + g4Radius && y > g4Y - g4Radius && y < g4Y + g4Radius)
        {
            lastTouchTime = currentTouchTime;                                                   // Cập nhật thời gian chạm cuối cùng
            state_function_icon[FUNCTION_ICON::LTE] = !state_function_icon[FUNCTION_ICON::LTE]; // Đổi trạng thái 4G
            draw_4g_icon(g4X, g4Y, g4Radius, state_function_icon[FUNCTION_ICON::LTE]);          // Vẽ lại biểu tượng 4G
        }

        // Check touch SMS icon
        uint16_t sms_x = FIRST_ICON_X, sms_y = FIRST_ICON_Y + 100, sms_radius = ICON_RADIUS;
        if (x > sms_x - sms_radius && x < sms_x + sms_radius && y > sms_y - sms_radius && y < sms_y + sms_radius)
        {
            // Serial.println("Touch SMS icon");
            lastTouchTime = currentTouchTime; // Cập nhật thời gian chạm cuối cùng

            // g4Active = !g4Active;
            state_function_icon[FUNCTION_ICON::SMS] = !state_function_icon[FUNCTION_ICON::SMS]; // Đổi trạng thái 4G
            draw_sms_icon(sms_x, sms_y, sms_radius, state_function_icon[FUNCTION_ICON::SMS]);   // Vẽ lại biểu tượng 4G
        }

        // Check touch call icon
        uint16_t call_x = FIRST_ICON_X, call_y = FIRST_ICON_Y + 150, call_radius = ICON_RADIUS;
        if (x > call_x - call_radius && x < call_x + call_radius && y > call_y - call_radius && y < call_y + call_radius)
        {
            lastTouchTime = currentTouchTime; // Cập nhật thời gian chạm cuối cấp
            state_function_icon[FUNCTION_ICON::CALL] = !state_function_icon[FUNCTION_ICON::CALL];
            draw_call_icon(call_x, call_y, call_radius, state_function_icon[FUNCTION_ICON::CALL]);
        }
    }
}

void display_tft_espi::PM(float PM1_0, float PM2_5, float PM10)
{
    // Display PM1.0
    int i = COMPONENT_DISPLAY::PM1_0;
    // Set up color base on threshold
    tft.setTextColor(color(PM1_0, COMPONENT_DISPLAY::PM1_0));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title_display_tft[i]);
    del(12, TFT_BLACK);
    tft.printf("%4.2f ug/m3", PM1_0);
    // Display PM2.5
    i = COMPONENT_DISPLAY::PM2_5;
    // Set up color base on threshold
    tft.setTextColor(color(PM2_5, COMPONENT_DISPLAY::PM2_5));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title_display_tft[i]);
    del(12, TFT_BLACK);
    tft.printf("%4.2f ug/m3", PM2_5);

    // Display PM10
    i = COMPONENT_DISPLAY::PM10;
    // Set up color base on threshold
    tft.setTextColor(color(PM10, COMPONENT_DISPLAY::PM10));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title_display_tft[i]);
    del(12, TFT_BLACK);
    tft.printf("%4.2f ug/m3", PM10);
}

void display_tft_espi::cacborn_mono(float CO)
{
    // Display CO
    int i = COMPONENT_DISPLAY::CACBON_MONOOXIT;
    // Set up color base on threshold
    tft.setTextColor(color(CO, COMPONENT_DISPLAY::CACBON_MONOOXIT));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title_display_tft[i]);
    del(13, TFT_BLACK);
    tft.printf("%4.2f ppm   ", CO);
}

uint16_t display_tft_espi::color(float value, COMPONENT_DISPLAY component)
{
    switch (component)
    {
    case TEMPERATURE:
        if (value > 25)
        {
            return TFT_YELLOW;
        }

        if (value > 30)
        {
            return TFT_RED;
        }

        return TFT_GREEN;
        break;
    case HUMIDITY:
        if (value < 20)
        {
            return TFT_RED;
        }

        if (value < 80)
        {
            return TFT_YELLOW;
        }

        return TFT_GREEN;
        break;
    case PM2_5:
    case PM10:
    case PM1_0:
    case CACBON_MONOOXIT:
        if (value < 50)
        {
            return TFT_GREEN;
        }
        if (value < 100)
        {
            return TFT_YELLOW;
        }
        return TFT_RED;
        break;
    case COMPONENT_DISPLAY::SOUND:
        if (value > 55)
        {
            return TFT_YELLOW;
        }
        return TFT_GREEN;
    default:
        return TFT_WHITE;
        break;
    }
}
void display_tft_espi::GPS(float latitude, float longitude, const char *time)
{
    // Display Latitude
    int i = COMPONENT_DISPLAY::LATITUDE;
    tft.setTextColor(color(latitude, COMPONENT_DISPLAY::LATITUDE));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title_display_tft[i]);
    del(7, TFT_BLACK);
    tft.printf("%10.6f", latitude);
    // Display Longitude
    i = COMPONENT_DISPLAY::LONGITUDE;
    tft.setTextColor(color(longitude, COMPONENT_DISPLAY::LONGITUDE));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title_display_tft[i]);
    del(6, TFT_BLACK);
    tft.printf("%4.6f", longitude);
    // Display Time
    i = COMPONENT_DISPLAY::TIME_GPS;
    tft.setTextColor(color(0, COMPONENT_DISPLAY::TIME_GPS));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title_display_tft[i]);
    del(12, TFT_BLACK);
    tft.print(time);
}

void display_tft_espi::SOUND(float sound)
{
    // Display Sound
    int i = COMPONENT_DISPLAY::SOUND;
    tft.setTextColor(color(sound, COMPONENT_DISPLAY::SOUND));
    tft.setCursor(FIRST_LINE_X, FIRST_LINE_Y + i * LINE_WIDTH);
    tft.print(title_display_tft[i]);
    del(10, TFT_BLACK);
    tft.printf("%4.2f dBA", sound);
}

void display_tft_espi::del(int num_char, uint16_t color)
{
    tft.fillRect(tft.getCursorX(), tft.getCursorY(), 12 * num_char, 16, color);
}

void display_tft_espi::set_state_function_icon(FUNCTION_ICON icon, bool state)
{
    state_function_icon[icon] = state;
}

void display_tft_espi::end()
{
    // tft.end_nin_write();
    tft.endWrite();
}





void display_tft_espi::init()
{

    tft.startWrite();
}

TFT_eSPI *display_tft_espi::get_tft()
{
    return &(tft);
}

void display_tft_espi::begin()
{
    for (int i = 0; i < sizeof(title_display_tft) / sizeof(title_display_tft[0]); i++)
    {
        state_component[i] = true;
    }

    for (int i = 0; i < NUM_FUNCTION_ICON; i++)
    {
        state_function_icon[i] = false;
    }

    state_function_icon[FUNCTION_ICON::WIFI] = true;
    this->tft.begin();

    

    this->tft.fillScreen(TFT_BLACK);
    this->tft.setRotation(ROTATION_TFT);
    this->tft.setTextSize(TEXT_SIZE);
}

void display_tft_espi::draw_button(size_t BUTTON_X, size_t BUTTON_Y, size_t BUTTON_WIDTH, size_t BUTTON_HEIGHT, uint16_t color)
{
    // tft.setRotation(1);
    // Chọn màu nút
    uint16_t buttonColor = color;

    // Vẽ bóng đổ cho nút
    // tft.fillRoundRect(BUTTON_X + 4, BUTTON_Y + 4, BUTTON_WIDTH, BUTTON_HEIGHT, 3, TFT_DARKGREY);

    // Vẽ nút nhấn với bo góc
    tft.fillRoundRect(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 3, buttonColor);

    // Vẽ viền nút nhấn
    tft.drawRoundRect(BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT, 3, TFT_WHITE);
}

void display_tft_espi::draw_wifi_icon(uint16_t x, uint16_t y, uint16_t radius, bool isActive)
{
    uint16_t color = isActive ? ICON_BACKGROUND_COLOR_ENABLE : ICON_BACKGROUND_COLOR_DISABLE;

    // Vẽ đường viền
    tft.drawSmoothCircle(x, y, radius, ICON_BORDER_COLOR, color);

    // Vẽ nền
    tft.fillCircle(x, y, radius - ICON_BORDER_WIDTH, color);
    // Vẽ biểu tượng WiFi

    int arcWidth = 3;
    tft.drawArc(x, y + radius / 3 + 3, radius - 3, radius - 6, 135, 225, ICON_BORDER_COLOR, color);   // Vòng cung ngoài cùng
    tft.drawArc(x, y + radius / 3 + 3, radius - 9, radius - 12, 135, 225, ICON_BORDER_COLOR, color);  // Vòng cung giữa
    tft.drawArc(x, y + radius / 3 + 3, radius - 15, radius - 18, 135, 225, ICON_BORDER_COLOR, color); // Vòng cung trong cùng
    // tft.fillCircle(x, y + radius/3+3, radius / 8, TFT_WHITE); // Điểm tròn ở trung tâm
}

void display_tft_espi::draw_4g_icon(uint16_t x, uint16_t y, uint16_t radius, bool isActive)
{
    uint16_t color = isActive ? ICON_BACKGROUND_COLOR_ENABLE : ICON_BACKGROUND_COLOR_DISABLE;

    // Vẽ đường viền
    tft.drawSmoothCircle(x, y, radius, ICON_BORDER_COLOR, color);

    // Vẽ nền
    tft.fillCircle(x, y, radius - ICON_BORDER_WIDTH, color);

    // Vẽ ký tự 4G
    tft.setTextDatum(MC_DATUM);
    tft.setTextColor(ICON_BORDER_COLOR);
    tft.drawString("4G", x, y);
}

void display_tft_espi::draw_sms_icon(int x, int y, int radius, bool isActive)
{
    uint16_t color = isActive ? ICON_BACKGROUND_COLOR_ENABLE : ICON_BACKGROUND_COLOR_DISABLE;

    // Vẽ đường viền
    tft.drawSmoothCircle(x, y, radius, ICON_BORDER_COLOR, color);

    // Vẽ nền
    tft.fillCircle(x, y, radius - ICON_BORDER_WIDTH, color);

    // Draw SMS icon
    // Draw botom line
    tft.drawLine(x - radius / 2, y - 2 + radius / 2, x + radius / 2, y - 2 + radius / 2, ICON_BORDER_COLOR);
    // Draw top line
    tft.drawLine(x - radius / 2, y - 2 - radius / 3, x + radius / 2, y - 2 - radius / 3, ICON_BORDER_COLOR);
    // Draw left line
    tft.drawLine(x - radius / 2, y - 2 - radius / 3, x - radius / 2, y - 2 + radius / 2, ICON_BORDER_COLOR);
    // Draw right line
    tft.drawLine(x + radius / 2, y - 2 - radius / 3, x + radius / 2, y - 2 + radius / 2, ICON_BORDER_COLOR);
    // Draw left center line
    tft.drawLine(x - radius / 2, y - 2 - radius / 3, x, y - 2, ICON_BORDER_COLOR);
    // Draw right center line
    tft.drawLine(x + radius / 2, y - 2 - radius / 3, x, y - 2, ICON_BORDER_COLOR);
}

void display_tft_espi::draw_battery_icon(int x, int y, int w, int h, float percent)
{
    tft.setTextSize(1);
    tft.drawString("52%", x - 15, y + 5);
    tft.setTextSize(TEXT_SIZE);
    // uint16_t color = isActive ? ICON_BACKGROUND_COLOR_ENABLE : ICON_BACKGROUND_COLOR_DISABLE;
    if (percent >= 1)
    {
        tft.fillRect(x - 4, y + h / 4, 4, h / 2, TFT_GREEN);
    }
    tft.drawRect(x - 4, y + h / 4, 4, h / 2, ICON_BORDER_COLOR);
    tft.fillRect(x + w * percent, y, w * percent, h, TFT_GREEN);
    tft.drawRect(x, y, w, h, ICON_BORDER_COLOR);
    // draw_button(x,y,w,h,TFT_GREEN);
}

void display_tft_espi::draw_call_icon(int x, int y, int radius, bool isActive)
{
    uint16_t color = isActive ? ICON_BACKGROUND_COLOR_ENABLE : ICON_BACKGROUND_COLOR_DISABLE;

    // Vẽ đường viền
    tft.drawSmoothCircle(x, y, radius, ICON_BORDER_COLOR, color);

    // Vẽ nền
    tft.fillCircle(x, y, radius - ICON_BORDER_WIDTH, color);

    // Draw call icon
    // Draw top line
    tft.drawArc(x, y + radius / 3 - 2, radius / 2, radius / 2 - 4, 120, 120 + 120, ICON_BORDER_COLOR, color);
    // Draw headphone

    tft.fillRect(x - radius / 2 * cos(30) - 8, y - (radius / 2 * sin(30) - 4 * sin(30)) - 5, 7, 6, ICON_BORDER_COLOR);
    tft.fillRect(x + radius / 2 * cos(30) + 3, y - (radius / 2 * sin(30) - 4 * sin(30)) - 5, 7, 6, ICON_BORDER_COLOR);
}

void display_tft_espi::draw_sim_signal_icon(int x, int y, float percent)
{
    // Draw cot dien
    //  y=y-16/2+1;
    tft.fillRect(x, y + 10 - 2, 4, 2, ICON_BORDER_COLOR);
    tft.fillRect(x + 5, y + 10 - 4, 4, 4, ICON_BORDER_COLOR);
    tft.fillRect(x + 10, y + 10 - 6, 4, 6, ICON_BORDER_COLOR);
    tft.fillRect(x + 15, y + 10 - 8, 4, 8, ICON_BORDER_COLOR);
    tft.fillRect(x + 20, y, 4, 10, ICON_BORDER_COLOR);

    // tft.fillRect(x-4,y+4,9,2,ICON_BORDER_COLOR);
}

// // Callback that is triggered when btn_component is clicked
// static void button_event_cb(lv_event_t *e)
// {
// }

// void display_home_page()
// {
//     int TITTLE_POSITION_X = 10;
//     int TITTLE_POSITION_Y_FIRST = 20;
//     int TITLE_WIDTH = 165;
//     int TITLE_HEIGHT = 18;
//     int BUTTON_WIDTH = 30;
//     int BUTTON_HEIGHT = 18;
//     int BUTTON_POSITION_X = -110;
//     int BUTTON_POSITION_Y_FIRST = 20;
//     // Create title display
//     lv_obj_t *tit[sizeof(title_display_tft) / sizeof(title_display_tft[0])];
//     for (int i = 0; i < sizeof(title_display_tft) / sizeof(title_display_tft[0]); i++)
//     {
//         lv_obj_t *tit_buf = lv_textarea_create(lv_scr_act());
//         tit[i] = tit_buf;
//         lv_obj_t *tit_label = lv_label_create(tit[i]);
//         lv_label_set_text(tit_label, title_display_tft[i]);
//         lv_obj_align(tit_label, LV_ALIGN_LEFT_MID, 0, 0);

//         lv_obj_set_size(tit[i], TITLE_WIDTH, TITLE_HEIGHT);
//         lv_obj_align(tit[i], LV_ALIGN_TOP_LEFT, TITTLE_POSITION_X, TITTLE_POSITION_Y_FIRST * (i + 1));
//     }

//     // Create button
//     lv_obj_t *button_compo[sizeof(title_display_tft) / sizeof(title_display_tft[0])];
//     for (int i = 0; i < sizeof(title_display_tft) / sizeof(title_display_tft[0]); i++)
//     {
//         lv_obj_t *button_buf = lv_btn_create(lv_scr_act());
//         button_compo[i] = button_buf;
//         lv_obj_set_size(button_compo[i], 30, 18);
//         lv_obj_align(button_compo[i], LV_ALIGN_TOP_RIGHT, -110, 20 * (i + 1));
//         lv_obj_add_event_cb(button_compo[i], button_event_cb, LV_EVENT_CLICKED, NULL);
//     }
// }