#include "Menu.h"

Menu::Menu(Display* display, std::string title, byte left_icon, byte right_icon, void (*left_funct)(), void (*right_funct)(), Menu* upper_menu)
: _upper_menu(upper_menu)
, _display(display)
, _title(title)
, _left_icon(left_icon)
, _right_icon(right_icon)
, _left_opt(left_funct)
, _right_opt(right_funct)
, _left_nav(NULL)
, _right_nav(NULL)
{}

Menu::Menu(Display* display, std::string title, byte left_icon, byte right_icon, void (*left_funct)(), void (*right_funct)(), void (*left_nav)(), void (*right_nav)(), Menu* upper_menu)
: _upper_menu(upper_menu)
, _display(display)
, _title(title)
, _left_icon(left_icon)
, _right_icon(right_icon)
, _left_opt(left_funct)
, _right_opt(right_funct)
, _left_nav(left_nav)
, _right_nav(right_nav)
{}

bool Menu::create_submenu(std::string title)
{
  return create_submenu(title, _left_icon, _right_icon, _left_opt, _right_opt);
}

bool Menu::create_submenu(std::string title, byte left_icon, byte right_icon, void (*left_funct)(), void (*right_funct)())
{
  try{
    _submenus.push_back(Menu(_display, title, left_icon, right_icon, left_funct, right_funct, this));
    return true;
  }catch(...){
    return false;
  };
}

void Menu::show()
{
  _display->draw_header(_left_icon, _right_icon, _title);
  _display->draw_body("----------------");
}

void Menu::show_submenu(int index)
{
  if(_submenus.size() > 0 && index >= 0 && index < _submenus.size()){
    byte l_icon = index>0 ? 0x3C : 0x2A;
    byte r_icon = index==_submenus.size()-1 ? 0x2A : 0x3E;
    _display->draw_body(l_icon, r_icon, _submenus[index]._title);
  }
  if(index == -1) show();
}

Menu* Menu::opt_left(){
  if(_left_opt != NULL){
    _left_opt();
    return this;
  }

  if(_current_submenu == -1){
    if(_upper_menu == NULL){
      return this;
    }else{
      _upper_menu->show();
      return _upper_menu;
    }
  }else{
    Menu* submenu = &_submenus[_current_submenu];
    if(submenu->_left_opt != NULL) submenu->_left_opt();
    close_submenus();
    return this;
  }
  return this;
}

Menu* Menu::opt_right(){
  if(_right_opt != NULL){
    _right_opt();
    return this;
  }

  if(_current_submenu == -1){
    open_submenus();
    return this;
  }else{
    Menu* submenu = &_submenus[_current_submenu];
    if(submenu->_right_opt == NULL){
      submenu->show();
    }else{
      submenu->_right_opt();
    }
    return submenu;
  }
}

Menu* Menu::nav_left(){
  if(_current_submenu >= 0){
    if(_current_submenu > 0){
      _current_submenu--;
      show_submenu(_current_submenu);
    }
  }
  return this;
}

Menu* Menu::nav_right(){
  if(_current_submenu >= 0){
    if(_submenus.size() - 1 > _current_submenu){
      _current_submenu++;
      show_submenu(_current_submenu);
    }
  }
  return this;
}

void Menu::print()
{
  Serial.println((int)this);
}

void Menu::open_submenus(){
  _current_submenu = 0;
  show_submenu(_current_submenu);
}

void Menu::close_submenus(){
  _current_submenu = -1;
  show_submenu(_current_submenu);
}
