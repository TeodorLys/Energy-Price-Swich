#pragma once
#include <SPI.h>
#include <EPD1in54.h>
#include <EPDPaint.h>
#include "element.h"
#include "edit_element.h"
#include "page.h"
#include "page_handler.h"
/*
DIN - D7
CLK - D5
CS  - D8
DC  - D3
RST - D4
BUSY- D2
*/

class graphics {
private:
EPD1in54 *edp;
unsigned char image[1024];
EPDPaint *paint; 
public:
    // Initialize ePaper api
    bool begin();
    //Sets the ePaper pixel buffers to white.
    void clear_frame_buffer();
    void draw_element(element_base* _e, int x, int y);
    // This includes both edit and action(TODO: Change name.)
    void draw_edit_element(element_base* _e, int x, int y);
    // Title are always a regular element
    void draw_title_element(element _e, int x, int y);
    // Draw single page, ex. boot screen
    void draw_page(page _p);
    // Draw functions does not send the data to the display only "compiles" it,
    // Then this sends data to the display 
    void display_image();
    void add_element_to_frame(element _e);  // *NOT IN USE*
    // Draw the entire page-handler
    void draw_handler(page_handler *_ph);
};