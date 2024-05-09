#include "page.h"
#include <fonts.h>

page::page(String title){
    _title = title;
    _title.toUpperCase();
    el_title.set_name(_title);
    el_title.set_font(&Font24);
    el_title.set_inverted(false);
    el_title.set_alignment(element::TEXT_ALIGNMENT::CENTER);
    el_title.set_y(2);
}

void page::add_element(element _e, String prev, String next, String name){
    if(page_size() == 0 && !splash_page){
        _e.set_selected(true);
        _e.set_inverted(true);
    }else {
        _e.set_inverted(false);
    }
    _e.set_y((_e.get_font()->Height * page_size()) + 35);
    for(byte a = 0; a < 10; a++){
        if(_page[a] == nullptr){
            _page[a] = new page_element();
            _page[a]->_el = new element(_e);
            _page[a]->_next = next;
            _page[a]->last = prev;
            _page[a]->_name = name;
            break;
        }
    }
}

void page::add_element(edit_element _e, String prev, String next, String name){
    if(page_size() == 0){
        _e.set_selected(true);
        _e.set_inverted(true);
    }else {
        _e.set_inverted(false);
    }
    _e.set_y((_e.get_font()->Height * page_size()) + 35);
    for(byte a = 0; a < 10; a++){
        if(_page[a] == nullptr){
            _page[a] = new page_element();
            _page[a]->_el = new edit_element(_e);
            _page[a]->_next = next;
            _page[a]->last = prev;
            _page[a]->_name = name;
            break;
        }
    }
}
void page::add_element(action_element _e, String prev, String next, String name){
    if(page_size() == 0){
        _e.set_selected(true);
        _e.set_inverted(true);
    }else {
        _e.set_inverted(false);
    }
    _e.set_y((_e.get_font()->Height * page_size()) + 35);
    for(byte a = 0; a < 10; a++){
        if(_page[a] == nullptr){
            _page[a] = new page_element();
            _page[a]->_el = new action_element(_e);
            _page[a]->_next = next;
            _page[a]->last = prev;
            _page[a]->_name = name;
            break;
        }
    }
}

void page::select_element(byte i){
    if(i > page_size()){
        return;
    }
    //Reset all elements to a unselected state(a selected element are inverted)
    for(byte a = 0; a < page_size(); a++){
        _page[a]->_el->set_selected(false);
        _page[a]->_el->set_inverted(false);
    }
    //Set the element by parameter index as selected
    _page[i]->_el->set_selected(true);
    _page[i]->_el->set_inverted(true);
}

void page::element_down(){
    byte _sel = 0;
    byte _max = page_size();
    /*
    Lookup the currently selected state, save the index and add 1(if end of array loop around, index = 0).
    Also reset all elements to unselected state.
    */
    for(byte a = 0; a < _max; a++){
        if(_page[a]->_el->get_selected()){
            if(a == _max - 1)
                _sel = 0;
            else{
                _sel = a;
                _sel++;
            }
        }
        _page[a]->_el->set_selected(false);
        _page[a]->_el->set_inverted(false);
    }
    //Set the lookup index to be selected.
    _page[_sel]->_el->set_selected(true);
    _page[_sel]->_el->set_inverted(true);
}

void page::element_up(){
    //See element_down function above.
    byte _sel = 0;
    byte _max = page_size();
    for(byte a = 0; a < _max; a++){
        if(_page[a]->_el->get_selected()){
            if(a == 0)
                _sel = _max-1;
            else{
                _sel = a;
                _sel--;
            }
        }
        _page[a]->_el->set_selected(false);
        _page[a]->_el->set_inverted(false);
    }
    _page[_sel]->_el->set_selected(true);
    _page[_sel]->_el->set_inverted(true);
}