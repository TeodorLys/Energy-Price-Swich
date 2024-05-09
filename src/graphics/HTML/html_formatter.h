#pragma once
#include "html_element.h"
#include "html_style_class.h"

class html_formatter{
private:
    html_element *_e[20];
    html_style_class *_s_c[20];
public:
    void register_html_element(html_element e);
    void register_html_style_class(html_style_class sc);
    String get_page();
private:
    int get_element_size(){
        for (int a = 0; a < 20; a++){
            if(_e[a] == nullptr)
                return a;
        }
    }
    int get_style_class_size(){
        for (int a = 0; a < 20; a++){
            if(_e[a] == nullptr)
                return a;
        }
    }
};