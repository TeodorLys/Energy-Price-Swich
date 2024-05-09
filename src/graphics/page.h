#pragma once
#include <Arduino.h>
#include "element.h"
#include "element_base.h"
#include "edit_element.h"
#include "action_element.h"

class page {
private:
    struct page_element{
        element_base *_el;
        String _next, last, _name;
    };
    String _title;
    page_element *_page[10] = {nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr, nullptr};
    element el_title;
    bool splash_page = false;
    String splash_next;
public:
    page(String title);
    void add_element(element _e, String prev, String next, String name);
    void add_element(edit_element _e, String prev, String next, String name);
    void add_element(action_element _e, String prev, String next, String name);
    void select_element(byte i);
    void element_down();
    void element_up();
public:
    /*
    Splash page are a page that you cant interact with. no elements can be selected.
    this page only has a next link, so when we call the select_page function in the page_handler class
    we just jump to the linked page.
    */
    void set_splash_page(bool _splash, String _next) { splash_page = _splash; splash_next = _next;}
    bool is_splash_page() const { return splash_page; }
    // Returns the splash page linked page.
    String get_splash_next() const { return splash_next; }
    // Title elements are always regular elements.
    element get_title_element(){
        return el_title;
    }

    element_base* get_element_by_index(byte i){
        if(i > page_size())
            return new element("", NULL, false);
        return _page[i]->_el;
    }

    page_element* get_selected_element(){
       for(int a = 0; a < 10; a++){
            if(_page[a] != nullptr)
                if((*_page[a]->_el).get_selected())
                    return _page[a];
        }
        return nullptr; 
    }


    bool is_element_present(String name){
        return (!get_element_by_name(name)->get_name().isEmpty());
    }

    element_base* get_element_by_name(String name){
        for (int a = 0; a < 10; a++){
            if(_page[a] == nullptr)
                break;
            if(_page[a]->_name == name)
                return _page[a]->_el;
        }
            return new element("", NULL, false);
    }

    int page_size() {
        if(_page[0] == nullptr)
            return 0;
        for(int a = 0; a < 10; a++){
            if(_page[a] == nullptr)
                return a;
        }
        return 0;
    }
};