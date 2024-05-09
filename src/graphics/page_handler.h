#pragma once
#include "page.h"
#include "settings.h"

class page_handler {
private:
    byte i = 0;
    bool update = false;
    bool update_full = false;
    settings *setting;

public:
    page *_buffer[10];
    page_handler();
    // We select the element above the current one, if at top; loop around
    void select_element_up();
    // We select the element below the current one, if at top; loop around
    void select_element_down();
    /*
    Edit_element: Loop through the states
    Action_element: call function
    Link_element(element.h): Go to another page.
    */
    void select_page();
    void back_page();
    void create_default_pages();

public:
    page* to_draw() {return _buffer[i];}
    /*
    For external function to know if graphics need to update.
    And also for external functions to tell this class that graphics have updated
    */
    bool graphics_update() const {return update;}
    void updated_graphics() { update = false; }
    void update_graphics() { update = true; }
    bool update_full_page() const {return update_full;}
    void updated_full_page() { update_full = false; }
};