#include "graphics.h"
#include "debugging/debugging.h"
#define UNCOLORED 1
#define COLORED 0

//Initializes the ePaper display.
bool graphics::begin(){
    edp = new EPD1in54(GPIO_NUM_1, GPIO_NUM_10, GPIO_NUM_7, GPIO_NUM_0);
    paint = new EPDPaint(image, 0, 0);
    paint->setRotate(ROTATE_0);
    if(edp->init(lutFullUpdate) != 0) {
        Serial.println("Could not init!");
        return false;
    }
    return true;
}

//Reset each of the memory buffers inside the Waveshare 1.54inch
void graphics::clear_frame_buffer() {
    edp->clearFrameMemory(0xFF);
    edp->displayFrame();
    edp->clearFrameMemory(0xFF);
    edp->displayFrame();
}

// Draw and element, and check if it is an editable element(edit_element.h) or regular 
// "Directory"
void graphics::draw_element(element_base* _e, int x, int y){
    if(_e->get_element_type() == element_base::ELEMENT_TYPE::EDIT){
        draw_edit_element(_e, y, x);
        return;
    }
    paint->setWidth(200);
    paint->setHeight(24);
    paint->clear(!_e->get_inverted());
    paint->drawStringAt(_e->get_x_from_alignment(), 0, _e->get_name().c_str(), &*_e->get_font(), _e->get_inverted());
    edp->setFrameMemory(paint->getImage(), x, _e->get_y(), paint->getWidth(), paint->getHeight());
}

// Editable elements have two different elements. name of the element and its value. thats why we need to 
// have a seperate function for it.
void graphics::draw_edit_element(element_base* _e, int x, int y){
    paint->setWidth(200);
    paint->setHeight(24);
    paint->clear(!_e->get_inverted());
    paint->drawStringAt(_e->get_x_from_alignment(_e->get_name_align(), _e->get_name()), 0, _e->get_name().c_str(), &*_e->get_font(), _e->get_inverted());
    paint->drawStringAt(_e->get_x_from_alignment(_e->get_state_align(), _e->get_state_name()), 0, _e->get_selected_state().c_str(), &*_e->get_font(), _e->get_inverted());
    edp->setFrameMemory(paint->getImage(), x, _e->get_y(), paint->getWidth(), paint->getHeight());
}

// Titles are different. Its always uppercase, it has a line drawn under it and it is always centered.
void graphics::draw_title_element(element _e, int x, int y){
    paint->setWidth(200);
    paint->setHeight(35);
    paint->clear(!_e.get_inverted());
    paint->drawStringAt(_e.get_x_from_alignment(), 0, _e.get_name().c_str(), &*_e.get_font(), _e.get_inverted());
    paint->drawHorizontalLine(0, 26, 200, COLORED);
    edp->setFrameMemory(paint->getImage(), x, _e.get_y(), paint->getWidth(), paint->getHeight());

}

// For example splash screen.
// Just draws an entire page class.
void graphics::draw_page(page _p){
    draw_title_element(_p.get_title_element(), 0, 0);
    for(int a = 0; a < _p.page_size(); a++){
      draw_element(_p.get_element_by_index(a), 0, 0);
    }
    display_image();
}

// This draws the page handler, which contains all of the system pages(settings, status and setup pages).
// Main(usually) only calls this function.
void graphics::draw_handler(page_handler *_ph){
    if(_ph->update_full_page()){
        clear_frame_buffer();
        _ph->updated_full_page();
    }
    draw_title_element(_ph->to_draw()->get_title_element(), 0, 0);
    for(int a = 0; a < _ph->to_draw()->page_size(); a++){
      draw_element(_ph->to_draw()->get_element_by_index(a), 0, 0);
    }
    _ph->updated_graphics();
    display_image();
}

// Sends the data to the ePaper display of the "draw_xxx" functions
void graphics::display_image() {
    edp->displayFrame();
    //edp->displayFrame();
}

/*NOT IN USE*/
void graphics::add_element_to_frame(element _e){
    paint->drawStringAt(_e.get_x_from_alignment(), 0, _e.get_name().c_str(), &*_e.get_font(), _e.get_inverted());
}