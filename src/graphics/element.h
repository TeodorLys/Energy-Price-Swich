#pragma once
#include <Arduino.h>
#include <fonts.h>  
#include "element_base.h"
//SEE EDIT_ELEMENTS.H!!
class element : public element_base {
private:
    String name;
    sFONT *font;
    bool inverted;
    TEXT_ALIGNMENT _align;
    int y = 0;
    bool selected = false;
public:
    void set_font(sFONT *_f) { font = _f; }
    sFONT *get_font() { return font; }
    void set_selected(bool _s) { selected = _s; }
    void set_inverted(bool _i) { inverted = _i; }
    bool get_selected() const { return selected; }
    bool get_inverted() const { return inverted; }
    String get_name() const { return name; }
    void set_name(String _s) { name = _s; }
    void set_y(int _y) { y = _y; }
    int get_y() const { return y; }
    void set_alignment(TEXT_ALIGNMENT _at) { _align = _at; }
    element(String _name, sFONT *_font, bool _inverted, TEXT_ALIGNMENT _at = TEXT_ALIGNMENT::CENTER) : name(_name), font(_font), inverted(_inverted), _align(_at) {}
    element() {}
    int get_x_from_alignment(){
        switch (_align)
        {
        case TEXT_ALIGNMENT::CENTER:
            return (200 - (name.length() * Font24.Width))/2;
        case TEXT_ALIGNMENT::RIGHT:
            return (200 - (name.length() * Font24.Width));
        case TEXT_ALIGNMENT::LEFT:
            return 0;
        default:
            return 0;
        }
    }
};