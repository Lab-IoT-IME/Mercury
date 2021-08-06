#include <Arduino.h>
#include <LiquidCrystal.h>
#include <tuple>
#include <thread>

extern byte dp_menu;
extern byte dp_close;
extern byte dp_x_line;
extern byte dp_y_line;
extern byte dp_wifi;
extern byte dp_share;
extern byte dp_heart;

class Display
{
  public:
    Display(int RS, int E, int D4, int D5, int D6, int D7);

    void begin();
    void draw_header(byte left_icon, byte right_icon, std::string title);
    void draw_status(byte left_icon, byte right_icon);

    void draw_body(std::string text);
    void draw_body_fast(std::string text);
    void draw_body(byte left_icon, byte right_icon, std::string text);

    void nth_blink();

  private:
    LiquidCrystal _lcd;
    int _display_size[2] = {16, 2};
    bool _head_struct_status = false;

    std::thread _dph_thread;
    bool _h_thread_stop = true;
    std::thread _dpb_thread;
    bool _b_thread_stop = true;


};