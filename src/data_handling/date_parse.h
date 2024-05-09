#pragma once
#include <Arduino.h>
#include "debugging/debugging.h"
//"http://worldtimeapi.org/api/timezone/Europe/Stockholm"
/*
This class were fightning me alot... so
everytime we call ex. "get_hour" we parse the string. really stupid an wasteful.
but...I got annoyed!

TODO: Convert this to only use the date_data structure.
*/
struct date_data{
    int day, month, year;

    date_data(int _d, int _m, int _y) : day(_d), month(_m), year(_y) {}
    date_data() {}
    bool operator!=(date_data _d){
        return(_d.day != day && _d.month != month && _d.year != year);
    }
    bool operator>(date_data _d){
        String _tmp = "";
        if(day > _d.day)
            _tmp += "d>_d, ";
        if(month > _d.month)
            _tmp += "m>_m, ";
        if(year > _d.year)
            _tmp += "y>_y, ";
        _debugging::push_error_message(_tmp);
        return (day > _d.day || month > _d.month || year > _d.year);
    }
    bool operator==(date_data _d){
        return (day == _d.day && month == _d.month && year == _d.year);
    }
};
class date_parse {
private:
    char _internal[19];
    date_data __internal;
    byte hour;
    byte minute;

public:
    date_parse() {}

    int get_hour(){
        return hour;
    }

    int get_minute(){
        return minute;
    }

    int get_year(){
        return __internal.year;
    }
    
    int get_month(){
        return __internal.month;
    }

    int get_day(){
        return __internal.day;
    }

    void operator=(const char* raw){
        String buff = (String)raw;
        __internal.day = buff.substring(8, 10).toInt();
        __internal.month = buff.substring(5, 7).toInt();
        __internal.year = buff.substring(0, 4).toInt();
        hour = buff.substring(buff.indexOf("T") + 1, (buff).indexOf("T") + 3).toInt();
        minute = buff.substring(buff.indexOf("T") + 4, (buff).indexOf("T") + 6).toInt();
    }

    long get_deep_sleep_time(){
        if(get_minute() > 30){
            return(((60 - (get_minute())) * 60) * 1e6) + 60e6;
        }else if(get_minute() < 30){
            return(((30 - (get_minute())) * 60) * 1e6) + 60e6;
        }else {
            return ((30 * 60) * 1e6) + 60e6;
        }
    }

    String get_deep_sleep_wake_time(){
        int _t = (get_deep_sleep_time() / 1e6);
        long _c = ((get_hour() * 60) * 60) + _t;
        _c += (get_minute() * 60);
        float _hour = (_c / 60.0 / 60.0);
        float _min = ((_c / 60.0 / 60.0) - (int)_hour) * 60.0;

        _debugging::push_error_message("_t = " + String(_t) + ", _c = " + (String)_c);
        _debugging::push_error_message("_hour = " + String(_hour) + ", _min = " + (String)_min);

        int _h = (int)_hour;
        int _m = (int)_min;
        String str = "";
        str += _h < 10 ? "0" + (String)_h : (String)_h;
        str += ":";
        str += _m < 10 ? "0" + (String)_m : (String)_m;
        return str;
    }
    
    //Convert the string stuff to the date_data structure(which most data parsing classes use)
    date_data to_struct(){
        return __internal;
    }
};