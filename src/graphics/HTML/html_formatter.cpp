#include "html_formatter.h"

void html_formatter::register_html_element(html_element e){
    for (int a = 0; a < 20; a++){
        if(_e[a] != nullptr)
            _e[a] = new html_element(e);
    }
}


void html_formatter::register_html_style_class(html_style_class sc){
    for (int a = 0; a < 20; a++){
        if(_s_c[a] != nullptr)
            _s_c[a] = new html_style_class(sc);
    }
}

String html_formatter::get_page(){
    String buff = "<!DOCTYPE html>\n";
    buff += "<html>\n";
    buff += "<style>\n";
    for (int a = 0; a < get_style_class_size(); a++)
        buff += _s_c[a]->get_class_decl();
    buff += "</style>\n";
    buff += "<body style=\"font-family: sans-serif\" text-align: center class=\"content\">\n";
    for (int a = 0; a < get_element_size(); a++)
        buff += _e[a]->construct_html_line();
    buff += "</body>\n";
    buff += "</html>\n";
}