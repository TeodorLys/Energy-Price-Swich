#pragma once
#include <Arduino.h>
#include <fonts.h>
#include "element_base.h"
//SEE EDIT_ELEMENTS.H!!
class action_element : public element_base {
private:
    String name;
    sFONT *font;
    const TEXT_ALIGNMENT _name_align = TEXT_ALIGNMENT::LEFT;
    const TEXT_ALIGNMENT _state_align = TEXT_ALIGNMENT::RIGHT;
    int y = 0;
    bool selected = false;
    bool inverted;
    byte state = 0;  // For the changeability
    void (*action)();  // Function pointer

public:
// Polymorphic functions
    void set_font(sFONT *_f) { font = _f; }
    sFONT *get_font() { return font; }
    void set_selected(bool _s) { selected = _s; }  
    void set_inverted(bool _i) { inverted = _i; }  // White background and black text, or vice versa
    bool get_selected() const { return selected; }
    bool get_inverted() const { return inverted; }
    String get_name() const { return name; }
    void set_name(String _s) { name = _s; }
    TEXT_ALIGNMENT get_name_align() const { return _name_align; }
    ELEMENT_TYPE get_element_type() { return ELEMENT_TYPE::ACTION; }
    int get_y() const { return y; }
    void set_y(int _y) { y = _y; }
    action_element(String _name, sFONT *_font, bool _inverted) : name(_name), font(_font), inverted(_inverted) {}
    action_element() {}
    //When pressed the element we just increment. we CANT go up or down.
    // Returns a calculated X position by the 3 different predefined alignments 
    int get_x_from_alignment(TEXT_ALIGNMENT _ta, String _s)
    {
        switch (_ta)
        {
        case TEXT_ALIGNMENT::CENTER:
            return (200 - (_s.length() * Font24.Width))/2;
        case TEXT_ALIGNMENT::RIGHT:
            return (200 - (_s.length() * Font24.Width));
        case TEXT_ALIGNMENT::LEFT:
            return 0;
        default:
            return 0;
        }
    }
      
    //Register a function to call when pressing this element.
    void register_callback_function(void (*func)()){
        action = func;
    }

    // Call said function when pressing element
    void call_action(){
        action();
    }
};