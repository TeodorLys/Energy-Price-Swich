#include "page_handler.h"
#include "edit_element.h"
#include "action_element.h"
#include "data_handling/data.h"
#include "debugging/debugging.h"
#include "settings.h"

page_handler::page_handler() {
    
}

void page_handler::create_default_pages(){
    setting = new settings;
    data::limit = setting->load_setting("limit", 1);
    data::region = setting->load_setting("region", 3);
    _buffer[0] = new page("Status");
    _buffer[0]->set_splash_page(true, "Setup");

    _buffer[1] = new page("Setup");
    edit_element _limit("Limit", &Font24, false);
    _limit.assign_state_value_name(0, "0%");
    _limit.assign_state_value_name(1, "25%");
    _limit.assign_state_value_name(2, "50%");
    _limit.assign_state_value_name(3, "75%");
    _limit.register_pointer_variable(&data::limit);
    _limit.set_saved_state_value_by_index(data::limit);
    _buffer[1]->add_element(_limit, "", "", "limit");
    edit_element _region("Region", &Font24, false);
    _region.assign_state_value_name(0, "SE1");
    _region.assign_state_value_name(1, "SE2");
    _region.assign_state_value_name(2, "SE3");
    _region.assign_state_value_name(3, "SE4");
    _region.register_pointer_variable(&data::region);
    _region.set_saved_state_value_by_index(data::region);
    _buffer[1]->add_element(_region, "", "", "region");
    edit_element _timer("Timer", &Font24, false);
    _timer.assign_state_value_name(0, "15min");
    _timer.assign_state_value_name(1, "30min");
    _timer.assign_state_value_name(2, "45min");
    _timer.assign_state_value_name(3, "AUTO");
    _timer.register_pointer_variable(&data::timer);
    _timer.set_saved_state_value_by_index(data::timer);
    _buffer[1]->add_element(_region, "", "", "region");
    _buffer[1]->add_element(element("Back", &Font24, false), "Status", "", "");

    _buffer[2] = new page("WiFi");
    _buffer[2]->add_element(element("Test", &Font24, false), "Setup", "", "");
    _buffer[2]->add_element(element("Back", &Font24, false), "Setup", "", "");
    i = 0;
}

void page_handler::select_element_down() {
    if(_buffer[i]->is_splash_page())
        return;
    if(_buffer[i]->page_size() > 1){
        _buffer[i]->element_down();
        update = true;
    }
    return;
}

void page_handler::select_element_up(){
    if(_buffer[i]->is_splash_page())
        return;
    if(_buffer[i]->page_size() > 1){
            _buffer[i]->element_up();
            update = true;
    }
    return;
}

void page_handler::select_page(){
    String title_check = "";
    if(!_buffer[i]->is_splash_page()){
        /*
        Here we check if the selected element is a edit or action, since these have different functions.
        */
        if(_buffer[i]->get_selected_element()->_el->get_element_type() == element_base::ELEMENT_TYPE::EDIT){
            _buffer[i]->get_selected_element()->_el->increment_state();
            //TODO: If more settings gets added, make a function or class specifically for checking what data was changed and save accordingly
            setting->save_setting("limit", data::limit);
            setting->save_setting("region", data::region);
            update = true;
            return;
        }else if(_buffer[i]->get_selected_element()->_el->get_element_type() == element_base::ELEMENT_TYPE::ACTION){
            _buffer[i]->get_selected_element()->_el->call_action();
            update = true;
            return;   
        }
        /*
        If it is a regulare link_element, we need to check what page it linkes to.
        And if the default(always added at bottom of page) back button is selected, we check
        what the previous page was.
        */
        if(_buffer[i]->get_selected_element()->_el->get_name() == "Back")
            title_check = _buffer[i]->get_selected_element()->last;
        else
            title_check = _buffer[i]->get_selected_element()->_next;
    }else 
        title_check = _buffer[i]->get_splash_next();
    title_check.toLowerCase();
    /*
    Here we look for the page with the link name gotten from the if condition above
    */
    for(int a = 0; a < 10; a++){
        if(_buffer[a] != nullptr){
            String name_check = _buffer[a]->get_title_element().get_name();
            name_check.toLowerCase();        
            if(name_check == title_check){
                i = a;
                update = true;
                break;
            }
            
        }else break;
    }
    update_full = true;
}

void page_handler::back_page(){
    for(int a = 0; a < 10; a++){
        if(_buffer[a] != nullptr){
            if(_buffer[a]->get_title_element().get_name() == _buffer[i]->get_selected_element()->last){
                i = a;
                update = true;
                break;
            }
        } else break;
    }
    update_full = true;
}