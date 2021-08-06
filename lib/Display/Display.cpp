#include "Display.h"

// --- Default characters ---
byte _blank = byte(0xFE);

// --- Special characters index ---
byte dp_menu   =  byte(0);
byte dp_close  =  byte(1);
byte dp_x_line =  byte(2);
byte dp_y_line =  byte(3);
byte dp_wifi   =  byte(4);
byte dp_share  =  byte(5);
byte dp_heart  =  byte(6);

// --- Special characters ---
static byte _menu[8]    = {0x00,0x1F,0x00,0x1F,0x00,0x1F,0x00,0x00};
static byte _close[8]   = {0x00,0x11,0x0A,0x04,0x0A,0x11,0x00,0x00};
static byte _x_line[8]  = {0x00,0x00,0x00,0x1F,0x00,0x00,0x00,0x00};
static byte _y_line[8]  = {0x04,0x04,0x04,0x04,0x04,0x04,0x04,0x04};
static byte _wifi[8]    = {0x00,0x0E,0x11,0x04,0x0A,0x00,0x04,0x00};
static byte _share[8]   = {0x03,0x03,0x04,0x18,0x18,0x04,0x03,0x03};
static byte _heart[8]   = {0x00,0x0A,0x1F,0x1F,0x0E,0x04,0x00,0x00};

// --- Display functions ---
void startup(LiquidCrystal &lcd)
{
  lcd.setCursor(0,0);
  lcd.write("    Lab IoT    ");
  lcd.setCursor(0,1);
  lcd.write("      IME      ");
}

void clear_interval(LiquidCrystal &lcd, int display_size[2], std::array<int,2> interval)
{
  int x = interval[0]%display_size[0];
  int y = interval[0]/display_size[0];

  lcd.setCursor(x, y);

  for(int i = interval[0]; i <= interval[1]; i++)
  {
    if(i/display_size[0] > y){
      y += 1;
      lcd.setCursor(0, y);
    };

    //Empty character
    lcd.write(_blank);
  }
}

void header_structure(LiquidCrystal &lcd, int display_size[2], bool &header_status)
{
  if(!header_status){
    int x_limit = display_size[0]-1;

    lcd.setCursor(1,0);
    lcd.write(dp_y_line);

    lcd.setCursor(x_limit-4,0);
    lcd.write(dp_y_line);

    lcd.setCursor(x_limit-1,0);
    lcd.write(dp_y_line);

    header_status = true;
  }
}

void text_flow(LiquidCrystal &lcd, std::string text, int line, std::array<int,2> interval, bool &stop_flag)
{
  int base = 0;
  int delta = interval[1] - interval[0] + 1;
  int shift = 3;

  int counter = 0;
  int multiplier = 10;

  for(int i = 0; i < shift; i++){
    text.append(" ");
  }

  while(!stop_flag){
    if(counter == 0){
      lcd.setCursor(interval[0], line);
      for(int idx = 0; idx < delta; idx++){
        lcd.write(text[(text.length() + base + idx) % text.length()]);
      }
      base = (base + 1) % text.length();
    }
    counter = (counter + 1) % multiplier;
    delay(1000/multiplier);
  }
}

void text_center(LiquidCrystal &lcd, std::string text, int line, std::array<int,2> interval)
{
  int delta = (interval[1] - interval[0] + 1) - text.length();
  lcd.setCursor(interval[0] + delta/2, line);
  lcd.print(text.c_str());
}

void text_print(LiquidCrystal &lcd, int display_size[2], std::string text, int line, std::array<int,2> interval, std::thread &dp_thread, bool &thread_flag)
{
  //Clear text interval
  std::array<int,2> real_interval = {interval[0] + line*display_size[0],interval[1] + line*display_size[0]};
  clear_interval(lcd, display_size, real_interval);

  //Stop current thread
  thread_flag = true;
  if(dp_thread.joinable()){
    dp_thread.join();
  }

  if(text.length() <= (interval[1] - interval[0] + 1)){
    text_center(lcd, text, line, interval);
  }else{
    thread_flag = false;
    dp_thread = std::thread(text_flow, std::ref(lcd), text, line, interval, std::ref(thread_flag));
  }
}

// --- Class implementation ---
Display::Display(int RS, int E, int D4, int D5, int D6, int D7)
:_lcd(RS, E, D4, D5, D6, D7){}

void Display::begin()
{
  //Lcd initialize
  _lcd.begin(_display_size[0],_display_size[1]);

  //Create special characters
  _lcd.createChar(dp_menu, _menu);
  _lcd.createChar(dp_close, _close);
  _lcd.createChar(dp_x_line, _x_line);
  _lcd.createChar(dp_y_line, _y_line);
  _lcd.createChar(dp_wifi, _wifi);
  _lcd.createChar(dp_share, _share);
  _lcd.createChar(dp_heart, _heart);

  //Initial setup
  _lcd.clear();
  startup(_lcd);
}

void Display::draw_header(byte left_icon, byte right_icon, std::string title)
{
  //Set header structure
  header_structure(_lcd, _display_size, _head_struct_status);

  //Set header button icons
  _lcd.setCursor(0,0);
  _lcd.write(left_icon);
  _lcd.setCursor(_display_size[0]-1,0);
  _lcd.write(right_icon);

  //Set header title
  std::array<int,2> interval = {2, _display_size[0]-6};
  text_print(_lcd, _display_size, title, 0, interval, _dph_thread, _h_thread_stop);
}

void Display::draw_status(byte left_icon, byte right_icon)
{
  if(left_icon == NULL) left_icon = _blank;
  if(right_icon == NULL) right_icon = _blank;
  _lcd.setCursor(_display_size[0]-4,0);
  _lcd.write(left_icon);
  _lcd.write(right_icon);
}

void Display::draw_body_fast(std::string text)
{
  _lcd.setCursor(0,1);
  _lcd.write(text.c_str());
}

void Display::draw_body(std::string text)
{
  _lcd.setCursor(0,1);
  std::array<int,2> interval = {0, _display_size[0]-1};
  text_print(_lcd, _display_size, text, _display_size[1], interval, _dpb_thread, _b_thread_stop);
}

void Display::draw_body(byte left_icon, byte right_icon, std::string text)
{
  _lcd.setCursor(0,1);
  _lcd.write(left_icon);
  _lcd.setCursor(_display_size[0]-1,1);
  _lcd.write(right_icon);

  std::array<int,2> interval = {1, _display_size[0]-2};
  text_print(_lcd, _display_size, text, _display_size[1], interval, _dpb_thread, _b_thread_stop);
}