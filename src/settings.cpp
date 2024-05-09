#include "settings.h"
#include "debugging/debugging.h"
#include "data_handling/data.h"

Preferences pref;

settings::settings(){
}

void settings::init_settings(){
    _debugging::push_error_message(pref.begin("settings", false));
}

void settings::save_setting(String name, byte value){
    _debugging::push_error_message(pref.putChar(name.c_str(), value));
}

int settings::load_setting(String name, byte def_value){
    int _tmp = pref.getChar(name.c_str(), 0);
    return _tmp;
}

void settings::save_hour_data(byte day){
    // Saves the hour data arrays as bytes, to minimize waste of space.
    // Had error code, from NVS, NOT ENOUGH SPACE before.
    if(day == 1)
        pref.putBytes("p_t1", data::today_prices, sizeof(data::today_prices));
    else 
        pref.putBytes("p_t2", data::tomorrow_prices, sizeof(data::tomorrow_prices));
}

void settings::load_hour_data(){
    // Retrieves the byte array and checks it with the data.h functions size
    // to see if they match
    // TODO: Change this function to bool, and return whether sizes match, if not call scheduler.create_daily_min_max
    size_t len = pref.getBytesLength("p_t1");
    if(len != sizeof(data::today_prices)){
        _debugging::push_error_message("today_prices not same length");
        return;
    }
    pref.getBytes("p_t1", &data::today_prices, sizeof(data::today_prices));

    len = pref.getBytesLength("p_t2");
    if(len != sizeof(data::tomorrow_prices)){
        _debugging::push_error_message("tomorrow_prices not same length");
        return;
    }
    pref.getBytes("p_t2", &data::tomorrow_prices, sizeof(data::tomorrow_prices));
}


bool settings::has_data(){
    return pref.getBool("hd", false);
}

void settings::set_verifier(){
    _debugging::push_error_message(pref.putBool("hd", true));
}

void settings::save_dates(){
    pref.putInt("d_t", data::today.day);
    pref.putInt("m_t", data::today.month);
    pref.putInt("y_t", data::today.year);
    pref.putInt("d_t2", data::tomorrow.day);
    pref.putInt("m_t2", data::tomorrow.month);
    pref.putInt("y_t2", data::tomorrow.year);
}

void settings::load_dates(){
    data::today.day = pref.getInt("d_t", 0);
    data::today.month = pref.getInt("m_t", 0);
    data::today.year = pref.getInt("y_t", 0);

    data::tomorrow.day = pref.getInt("d_t2", 0);
    data::tomorrow.month = pref.getInt("m_t2", 0);
    data::tomorrow.year = pref.getInt("y_t2", 0);
}

void settings::save_min_max(){
    for (int a = 0; a < 4; a++){
        String max_1 = "max.1-" + (String)a;
        String min_1 = "min.1-" + (String)a;
        String max_2 = "max.2-" + (String)a;
        String min_2 = "min.2-" + (String)a;
        pref.putFloat(max_1.c_str(), data::max_today[a]);
        pref.putFloat(min_1.c_str(), data::min_today[a]);
        pref.putFloat(max_2.c_str(), data::max_tomorrow[a]);
        pref.putFloat(min_2.c_str(), data::min_tomorrow[0]);
    }
}

void settings::load_min_max(){
    for (int a = 0; a < 4; a++){
        String max_1 = "max.1-" + (String)a;
        String min_1 = "min.1-" + (String)a;
        String max_2 = "max.2-" + (String)a;
        String min_2 = "min.2-" + (String)a;
        data::max_today[a] = pref.getFloat(max_1.c_str(), 0.0);
        data::min_today[a] = pref.getFloat(min_1.c_str(), 0.0);
        data::max_tomorrow[a] = pref.getFloat(max_2.c_str(), 0.0);
        data::min_tomorrow[a] = pref.getFloat(min_2.c_str(), 0.0);
    }
}

void settings::wipe(){
    pref.clear();
}