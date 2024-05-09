#pragma once
#include <Arduino.h>
#include <fonts.h>
#include "element_base.h"
/*
Consider using a templated class for state array size
*/
class edit_element : public element_base {
private:
    String name;
    sFONT *font;
    const TEXT_ALIGNMENT _name_align = TEXT_ALIGNMENT::LEFT;
    const TEXT_ALIGNMENT _state_align = TEXT_ALIGNMENT::RIGHT;
    int y = 0;
    bool selected = false;
    bool inverted;
    byte state = 0;  // For the changeability
    const byte MAX_STATES = 6;
    String state_value[6];
    int *callback;
public:
// Polymorphic functions
    void set_font(sFONT *_f) { font = _f; }
    sFONT *get_font() { return font; }
    void set_selected(bool _s) { selected = _s; }
    void set_inverted(bool _i) { inverted = _i; }
    bool get_selected() const { return selected; }
    bool get_inverted() const { return inverted; }
    String get_name() const { return name; }
    String get_state_name() const { return state_value[state]; }
    void set_name(String _s) { name = _s; }
    TEXT_ALIGNMENT get_name_align() const { return _name_align; }
    TEXT_ALIGNMENT get_state_align() const {return _state_align;}
    ELEMENT_TYPE get_element_type() { return ELEMENT_TYPE::EDIT; }
    int get_y() const { return y; }
    void set_y(int _y) { y = _y; }
    edit_element(String _name, sFONT *_font, bool _inverted) : name(_name), font(_font), inverted(_inverted) {}
    edit_element() {}
    //When pressed the element we just increment. we CANT go up or down.
    void increment_state() {
        int _max = 0;
        /*
        Loops through the states and checks which one is empty
        because states are not dynamic, we allocate a array of 10, always(TODO: pointer?)
        We rarely use all state-elements, so we need to check when we reach an empty element 
        */
        for (int a = 0; a < MAX_STATES; a++){
            if(state_value[a].isEmpty()){
                _max = a - 1;
                break;
            }
        }
        //If the used element size is the same as the currently selected state we loop around
        if((int)state >= _max)
            state = 0;
        else
            state++;
        //Sets the pointer variable, SEE register_pointer_variable
        *callback = state;
    }
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
    //A maximum of 10 different setting states, and we can set the value of each
    void assign_state_value_name(byte index, String name){
        if(index > MAX_STATES - 1 || index < 0)
            return;
        state_value[index] = name;
    }
    // When loading settings from preferences so we can set default state.
    void set_saved_state_value_by_name(String name){
        name.toLowerCase();
        for(int a = 0; a < MAX_STATES; a++){
            String check = state_value[a];
            check.toLowerCase();
            if(state_value[a] == name){
                state = a;
            }
        }
    }
    // Se above^
    void set_saved_state_value_by_index(int index){
        if(index > MAX_STATES - 1 || index < 0)
            return;
        state = index;
    }
    //Returns the value of the currently active state.
    String get_selected_state(){
        return state_value[(int)state];
    }
    /*
    Register a variable of which to assign the state value to.
    No need to callback or call any function in here since this is a pointer
    Everything registered here should be in the global data class
    */
    void register_pointer_variable(int* var){
        callback = &*var;
    } 
};