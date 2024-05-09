#pragma once
#include <Arduino.h>
#include <fonts.h>

//Polymorphic base for elements!

class element_base {
public:
    enum class TEXT_ALIGNMENT{
        CENTER,
        LEFT,
        RIGHT
    };
    enum class ELEMENT_TYPE
    {
        EDIT,
        ACTION,
        LINK
    };

public:
    virtual void set_font(sFONT *_f) {}
    virtual sFONT *get_font() { return nullptr; }
    virtual void set_selected(bool _s) {}
    virtual void set_inverted(bool _i) {}
    virtual bool get_selected() const { return false; }
    virtual bool get_inverted() const { return false; }
    virtual void set_name(String _s) {}
    virtual String get_name() const { return ""; }
    virtual String get_state_name() const { return ""; }
    virtual void set_alignment(TEXT_ALIGNMENT _at) { }
    virtual TEXT_ALIGNMENT get_name_align() const { return TEXT_ALIGNMENT::CENTER; }
    virtual TEXT_ALIGNMENT get_state_align() const { return TEXT_ALIGNMENT::CENTER; }
    virtual String get_selected_state() {return "";}
    virtual void increment_state() {}
    virtual int get_y() const { return 0; }
    virtual void set_y(int _y) {}
    virtual int get_x_from_alignment() { return 0; }
    virtual int get_x_from_alignment(TEXT_ALIGNMENT _ta, String _s) { return 0; }
    virtual ELEMENT_TYPE get_element_type() { return ELEMENT_TYPE::LINK; }
    virtual void register_pointer_variable(String var) {}
    virtual void register_callback_function(void(*func)()){}
    virtual void call_action(){}
};