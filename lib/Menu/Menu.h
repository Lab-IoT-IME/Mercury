#include <Arduino.h>
#include <vector>
#include "Display.h"

class Menu
{
  public:
    Menu(Display* display, std::string title, byte left_icon, byte right_icon, void (*opt_left)(), void (*opt_right)(), Menu* upper_menu = NULL);
    Menu(Display* display, std::string title, byte left_icon, byte right_icon, void (*opt_left)(), void (*opt_right)(), void (*nav_left)(), void (*nav_right)(), Menu* upper_menu = NULL);

    bool create_submenu(std::string title);
    bool create_submenu(std::string title,byte left_icon, byte right_icon, void(*left_funct)(), void(*right_funct)());

    void show();
    void show_submenu(int index = 0);

    Menu* nav_left();
    Menu* nav_right();
    Menu* opt_left();
    Menu* opt_right();

    void open_submenus();
    void close_submenus();

    //Debugging Methods
    void print();

  private:
    Menu* _upper_menu;
    Display* _display;
    std::vector<Menu> _submenus;
    int _current_submenu = -1;

    std::string _title;

    byte _left_icon;
    byte _right_icon;
    void (*_left_opt)();  //Option left  
    void (*_right_opt)(); //Option right
    void (*_left_nav)();  //Navigation left
    void (*_right_nav)(); //Navigation right
};